all: ws_relay

ws_relay: ws_relay.c 
	gcc -ggdb3 -O0 -Ilibmodbus/src/ -o  ws_relay ws_relay.c libmodbus/src/.libs/libmodbus.a

libmodbus: libmodbus
	git clone https://github.com/merbanan/libmodbus.git
	cd libmodbus ; ./autogen.sh ; ./configure --enable-static ; make

clean:
	rm -f *.o ws_relay

install:

