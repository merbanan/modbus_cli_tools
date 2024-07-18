all: ws_relay sdm120m orno_we_514 orno_we_516 ddm18sd mper sdm630

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

libmodbus: libmodbus
	git clone https://github.com/merbanan/libmodbus.git
	cd libmodbus ; ./autogen.sh ; ./configure --enable-static ; make

clean:
	rm -f *.o ws_relay sdm120m orno_we_514 orno_we_516 ddm18sd mper

install:

