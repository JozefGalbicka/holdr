
#ifndef DNS_PACKET_H
#define DNS_PACKET_H

#include <stdbool.h>
#include <stdint.h> // for uints
#include <stdio.h>
#include "core/dns_db.h"

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
    RRType_SRV = 33,
    RRTypeMax = RRType_SRV
};

/* Resource Record Classes */
enum {
    RRClass_IN = 1,
    RRClass_CS = 2,
    RRClass_CH = 3,
    RRClass_HS = 4
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
    // ResponseCode_YXDOMAIN = 6,
    // ResponseCode_XRRSET = 7,
    // ResponseCode_NOTAUTH = 8,
    // ResponseCode_NOTZONE = 9
};

/* Question Section */
struct Question {
    char *qName; // plaintext domain (with dots)
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

    struct {
        uint8_t addr[4];
    } a_record;
    struct {
        uint8_t addr[16];
    } aaaa_record;
    struct {
        char *cname; // labeled
    } cname_record;
    struct {
        uint16_t preference;
        char *exchange; // labeled
    } mx_record;
    struct {
        char *nsdname; // labeled
    } ns_record;
    struct {
        char *mname; // labeled
        char *rname; // labeled
        uint32_t serial;
        uint32_t refresh;
        uint32_t retry;
        uint32_t expire;
        uint32_t minimum;
    } soa_record;
    struct {
        uint8_t txt_data_len;
        char *txt_data;
    } txt_record;
};

/* Resource Record Section */
struct ResourceRecord {
    char *name; // plaintext domain (with dots)
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

    uint16_t qd_count; /* Question Count */
    uint16_t an_count; /* Answer Record Count */
    uint16_t ns_count; /* Authority Record Count */
    uint16_t ar_count; /* Additional Record Count */

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

void resource_record_print(struct ResourceRecord *rr);
void resource_record_destroy(struct ResourceRecord *rd);
void resource_record_destroy_chain(struct ResourceRecord *rr);
size_t resource_record_count_chain(struct ResourceRecord *rr);
struct ResourceRecord *resource_record_get_last(struct ResourceRecord *rr);
void message_print(struct Message *msg);
uint16_t get16bits(const uint8_t **buffer);
void put8bits(uint8_t **buffer, uint8_t value);
void put16bits(uint8_t **buffer, uint16_t value);
void put32bits(uint8_t **buffer, uint32_t value);

void message_destroy(struct Message *msg);
static void message_decode_header(struct Message *msg, const uint8_t **buffer);
void message_encode_header(struct Message *msg, uint8_t **buffer);

char *decode_domain_name(const uint8_t **buf, size_t len, bool move_buffer);
void encode_domain_name(uint8_t **buffer, const char *domain, bool move_buffer);

bool message_decode(struct Message *msg, const uint8_t *buffer, size_t size);
bool message_encode(struct Message *msg, uint8_t **buffer);

bool resource_record_encode_chain(struct ResourceRecord *rr, uint8_t **buffer);
void message_resolve_query(struct Message *msg, struct Database *db);

#endif
