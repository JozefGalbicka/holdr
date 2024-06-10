
#include "core/dns_packet.h"

#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>

#define MIN(x, y) ((x) <= (y) ? (x) : (y))

/*
 * HEADER - Masks and constants.
 */
static const uint16_t QR_MASK = 0x8000;     // 1000 0000 0000 0000
static const uint16_t OPCODE_MASK = 0x7800; // 0111 1000 0000 0000
static const uint16_t AA_MASK = 0x0400;     // 0000 0100 0000 0000
static const uint16_t TC_MASK = 0x0200;     // 0000 0010 0000 0000
static const uint16_t RD_MASK = 0x0100;     // 0000 0001 0000 0000
static const uint16_t RA_MASK = 0x0080;     // 0000 0000 1000 0000
static const uint16_t RCODE_MASK = 0x000F;  // 0000 0000 0000 1111
                                            //
void resource_record_destroy_chain(struct ResourceRecord *rr)
{
    struct ResourceRecord *rrd = rr;
    struct ResourceRecord *tmp = NULL;
    while (rrd != NULL) {
        tmp = rrd->next;
        resource_record_destroy(rrd);
        free(rrd);

        rrd = tmp;
    }
}
void resource_record_destroy(struct ResourceRecord *rr)
{
    free(rr->name);
    // int i;
    // while (rr) {
    union ResourceData *rd = &rr->rd_data;
    switch (rr->type) {
    case RRType_A:
        break;
    case RRType_NS:
        free(rd->ns_record.nsdname);
        break;
    case RRType_CNAME:
        free(rd->cname_record.cname);
        break;
    case RRType_SOA:
        free(rd->soa_record.mname);
        free(rd->soa_record.rname);
        break;
    // case RRType_PTR:
    //     break;
    case RRType_MX:
        free(rd->mx_record.exchange);
        break;
    case RRType_TXT:
        free(rd->txt_record.txt_data);
        break;
    case RRType_AAAA:
        break;
    // case RRType_SRV:
    //     break;
    default:
        printf("Unknown Resource Record { %d }. Cannot free the memory.", rr->type);
    }
}
size_t resource_record_count_chain(struct ResourceRecord *rr)
{
    struct ResourceRecord *tmp = rr;
    size_t count = 0;
    while (tmp != NULL) {
        tmp = tmp->next;
        count++;
    }
    return count;
}

struct ResourceRecord *resource_record_get_last(struct ResourceRecord *rr)
{
    if (!rr) {
        return rr;
    }
    struct ResourceRecord *tmp = rr;
    while (tmp->next != NULL) {
        tmp = tmp->next;
    }
    return tmp;
}

void resource_record_print(struct ResourceRecord *rr)
{
    int i;
    char *decoded_str = NULL;
    while (rr) {
        printf("  ResourceRecord { name '%s', type %u, class %u, ttl %u, rd_length %u, ", rr->name, rr->type, rr->class,
               rr->ttl, rr->rd_length);

        union ResourceData *rd = &rr->rd_data;
        switch (rr->type) {
        case RRType_A:
            printf("Address Resource Record { address ");

            for (i = 0; i < 4; i += 1)
                printf("%s%u", (i ? "." : ""), rd->a_record.addr[i]);

            printf(" }");
            break;
        case RRType_NS:
            printf(" Record { ");

            decoded_str =
                decode_domain_name((const uint8_t **)&rd->ns_record.nsdname, strlen(rd->ns_record.nsdname) + 1, false);
            printf("nsdname: %s", decoded_str);
            free(decoded_str);

            printf(" }");
            break;
        case RRType_CNAME:
            printf(" Record { ");

            decoded_str = decode_domain_name((const uint8_t **)&rd->cname_record.cname,
                                             strlen(rd->cname_record.cname) + 1, false);
            printf("cname: %s", decoded_str);
            free(decoded_str);

            printf(" }");
            break;
        case RRType_SOA:
            printf("SOA Record { ");

            decoded_str =
                decode_domain_name((const uint8_t **)&rd->soa_record.mname, strlen(rd->soa_record.mname) + 1, false);
            printf("mname: %s ", decoded_str);
            free(decoded_str);

            decoded_str =
                decode_domain_name((const uint8_t **)&rd->soa_record.rname, strlen(rd->soa_record.rname) + 1, false);
            printf("rname: %s ", decoded_str);
            free(decoded_str);
            printf("serial: %u ", rd->soa_record.serial);
            printf("refresh: %u ", rd->soa_record.refresh);
            printf("retry: %u ", rd->soa_record.refresh);
            printf("expire: %u ", rd->soa_record.expire);
            printf("minimum: %u", rd->soa_record.minimum);

            printf(" }");
            break;
            // case RRType_PTR:
            //     break;
        case RRType_MX:
            printf(" Record { ");

            printf("preference: %hu ", rd->mx_record.preference);
            decoded_str = decode_domain_name((const uint8_t **)&rd->mx_record.exchange,
                                             strlen(rd->mx_record.exchange) + 1, false);
            printf("exchange: %s", decoded_str);
            free(decoded_str);

            printf(" }");
            break;
        case RRType_TXT:
            printf("Text Resource Record { txt_data '%s', txt_data_len '%d' }", rd->txt_record.txt_data,
                   rd->txt_record.txt_data_len);
            break;
        case RRType_AAAA:
            printf("AAAA Resource Record { address ");

            for (i = 0; i < 16; i += 1)
                printf("%s%02x", (i ? ":" : ""), rd->aaaa_record.addr[i]);

            printf(" }");
            break;
            // case RRType_SRV:
            //     break;
        default:
            printf("Unknown Resource Record { ??? }");
        }
        printf("}\n");
        rr = rr->next;
    }
}

void message_print(struct Message *msg)
{
    struct Question *q;

    // printf("QUERY { ID: %02x", msg->id); // hex print
    printf("QUERY { ID(%02d)", msg->id); // dec print
    // printf(". FIELDS: [ QR: %u, OpCode: %u ]", msg->qr, msg->opcode);
    printf(" FIELDS[ ");
    printf("qr(%d) ", msg->qr);
    printf("opcode(%d) ", msg->opcode);
    printf("aa(%d) ", msg->aa);
    printf("tc(%d) ", msg->tc);
    printf("rd(%d) ", msg->rd);
    printf("ra(%d) ", msg->ra);
    printf("rcode(%d)", msg->rcode);
    printf(" ]");

    printf(", QDcount: %u", msg->qd_count);
    printf(", ANcount: %u", msg->an_count);
    printf(", NScount: %u", msg->ns_count);
    printf(", ARcount: %u,\n", msg->ar_count);

    q = msg->questions;
    while (q) {
        printf("  Question { qName '%s', qType %u, qClass %u }\n", q->qName, q->qType, q->qClass);
        q = q->next;
    }

    resource_record_print(msg->answers);
    resource_record_print(msg->authorities);
    resource_record_print(msg->additionals);

    printf("}\n");
}

uint16_t get16bits(const uint8_t **buffer)
{
    uint16_t value;

    memcpy(&value, *buffer, 2);
    *buffer += 2;

    return ntohs(value);
}

void put8bits(uint8_t **buffer, uint8_t value)
{
    memcpy(*buffer, &value, 1);
    *buffer += 1;
}

void put16bits(uint8_t **buffer, uint16_t value)
{
    value = htons(value);
    memcpy(*buffer, &value, 2);
    *buffer += 2;
}

void put32bits(uint8_t **buffer, uint32_t value)
{
    value = htonl(value);
    memcpy(*buffer, &value, 4);
    *buffer += 4;
}

static void message_decode_header(struct Message *msg, const uint8_t **buffer)
{
    msg->id = get16bits(buffer);
    uint16_t fields = get16bits(buffer); // we have to convert to host because of '&' operation
    // print_bytes(&fields, 2);
    // print_bits((uint8_t *)&QR_MASK, 2);
    msg->qr = (fields & QR_MASK) >> 15;
    msg->opcode = (fields & OPCODE_MASK) >> 11;
    msg->aa = (fields & AA_MASK) >> 10;
    msg->tc = (fields & TC_MASK) >> 9;
    msg->rd = (fields & RD_MASK) >> 8;
    msg->ra = (fields & RA_MASK) >> 7;
    msg->rcode = (fields & RCODE_MASK) >> 0;

    msg->qd_count = get16bits(buffer);
    msg->an_count = get16bits(buffer);
    msg->ns_count = get16bits(buffer);
    msg->ar_count = get16bits(buffer);
}

char *decode_domain_name(const uint8_t **buf, size_t len, bool move_buffer)
{
    char domain[256];
    for (int i = 1; i < MIN(256, len); i += 1) {
        uint8_t c = (*buf)[i];
        if (c == 0) {
            domain[i - 1] = '.';
            domain[i] = 0;
            if (move_buffer) {
                *buf += i + 1;
            }
            return strdup(domain);
        } else if ((c >= 'a' && c <= 'z') || c == '-') {
            domain[i - 1] = c;
        } else {
            domain[i - 1] = '.';
        }
    }

    return NULL;
}

// sub.example.com  => 3sub7example3com0
// sub.example.com. => 3sub7example3com0
// (trailing dot in provided `domain` is optional, works either way)
void encode_domain_name(uint8_t **buffer, const char *domain, bool move_buffer)
{
    uint8_t *buf = *buffer;
    const char *beg = domain; // always points to start of every label
    const char *pos;          // used to find position of delimiters '.'
    int len = 0;              // length of every label
    int i = 0;                // size in bytes written into `buf`

    while ((pos = strchr(beg, '.'))) {
        len = pos - beg;
        buf[i] = len;
        i += 1;
        memcpy(buf + i, beg, len);
        i += len;

        beg = pos + 1; // first character after `.` (dot)
    }

    len = strlen(domain) - (beg - domain); // length - (processed length)

    buf[i] = len; // last label length
    i += 1;
    if (len != 0) { // if the supllied domain doesn't end with dot ('.'), then add final label and force-append '0'
        memcpy(buf + i, beg, len);
        i += len;

        buf[i] = 0;
        i += 1;
    }

    if (move_buffer) {
        *buffer += i;
    }
}

bool message_decode(struct Message *msg, const uint8_t *buffer, size_t size)
{
    int i;

    message_decode_header(msg, &buffer);

    if (msg->an_count != 0 || msg->ns_count != 0) {
        printf("Only questions expected!\n");
        return false;
    }

    // read questions
    for (i = 0; i < msg->qd_count; i += 1) {
        struct Question *q = malloc(sizeof(struct Question));

        q->qName = decode_domain_name(&buffer, size, true);
        q->qType = get16bits(&buffer);
        q->qClass = get16bits(&buffer);

        if (q->qName == NULL) {
            printf("Failed to decode domain name in question %d!\n", i);
            return false;
        }

        // prepend question to questions list
        q->next = msg->questions;
        msg->questions = q;
    }

    // No resources expected to be parsed (Except for 'additional' section, but that's an issue for roadmap)

    return true;
}

bool resource_record_encode_chain(struct ResourceRecord *rr, uint8_t **buffer)
{
    int i;

    while (rr) {
        // Answer questions by attaching resource sections.
        encode_domain_name(buffer, rr->name, true);
        put16bits(buffer, rr->type);
        put16bits(buffer, rr->class);
        put32bits(buffer, rr->ttl);
        put16bits(buffer, rr->rd_length);

        switch (rr->type) {
        case RRType_A:
            for (i = 0; i < 4; i += 1)
                put8bits(buffer, rr->rd_data.a_record.addr[i]);
            break;
        case RRType_NS:
            for (i = 0; i < strlen(rr->rd_data.ns_record.nsdname) + 1; i += 1)
                put8bits(buffer, rr->rd_data.ns_record.nsdname[i]);
            break;
        case RRType_CNAME:
            for (i = 0; i < strlen(rr->rd_data.cname_record.cname) + 1; i += 1)
                put8bits(buffer, rr->rd_data.cname_record.cname[i]);
            break;
        case RRType_SOA:
            for (i = 0; i < strlen(rr->rd_data.soa_record.mname) + 1; i += 1)
                put8bits(buffer, rr->rd_data.soa_record.mname[i]);
            for (i = 0; i < strlen(rr->rd_data.soa_record.rname) + 1; i += 1)
                put8bits(buffer, rr->rd_data.soa_record.rname[i]);
            put32bits(buffer, rr->rd_data.soa_record.serial);
            put32bits(buffer, rr->rd_data.soa_record.refresh);
            put32bits(buffer, rr->rd_data.soa_record.retry);
            put32bits(buffer, rr->rd_data.soa_record.expire);
            put32bits(buffer, rr->rd_data.soa_record.minimum);
            break;
        // case RRType_PTR:
        //     break;
        case RRType_MX:
            put16bits(buffer, rr->rd_data.mx_record.preference);
            for (i = 0; i < strlen(rr->rd_data.mx_record.exchange) + 1; i++) {
                put8bits(buffer, rr->rd_data.mx_record.exchange[i]);
            }
            break;
        case RRType_TXT:
            put8bits(buffer, rr->rd_data.txt_record.txt_data_len);
            for (i = 0; i < rr->rd_data.txt_record.txt_data_len; i++)
                put8bits(buffer, rr->rd_data.txt_record.txt_data[i]);
            break;
        case RRType_AAAA:
            for (i = 0; i < 16; i += 1)
                put8bits(buffer, rr->rd_data.aaaa_record.addr[i]);
            break;
        // case RRType_SRV:
        //     break;
        default:
            fprintf(stderr, "Unknown type %u. => Ignore resource record.\n", rr->type);
            return false;
        }

        rr = rr->next;
    }

    return true;
}

void message_encode_header(struct Message *msg, uint8_t **buffer)
{
    put16bits(buffer, msg->id);

    int fields = 0;
    fields |= (msg->qr << 15) & QR_MASK;
    fields |= (msg->rcode << 0) & RCODE_MASK;
    // TODO: insert the rest of the fields
    put16bits(buffer, fields);

    put16bits(buffer, msg->qd_count);
    put16bits(buffer, msg->an_count);
    put16bits(buffer, msg->ns_count);
    put16bits(buffer, msg->ar_count);
}

bool message_encode(struct Message *msg, uint8_t **buffer)
{
    message_encode_header(msg, buffer);

    struct Question *q = msg->questions;
    while (q) {
        encode_domain_name(buffer, q->qName, true);
        put16bits(buffer, q->qType);
        put16bits(buffer, q->qClass);

        q = q->next;
    }

    if (!resource_record_encode_chain(msg->answers, buffer)) {
        return false;
    }

    if (!resource_record_encode_chain(msg->authorities, buffer)) {
        return false;
    }

    if (!resource_record_encode_chain(msg->additionals, buffer)) {
        return false;
    }

    return true;
}

void message_resolve_query(struct Message *msg, struct Database *db)
{
    struct ResourceRecord *rr;
    struct Question *q;

    msg->qr = 1; // message is a response
    msg->aa = 1; // responding server is authoritative
    msg->ra = 0; // no recursion available
    msg->rcode = ResponseCode_NOERROR;

    // just checking - should be 0 already (from the question)
    msg->an_count = 0;
    msg->ns_count = 0;
    msg->ar_count = 0;

    // NOTE: in general, there should be only one question - so we are processing only the first one
    q = msg->questions;

    rr = database_search_record(db, msg->questions->qName, msg->questions->qType);

    if (!rr) {
        msg->rcode = ResponseCode_NXDOMAIN;
        printf("RR not found, responding with NXDOMAIN\n");
    }

    msg->an_count += resource_record_count_chain(rr);
    msg->answers = rr;
}
