#include "trie_node.h"

void init_trie_node(trie_node* this, char value)  //'\0' means root node
{
    this->value = value;
    this->is_leaf = false;
    this->is_resize = false;
    this->children = calloc(DEFAULT_SIZE,sizeof(trie_node*));
}

static void resize_children_trie_node(trie_node* this)
{
    trie_node** new = realloc(this->children, sizeof(trie_node*) * RE_SIZE);
    this->children = new;
    this->is_resize = true;
}


void insert_child_trie_node(trie_node* this, trie_node* child, int index)
{
    if(index >= DEFAULT_SIZE && !this->is_resize)
        resize_children_trie_node(this);

    this->children[index] = child;
}

void print_trie_node(trie_node* this)
{
    if (this == NULL)
        return;

    printf("%c ",this->value);

    if(this->is_resize)
    {
        for(int i = 0; i < RE_SIZE; i++)
        {
            if(this->children != NULL && this->children[i] != NULL)
                print_trie_node(this->children[i]);
        }
    }
    else
    {
        for(int i = 0; i < DEFAULT_SIZE; i++)
        {
            if(this->children != NULL && this->children[i] != NULL)
                print_trie_node(this->children[i]);
        }
    }
}

void destruct_trie_node(trie_node* this)
{
    if (this == NULL)
        return;

    int size = this->is_resize ? RE_SIZE : DEFAULT_SIZE;
    for (int i = 0; i < size; ++i)
    {
        if (this->children[i] != NULL)
        {
            destruct_trie_node(this->children[i]);
        }
    }
    free(this->children);
    free(this);
}

bool has_child_trie_node(trie_node* this)
{
    int size = this->is_resize ? RE_SIZE : DEFAULT_SIZE;

    for(int i = 0; i < size; i++ )
    {
        if(this->children[i] != NULL)
        {
            return true;
        }
    }
    return false;
}