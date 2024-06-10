
#include "core/dns_db.h"
#include "loaders/config.h"
#include "core/dns_packet.h"
#include <stdio.h>

static void found_ptr_add(void *ptr, size_t *count)
{
    if(ptr) {
        (*count)++;
    }
}
void test_database_search_record()
{
    struct HoldrConfig *conf = calloc(1, sizeof(struct HoldrConfig));
    conf->address = NULL;
    arraylist_init(&conf->zones, sizeof(struct HoldrZone));
    struct HoldrZone holdr_zone;
    holdrzone_init(&holdr_zone, "example.com", "core/example-com.db");
    arraylist_add(&conf->zones, &holdr_zone);

    struct Database *db = calloc(1, sizeof(struct Database));
    bool loaded = database_load_zones(db, conf);
    if (!loaded) {
        printf("File could not be loaded\n");
        exit(1);
    }

    struct ResourceRecord *found_rr;
    size_t found_count = 0;
    found_rr = database_search_record(db, "ftp.example.com.", RRType_SRV);
    found_ptr_add(found_rr, &found_count);
    found_rr = database_search_record(db, "bruh.example.com.", RRType_A);
    found_ptr_add(found_rr, &found_count);
    found_rr = database_search_record(db, "ftp.example.com.", RRType_AAAA);
    found_ptr_add(found_rr, &found_count);
    found_rr = database_search_record(db, "xample.com.", RRType_A);
    found_ptr_add(found_rr, &found_count);

    if (found_count != 0) {
        printf("Found records that aren't there!\n");
        exit(1);
    }

    found_rr = database_search_record(db, "example.com.", RRType_SOA);
    found_ptr_add(found_rr, &found_count);
    found_rr = database_search_record(db, "www.example.com.", RRType_AAAA);
    found_ptr_add(found_rr, &found_count);
    found_rr = database_search_record(db, "www.example.com.", RRType_A);
    found_ptr_add(found_rr, &found_count);
    found_rr = database_search_record(db, "ftp.example.com.", RRType_A);
    found_ptr_add(found_rr, &found_count);
    found_rr = database_search_record(db, "txt.example.com.", RRType_TXT);
    found_ptr_add(found_rr, &found_count);
    found_rr = database_search_record(db, "txt.example.com.", RRType_CNAME);
    found_ptr_add(found_rr, &found_count);
    found_rr = database_search_record(db, "example.com.", RRType_MX);
    found_ptr_add(found_rr, &found_count);
    found_rr = database_search_record(db, "wtf.example.com.", RRType_NS);
    found_ptr_add(found_rr, &found_count);

    if (found_count != 8) {
        printf("Found only %zu out of %d records!\n", found_count, 8);
        exit(1);
    }

    database_destroy(db, conf);
    free(db);

    holdrconfig_destroy(conf);
    free(conf);
    return;
}
int main(void)
{
    printf("\n");
    test_database_search_record();
    return 0;
}
