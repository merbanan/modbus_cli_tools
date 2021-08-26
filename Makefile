all: ws_relay sdm120m orno_we_514

ws_relay: ws_relay.c 
	gcc -ggdb3 -O0 -Ilibmodbus/src/ -o ws_relay ws_relay.c libmodbus/src/.libs/libmodbus.a

sdm120m: sdm120m.c
	gcc -ggdb3 -O0 -Ilibmodbus/src/ -o sdm120m sdm120m.c libmodbus/src/.libs/libmodbus.a

orno_we_514: orno_we_514.c
	gcc -ggdb3 -O0 -Ilibmodbus/src/ -o orno_we_514 orno_we_514.c libmodbus/src/.libs/libmodbus.a

libmodbus: libmodbus
	git clone https://github.com/merbanan/libmodbus.git
	cd libmodbus ; ./autogen.sh ; ./configure --enable-static ; make

clean:
	rm -f *.o ws_relay sdm120m

install:

