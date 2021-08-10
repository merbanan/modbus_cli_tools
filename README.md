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

sdm120m:
