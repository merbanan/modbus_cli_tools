all: bge_tech_DS100-00B ws_relay sdm120m orno_we_514 orno_we_516 ddm18sd mper sdm630 hhc-r4i4d xy-md02 web_io_8relay cwt-sl

ws_relay: ws_relay.c
	gcc -ggdb3 -O0 -Ilibmodbus/src/ -o ws_relay ws_relay.c libmodbus/src/.libs/libmodbus.a

sdm120m: sdm120m.c
	gcc -ggdb3 -O0 -Ilibmodbus/src/ -o sdm120m sdm120m.c libmodbus/src/.libs/libmodbus.a

orno_we_514: orno_we_514.c
	gcc -ggdb3 -O0 -Ilibmodbus/src/ -o orno_we_514 orno_we_514.c libmodbus/src/.libs/libmodbus.a

orno_we_516: orno_we_516.c
	gcc -ggdb3 -O0 -Ilibmodbus/src/ -o orno_we_516 orno_we_516.c libmodbus/src/.libs/libmodbus.a

ddm18sd: ddm18sd.c
	gcc -ggdb3 -O0 -Ilibmodbus/src/ -o ddm18sd ddm18sd.c libmodbus/src/.libs/libmodbus.a

mper: mper.c
	gcc -ggdb3 -O0 -Ilibmodbus/src/ -o mper mper.c libmodbus/src/.libs/libmodbus.a

sdm630: sdm630.c
	gcc -ggdb3 -O0 -Ilibmodbus/src/ -o sdm630 sdm630.c libmodbus/src/.libs/libmodbus.a

hhc-r4i4d: hhc-r4i4d.c
	gcc -ggdb3 -O0 -Ilibmodbus/src/ -o hhc-r4i4d hhc-r4i4d.c libmodbus/src/.libs/libmodbus.a

xy-md02: xy-md02.c
	gcc -ggdb3 -O0 -Ilibmodbus/src/ -o xy-md02 xy-md02.c libmodbus/src/.libs/libmodbus.a

web_io_8relay: web_io_8relay.c
	gcc -ggdb3 -O0 -Ilibmodbus/src/ -o web_io_8relay web_io_8relay.c libmodbus/src/.libs/libmodbus.a

cwt-sl: cwt-sl.c
	gcc -ggdb3 -O0 -Ilibmodbus/src/ -o cwt-sl cwt-sl.c libmodbus/src/.libs/libmodbus.a

bge_tech_DS100-00B: bge_tech_DS100-00B.c
	gcc -ggdb3 -O0 -Ilibmodbus/src/ -o bge_tech_DS100-00B bge_tech_DS100-00B.c libmodbus/src/.libs/libmodbus.a

libmodbus: libmodbus
	git clone https://github.com/merbanan/libmodbus.git
	cd libmodbus ; ./autogen.sh ; ./configure --enable-static ; make

clean:
	rm -f *.o ws_relay sdm120m orno_we_514 orno_we_516 ddm18sd mper hhc-r4i4d xy-md02 web_io_8relay bge_tech_DS100-00B

install:

