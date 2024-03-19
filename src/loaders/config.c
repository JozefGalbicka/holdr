
#include "config.h"

#include "tomlc/toml.h"
#include <errno.h>
#include <stdlib.h>
#include <string.h>

void holdrzone_init(struct HoldrZone *zone, const char *domain, const char *file)
{
    zone->domain = strdup(domain);
    zone->file = strdup(file);
}

void holdrzone_destroy(struct HoldrZone *zone)
{
    free(zone->domain);
    free(zone->file);
}

void holdrzone_print(struct HoldrZone *zone)
{
    printf("Domain: %s\n", zone->domain);
    printf("File: %s\n", zone->file);
}

static void holdrconfig_init(struct HoldrConfig *conf)
{
    conf->address = NULL;
    arraylist_init(&conf->zones, sizeof(struct HoldrZone));
}

void holdrconfig_destroy(struct HoldrConfig *conf)
{
    if (conf->address) {
        free(conf->address);
        conf->address = NULL;
    }

    ArrayListIterator it;
    arraylist_iterator_init(&it, &conf->zones);
    while (arraylist_iterator_has_next(&it)) {
        void *tmp = arraylist_iterator_move_next(&it);
        holdrzone_destroy((struct HoldrZone *)tmp);
    }
    arraylist_iterator_destroy(&it);

    arraylist_destroy(&conf->zones);
}

struct HoldrConfig *holdrconfig_load(const char *filename)
{

    // Read and parse toml file
    FILE *f;
    char errbuf[200];
    f = fopen(filename, "r");
    if (!f) {
        fprintf(stderr, "Cannot open `%s` - %s\n", filename, strerror(errno));
        return NULL;
    }

    toml_table_t *toml_conf = toml_parse_file(f, errbuf, sizeof(errbuf));
    fclose(f);

    if (!toml_conf) {
        fprintf(stderr, "Cannot parse config - %s\n", errbuf);
        toml_free(toml_conf);
        return NULL;
    }
    //

    // create holdr_conf
    struct HoldrConfig *holdr_conf = malloc(sizeof(struct HoldrConfig));
    holdrconfig_init(holdr_conf);
    //

    // parse table "server"
    toml_table_t *server = toml_table_in(toml_conf, "server");
    if (!server) {
        fprintf(stderr, "missing [server] table\n");
        goto error_cleanup;
    }
    //

    // parse values in table "server"
    toml_datum_t address = toml_string_in(server, "address");
    if (!address.ok) {
        fprintf(stderr, "cannot read server.address\n");
        free(address.u.s);
        goto error_cleanup;
    }
    holdr_conf->address = strdup(address.u.s);
    free(address.u.s);

    toml_datum_t port = toml_int_in(server, "port");
    if (!port.ok) {
        fprintf(stderr, "cannot read server.port\n");
        goto error_cleanup;
    }
    holdr_conf->port = port.u.i;
    //

    toml_array_t *zones_array = toml_array_in(toml_conf, "zones");
    if (!zones_array) {
        fprintf(stderr, "cannot read zones array\n");
        goto error_cleanup;
    }

    int zones_count = toml_array_nelem(zones_array);
    // printf("Found %d zones to laod\n", zones_count);

    struct HoldrZone holdr_zone;
    for (int i = 0; i < zones_count; i++) {
        toml_table_t *zone = toml_table_at(zones_array, i);

        // parse values in table "server"
        toml_datum_t domain = toml_string_in(zone, "domain");
        if (!domain.ok) {
            fprintf(stderr, "cannot read zone.domain\n");
            free(domain.u.s);
            goto error_cleanup;
        }
        toml_datum_t file = toml_string_in(zone, "file");
        if (!file.ok) {
            fprintf(stderr, "cannot read zone.file\n");
            free(domain.u.s);
            free(file.u.s);
            goto error_cleanup;
        }
        holdrzone_init(&holdr_zone, domain.u.s, file.u.s);
        arraylist_add(&holdr_conf->zones, &holdr_zone);
        // printf("Domain: %s\n", domain.u.s);
        // printf("File: %s\n", file.u.s);
        free(domain.u.s);
        free(file.u.s);
    }
    printf("\n");

    toml_free(toml_conf);
    goto out;

error_cleanup:
    toml_free(toml_conf);
    holdrconfig_destroy(holdr_conf);
    free(holdr_conf);
    holdr_conf = NULL;
out:
    return holdr_conf;
}

void holdrconfig_print(struct HoldrConfig *conf)
{
    printf("=== Holdr config ===\n");
    printf("Port: %d\n", conf->port);
    printf("Address: %s\n", conf->address);

    printf("\n--- ZONES ---");
    ArrayListIterator it;
    arraylist_iterator_init(&it, &conf->zones);
    while (arraylist_iterator_has_next(&it)) {
        printf("\n");
        void *tmp = arraylist_iterator_move_next(&it);
        holdrzone_print((struct HoldrZone *)tmp);
    }
    arraylist_iterator_destroy(&it);
    printf("-------------\n");

    printf("====================\n");
}
