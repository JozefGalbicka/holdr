#ifndef DOUBLE_LINKED_LIST_NODE_H
#define DOUBLE_LINKED_LIST_NODE_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>


typedef struct double_linked_list_node
{
    struct double_linked_list_node* next;
    struct double_linked_list_node* prev;
    char* value;
    void* pointer_data;   //constant size
}double_linked_list_node;

void double_linked_list_node_init(double_linked_list_node* this, double_linked_list_node* next,double_linked_list_node* prev,void* pointer_data ,char* value);
void double_linked_list_node_destructor(double_linked_list_node* this);

#endif