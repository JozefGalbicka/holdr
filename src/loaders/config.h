
#include "structures/list/array_list.h"

struct HoldrZone {
    char *domain;
    char *file;
};

void holdrzone_init(struct HoldrZone *zone, const char *domain, const char *file);
void holdrzone_destroy(struct HoldrZone *zone);
void holdrzone_print(struct HoldrZone *zone);

struct HoldrConfig {
    int port;
    char *address;
    ArrayList zones;
};

struct HoldrConfig *holdrconfig_load(const char *filename);
void holdrconfig_print(struct HoldrConfig *conf);
void holdrconfig_destroy(struct HoldrConfig *conf);
