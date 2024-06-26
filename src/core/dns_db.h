
#ifndef DNS_DB_H
#define DNS_DB_H

#include "loaders/config.h"
#include "structures/table/hash_table.h"
#include "structures/tree/red_black_tree.h"
#include "structures/tree/trie.h"

// hash_table *my;
struct Database {
    trie *data;
};

struct DomainNameDB {
    struct ResourceRecord *data[50]; // TODO change to RRTypeMax
};

bool database_load_zones(struct Database *db, struct HoldrConfig *conf);
void database_destroy(struct Database *db, struct HoldrConfig *conf);

hash_table *database_search_zone(struct Database *db, const char *domain);
struct ResourceRecord *database_search_record(struct Database *db, const char *domain, uint16_t type);
struct ResourceRecord *database_search_soa(struct Database *db, const char *domain);

#endif
