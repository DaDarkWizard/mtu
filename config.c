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
    printf("Scanned text: '%s'\n", scanResult);
    if ( scanResult == NULL )
    {
        printf("Failed when parsing mtu conf arg.\n");
        return -1;
    }
    printf("found arg: %s\n", scanResult);
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
    #ifdef _WIN32
    const char *home_dir = getenv("USERPROFILE");
    if(!home_dir)
    {
        printf("No home dir variable set.\n");
        return -1;
    }
    #endif

    #ifdef unix

    const char *username = getlogin();
    char home_dir[strlen("/home/") + strlen(username) + 1];
    strcpy(home_dir, "/home/");
    strcpy(strlen(home_dir) + home_dir, username);
    
    #endif

    char *buffer = malloc(1024);
    int bufferLength = strlen(home_dir);
    int bufferSize = 1024;
    if ( !buffer )
    {
        printf("Buffer failed to malloc!\n");
        return -1;
    }

    strcpy(buffer, home_dir);
    #ifdef _WIN
    strcpy(&buffer[bufferLength], "\\.mtu\\config");
    #endif
    #ifdef unix
    strcpy(&buffer[bufferLength], "/.mtu/config");
    #endif
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
        else
        {
            printf("Unknown arg: '%s'\n", buffer);
        }

        scanResult = fscanf(configFile, "%1023s", buffer);
    }

    fclose(configFile);
    free(buffer);

    return 0;
}
