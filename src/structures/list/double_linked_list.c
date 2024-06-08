
#include "double_linked_list.h"
#define EMPTY 0


void double_linked_list_init(double_linked_list* this)
{
    this->first = NULL;
    this->last = NULL;
    this->size = EMPTY;
}

void double_linked_list_destructor(double_linked_list* this)
{
    if(this == NULL || this->size == EMPTY)
        return;

    double_linked_list_node* prev_node = this->last->prev;
    double_linked_list_node* tmp_last = this->last;

    while(prev_node != NULL)
    {
        double_linked_list_node_destructor(this->last);
        this->last = prev_node;
        this->last->next = NULL;
        prev_node = this->last->prev;
        this->size--;
    }
    double_linked_list_node_destructor(this->first);
    free(this->last);
    this->last = NULL;
    this->first = NULL;
    this->size = EMPTY;
}


void double_linked_list_add(double_linked_list* this, void* data, char* name)
{
    double_linked_list_node *new = (double_linked_list_node *)malloc(sizeof(double_linked_list_node));

    if(this->size == EMPTY)
    {
        double_linked_list_node_init(new,NULL,NULL,data,name);
        this->first = new;
    }
    else
    {
        double_linked_list_node_init(new,NULL,this->last,data,name);
        this->last->next = new;
    }
    this->last = new;
    this->size++;
}

double_linked_list_node* double_linked_list_remove_by_value(double_linked_list* this, char* name)
{
    if(this->size == EMPTY)
        return NULL;

    double_linked_list_node* tmp = this->first;

    while(tmp != NULL)
    {
        if(strcmp(tmp->value,name) == EMPTY)
        {
            if(tmp->prev != NULL)
                tmp->prev->next = tmp->next;
            if(tmp->next != NULL)
                tmp->next->prev = tmp->prev;

            tmp->next = NULL;
            tmp->prev = NULL;

            this->size--;
            return tmp;
        }
        tmp = tmp ->next;
    }

    return NULL;
}

double_linked_list_node* double_linked_list_search_by_value(double_linked_list* this, char* name)
{
    if(this == EMPTY || this->size == EMPTY)
        return NULL;

    double_linked_list_node* tmp = this->first;

    while(tmp != NULL)
    {
        if(strcmp(tmp->value,name) == EMPTY)
            return tmp;

        tmp = tmp ->next;
    }

    return NULL;
}

void double_linked_list_print(double_linked_list* this)
{
    if(this->size == EMPTY)
    {
        printf("Double Linked List is empty\n");
        return;
    }

    double_linked_list_node* tmp = this->first;

    while(tmp != NULL)
    {
        printf("%s\n",tmp->value);
        tmp = tmp ->next;
    }
}
