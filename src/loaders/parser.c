#include "loaders/parser.h"
#include "core/dns_packet.h"
#include "utils/file_reader.h"
#include "utils/strings.h"
#include <string.h>
#include <strings.h>
#include <stdio.h>
#include <stdlib.h>

int class_check(char * class_val) {
    if (strcmp(class_val, "CH") == 0 ||
        strcmp(class_val, "CS") == 0 ||
        strcmp(class_val, "IN") == 0 ||
        strcmp(class_val, "HS") == 0) {
        return 0;
    }
    return -1;
}

int type_check(char * type_val) {
    if (strcmp(type_val, "A") == 0 ||
        strcmp(type_val, "AAAA") == 0 ||
        strcmp(type_val, "SOA") == 0 ||
        strcmp(type_val, "TXT") == 0 ||
        strcmp(type_val, "PTR") == 0 ||
        strcmp(type_val, "MX") == 0) {
        return 0;
    }
    return -1;
}


int class_choose(char * class_val) {
    if (strcmp(class_val, "CH") == 0) {
        return RRClass_CH;
    }
    if (strcmp(class_val, "IN") == 0) {
        return RRClass_IN;
    }
    if (strcmp(class_val, "CS") == 0) {
        return RRClass_CS;
    }
    if (strcmp(class_val, "HS") == 0) {
        return RRClass_HS;
    }
    return -1;
}

int mx_check(char ** values, int values_count) {
    for (int i = 0; i < values_count; i++) {
        if (strcmp(values[i], "MX") == 0) {
            return 0;
        }
    }
    return -1;
}

int parse_a_rr(union ResourceData * rd, char * ip_addr) {
    char * delimeter = ".";
    int split_count;
    char ** ip_addr_split = split_str(ip_addr, delimeter, &split_count);

    if (split_count != 4) {
        printf("Bad IP address: %s\n", ip_addr);
        for (int i = 0; i < split_count; i++) {
            free(ip_addr_split[i]);
        }
        free(ip_addr_split);
        return -1;
    }

    for (int i = 0; i < split_count; i++) {
        uint8_t octet = (uint8_t)strtoul(ip_addr_split[i], NULL, 10);
        rd->a_record.addr[i] = octet;
    }

    for (int i = 0; i < split_count; i++) {
        free(ip_addr_split[i]);
    }
    free(ip_addr_split);
    return 0;
}

int parse_aaaa_rr(union ResourceData * rd, char * ipv6_addr) {
    char * delimeter = ":";
    int split_count;
    char ** ipv6_addr_split = split_str(ipv6_addr, delimeter, &split_count);

    if (split_count > 8) {
        printf("Bad IP address: %s.\n", ipv6_addr);
        for (int i = 0; i < split_count; i++) {
            free(ipv6_addr_split[i]);
        }
        free(ipv6_addr_split);
        return -1;
    }

    char last_char = ipv6_addr[strlen(ipv6_addr) - 1];

    for (int i = 0; i < 8; i++) {
        if ((last_char != ':' && i < split_count - 1) ||
            (last_char == ':' && i < split_count)) {
            uint16_t group = (uint16_t)strtoul(ipv6_addr_split[i], NULL, 16);
            uint8_t byte1 = (uint8_t)(group >> 8);
            uint8_t byte2 = (uint8_t)(group & 0xFF);

            // add group
            rd->aaaa_record.addr[i * 2] = byte1;
            rd->aaaa_record.addr[i * 2 + 1] = byte2;
        } else if (i != 7 || (i == 7 && last_char == ':')) {
            // add blank group
            rd->aaaa_record.addr[i * 2] = 0;
            rd->aaaa_record.addr[i * 2 + 1] = 0;
        } else {
            uint16_t group = (uint16_t)strtoul(ipv6_addr_split[split_count - 1], NULL, 16);
            uint8_t byte1 = (uint8_t)(group >> 8);
            uint8_t byte2 = (uint8_t)(group & 0xFF);

            // add last group
            rd->aaaa_record.addr[i * 2] = byte1;
            rd->aaaa_record.addr[i * 2 + 1] = byte2;
        }
    }

    for (int i = 0; i < split_count; i++) {
        free(ipv6_addr_split[i]);
    }
    free(ipv6_addr_split);
    return 0;
}

int parse_txt_rr(union ResourceData * rd, char * txt_data) {
    rd->txt_record.txt_data = calloc(1, strlen(txt_data) + 1);
    strcpy(rd->txt_record.txt_data, txt_data);
    rd->txt_record.txt_data_len = strlen(txt_data);
    return 0;
}

int parse_mx_rr(union ResourceData * rd, char * preference, char * host) {
    rd->mx_record.exchange = calloc(1, strlen(host) + 3);
    encode_domain_name((uint8_t **)&rd->mx_record.exchange, host, false);
    uint16_t preference_cast = (uint16_t)strtoul(preference, NULL, 10);
    rd->mx_record.preference = preference_cast;
    return 0;
}

int parse_cname_rr(union ResourceData * rd, char * cname) {
    rd->cname_record.cname = calloc(1, strlen(cname) + 3);
    encode_domain_name((uint8_t **)&rd->cname_record.cname, cname, false);
    return 0;
}

int parse_ns_rr(union ResourceData * rd, char * ns) {
    rd->ns_record.nsdname = calloc(1, strlen(ns) + 3);
    encode_domain_name((uint8_t **)&rd->ns_record.nsdname, ns, false);
    return 0;
}

int parse_soa_rr(union ResourceData * rd, char ** data) {
    rd->soa_record.mname = calloc(1, strlen(data[0]) + 3);
    encode_domain_name((uint8_t **)&rd->soa_record.mname, data[0], false);
    rd->soa_record.rname= calloc(1, strlen(data[1]) + 3);
    encode_domain_name((uint8_t **)&rd->soa_record.rname, data[1], false);

    rd->soa_record.serial = strtoul(data[2], NULL, 10);
    rd->soa_record.refresh = strtoul(data[3], NULL, 10);
    rd->soa_record.retry = strtoul(data[4], NULL, 10);
    rd->soa_record.expire = strtoul(data[5], NULL, 10);
    rd->soa_record.minimum = strtoul(data[6], NULL, 10);
    return 0;
}

int rr_parse(char * rr_raw, struct ResourceRecord * rr) {
    char * delimeter = " \t()";
    int split_count;
    char ** rr_split = split_str(rr_raw, delimeter, &split_count);

    // skip invalid lines
    if (split_count < 3) {
        for (int i = 0; i < split_count; i++) {
            free(rr_split[i]);
        }
        free(rr_split);
        return -1;
    }

    int rr_index = 0;
    // name
    if (class_check(rr_split[rr_index]) == 0 ||
        type_check(rr_split[rr_index]) == 0) {
        rr->name = NULL;
    } else {
        rr->name = calloc(strlen(rr_split[rr_index]) + 1, sizeof(char));
        strcpy(rr->name, rr_split[rr_index]);
        rr_index++;
    }

    // class
    if (class_check(rr_split[rr_index]) == 0) {
        rr->class = class_choose(rr_split[rr_index]);
        rr_index++;
    } else {
        rr->class = RRClass_IN;
    }

    // type
    if (strcmp(rr_split[rr_index], "A") == 0) {
        rr->type = RRType_A;
        //parse A
        if (parse_a_rr(&rr->rd_data, rr_split[++rr_index]) != 0) {
            return -1;
        }
        rr->rd_length = sizeof(rr->rd_data.a_record);
    } else if (strcmp(rr_split[rr_index], "AAAA") == 0) {
        rr->type = RRType_AAAA;
        //parse AAAA
        if (parse_aaaa_rr(&rr->rd_data, rr_split[++rr_index]) != 0) {
            return -1;
        }
        rr->rd_length = sizeof(rr->rd_data.aaaa_record);
    } else if (strcmp(rr_split[rr_index], "TXT") == 0) {
        rr->type = RRType_TXT;
        //parse TXT
        if (parse_txt_rr(&rr->rd_data, rr_split[++rr_index]) != 0) {
            return -1;
        }
        rr->rd_length = sizeof(rr->rd_data.txt_record);
    } else if (strcmp(rr_split[rr_index], "MX") == 0) {
        rr->type = RRType_MX;
        //parse MX
        int preference_index = ++rr_index;
        int exchange_index = ++rr_index;
        if (parse_mx_rr(&rr->rd_data, rr_split[preference_index], rr_split[exchange_index]) != 0) {
            return -1;
        }
        rr->rd_length = sizeof(rr->rd_data.mx_record);
    } else if (strcmp(rr_split[rr_index], "CNAME") == 0) {
        rr->type = RRType_CNAME;
        //parse CNAME
        if (parse_cname_rr(&rr->rd_data, rr_split[++rr_index]) != 0) {
            return -1;
        }
        rr->rd_length = sizeof(rr->rd_data.cname_record);
    } else if (strcmp(rr_split[rr_index], "NS") == 0) {
        rr->type = RRType_NS;
        //parse NS
        if (parse_ns_rr(&rr->rd_data, rr_split[++rr_index]) != 0) {
            return -1;
        }
        rr->rd_length = sizeof(rr->rd_data.ns_record);
    } else if (strcmp(rr_split[rr_index], "SOA") == 0) {
        //parse SOA
        rr->type = RRType_SOA;
        rr_index++;
        if (parse_soa_rr(&rr->rd_data, rr_split + rr_index) != 0) {
            return -1;
        }
        rr->rd_length = sizeof(rr->rd_data.soa_record);

    } else {
        printf("This RR type is not supported: %s.\n", rr_split[rr_index]);
        return -1;
    }

    for (int i = 0; i < split_count; i++) {
        free(rr_split[i]);
    }
    free(rr_split);

    return 0;
}

int control_entry_parse(char * rr_raw, char * value_out, const char * entry_name) {
    char * delimeter = " \t";
    int split_count;
    char ** rr_split = split_str(rr_raw, delimeter, &split_count);
    int exit_value = 0;

    // skip if not desired control entry
    if (strcmp(rr_split[0], entry_name) != 0) {
        exit_value = -1;
        //return -1;
    }
    // skip if without value
    if (split_count < 2) {
        exit_value = -1;
        //return -1;
    }

    if (exit_value == 0) {
        strcpy(value_out, rr_split[1]);
    }

    for (int i = 0; i < split_count; i++) {
        free(rr_split[i]);
    }
    free(rr_split);
    return exit_value;
}

struct ResourceRecord * zone_file_parse(char * filename) {
    size_t size;
    char * file_content;
    if ((file_content = read_plaintext_file(filename, &size)) == NULL) {
        return NULL;
    }

    // split file by newlines
    char * delimeter = "\n";
    char ** lines;
    int lines_count;
    lines = split_str(file_content, delimeter, &lines_count);
    free(file_content);

    struct ResourceRecord * rr_first = NULL;
    struct ResourceRecord * rr_current = NULL;

    int ttl = 0;
    char origin[256] = {0};
    char last_used_name[256] = {0};

    for (int i = 0; i < lines_count; i++) {
        char * line = trim(lines[i]);
        // skip unnecessary lines
        if (line[0] == ';' || line[0] == '\n' || line[0] == '\r') {
            continue;
        }

        //parse origin and ttl if present
        if (line[0] == '$') {
            char entry_value[256] = {0};
            char * ttl_entry = "$TTL";
            if (control_entry_parse(line, entry_value, ttl_entry) == 0) {
                ttl = atoi(entry_value);
                continue;
            }
            char * origin_entry = "$ORIGIN";
            if (control_entry_parse(line, entry_value, origin_entry) == 0) {
                strcpy(origin, entry_value);
            }
            continue;
        }

        struct ResourceRecord * rr = calloc(1, sizeof(struct ResourceRecord));
        if (rr_parse(line, rr) != 0) {
            free(rr);
            continue;
        }

        // set previous name if not present
        if (rr->name == NULL && last_used_name != NULL) {
            rr->name = calloc(strlen(last_used_name) + 1, sizeof(char));
            strcpy(rr->name, last_used_name);
        } else if (rr->name != NULL) { // set for later if present
            bzero(last_used_name, 256);
            strcpy(last_used_name, rr->name);
        }

        // add origin
        if (strcmp(rr->name, "@") == 0 && origin != NULL) { // replace @ by origin
            free(rr->name);
            rr->name = calloc(strlen(origin) + 1, sizeof(char));
            strcpy(rr->name, origin);
        } else if (rr->name[strlen(rr->name) - 1] != '.') { // append origin
            size_t new_size = strlen(rr->name) + strlen(origin) + 2;
            char * new_name = calloc(new_size, sizeof(char));
            memcpy(new_name, rr->name, strlen(rr->name));
            memcpy(new_name + strlen(rr->name), ".", 1);
            memcpy(new_name + strlen(rr->name) + 1, origin, strlen(origin));
            memcpy(new_name + new_size - 1, "\0", 1);

            free(rr->name);
            rr->name = new_name;
        }
        // add ttl
        rr->ttl = ttl;

        // set rr chain
        if (rr_first == NULL) {
            rr_first = rr;
            rr_current = rr;
        } else {
            rr_current->next = rr;
            rr_current = rr;
        }
    }

    for (int i = 0; i < lines_count; i++) {
        free(lines[i]);
    }
    free(lines);

    return rr_first;
}
