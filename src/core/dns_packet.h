
#ifndef DNS_PACKET_H
#define DNS_PACKET_H

#include <stdbool.h>
#include <stdint.h> // for uints
#include <stdio.h>

/* Resource Record Types */
enum {
    RRType_A = 1,
    RRType_NS = 2,
    RRType_CNAME = 5,
    RRType_SOA = 6,
    RRType_PTR = 12,
    RRType_MX = 15,
    RRType_TXT = 16,
    RRType_AAAA = 28,
    RRType_SRV = 33
};

/* Operation Code */
enum {
    OperationCode_QUERY = 0,  /* standard query */
    OperationCode_IQUERY = 1, /* inverse query */
    OperationCode_STATUS = 2, /* server status request */
    OperationCode_NOTIFY = 4, /* request zone transfer */
    OperationCode_UPDATE = 5  /* change resource records */
};

/* Response Code */
enum {
    ResponseCode_NOERROR = 0,
    ResponseCode_FORMERR = 1,
    ResponseCode_SERVFAIL = 2,
    ResponseCode_NXDOMAIN = 3,
    ResponseCode_NOTIMP = 4,
    ResponseCode_REFUSED = 5,
    //ResponseCode_YXDOMAIN = 6,
    //ResponseCode_XRRSET = 7,
    //ResponseCode_NOTAUTH = 8,
    //ResponseCode_NOTZONE = 9 
};

/* Question Section */
struct Question {
    char *qName;
    uint16_t qType;
    uint16_t qClass;
    struct Question *next; // for next question (if present)
};

/* Data part of a Resource Record */
union ResourceData {
    // domain-name
    // CNAME
    // NS + additional
    // PTR
    //
    // character-string
    // TXT (one or more <character-strings>)
    //
    // uint16_t + domain_name
    // MX + additional
    //
    // 4x uint8_t
    // A
    //
    // 8x2x uint8_t
    //
    // Custom
    // SOA
    // HINFO
    // SRV
    struct { //
        uint8_t txt_data_len;
        char *txt_data;
    } txt_record;
    struct { //
        uint8_t addr[4];
    } a_record;
    struct { //
        uint8_t addr[16];
    } aaaa_record;
};

/* Resource Record Section */
struct ResourceRecord {
    char *name;
    uint16_t type;
    uint16_t class;
    uint32_t ttl;
    uint16_t rd_length;
    union ResourceData rd_data;
    struct ResourceRecord *next; // for next RR (if present)
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
    uint16_t anCount; /* Answer Record Count */
    uint16_t nsCount; /* Authority Record Count */
    uint16_t arCount; /* Additional Record Count */

    /* At least one question; questions are copied to the response 1:1 */
    struct Question *questions;

    /*
     * Resource records to be send back.
     * Every resource record can be in any of the following places.
     * But every place has a different semantic.
     */
    struct ResourceRecord *answers;
    struct ResourceRecord *authorities;
    struct ResourceRecord *additionals;
};

void print_resource_record(struct ResourceRecord *rr);
void print_message(struct Message *msg);
uint16_t get16bits(const uint8_t **buffer);

void decode_header(struct Message *msg, const uint8_t **buffer);
char *decode_domain_name(const uint8_t **buf, size_t len);
bool decode_msg(struct Message *msg, const uint8_t *buffer, size_t size);

#endif
