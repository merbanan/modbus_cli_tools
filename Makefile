all: ws_relay

ws_relay: ws_relay.c 
	gcc -ggdb3 -O0 -o  ws_relay ws_relay.c libmodbus/src/.libs/libmodbus.a

clean:
	rm -f *.o ws_relay

install:

