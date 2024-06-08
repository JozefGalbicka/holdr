#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include <malloc.h>
#include "mersenne_twister.h"
#include "murmur_hash.h"
#include "structures/list/double_linked_list.h"

typedef struct hash_table
{
    uint32_t seed;
    double_linked_list** table;
    size_t table_size;
} hash_table;

void init_hash_table(hash_table* this, size_t size);
void insert_hash_table(hash_table* this, void* data_pointer, char* value);
double_linked_list_node* delete_hash_table_record(hash_table* this, char* value);
double_linked_list_node* search_hash_table_record(hash_table* this,char* value);
void destruct_hash_table(hash_table* this);
void print_hash_table(hash_table* this);

#endif
