#include <stdint-gcc.h>
#include <malloc.h>
#include "mersenne_twister.h"
#include "hash_table_node.h"

typedef struct hash_table
{
    uint32_t seed;
    hash_table_node** table;
    size_t table_size;
} hash_table;

void init_hash_table(hash_table* this, size_t size);
void insert_hash_table(hash_table* this, char* value);
hash_table_node* search_hash_table(hash_table* this,char* value);
void destruct_hash_table(hash_table* this);
void delete_hash_table(hash_table* this, char* value); //todo linked list bude sa prerabat