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

// Functions
#define READ_STATE_OF_RELAY 0x01
#define READ_ADDRESS        0x03
#define WRITE_SINGLE_RELAY  0x05
#define WRITE_CONFIGURATION 0x06
#define WRITE_ALL_RELAYS    0x0F

// Commands
#define OPEN_RELAY  0xFF00
#define CLOSE_RELAY 0x0000
#define FLIP_RELAY  0x5500
#define FLIP_ALL_RELAY  0x5A00
#define OPEN_ALL_RELAY  0xFFFF


// Relays
#define RELAY_ID_0   0x0000
#define RELAY_ID_1   0x0001
#define RELAY_ID_2   0x0002
#define RELAY_ID_3   0x0003
#define RELAY_ID_4   0x0004
#define RELAY_ID_5   0x0005
#define RELAY_ID_6   0x0006
#define RELAY_ID_7   0x0007
#define RELAY_ID_ALL 0x00FF

// Registers
#define VERSION_REG 0x0200
#define DEVICE_ADDRESS_REG 0x4000

int probe_relay_address_fn(modbus_t* mb, uint16_t* tab_reg) {
    int rc, i;
    rc = modbus_read_registers(mb, DEVICE_ADDRESS_REG, 1, tab_reg);
// 
    printf("Relay at address 0x%x", tab_reg[0]);
    return rc;
}

int get_hardware_version(modbus_t* mb, uint16_t* tab_reg) {
    int rc, i;
    rc = modbus_read_registers(mb, VERSION_REG, 1, tab_reg);
    
    for (i=0; i < rc; i++) {
        printf("reg[%d]=%d (0x%X)\n", i, tab_reg[i], tab_reg[i]);
    }

    printf("Hardware version% d\n", tab_reg[0]);
    return rc;
}

int get_relay_status(modbus_t* mb, uint8_t* tab_bits) {
    int rc=0, i;

    rc = modbus_read_bits(mb, RELAY_ID_ALL, READ_STATE_OF_RELAY, tab_bits);
    printf("{ ");
    for (i=0; i < 8; i++) {
        printf("\"relay_%d\" : %d", i+1, tab_bits[i]);
	if (i < 7)
	    printf(", ");
	else
	    printf(" ");
    }
    printf("}\n");
    return rc;
}



int set_relay_open(modbus_t* mb, uint8_t* tab_bits, int relay_id) {
    int rc=0, i;
    uint16_t address = 0x0000 + relay_id-1;
    
    rc = modbus_write_bit(mb, address, OPEN_RELAY);
    
    //printf("address=%d\n", address);
    
    return rc;
}

int set_relay_close(modbus_t* mb, uint8_t* tab_bits, int relay_id) {
    int rc=0, i;
    uint16_t address = 0x0000 + relay_id-1;
    
    rc = modbus_write_bit(mb, address, CLOSE_RELAY);

    //printf("address=%d\n", address);
    
    return rc;
}


int main(int argc,char** argv) {
    modbus_t *mb = NULL;
    uint16_t tab_reg[32];
    uint8_t tab_bits[32];
    int ret, rc, i;
    int option = 0;
    int port = 12345;
    int modbus_tcp = 0;
    int slave = 0;
    int debug = 0;
    int probe_relay_address = 0;
    int get_version = 0;
    int relay_status = 0;
    int open_relay = 0;
    int close_relay = 0;
    char* host = "192.168.1.8";

    while ((option = getopt(argc, argv,"h:p:m:as:d:vSo:c:")) != -1) {
        switch (option) {
            case 'h' : host = optarg; 
                break;
            case 'p' : port = atoi(optarg);
                break;
            case 'm' : modbus_tcp = atoi(optarg);
                break;
            case 'a' : probe_relay_address = 1;
                break;
            case 's' : slave = atoi(optarg);
                break;
            case 'd' : debug = atoi(optarg);
                break;
            case 'v': get_version = 1;
                break;
            case 'S': relay_status = 1;
                break;
            case 'o': open_relay = atoi(optarg);
                break;
            case 'c': close_relay = atoi(optarg);
                break;
            default: ; 
                 exit(EXIT_FAILURE);
        }
    }
    if (argc < 2) {
        //print_usage();
        goto exit;
    }

    if (modbus_tcp)
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
    
    /* Read 1 registers from the address 0 */
//    rc = modbus_read_registers(mb, 0x4000, 1, tab_reg);
//    rc = modbus_read_registers(mb, 0x2000, 1, tab_reg);

    if (get_version)
        rc = get_hardware_version(mb, tab_reg);
    
    if (probe_relay_address)
        rc = probe_relay_address_fn(mb, tab_reg);

    if (open_relay)
        rc = set_relay_open(mb, tab_bits, open_relay);

    if (close_relay)
        rc = set_relay_close(mb, tab_bits, close_relay);

    if (relay_status)
        rc = get_relay_status(mb, tab_bits);
exit:
    if (mb) {
        modbus_close(mb);
        modbus_free(mb);
    }
}
