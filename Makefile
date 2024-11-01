C_FILES=main.c vpn_handler.c print_handler.c config.c
H_FILES=vpn_handler.h print_handler.h config.h

mtu: $(C_FILES) $(H_FILES)
	gcc $(C_FILES) -o mtu


.PHONY: clean

clean:
	rm -rf *.o mtu.exe
