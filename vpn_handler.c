/*
 * Handles vpn commands for the mtu tool
 *
 * @author Daniel Masker
 * @email dtmasker@mtu.edu
 * @date 9/11/2024
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "config.h"


/*
 * Handler for the vpn command.
 */
int vpn_handler(int argc, char **argv, Config *config)
{
    char *buffer = malloc(1024);
    if ( !buffer )
    {
        printf("Failed to malloc buffer.\n");
        return -1;
    }

    if(strcmp(argv[2], "connect") == 0 ||
        strcmp(argv[2], "start") == 0)
    {
        #if _WIN32
        sprintf(buffer, "f5fpc -start /h https://vpn.mtu.edu /u %s /p %s", config->mtuId, config->mtuPassword);
        #endif
        #if unix
        sprintf(buffer, "f5fpc --start -t https://vpn.mtu.edu -x -u %s -p \"%s\"", config->mtuId, config->mtuPassword);
        #endif
        FILE *command = popen(buffer, "r");
        pclose(command);
        return 0;
    }
    else if(strcmp(argv[2], "disconnect") == 0 ||
            strcmp(argv[2], "stop") == 0 ||
            strcmp(argv[2], "close") == 0)
    {
        #if _WIN32
        sprintf(buffer, "f5fpc -info");
        FILE *command = popen(buffer, "r");
        if( !command )
        {
            printf("Failed to open info.\n");
            return -1;
        }

        // Get the 5th word.
        for(int i = 0; i < 5; i++)
        {
            fscanf(command, "%s", buffer);
        }

        if(strcmp("session:", buffer) != 0)
        {
            printf("VPN is not started.\n");
            return -1;
        }

        // Get the 3rd word after confirmation - this is the session id.
        fscanf(command, "%s", buffer);
        fscanf(command, "%s", buffer);
        char smallBuffer[30];
        fscanf(command, "%29s", smallBuffer);
        pclose(command);

        
        sprintf(buffer, "f5fpc -stop /s %s", smallBuffer);
        #endif
        #if unix
        sprintf(buffer, "f5fpc -o");
        #endif
        command = popen(buffer, "r");
        pclose(command);
        return 0;
    }

    return 0;
}
