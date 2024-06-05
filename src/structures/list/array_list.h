
#ifndef ARRAY_LIST_H
#define ARRAY_LIST_H

#include <stddef.h>
#include <stdio.h>

typedef struct ArrayList {
    void *array;
    size_t item_size;
    size_t size;
    size_t capacity;
} ArrayList;

typedef struct ArrayListIterator {
    ArrayList *list;
    size_t index;
} ArrayListIterator;

// Base operations
void arraylist_init(ArrayList *self, size_t item_size);
void arraylist_destroy(ArrayList *self);
// void array_list_clear(ARRAY_LIST *self); // Not implemented

// Data operations
_Bool arraylist_get_at(ArrayList *self, size_t index, void *item);
void arraylist_add(ArrayList *self, const void *item);
_Bool arraylist_insert_at(ArrayList *self, size_t index, const void *item);
_Bool arraylist_set_at(ArrayList *self, size_t index, const void *item);
_Bool arraylist_remove_at(ArrayList *self, size_t index, void *item);
_Bool arraylist_assign(ArrayList *self, ArrayList *other);
_Bool arraylist_equals(ArrayList *self, ArrayList *other);

// File operations
_Bool arraylist_save_to_file(ArrayList *self, const char *file_name);
_Bool arraylist_read_from_file(ArrayList *self, const char *file_name);
void arraylist_to_txt(ArrayList *self, FILE *f, void (*item_to_file)(void *item, FILE *f));

// Sorting operations
void arraylist_bubblesort(ArrayList *self, int (*cmp)(const void *itemA, const void *itemB));
void arraylist_qsort(ArrayList *self, int (*cmp)(const void *, const void *));

// Iterators
void arraylist_iterator_init(ArrayListIterator *self, ArrayList *list);
void arraylist_iterator_destroy(ArrayListIterator *self);
_Bool arraylist_iterator_has_next(ArrayListIterator *self);
void *arraylist_iterator_move_next(ArrayListIterator *self);

#endif
