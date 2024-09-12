#include "config.h"

#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

/*
 * Helper function to get the next argument from an input file and store
 * it in a variable.
 */
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

/*
 * Helper function to load configuration settings.
 */
int load_config(Config *config)
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
            if ( !get_next_arg(configFile, buffer, &config->mtuId) )
            {
                return -1;
            }
        }
        else if ( strcmp(buffer, "mtu_password") == 0 )
        {
            if ( !get_next_arg(configFile, buffer, &config->mtuPassword) )
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
