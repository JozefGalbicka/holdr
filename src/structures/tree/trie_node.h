#include <stdbool.h>
#include <stdio.h>
#include <malloc.h>

#define DEFAULT_SIZE 58
#define RE_SIZE 256


typedef struct trie_node
{
    char value;
    bool is_leaf;
    bool is_resize;
    struct trie_node** children;
}trie_node;

void init_trie_node(trie_node* this, char value);
static void resize_children_trie_node(trie_node* this);
void insert_child_trie_node(trie_node* this, trie_node* child, int index);
void print_trie_node(trie_node* this);
void destruct_trie_node(trie_node* this);
bool has_child_trie_node(trie_node* this);