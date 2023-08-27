
#include <stdio.h>
#include <stdlib.h>
#include <modbus.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <getopt.h>
#include <time.h>


int get_volt(modbus_t* mb, uint16_t* tab_reg) {
    int rc, i;
    rc = modbus_read_registers(mb, 0x0131, 0x0001, tab_reg);

    printf("Volt %f\n", tab_reg[0]*0.01);
    return rc;
}

int get_frequency(modbus_t* mb, uint16_t* tab_reg) {
    int rc, i;
    rc = modbus_read_registers(mb, 0x0130, 0x0001, tab_reg);

    printf("Frequency %f\n",  tab_reg[0]*0.01);
    return rc;
}

int get_total_active_energy(modbus_t* mb, uint16_t* tab_reg) {
    int rc, i;
    rc = modbus_read_registers(mb, 0xA000, 0x0002, tab_reg);

    printf("Energy %f\n",  MODBUS_GET_INT32_FROM_INT16(tab_reg,0)*0.01);
    return rc;
}


int get_all(modbus_t* mb, uint16_t* tab_reg, char* host, int slave_id) {
    int rc, i;
    float volt, energy, frequency, power, current;

    // get volt
    rc = modbus_read_registers(mb, 0x0131, 0x0001, tab_reg);
    if (rc < 0) return rc;
    volt = tab_reg[0]*0.01;

    // get frequency
    rc = modbus_read_registers(mb, 0x0130, 0x0001, tab_reg);
    if (rc < 0) return rc;
    frequency = tab_reg[0]*0.01;

    // get energy
    rc = modbus_read_registers(mb, 0xA000, 0x0002, tab_reg);
    if (rc < 0) return rc;
    energy = ((tab_reg[0]<<16) | (tab_reg[1]))*0.01;

    // get power
    rc = modbus_read_registers(mb, 0x0140, 0x0002, tab_reg);
    if (rc < 0) return rc;
    power = ((tab_reg[0]<<8) | tab_reg[1]);

    // get current
    rc = modbus_read_registers(mb, 0x0139, 0x0002, tab_reg);
    if (rc < 0) return rc;
    current = ((tab_reg[0]<<8) | tab_reg[1])*0.001;

    printf("{ \"id\" : %d, \"energy\" : %f, \"volt\" : %f, \"frequency\" : %f, \"power\" : %f, \"current\" : %f, \"host\" : \"%s\" }\n", slave_id, energy, volt, frequency, power, current, host);

    return rc;
}


// Parity is undocumented and only used in the windows software
int get_modbus_status(modbus_t* mb, uint16_t* tab_reg) {
    int rc, i, baud_value, parity_value;
    // 1=1200,2=2400,3=4800,4=9600
    char* bauds[] = {"Invalid", "1200", "2400", "4800", "9600"};
    char* parity[] = {"Invalid", "None", "Odd", "Even"};
    rc = modbus_read_registers(mb, 0x0002, 0x0001, tab_reg);
    
    printf("Modbus id %d\n", tab_reg[0]);

    rc = modbus_read_registers(mb, 0x0003, 0x0001, tab_reg);
    baud_value   = tab_reg[0];
//    parity_value = (tab_reg[0] >> 8) & 0xF; 
    
    printf("Baud rate %d\n", baud_value);
//    printf("Parity %s\n", parity[parity_value]);
    rc = modbus_read_registers(mb, 0x0041, 0x0001, tab_reg);

    return rc;
}

int configure_modbus_address(modbus_t* mb, uint16_t* tab_reg, int modbus_address
) {
    int rc, i;

    rc = modbus_write_register(mb, 0x0002, modbus_address);

    return rc;
}

int configure_baud_rate(modbus_t* mb, uint16_t* tab_reg, int baud_rate, int parity) {
    int rc, i;
    tab_reg[0] = baud_rate&0xFFFF;

    rc = modbus_write_registers(mb, 0x0003, 0x0001, tab_reg);

    return rc;
}


int get_hardware_version(modbus_t* mb, uint16_t* tab_reg) {
    int rc, i;
    rc = modbus_read_registers(mb, 0x0000, 0x0002, tab_reg);
    
    printf("Serial number: %04x%04x\n", tab_reg[0], tab_reg[1]);
    rc = modbus_read_registers(mb, 0x0004, 0x0002, tab_reg);
    printf("Software version: %04x\n", tab_reg[0]);
    rc = modbus_read_registers(mb, 0x0006, 0x0002, tab_reg);
    printf("Hardware version: %04x\n", tab_reg[0]);

    return rc;
}




int main(int argc,char** argv) {
    modbus_t *mb = NULL;
    uint16_t tab_reg[32] = {};
    uint8_t tab_bits[32] = {};
    int ret, rc, i;
    int option = 0;
    int port = 12345;
    int modbus_tcp = 0;
    int slave = 0;
    int debug = 0;
    int get_version = 0;
    int read_volt = 0;
    int read_frequency = 0;
    int read_total_active_energy = 0;
    int read_modbus_status = 0;
    int set_modbus_address = 0;
    int set_baud_rate = -1;
    int set_parity = 2;
    int read_all = 0;
    char* host = "192.168.1.8";
    char* serial_device = "/dev/ttyUSB0";
    int serial_baud_rate = 9600;
    char serial_parity = 'E';
    int serial_stop_bits = 1;

    while ((option = getopt(argc, argv,"h:p:m:as:d:vVfIM:B:EAD:b:S:P:Y:")) != -1) {
        switch (option) {
            case 'h' : host = optarg; 
                break;
            case 'p' : port = atoi(optarg);
                break;
            case 'm' : modbus_tcp = atoi(optarg);
                break;
            case 's' : slave = atoi(optarg);
                break;
            case 'd' : debug = atoi(optarg);
                break;
            case 'v' : get_version = 1;
                break;
            case 'V' : read_volt = 1;
                break;
            case 'f' : read_frequency = 1;
                break;
            case 'I' : read_modbus_status = 1;
                break;
            case 'E' : read_total_active_energy = 1;
                break;
            case 'M' : set_modbus_address = atoi(optarg);
                break;
            case 'B' : set_baud_rate = atoi(optarg);
                break;
            case 'Y' : set_parity = atoi(optarg);
                break;
            case 'A' : read_all = 1;
                break;
            case 'D' : serial_device = optarg;
                break;
            case 'b' : serial_baud_rate = atoi(optarg);
                break;
            case 'P' : serial_parity = optarg[0];
                break;
            case 'S' : serial_stop_bits = atoi(optarg);
                break;
            default: ; 
                 exit(EXIT_FAILURE);
        }
    }
    if (argc < 2) {
        //print_usage();
        goto exit;
    }

    usleep(100000);
    if (modbus_tcp == 2)
	mb = modbus_new_rtu(serial_device, serial_baud_rate, serial_parity, 8, serial_stop_bits);
    else if (modbus_tcp == 1)
        mb = modbus_new_tcp(host, port);
    else
        mb = modbus_new_rtutcp(host, port);

    modbus_set_debug(mb, debug);
    ret = modbus_connect(mb);

    if (ret == -1) {
        printf("Connect to host: %s port: %d failed, ret=%d", host, port, ret);
        exit(1);
    }
    modbus_set_slave(mb, slave);
    

    if (get_version)
        rc = get_hardware_version(mb, tab_reg);

    if (read_volt)
        rc = get_volt(mb, tab_reg);

    if (read_frequency)
        rc = get_frequency(mb, tab_reg);

    if (read_modbus_status)
        rc = get_modbus_status(mb, tab_reg);

    if (set_modbus_address)
        rc = configure_modbus_address(mb, tab_reg, set_modbus_address);

    if (set_baud_rate != -1)
        rc = configure_baud_rate(mb, tab_reg, set_baud_rate, set_parity);

    if (read_total_active_energy)
        rc = get_total_active_energy(mb, tab_reg);

    if (read_all)
	rc = get_all(mb, tab_reg, host, slave);
exit:
    if (mb) {
        modbus_close(mb);
        modbus_free(mb);
    }
}
