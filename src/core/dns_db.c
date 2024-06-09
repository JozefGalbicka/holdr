
#include "dns_db.h"
#include "core/dns_packet.h"
#include "loaders/parser.h"
#include "structures/list/double_linked_list.h"
#include "structures/table/hash_table.h"
#include "structures/tree/trie.h"
#include "utils/strings.h"
#include <stdlib.h>
#include <string.h>

bool database_load_zones(struct Database *db, struct HoldrConfig *conf)
{
    // Init Trie
    db->data = malloc(sizeof(trie));
    init_trie(db->data);
    hash_table *zone = NULL;

    // To iterate through all the zones in the config
    struct HoldrZone *cur_zone = NULL;
    ArrayListIterator it;
    arraylist_iterator_init(&it, &conf->zones);
    // Iterate through all the zones in the config
    while (arraylist_iterator_has_next(&it)) {
        void *tmp = arraylist_iterator_move_next(&it);
        cur_zone = (struct HoldrZone *)tmp;
        printf("Loading zone: '%s' from file '%s'\n", cur_zone->domain, cur_zone->file);

        // Reverse domain string and store it in Trie
        char *reversed_domain = malloc(strlen(cur_zone->domain) + 1);
        strcpy(reversed_domain, cur_zone->domain);
        reverse_domain(reversed_domain);
        printf("Inserting zone into trie: '%s'\n", reversed_domain);
        zone = insert_trie(db->data, reversed_domain);
        free(reversed_domain);

        // DEBUG
        // print_trie(db->data);

        // Parse zone file
        struct ResourceRecord *rr;
        if ((rr = zone_file_parse(cur_zone->file)) == NULL) {
            printf("Zone file parse test failed, aborting\n");
            return false; // error
        }
        printf("Zone file parse result:\n");
        resource_record_print(rr);

        struct ResourceRecord *current_rr = rr;
        struct ResourceRecord *tmp_rr = NULL;
        struct ResourceRecord *found = NULL;
        int index = 1;
        // Iterate through all the RRs in the zonefile
        while (current_rr != NULL) {
            tmp_rr = current_rr->next;
            current_rr->next = NULL;

            printf("Inserting RR into HashTable: '%s'", current_rr->name);

            // Try to find DomainNameDB in HashTable
            struct DomainNameDB *records = NULL;
            double_linked_list_node *dlln = search_hash_table_record(zone, current_rr->name);
            if (dlln) {
                printf(" - existing DomainNameDB found");
                records = (struct DomainNameDB *)dlln->pointer_data;
                if (!records) {
                    printf("Unexpected error occurred. Record domain node found but not initialized\n");
                    exit(10);
                }
            } else {
                printf(" - Creating new DomainNameDB");
                records = calloc(1, sizeof(struct DomainNameDB));
                insert_hash_table(zone, records, current_rr->name);
            }

            found = resource_record_get_last(records->data[current_rr->type]);
            if (!found) {
                printf(" (First occurrence of this type - %d)\n", current_rr->type);
                records->data[current_rr->type] = current_rr;
            } else {
                printf(" (Inserting to existing chain of this type - %d)\n", current_rr->type);
                found->next = current_rr;
            }

            // relocate away
            // resource_record_destroy(current_rr);
            // free(current_rr);

            current_rr = tmp_rr;
            index++;
        }
    }
    arraylist_iterator_destroy(&it);

    // testing
    // database_search_zone(db, "mindme.aslkj.example.com");
    // database_search_zone(db, "mindme.aslkj.exampasdfklle.com");
    // database_search_zone(db, "mindme.aslkj.exampasdfklle.sdalkfcom");

    // relocate away
    // destruct_trie(db->data);
    // free(db->data);

    return true;
}

void database_destroy(struct Database *db, struct HoldrConfig *conf)
{
    hash_table *zone = NULL;
    // To iterate through all the zones in the config
    struct HoldrZone *cur_zone = NULL;
    ArrayListIterator it;
    arraylist_iterator_init(&it, &conf->zones);
    // Iterate through all the zones in the config
    while (arraylist_iterator_has_next(&it)) {
        void *tmp = arraylist_iterator_move_next(&it);
        cur_zone = (struct HoldrZone *)tmp;

        // Reverse domain string and store it in Trie
        char *reversed_domain = malloc(strlen(cur_zone->domain) + 1);
        strcpy(reversed_domain, cur_zone->domain);
        reverse_domain(reversed_domain);
        zone = search_trie(db->data, reversed_domain);
        free(reversed_domain);

        double_linked_list_node *node;
        struct DomainNameDB *dndb = NULL;
        struct ResourceRecord *rr;
        for (int i = 0; i < zone->table_size; i++) {
            if (zone->table[i]) {
                node = zone->table[i]->first;
                if (node) {
                    while (true) {
                        dndb = (struct DomainNameDB *)node->pointer_data;
                        if (dndb) {
                            for (int j = 0; j < RRTypeMax + 1; j++) {
                                rr = dndb->data[j];
                                if (rr) {
                                    resource_record_destroy_chain(rr);
                                }
                            }
                            free(dndb);
                        }

                        if (node == zone->table[i]->last) {
                            break;
                        }
                        node++;
                    }
                }
            }
        }
        destruct_hash_table(zone);
    }
    arraylist_iterator_destroy(&it);
    destruct_trie(db->data);
    free(db->data);
}

