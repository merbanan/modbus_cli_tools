# Command line tools for various rs485 compatible hardware

## Introduction
The tools support the following hardware:
  * SDM120M https://www.eastroneurope.com/images/uploads/products/protocol/SDM120-MODBUS_Protocol.pdf
  * Waveshare Modbus Relay https://www.waveshare.com/wiki/Protocol_Manual_of_Modbus_RTU_Relay

The tools uses a patched (and sligthly modified) version of libmodbus mainly to support the non standard ethernet to rs485 bridges.
  * Waveshare RS485 to ETH https://www.waveshare.com/wiki/RS485_TO_ETH

Note that Waveshare claims that their bridge does not support Modbus which it actually can support. You just have to handle the protocol details yourself.
And they actually say that their relay module is Modbus RTU compliant but libmodbus by default will complain about the data payload. Some hacky changes
are needed for libmodbus to get everything into a workable state.


## Building

```
make libmodbus
make
```

## Tools

ws_relay:

mper:
./mper -m 0 -h 192.168.1.200 -p 4196 -S -s 1 -S -d 1 -c 1

sdm120m:

orno_we_514

Run the following to set baud to 9600 and parity None
./orno_we_514 -d 1 -h 192.168.100.81 -s 41 -B 4 -Y 1


Run the following to set id 11 and 9600 baud
./sdm120m -d 1 -m 2 -D /dev/ttyUSB0 -s 0 -b2400 -M 11 -B 2

Setting the modbus id from 1 to 10
./orno_we_516 -d 1 -m 2 -D /dev/ttyUSB0 -s 1 -M 10

Get all info from Orno meter
./orno_we_516 -d 1 -m 0 -h 192.168.10.200 -p 12345 -s 1 -A

Read temp
./xy-md02 -d 1 -m 2 -D /dev/ttyUSB1 -s 51 -T
