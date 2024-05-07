
#ifndef PARSER_H
#define PARSER_H

#include "core/dns_packet.h"

int class_check(char *class_val);
int type_check(char * type_val);
int class_choose(char *class_val);
int mx_check(char **values, int values_count);

int parse_a_rr(union ResourceData * rd, char *ip_addr);
int parse_aaaa_rr(union ResourceData * rd, char *ipv6_addr);
int parse_txt_rr(union ResourceData * rd, char * txt_data);
int rr_parse(char *rr_raw, struct ResourceRecord *rr);

int control_entry_parse(char * rr_raw, char * value_out, const char * entry_name);
int origin_parse(char * rr_raw, char * origin_out);
struct ResourceRecord * zone_file_parse(char * filename);

#endif
