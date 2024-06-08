
#include "core/server.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[])
{

    char cwd[256];
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        printf("Current working dir: %s\n", cwd);
    } else {
        perror("getcwd() error");
        return 1;
    }

    if (argc != 2) {
        printf("Usage: %s <filename>\n", argv[0]);
        return 1;
    }
    char *filename = argv[1];

    printf("The provided config filename is: %s\n", filename);

    struct HoldrConfig *conf = holdrconfig_load(filename);
    if (!conf) {
        printf("Error loading config. Exiting\n");
        return 1;
    }

    holdrconfig_print(conf);

    run_server(conf);

    // unslave the memory
    holdrconfig_destroy(conf);
    free(conf);
    conf = NULL;
}
