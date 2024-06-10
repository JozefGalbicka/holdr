#include "structures/list/double_linked_list.h"
#include "structures/list/double_linked_list_node.h"
#include "utils/prints.h"
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

static double_linked_list* setup_basic_list()
{
    // Data1 -> Data2 -> Data3
    double_linked_list* akt = (double_linked_list*)malloc(sizeof(double_linked_list));
    double_linked_list_init(akt);
    return akt;
}

void test_double_linked_list_add()
{
    printf("double_linked_list_init()\n");
    double_linked_list* dllist = setup_basic_list();
    char* data1 = "Data1";
    char* data2 = "Data2";
    char* data3 = "Data3";
    double_linked_list_add(dllist, data1,"A");
    double_linked_list_add(dllist, data2,"B");
    double_linked_list_add(dllist, data3,"C");

    if(strcmp(dllist->first->value, "A") || strcmp(dllist->first->next->value, "B") || strcmp(dllist->last->value, "C")) {
        printf("Values are Incorrect\n");
        exit(1);
    }
    printf("Values are Correct.\n");
    if(strcmp((char*)dllist->first->pointer_data, "Data1") || strcmp((char*)dllist->first->next->pointer_data, "Data2") || strcmp((char*)dllist->last->pointer_data, "Data3")) {
        printf("Pointer Values are Incorrect.\n");
        exit(1);
    }
    printf("Pointer Values are Correct.\n");
    double_linked_list_destructor(dllist);
}

void test_double_linked_list_destroy()
{
    printf("test_double_linked_list_print()\n");
    double_linked_list* dllist = setup_basic_list();
    char* data1 = "Data1";
    char* data2 = "Data2";
    char* data3 = "Data3";
    double_linked_list_add(dllist, data1,"A");
    double_linked_list_add(dllist, data2,"B");
    double_linked_list_add(dllist, data3,"C");
    double_linked_list_destructor(dllist);

    if(dllist->first != NULL || dllist->last != NULL || dllist->size != 0) {
        printf("DLL is not destroyed correctly.\n");
        exit(1);
    }
    printf("DLL is destroyed correctly.\n");
}

void test_double_linked_list_print()
{
    printf("test_double_linked_list_print()\n");
    double_linked_list* dllist = setup_basic_list();
    char* data1 = "Data1";
    char* data2 = "Data2";
    char* data3 = "Data3";
    double_linked_list_add(dllist, data1,"A");
    double_linked_list_add(dllist, data2,"B");
    double_linked_list_add(dllist, data3,"C");

    printf("If you see ABC bellow, the test is correct.\n");
    double_linked_list_print(dllist);
    double_linked_list_destructor(dllist);
}

void test_double_linked_list_delete_by_value()
{
    printf("test_double_linked_list_delete_by_value()\n");
    double_linked_list* dllist = setup_basic_list();
    char* data1 = "Data1";
    char* data2 = "Data2";
    char* data3 = "Data3";
    double_linked_list_add(dllist, data1,"A");
    double_linked_list_add(dllist, data2,"B");
    double_linked_list_add(dllist, data3,"C");

    double_linked_list_node* akt = double_linked_list_search_by_value(dllist, "B");
    if (akt->value == "B") {
        printf("DLL has node with value: B.\n");
    }
    double_linked_list_remove_by_value(dllist, "B");
    akt = double_linked_list_search_by_value(dllist, "B");
    if (akt != NULL) {
        if (akt->value == "B") {
            printf("DLL has node with value: B even after delete was called.\n");
            exit(1);
        }
    }
    printf("DLL doesn't has node with value: B after delete was called.\n");
    double_linked_list_destructor(dllist);
}

int main(void)
{
    printf("\n");
    test_double_linked_list_add();
    test_double_linked_list_destroy();
    test_double_linked_list_delete_by_value();
    test_double_linked_list_print();
}