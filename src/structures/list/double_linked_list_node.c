#include "double_linked_list_node.h"

void double_linked_list_node_init(double_linked_list_node* this, double_linked_list_node* next,double_linked_list_node* prev,void* pointer_data ,char* value)
{
    this->prev = prev;
    this->next = next;
    this->value = value;
    this->pointer_data = pointer_data;
}

void double_linked_list_node_destructor(double_linked_list_node* this)
{
    this->prev = NULL;
    this->next = NULL;
    this->value = NULL;
    this->pointer_data = NULL;
}