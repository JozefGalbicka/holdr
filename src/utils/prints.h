#include <stddef.h>
#include <stdio.h>

void print_string(void *item, FILE *f);
void print_int(void *item, FILE *f);

int cmp_int(const void *itemA, const void *itemB);
int cmp_string(const void *itemA, const void *itemB);

void print_bytes(void *data, size_t size);
void print_bits(unsigned char data);
unsigned char get_first_byte(void *data);
