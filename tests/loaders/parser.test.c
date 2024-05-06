#include "loaders/parser.h"
#include "core/dns_packet.h"
#include <stdio.h>
#include <stdlib.h>

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

    return 0;
}


int main(){
    test_class_check();
    test_class_choose();
    test_mx_check();
    test_parse_a_rr();

    return 0;
}
