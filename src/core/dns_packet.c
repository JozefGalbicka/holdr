
#include "core/dns_packet.h"
#include "utils/prints.h"

#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>

#define MIN(x, y) ((x) <= (y) ? (x) : (y))


/*
 * HEADER - Masks and constants.
 */
static const uint16_t QR_MASK     = 0x8000;  // 1000 0000 0000 0000
static const uint16_t OPCODE_MASK = 0x7800;  // 0111 1000 0000 0000
static const uint16_t AA_MASK     = 0x0400;  // 0000 0100 0000 0000
static const uint16_t TC_MASK     = 0x0200;  // 0000 0010 0000 0000
static const uint16_t RD_MASK     = 0x0100;  // 0000 0001 0000 0000
static const uint16_t RA_MASK     = 0x0080;  // 0000 0000 1000 0000
static const uint16_t RCODE_MASK  = 0x000F;  // 0000 0000 0000 1111


void print_resource_record(struct ResourceRecord *rr)
{
  int i;
  while (rr) {
    printf("  ResourceRecord { name '%s', type %u, class %u, ttl %u, rd_length %u, ",
      rr->name,
      rr->type,
      rr->class,
      rr->ttl,
      rr->rd_length
   );

    union ResourceData *rd = &rr->rd_data;
    switch (rr->type) {
      case RRType_A:
        printf("Address Resource Record { address ");

        for (i = 0; i < 4; i += 1)
          printf("%s%u", (i ? "." : ""), rd->a_record.addr[i]);

        printf(" }");
        break;
      case RRType_AAAA:
        printf("AAAA Resource Record { address ");

        for (i = 0; i < 16; i += 1)
          printf("%s%02x", (i ? ":" : ""), rd->aaaa_record.addr[i]);

        printf(" }");
        break;
      case RRType_TXT:
        printf("Text Resource Record { txt_data '%s' }",
          rd->txt_record.txt_data
        );
        break;
      default:
        printf("Unknown Resource Record { ??? }");
    }
    printf("}\n");
    rr = rr->next;
  }
}

void print_message(struct Message *msg)
{
  struct Question *q;

  //printf("QUERY { ID: %02x", msg->id); // hex print
  printf("QUERY { ID(%02d)", msg->id); // dec print
  //printf(". FIELDS: [ QR: %u, OpCode: %u ]", msg->qr, msg->opcode);
  printf(" FIELDS[ ");
  printf("qr(%d) ", msg->qr);
  printf("opcode(%d) ", msg->opcode);
  printf("aa(%d) ", msg->aa);
  printf("tc(%d) ", msg->tc);
  printf("rd(%d) ", msg->rd);
  printf("ra(%d) ", msg->ra);
  printf("rcode(%d)", msg->rcode);
  printf(" ]");

  printf(", QDcount: %u", msg->qdCount);
  printf(", ANcount: %u", msg->anCount);
  printf(", NScount: %u", msg->nsCount);
  printf(", ARcount: %u,\n", msg->arCount);

  

  q = msg->questions;
  while (q) {
    printf("  Question { qName '%s', qType %u, qClass %u }\n",
      q->qName,
      q->qType,
      q->qClass
    );
    q = q->next;
  }

  print_resource_record(msg->answers);
  print_resource_record(msg->authorities);
  print_resource_record(msg->additionals);

  printf("}\n");
}

uint16_t get16bits(const uint8_t **buffer)
{
  uint16_t value;

  memcpy(&value, *buffer, 2);
  *buffer += 2;

  return ntohs(value);
}

void decode_header(struct Message *msg, const uint8_t **buffer)
{
  msg->id = get16bits(buffer);
  uint16_t fields = get16bits(buffer); // we have to convert to host because of '&' operation
  //print_bytes(&fields, 2);
  //print_bits((uint8_t *)&QR_MASK, 2);
  msg->qr = (fields & QR_MASK) >> 15;
  msg->opcode = (fields & OPCODE_MASK) >> 11;
  msg->aa = (fields & AA_MASK) >> 10;
  msg->tc = (fields & TC_MASK) >> 9;
  msg->rd = (fields & RD_MASK) >> 8;
  msg->ra = (fields & RA_MASK) >> 7;
  msg->rcode = (fields & RCODE_MASK) >> 0;

  msg->qdCount = get16bits(buffer);
  msg->anCount = get16bits(buffer);
  msg->nsCount = get16bits(buffer);
  msg->arCount = get16bits(buffer);
}

char *decode_domain_name(const uint8_t **buf, size_t len)
{
  char domain[256];
  for (int i = 1; i < MIN(256, len); i += 1) {
    uint8_t c = (*buf)[i];
    if (c == 0) {
      domain[i - 1] = 0;
      *buf += i + 1;
      return strdup(domain);
    } else if ((c >= 'a' && c <= 'z') || c == '-') {
      domain[i - 1] = c;
    } else {
      domain[i - 1] = '.';
    }
  }

  return NULL;
}

bool decode_msg(struct Message *msg, const uint8_t *buffer, size_t size)
{
  int i;

  decode_header(msg, &buffer);

  if (msg->anCount != 0 || msg->nsCount != 0) {
    printf("Only questions expected!\n");
    return false;
  }

  // read questions
  for (i = 0; i < msg->qdCount; i += 1) {
    struct Question *q = malloc(sizeof(struct Question));

    q->qName = decode_domain_name(&buffer, size);
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

