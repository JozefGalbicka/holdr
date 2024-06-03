
#include "hash_table.h"

void init_hash_table(hash_table* this, size_t size)
{
    this->seed = extract_mt();
    this->table_size = size;
    this->table = calloc(this->table_size,sizeof(hash_table_node*));
}

void insert_hash_table(hash_table* this, char* value)
{
    uint32_t hash = murmur3_32((const uint8_t *)value,strlen(value),this->seed);
    hash_table_node *tmp_node = malloc(sizeof(hash_table_node));
    init_hash_table_node(tmp_node, value);

    if (this->table[hash%this->table_size] == NULL)
    {
        this->table[hash % this->table_size] = tmp_node;
    }
    else
    {
        hash_table_node *current = this->table[hash%this->table_size];
        while(current->next != NULL)
            current = current->next;
        current->next = tmp_node;
    }
}

hash_table_node* search_hash_table(hash_table* this,char* value)
{
    hash_table_node *tmp = this->table[(murmur3_32((const uint8_t *)value,strlen(value),this->seed))%this->table_size];
    while(value != tmp->value)
    {
        tmp = tmp->next;
    }

    return tmp;
}

void destruct_hash_table(hash_table* this)
{
    if(this->table != NULL)
    {
        for (int i = 0; i < this->table_size; i++)
        {
            hash_table_node *tmp = this->table[i];
                while (tmp != NULL)
                {
                    this->table[i] = tmp->next;
                    destruct_hash_table_node(tmp);
                    tmp = this->table[i];
                }
            this->table[i] = NULL;
        }

        free(this->table);
        this->table = NULL;
    }
}
