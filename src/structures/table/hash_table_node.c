#include "hash_table_node.h"

void init_hash_table_node(hash_table_node* this, char* value)
{
    this->value = calloc(strlen(value), sizeof(char));
    if (this->value != NULL)
        strcpy(this->value,value);
    this->next = NULL;
}

void destruct_hash_table_node(hash_table_node* this)
{
    if (this->value != NULL)
    {
        free(this->value);
        this->value = NULL;
    }
}