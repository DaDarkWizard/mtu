/*
 * Program to handle mtu things.
 *
 * @author Daniel Masker
 * @email dtmasker@mtu.edu
 * @date 9/11/2024
 */

#include "vpn_handler.h"
#include "print_handler.h"

#include <stdio.h>
#include <getopt.h>
#include <string.h>


int main(int argc, char **argv)
{
    // Check if enough args are given.
    if(argc < 2)
    {
        printf("Usage: mtu <command> <args>");
        return 0;
    }

    Config config;

    if ( load_config(&config) != 0 )
    {
        printf("Failed to load config.\n");
        return -1;
    }

    // Check what command is being run and handle it.
    if(strcmp(argv[1], "vpn") == 0)
    {
        return vpn_handler(argc, argv, &config);
    }
    else if (strcmp(argv[1], "print") == 0)
    {
        return print_handler(argc, argv, &config);
    }
    else
    {
        printf("Unknown option %s\n", argv[1]);
        return 0;
    }


    return 0;
}
