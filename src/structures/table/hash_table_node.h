#include <malloc.h>
#include "murmur_hash.h"


typedef struct hash_table_node
{
    char* value;
    struct hash_table_node* next;
} hash_table_node;

void init_hash_table_node(hash_table_node* this, char* value);
void destruct_hash_table_node(hash_table_node* this);
