
#ifndef DNS_PACKET_H
#define DNS_PACKET_H

#include <stdio.h>
#include <stdbool.h>
#include <stdint.h> // for uints


/* Question Section */
struct Question {
    char *qName;
    uint16_t qType;
    uint16_t qClass;
    struct Question *next; // for next question (if present)
};

struct Message {
    uint16_t id; /* Identifier */

    /* Flags */
    uint16_t qr;     /* Query/Response Flag */
    uint16_t opcode; /* Operation Code */
    uint16_t aa;     /* Authoritative Answer Flag */
    uint16_t tc;     /* Truncation Flag */
    uint16_t rd;     /* Recursion Desired */
    uint16_t ra;     /* Recursion Available */
    uint16_t rcode;  /* Response Code */

    uint16_t qdCount; /* Question Count */
    uint16_t anCount;  /* Answer Record Count */
    uint16_t nsCount;  /* Authority Record Count */
    uint16_t arCount;  /* Additional Record Count */

    /* At least one question; questions are copied to the response 1:1 */
    struct Question *questions;

};

void print_message(struct Message *msg);
uint16_t get16bits(const uint8_t **buffer);

void decode_header(struct Message *msg, const uint8_t **buffer);
char *decode_domain_name(const uint8_t **buf, size_t len);
bool decode_msg(struct Message *msg, const uint8_t *buffer, size_t size);

#endif
