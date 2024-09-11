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

char *mtuId;
char *mtuPassword;

int get_next_arg(FILE *inputFile, char *buffer, char **dest)
{
    int scanResult = fscanf(inputFile, "%1023s", buffer);
    if ( scanResult == 0 || scanResult == EOF )
    {
        printf("Failed when parsing mtu conf arg.\n");
        return -1;
    }
    *dest = malloc(strlen(buffer) + 1);
    if( !*dest )
    {
        printf("Failed to malloc space for mtu conf arg.\n");
        return -1;
    }
    strcpy(*dest, buffer);
}

int load_config()
{
    const char *home_dir = getenv("USERPROFILE");
    if(!home_dir)
    {
        printf("No home dir variable set.\n");
        return -1;
    }

    char *buffer = malloc(1024);
    int bufferLength = strlen(home_dir);
    int bufferSize = 1024;
    if ( !buffer )
    {
        printf("Buffer failed to malloc!\n");
        return -1;
    }

    strcpy(buffer, home_dir);
    strcpy(&buffer[bufferLength], "\\.mtu\\config");
    bufferLength += 12;

    FILE *configFile;

    configFile = fopen(buffer, "r");

    int scanResult = fscanf(configFile, "%1023s", buffer);

    while ( scanResult != 0 && scanResult != EOF )
    {
        if ( strcmp(buffer, "mtu_id") == 0 )
        {
            if ( !get_next_arg(configFile, buffer, &mtuId) )
            {
                return -1;
            }
        }
        else if ( strcmp(buffer, "mtu_password") == 0 )
        {
            if ( !get_next_arg(configFile, buffer, &mtuPassword) )
            {
                return -1;
            }
        }

        scanResult = fscanf(configFile, "%1023s", buffer);
    }

    fclose(configFile);
    free(buffer);

    return 0;
}

int vpn_handler(int argc, char **argv)
{
    if(load_config())
    {
        return -1;
    }

    char *buffer = malloc(1024);
    if ( !buffer )
    {
        printf("Failed to malloc buffer.\n");
        return -1;
    }

    if(strcmp(argv[2], "connect") == 0 ||
        strcmp(argv[2], "start") == 0)
    {
        sprintf(buffer, "f5fpc -start /h https://vpn.mtu.edu /u %s /p %s", mtuId, mtuPassword);
        FILE *command = popen(buffer, "r");
        pclose(command);
        return 0;
    }
    else if(strcmp(argv[2], "disconnect") == 0 ||
            strcmp(argv[2], "stop") == 0 ||
            strcmp(argv[2], "close") == 0)
    {
        sprintf(buffer, "f5fpc -info");
        FILE *command = popen(buffer, "r");
        if( !command )
        {
            printf("Failed to open info.\n");
            return -1;
        }

        
        fscanf(command, "%s", buffer);
        fscanf(command, "%s", buffer);
        fscanf(command, "%s", buffer);
        fscanf(command, "%s", buffer);
        fscanf(command, "%s", buffer);

        if(strcmp("session:", buffer) != 0)
        {
            printf("VPN is not started.\n");
            return -1;
        }

        
        fscanf(command, "%s", buffer);
        fscanf(command, "%s", buffer);
        char smallBuffer[30];
        fscanf(command, "%29s", smallBuffer);
        pclose(command);

        sprintf(buffer, "f5fpc -stop /s %s", smallBuffer);
        command = popen(buffer, "r");
        pclose(command);
        return 0;
    }



    return 0;
}
