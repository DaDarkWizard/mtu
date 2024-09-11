/*
 * Program to handle mtu things.
 *
 * @author Daniel Masker
 * @email dtmasker@mtu.edu
 * @date 9/11/2024
 */

#include "vpn_handler.h"

#include <stdio.h>
#include <getopt.h>
#include <string.h>


int main(int argc, char **argv)
{
    if(argc < 2)
    {
        printf("Usage: mtu <command> <args>");
        return 0;
    }

    if(strcmp(argv[1], "vpn") == 0)
    {
        return vpn_handler(argc, argv);
    }
    else
    {
        printf("Unknown option %s\n", argv[1]);
        return 0;
    }


    return 0;
}
