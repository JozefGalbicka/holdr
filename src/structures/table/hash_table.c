
#include "hash_table.h"

void init_hash_table(hash_table* this, size_t size)
{
    this->seed = extract_mt();
    this->table_size = size;
    this->table = calloc(this->table_size,sizeof(double_linked_list*));
}

void insert_hash_table(hash_table* this, void* data_pointer, char* value)
{
    uint32_t hash = murmur3_32((const uint8_t *)value,strlen(value),this->seed);

    int index = hash%this->table_size;

    if (this->table[index] == NULL)
    {
        this->table[index] = malloc(sizeof(double_linked_list));
    }

    if(double_linked_list_search_by_value(this->table[index],value) != NULL)
    {
        printf("\nDuplicity!!\n");
        return;
    }

    double_linked_list_add(this->table[index],data_pointer,value);

}

double_linked_list_node* search_hash_table_record(hash_table* this,char* value)
{
    uint32_t hash = murmur3_32((const uint8_t *)value,strlen(value),this->seed);
    return double_linked_list_search_by_value(this->table[hash % this->table_size],value);
}

double_linked_list_node* delete_hash_table_record(hash_table* this, char* value)
{
    uint32_t hash = murmur3_32((const uint8_t *)value,strlen(value),this->seed);
    double_linked_list_node* del= double_linked_list_remove_by_value(this->table[hash % this->table_size],value);

    if(this->table[hash % this->table_size]->size == 0)
    {
        free(this->table[hash % this->table_size]);
        this->table[hash % this->table_size] =NULL;
    }

    return del;
}

void destruct_hash_table(hash_table* this)
{
    if(this->table != NULL)
    {
        for (int i = 0; i < this->table_size; i++)
        {
            if(this->table[i] != NULL)
            {
                double_linked_list_destructor(this->table[i]);
                free(this->table[i]);
                this->table[i] = NULL;
            }
        }
        free(this->table);
        this->table = NULL;
    }
}

void print_hash_table(hash_table* this)
{
    if(this->table == NULL)
        return;

    int record = 1;
    for (int i = 0; i < this->table_size; i++)
    {
        if(this->table[i] != NULL)
        {
            printf("Record %d:\n", record);
            double_linked_list_print(this->table[i]);
            record++;
        }
    }

}