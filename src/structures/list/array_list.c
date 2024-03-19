
#include "array_list.h"
#include <malloc.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#define DEFAULT_CAPACITY 10

static void array_list_ensure_capacity(ArrayList *self)
{
    if (self->size >= self->capacity) {
        self->capacity = (self->capacity > 0) ? 2 * self->capacity : DEFAULT_CAPACITY;
        self->array = realloc(self->array, self->capacity * self->item_size);
    }
}

// ---------------
// Base operations
// ---------------
void arraylist_init(ArrayList *self, size_t item_size)
{
    self->size = 0;
    self->item_size = item_size;
    self->capacity = DEFAULT_CAPACITY;
    self->array = malloc(self->capacity * sizeof(self->item_size));
}

void arraylist_destroy(ArrayList *self)
{
    free(self->array);
    self->array = NULL;
    self->item_size = 0;
    self->size = 0;
    self->capacity = 0;
}

// ---------------
// Data operations
// ---------------
_Bool arraylist_get_at(ArrayList *self, size_t index, void *item)
{
    if (index < self->size) {
        memcpy(item, (unsigned char *)self->array + index * self->item_size, self->item_size);
        return true;
    } else {
        return false;
    }
}

void arraylist_add(ArrayList *self, const void *item)
{
    array_list_ensure_capacity(self);
    memcpy((unsigned char *)self->array + self->size * self->item_size, item, self->item_size);
    self->size++;
}

_Bool arraylist_insert_at(ArrayList *self, size_t index, const void *item)
{
    if (index < self->size) {
        array_list_ensure_capacity(self);
        memmove((unsigned char *)self->array + (index + 1) * self->item_size,
                (unsigned char *)self->array + index * self->item_size, (self->size - index) * self->item_size);
        ++self->size;
        return true;
    } else if (index == self->size) {
        arraylist_add(self, item);
        return true;
    } else {
        return false;
    }
}

_Bool arraylist_set_at(ArrayList *self, size_t index, const void *item)
{
    if (index < self->size) {
        memcpy((unsigned char *)self->array + index * self->item_size, item, self->item_size);
        return true;
    } else {
        return false;
    }
}

_Bool arraylist_remove_at(ArrayList *self, size_t index, void *item)
{
    if (index < self->size) {
        memcpy(item, (unsigned char *)self->array + index * self->item_size, self->item_size);
        if (index < self->size - 1) {
            memmove((unsigned char *)self->array + index * self->item_size,
                    (unsigned char *)self->array + (index + 1) * self->item_size,
                    (self->size - index - 1) * self->item_size);
        }
        self->size--;
        return true;
    } else if (index == self->size - 1) {
        memcpy(item, (unsigned char *)self->array + index * self->item_size, self->item_size);
        self->size--;
        return true;
    } else {
        return false;
    }
}

_Bool arraylist_assign(ArrayList *self, ArrayList *other)
{
    if (self == other) {
        return true;
    }
    if (self->item_size != other->item_size) {
        return false;
    }
    if (self->capacity < other->size) {
        self->array = realloc(self->array, other->capacity * self->item_size);
        self->capacity = other->capacity;
    }
    memcpy(self->array, other->array, other->size * self->item_size);
    self->size = other->size;
    return true;
}

_Bool arraylist_equals(ArrayList *self, ArrayList *other)
{
    if (self == other) {
        return true;
    }
    if (self->item_size != other->item_size || self->size != other->size) {
        return false;
    }

    ArrayListIterator it;
    ArrayListIterator it2;
    _Bool result = true;

    arraylist_iterator_init(&it, self);
    arraylist_iterator_init(&it2, other);

    while (arraylist_iterator_has_next(&it)) {
        void *tmp = arraylist_iterator_move_next(&it);
        void *tmp2 = arraylist_iterator_move_next(&it2);
        if (memcmp(tmp, tmp2, self->item_size)) {
            result = false;
        }
    }
    arraylist_iterator_destroy(&it);
    arraylist_iterator_destroy(&it2);

    return result;
}

// ---------------
// File operations
// ---------------
_Bool arraylist_save_to_file(ArrayList *self, const char *file_name)
{

    FILE *f = fopen(file_name, "wb");
    if (f == NULL) {
        return false;
    }
    fwrite(&self->capacity, sizeof(self->capacity), 1, f);
    fwrite(&self->size, sizeof(self->size), 1, f);
    fwrite(&self->item_size, sizeof(self->item_size), 1, f);
    fwrite(self->array, self->item_size, self->size, f);

    fclose(f);
    return true;
}

_Bool arraylist_read_from_file(ArrayList *self, const char *file_name)
{
    FILE *f = fopen(file_name, "rb");
    if (f == NULL) {
        return false;
    }

    fread(&self->capacity, sizeof(self->capacity), 1, f);
    fread(&self->size, sizeof(self->size), 1, f);
    fread(&self->item_size, sizeof(self->item_size), 1, f);
    self->array = malloc(self->item_size * self->capacity);
    fread(self->array, self->item_size, self->size, f);

    fclose(f);
    return true;
}

void arraylist_to_txt(ArrayList *self, FILE *f, void (*item_to_file)(void *, FILE *))
{
    ArrayListIterator it;
    arraylist_iterator_init(&it, self);
    while (arraylist_iterator_has_next(&it)) {
        item_to_file(arraylist_iterator_move_next(&it), f);
    }
    arraylist_iterator_destroy(&it);
}

// ------------------
// Sorting operations
// ------------------
static void bubblesort(void *base, size_t nitems, size_t size, int (*compar)(const void *, const void *))
{
    int i, j;
    void *itemA;
    void *itemB;
    void *tmp = malloc(size);

    for (i = 0; i < nitems - 1; i++) {
        for (j = 0; j < nitems - i - 1; j++) {
            itemA = base + j * size;
            itemB = base + (j + 1) * size;
            if (compar(itemA, itemB) > 0) {
                memcpy(tmp, itemA, size);
                memcpy((unsigned char *)base + j * size, itemB, size);
                memcpy((unsigned char *)base + (j + 1) * size, tmp, size);
            }
        }
    }
    free(tmp);
}

void arraylist_bubblesort(ArrayList *self, int (*cmp)(const void *, const void *))
{
    bubblesort(self->array, self->size, self->item_size, cmp);
}

void arraylist_qsort(ArrayList *self, int (*cmp)(const void *, const void *))
{
    qsort(self->array, self->size, self->item_size, cmp);
}

// ---------------
// Iterators
// ---------------
void arraylist_iterator_init(ArrayListIterator *self, ArrayList *list)
{
    self->index = 0;
    self->list = list;
}

void arraylist_iterator_destroy(ArrayListIterator *self)
{
    self->index = 0;
    self->list = NULL;
}

_Bool arraylist_iterator_has_next(ArrayListIterator *self)
{
    return self->index < self->list->size;
}

void *arraylist_iterator_move_next(ArrayListIterator *self)
{
    void *tmp = (unsigned char *)self->list->array + self->index * self->list->item_size;
    self->index++;
    return tmp;
}
