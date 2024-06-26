#include "loaders/parser.h"
#include "core/dns_packet.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

int test_class_check() {
    printf("#########################\n");
    printf("### test_class_check()\n");
    printf("#########################\n");

    char * classes[4] = {"CH", "CS", "IN", "HS"};
    for (int i = 0; i < 4; i++) {
        if (class_check(classes[i]) != 0) {
            printf("Class check test failed, aborting\n");
            exit(1);
        }
    }

    if (class_check("INVALID") == 0) {
        printf("Class check test failed, aborting\n");
        exit(1);
    }

    printf("Class check test: OK\n");

    return 0;
}

int test_class_choose() {
    printf("#########################\n");
    printf("### test_class_choose()\n");
    printf("#########################\n");

    if (class_choose("IN") != RRClass_IN) {
        printf("Class choose failed, aborting\n");
        exit(1);
    }

    if (class_choose("INVALID") != -1) {
        printf("Class choose failed, aborting\n");
        exit(1);
    }

    printf("Class choose test: OK\n");

    return 0;
}

int test_mx_check() {
    printf("#########################\n");
    printf("### test_mx_check()\n");
    printf("#########################\n");

    char * vals1[5] = {"A", "B", "C", "MX", "E"};
    char * vals2[5] = {"A", "B", "C", "D", "E"};

    if (mx_check(vals1, 5) != 0) {
        printf("MX check test failed, aborting\n");
        exit(1);
    }

    if (mx_check(vals2, 5) != -1) {
        printf("MX check test failed, aborting\n");
        exit(1);
    }

    printf("MX check test: OK\n");

    return 0;
}

int test_parse_a_rr() {
    printf("#########################\n");
    printf("### test_parse_a_rr()\n");
    printf("#########################\n");

    struct ResourceRecord * rr = calloc(1, sizeof(struct ResourceRecord));
    rr->type = RRType_A;
    char * ip_addr = "192.168.1.1";

    if (parse_a_rr(&rr->rd_data, ip_addr) != 0) {
        printf("Parse A RR test failed, aborting\n");
        exit(1);
    }

    printf("Parse A RR result: %d.%d.%d.%d\n",
           rr->rd_data.a_record.addr[0],
           rr->rd_data.a_record.addr[1],
           rr->rd_data.a_record.addr[2],
           rr->rd_data.a_record.addr[3]);

    if (rr->rd_data.a_record.addr[0] != 192 ||
        rr->rd_data.a_record.addr[1] != 168 ||
        rr->rd_data.a_record.addr[2] != 1 ||
        rr->rd_data.a_record.addr[3] != 1) {
        printf("Parse A RR result address not matching, aborting\n");
        exit(1);
    }

    printf("Parse A RR test: OK\n");
    resource_record_destroy(rr);
    free(rr);
    return 0;
}

int test_parse_aaaa_rr() {
    printf("#########################\n");
    printf("### test_parse_aaaa_rr()\n");
    printf("#########################\n");

    struct ResourceRecord * rr = calloc(1, sizeof(struct ResourceRecord));
    rr->type = RRType_AAAA;
    char * ipv6_addr = "2001:db8:123:456::";

    if (parse_aaaa_rr(&rr->rd_data, ipv6_addr) != 0) {
        printf("Parse AAAA RR test failed, aborting\n");
        exit(1);
    }

    printf("Parse AAAA RR result: %02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x\n",
           rr->rd_data.aaaa_record.addr[0],
           rr->rd_data.aaaa_record.addr[1],
           rr->rd_data.aaaa_record.addr[2],
           rr->rd_data.aaaa_record.addr[3],
           rr->rd_data.aaaa_record.addr[4],
           rr->rd_data.aaaa_record.addr[5],
           rr->rd_data.aaaa_record.addr[6],
           rr->rd_data.aaaa_record.addr[7],
           rr->rd_data.aaaa_record.addr[8],
           rr->rd_data.aaaa_record.addr[9],
           rr->rd_data.aaaa_record.addr[10],
           rr->rd_data.aaaa_record.addr[11],
           rr->rd_data.aaaa_record.addr[12],
           rr->rd_data.aaaa_record.addr[13],
           rr->rd_data.aaaa_record.addr[14],
           rr->rd_data.aaaa_record.addr[15]);

    printf("Parse AAAA RR test: OK\n");
    resource_record_destroy(rr);
    free(rr);
    return 0;
}

int test_parse_txt_rr() {
    printf("#########################\n");
    printf("### test_parse_txt_rr()\n");
    printf("#########################\n");

    struct ResourceRecord * rr = calloc(1, sizeof(struct ResourceRecord));
    rr->type = RRType_TXT;
    char * txt_data = "Example TXT data";

    if (parse_txt_rr(&rr->rd_data, txt_data) != 0) {
        printf("Parse TXT RR test failed, aborting\n");
        exit(1);
    }

    printf("Parse TXT RR result: %s\n", rr->rd_data.txt_record.txt_data);
    printf("Parse TXT RR length result: %d\n", rr->rd_data.txt_record.txt_data_len);

    if (strcmp(rr->rd_data.txt_record.txt_data, txt_data) != 0) {
        printf("Parse TXT RR test failed, text not matching, aborting\n");
        exit(1);
    }

    if (rr->rd_data.txt_record.txt_data_len != strlen(txt_data)) {
        printf("Parse TXT RR test failed, text length not matching, aborting\n");
        exit(1);
    }

    printf("Parse TXT RR test: OK\n");
    resource_record_destroy(rr);
    free(rr);
    return 0;
}

int test_parse_mx_rr() {
    printf("#########################\n");
    printf("### test_parse_mx_rr()\n");
    printf("#########################\n");

    struct ResourceRecord * rr = calloc(1, sizeof(struct ResourceRecord));
    rr->type = RRType_MX;
    char * host = "mail.example.com";
    char * preference = "10";

    if (parse_mx_rr(&rr->rd_data, preference, host) != 0) {
        printf("Parse MX RR test failed, aborting\n");
        exit(1);
    }

    printf("Parse MX RR exchange result: %s\n", rr->rd_data.mx_record.exchange);
    printf("Parse MX RR preference result: %lu\n", rr->rd_data.mx_record.preference);

    if (rr->rd_data.mx_record.preference != 10) {
        printf("Parse MX RR test failed, preference not matching, aborting\n");
        exit(1);
    }

    int vals[19] = {4, 109, 97, 105, 108, 7, 101, 120, 97, 109, 112, 108, 101, 3, 99, 111, 109, 0, 0};
    int aborted = 0;
    printf("Exchange numeric values: ");
    for (int i = 0; i < strlen(host) + 3; i++) {
        if (rr->rd_data.mx_record.exchange[i] != vals[i]) {
            aborted = 1;
        }
        printf("%d ", rr->rd_data.mx_record.exchange[i]);
    }
    printf("\n");

    if (aborted != 0) {
        printf("Parse MX RR test failed, exchange not matching, aborting\n");
        exit(1);
    }

    printf("Parse MX RR test: OK\n");
    resource_record_destroy(rr);
    free(rr);
    return 0;
}

int test_parse_cname_rr() {
    printf("#########################\n");
    printf("### test_parse_cname_rr()\n");
    printf("#########################\n");

    struct ResourceRecord * rr = calloc(1, sizeof(struct ResourceRecord));
    rr->type = RRType_CNAME;
    char * cname = "example.com";

    if (parse_cname_rr(&rr->rd_data, cname) != 0) {
        printf("Parse CNAME RR test failed, aborting\n");
        exit(1);
    }

    printf("Parse CNAME result: %s\n", rr->rd_data.cname_record.cname);

    printf("Parse CNAME RR test: OK\n");
    resource_record_destroy(rr);
    free(rr);
    return 0;
}

int test_parse_ns_rr() {
    printf("#########################\n");
    printf("### test_parse_ns_rr()\n");
    printf("#########################\n");

    struct ResourceRecord * rr = calloc(1, sizeof(struct ResourceRecord));
    rr->type = RRType_NS;
    char * ns = "example.com";

    if (parse_ns_rr(&rr->rd_data, ns) != 0) {
        printf("Parse NS RR test failed, aborting\n");
        exit(1);
    }

    printf("Parse NS result: %s\n", rr->rd_data.ns_record.nsdname);

    printf("Parse NS RR test: OK\n");
    resource_record_destroy(rr);
    free(rr);
    return 0;
}

int test_parse_soa_rr() {
    printf("#########################\n");
    printf("### test_parse_soa_rr()\n");
    printf("#########################\n");

    struct ResourceRecord * rr = calloc(1, sizeof(struct ResourceRecord));
    rr->type = RRType_SOA;

    char * mname = "ns.example.com";
    char * rname = "admin.example.com";
    char * soa_arr[7] = {mname, rname, "1", "2", "3", "4", "5"};

    if (parse_soa_rr(&rr->rd_data, soa_arr) != 0) {
        printf("Parse SOA RR test failed, aborting\n");
        exit(1);
    }

    printf("Parse SOA mname result: %s\n", rr->rd_data.soa_record.mname);
    printf("Parse SOA rname result: %s\n", rr->rd_data.soa_record.rname);
    printf("Parse SOA serial result: %lu\n", rr->rd_data.soa_record.serial);
    printf("Parse SOA refresh result: %lu\n", rr->rd_data.soa_record.refresh);
    printf("Parse SOA retry result: %lu\n", rr->rd_data.soa_record.retry);
    printf("Parse SOA expire result: %lu\n", rr->rd_data.soa_record.expire);
    printf("Parse SOA minimum result: %lu\n", rr->rd_data.soa_record.minimum);

    if (rr->rd_data.soa_record.serial != 1) {
        printf("Parse SOA RR test failed, serial not matching, aborting\n");
        exit(1);
    }

    if (rr->rd_data.soa_record.refresh != 2) {
        printf("Parse SOA RR test failed, refresh not matching, aborting\n");
        exit(1);
    }

    if (rr->rd_data.soa_record.retry!= 3) {
        printf("Parse SOA RR test failed, retry not matching, aborting\n");
        exit(1);
    }

    if (rr->rd_data.soa_record.expire!= 4) {
        printf("Parse SOA RR test failed, expire not matching, aborting\n");
        exit(1);
    }

    if (rr->rd_data.soa_record.minimum != 5) {
        printf("Parse SOA RR test failed, minimum not matching, aborting\n");
        exit(1);
    }

    printf("Parse SOA RR test: OK\n");
    resource_record_destroy(rr);
    free(rr);
    return 0;
}


int test_rr_parse() {
    printf("#########################\n");
    printf("### test_rr_parse()\n");
    printf("#########################\n");

    struct ResourceRecord * rr = calloc(1, sizeof(struct ResourceRecord));
    char * rr_raw = "fsf.org. IN A 192.168.1.1";

    if (rr_parse(rr_raw, rr) != 0) {
        printf("Parse RR test failed, aborting\n");
        exit(1);
    }

    if (rr->rd_data.a_record.addr[0] != 192 ||
        rr->rd_data.a_record.addr[1] != 168 ||
        rr->rd_data.a_record.addr[2] != 1 ||
        rr->rd_data.a_record.addr[3] != 1) {
        printf("Parse RR result address not matching, aborting\n");
        exit(1);
    }

    if (rr->rd_length != sizeof(rr->rd_data.a_record)) {
        printf("Parse RR result RD length not matching, aborting\n");
        exit(1);
    }

    if (strcmp(rr->name, "fsf.org.") != 0) {
        printf("Parse RR result name not matching, aborting\n");
        exit(1);
    }

    if (rr->class != RRClass_IN) {
        printf("Parse RR result class not matching, aborting\n");
        exit(1);
    }

    if (rr->type != RRType_A) {
        printf("Parse RR result type not matching, aborting\n");
        exit(1);
    }

    printf("Parse RR test: OK\n");
    resource_record_destroy(rr);
    free(rr);
    return 0;
}

int test_control_entry_parse() {
    printf("#########################\n");
    printf("### test_control_entry_parse()\n");
    printf("#########################\n");

    char * rr_raw_ttl = "TTL 33";
    char control_entry_val[256];
    if (control_entry_parse(rr_raw_ttl, control_entry_val, "TTL") != 0) {
        printf("Control entry parse test failed, aborting\n");
        exit(1);
    }
    printf("TTL control entry result: %s\n", control_entry_val);
    if (atoi(control_entry_val) != 33) {
        printf("TTL not matching, aborting\n");
        exit(1);
    }

    char * rr_raw_origin = "ORIGIN fsf.org.";
    bzero(control_entry_val, 256);
    if (control_entry_parse(rr_raw_origin, control_entry_val, "ORIGIN") != 0) {
        printf("Control entry parse test failed, aborting\n");
        exit(1);
    }
    printf("Origin control entry result: %s\n", control_entry_val);
    if (strcmp(control_entry_val, "fsf.org.") != 0) {
        printf("Origin not matching, aborting\n");
        exit(1);
    }

    printf("Control entry parse test: OK\n");
    return 0;
}

int test_zone_file_parse() {
    printf("#########################\n");
    printf("### test_zone_file_parse()\n");
    printf("#########################\n");

    char * file_path = "../tests/loaders/zone_file.txt";
    struct ResourceRecord * rr;
    if ((rr = zone_file_parse(file_path)) == NULL) {
        printf("Zone file parse test failed, aborting\n");
        exit(1);
    }

    printf("Zone file parse test result:\n");
    struct ResourceRecord * current_rr = rr;
    struct ResourceRecord *tmp_rr = NULL;
    int index = 1;
    while (current_rr != NULL) {
        printf("%d. resource record:\n", index);
        printf("Name: %s\n", current_rr->name);
        printf("Type: %u\n", current_rr->type);
        printf("Class: %u\n", current_rr->class);
        printf("TTL: %u\n", current_rr->ttl);
        printf("RD length: %u\n", current_rr->rd_length);

        tmp_rr = current_rr->next;
        resource_record_destroy(current_rr);
        free(current_rr);
        current_rr = tmp_rr;
        index++;
    }

    printf("Zone file parse test: OK\n");
    return 0;
}

int main(){
    test_class_check();
    test_class_choose();
    test_mx_check();
    test_parse_a_rr();
    test_parse_aaaa_rr();
    test_parse_txt_rr();
    test_parse_mx_rr();
    test_parse_cname_rr();
    test_parse_ns_rr();
    test_parse_soa_rr();
    test_rr_parse();
    test_control_entry_parse();
    test_zone_file_parse();

    return 0;
}
