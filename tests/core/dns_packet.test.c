
#include "core/dns_packet.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static char *test_encode_domain_name_unit(const char *domain)
{
    printf("Processing domain:\n");
    printf("|%s|\n", domain);

    int len = strlen(domain) + 3;
    char *buffer = malloc(len);
    memset(buffer, 1, len); // fill buffer to see if encode function is not manipulating parts of memory it shouldn't
    encode_domain_name((uint8_t **)&buffer, domain, false);

    uint8_t count_left = 0;      // chars left to read in label
    bool print_overflow = false; // print rest of unused buffer space
    for (int i = 0; i < len; i++) {
        if (count_left == 0 || print_overflow) {
            count_left = buffer[i];
            if (count_left == 0) { // found end of domain
                print_overflow = true;
            }
            printf("%d", count_left);
            continue;
        }
        printf("%c", buffer[i]);
        count_left--;
    }
    printf("\n");

    return buffer;
}

static void test_encode_domain_name()
{
    char domain[100];
    char *encoded = NULL;
    int len;
    int count;

    printf("\n1\n");
    strcpy(domain, "sub.example.com"); // without trailing dot
    len = strlen(domain) + 3;
    count = 0;
    encoded = test_encode_domain_name_unit(domain);
    for (int i = 0; i < len; i++) {
        if (encoded[i] == 1) {
            count++;
        }
    }
    printf("Count: %d\n", count);
    if (count != 1) { // rest of the buffer was left intact
        exit(1);
    }
    free(encoded);

    strcpy(domain, "sub.example.com."); // with trailing dot
    len = strlen(domain) + 3;
    count = 0;
    encoded = test_encode_domain_name_unit(domain);
    for (int i = 0; i < len; i++) {
        if (encoded[i] == 1) {
            count++;
        }
    }
    printf("Count: %d\n", count);
    if (count != 2) { // rest of the buffer was left intact
        exit(1);
    }
    free(encoded);
}

int main(void)
{
    printf("\n");
    test_encode_domain_name();
    return 0;
}
