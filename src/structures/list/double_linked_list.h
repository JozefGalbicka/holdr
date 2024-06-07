#ifndef DOUBLE_LINKED_LIST_H
#define DOUBLE_LINKED_LIST_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "double_linked_list_node.h"

typedef struct double_linked_list
{
    double_linked_list_node* first;
    double_linked_list_node* last;
    size_t size;
}double_linked_list;


void double_linked_list_init(double_linked_list* this);
void double_linked_list_destructor(double_linked_list* this);
void double_linked_list_add(double_linked_list* this, void* data, char* name);
double_linked_list_node* double_linked_list_remove_by_value(double_linked_list* this, char* name);
double_linked_list_node* double_linked_list_search_by_value(double_linked_list* this, char* name);
void double_linked_list_print(double_linked_list* this);

#endif