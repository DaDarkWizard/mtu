#ifndef CONFIG_H
#define CONFIG_H

#include <stdio.h>

// Structure to hold settings.
struct _Config {
    char *mtuId;
    char *mtuPassword;
};

typedef struct _Config Config;

int get_next_arg(FILE *inputFile, char *buffer, char **dest);

int load_config(Config *config);

#endif
