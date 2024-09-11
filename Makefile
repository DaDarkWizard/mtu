

mtu: main.c vpn_handler.c vpn_handler.h
	gcc main.c vpn_handler.c -o mtu


.PHONY: clean

clean:
	rm -rf *.o mtu.exe
