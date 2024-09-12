

mtu: main.c vpn_handler.c vpn_handler.h config.h config.c
	gcc main.c vpn_handler.c config.c -o mtu


.PHONY: clean

clean:
	rm -rf *.o mtu.exe
