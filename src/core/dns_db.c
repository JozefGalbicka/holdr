
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

/*
 * @return boolean whether it was or was not trimmed
 */
static bool trim_one_domain_level(char *domain)
{
    char *end = domain + (strlen(domain) - 1);
    while (end != domain && (*end) != '.') {
        // printf("%c", *end);
        end--;
    }
    // printf("\n");
    if (end == domain) {
        // Nothing found
        return false;
    }
    *end = '\0';
    return true;
}

hash_table *database_search_zone(struct Database *db, const char *domain)
{
    printf("[TRIE] ");
    char *reversed_domain = malloc(strlen(domain) + 1);
    strcpy(reversed_domain, domain);
    reverse_domain(reversed_domain);
    hash_table *found = NULL;

    printf("Searching for zone '%s'", reversed_domain);
    while ((found = search_trie(db->data, reversed_domain)) == NULL) {
        if (!trim_one_domain_level(reversed_domain)) {
            break;
        }
        printf(" -> '%s'", reversed_domain);
    }
    if (found) {
        printf(" (found)\n");
    } else {
        printf(" (NOT FOUND)\n");
    }
    free(reversed_domain);
    return found;
}

struct ResourceRecord *database_search_record(struct Database *db, const char *domain, uint16_t type)
{
    printf("[SEARCH] for record '%s' of type '%d'\n", domain, type);
    char *domain_without_trailing_dot = malloc(strlen(domain) + 1);
    strcpy(domain_without_trailing_dot, domain);
    strip_trailing_dot(domain_without_trailing_dot);
    hash_table *found_zone = database_search_zone(db, domain_without_trailing_dot);
    free(domain_without_trailing_dot);
    struct ResourceRecord *found = NULL;
    if (found_zone) {
        // printf("- Found zone");
        //  Try to find DomainNameDB in HashTable
        printf("[HASHTABLE] ");
        struct DomainNameDB *records = NULL;
        double_linked_list_node *dlln = search_hash_table_record(found_zone, (char *)domain);
        if (dlln) {
            printf("existing DomainNameDB '%s' (found)\n", domain);
            records = (struct DomainNameDB *)dlln->pointer_data;
            if (!records) {
                printf("Unexpected error occurred. Record domain node found but not initialized\n");
                exit(10);
            }
            printf("[DOMAINNAMEDB] ");
            found = records->data[type];
            if (!found) {
                // not found
                printf("RR of type '%d' (NOT FOUND)\n", type);
                return NULL;
            } else {
                printf("RR of type '%d' (found)\n", type);
                return found;
            }
        } else {
            printf("DomainNameDB '%s' (NOT FOUND)\n", domain);
            return NULL;
        }
    } else {
        // not found
        // printf("Zone not found\n");
        return NULL;
    }
    return NULL;
}

/*
 * so about this one - rework it. this is kind of inefficient and redundant. Just rewrite `database_search_record() to return SOA in case of non-found resolution and flag responses through passed argument or smth. but oh well I already wrote this
 */
struct ResourceRecord *database_search_soa(struct Database *db, const char *domain)
{
    struct ResourceRecord *found_rr = NULL;
    printf("[SEARCH] for zone of domain '%s' in order to find SOA record\n", domain);
    printf("[TRIE] ");
    char *reversed_domain = malloc(strlen(domain) + 2);
    strcpy(reversed_domain, domain);
    strip_trailing_dot(reversed_domain);
    reverse_domain(reversed_domain);
    hash_table *found = NULL;

    printf("Searching for zone '%s'", reversed_domain);
    while ((found = search_trie(db->data, reversed_domain)) == NULL) {
        if (!trim_one_domain_level(reversed_domain)) {
            break;
        }
        printf(" -> '%s'", reversed_domain);
    }
    if (found) {
        printf(" (found)\n");
    } else {
        printf(" (NOT FOUND)\n");
    }

    if (found) {
        reverse_domain(reversed_domain);
        reversed_domain[strlen(reversed_domain)+1] = '\0'; // we have to append `\0` first, as removing the existing one would result in breaking the next strlen()
        reversed_domain[strlen(reversed_domain)] = '.';
        printf("[SEARCH] for SOA record of domain name '%s'\n", reversed_domain);
        found_rr = database_search_record(db, reversed_domain, RRType_SOA);
    }

    free(reversed_domain);
    return found_rr;
}
