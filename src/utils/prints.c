#include "prints.h"
#include <stdio.h>
#include <string.h>

int cmp_int(const void *itemA, const void *itemB)
{
    return *(int *)itemA > *(int *)itemB;
}

int cmp_string(const void *itemA, const void *itemB)
{
    char *tmpA = *((char **)itemA);
    char *tmpB = *((char **)itemB);
    return strcmp(tmpA, tmpB);
}

void print_int(void *item, FILE *f)
{
    fprintf(f, "%d ", *(int *)item);
}

void print_string(void *item, FILE *f)
{
    fprintf(f, "%s \n", *(char **)item);
}

void print_bytes(void *data, size_t size)
{
    for (int i = 0; i < size; ++i) {
        print_bits(*((unsigned char *)data + i));
        printf(" ");
    }
    printf("\n");
}

void print_bits(unsigned char byte)
{
    for (int i = 0; i < 8; ++i) {
        printf("%d", (byte >> (7 - i)) & 1);
    }
}

unsigned char get_first_byte(void *data)
{
    return *((unsigned char *)data);
}
