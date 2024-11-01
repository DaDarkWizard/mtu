#include "print_handler.h"

void print_usage();

int print_handler(int argc, char **argv, Config *config)
{
    if ( argc < 3 )
    {
        print_usage();
        return 0;
    }
    
    if ( strcmp( argv[2], "start" ) == 0 )
    {
        system("\"C:\\Program Files\\PaperCut MF Client\\pc-client.exe\"");
        return 0;
    }
    else
    {
        print_usage();
        return 0;
    }

    return 0;
}


void print_usage()
{
    printf("Usage: mtu print start");
}
