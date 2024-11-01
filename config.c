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
    char *scanResult = fgets(buffer, 1024, inputFile);
    if ( scanResult == NULL )
    {
        printf("Failed when parsing mtu conf arg.\n");
        return -1;
    }
    // Trim the string.
    int whiteSpaceCount = 0;
    int bufferlen = strnlen(buffer, 1024);
    for(; whiteSpaceCount < bufferlen
            && (buffer[whiteSpaceCount] == ' ' ||
                buffer[whiteSpaceCount] == '\t') ; whiteSpaceCount++);

    for(int i = whiteSpaceCount; i <= bufferlen; i++)
    {
        buffer[i - whiteSpaceCount] = buffer[i];
    }

    if ( buffer[strnlen(buffer, 1024) - 1] == '\n' )
    {
        buffer[strnlen(buffer, 1024) - 1] = 0;
    }

    // Copy the string into the buffer.
    *dest = malloc(strlen(buffer) + 1);
    if( !*dest )
    {
        printf("Failed to malloc space for mtu conf arg.\n");
        return -1;
    }
    strcpy(*dest, buffer);
    return 0;
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
