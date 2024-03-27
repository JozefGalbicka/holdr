
#include <stdio.h>
#include <stdlib.h>
#include "loaders/config.h"
#include "core/server.h"

int main(int argc, char *argv[])
{
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

    run_server(conf->address, conf->port);
    // unslave the memory
    holdrconfig_destroy(conf);
    free(conf);
    conf = NULL;
}
