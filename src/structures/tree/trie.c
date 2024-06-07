#include "trie.h"

#define PRIME_SIZE 257

void init_trie(trie* this)
{
    this->root = calloc(1,sizeof(trie_node));
    init_trie_node(this->root,'\0');
}

hash_table* insert_trie(trie* this, const char* word)
{
    if(search_trie(this,word) != NULL)
        return NULL;

    trie_node* tmp = this->root;
    int index = 0;

    for(int i = 0; word[i] != '\0'; i++)
    {
        index = (int) word[i] - 'A';
        if (index < 0 || index >= RE_SIZE)
            index = RE_SIZE - 1 + index;

        if (tmp->children[index] == NULL)
        {
            trie_node *new = calloc(1, sizeof(trie_node));
            init_trie_node(new, word[i]);
            insert_child_trie_node(tmp, new, index);
        }
        tmp = tmp->children[index];
    }
    tmp->is_leaf = true;
    tmp->table = malloc(sizeof(hash_table));
    init_hash_table(tmp->table,PRIME_SIZE);
    return tmp->table;
}

hash_table* delete_trie(trie* this, const char* word)
{
    trie_node* tmp = this->root;
    trie_node* nodes[strlen(word)+1];
    int ind[strlen(word)];
    bzero(ind,strlen(word));
    int index = 0;

    for(int i = 0; word[i] != '\0'; i++)
    {
        index = (int) word[i] - 'A';
        if (index < 0 || index >= RE_SIZE)
        {
            if (!tmp->is_resize)
                return NULL;
            index = RE_SIZE - 1 + index;
        }

        if(tmp->children[index] == NULL || tmp->children[index]->value != word[i])
            return NULL;

        ind[i] = index;
        nodes[i] = tmp;
        tmp = tmp->children[index];
    }

    if(!tmp->is_leaf)
        return NULL;

    tmp->is_leaf = false;
    hash_table* del = tmp->table;

    bool has_child = has_child_trie_node(tmp);

    if(!has_child)
    {
        destruct_trie_node(tmp);
        nodes[strlen(word)-1]->children[index] = NULL;
    }

    for(int i = (int)strlen(word) - 1; i >= 1; i--)
    {
        if(has_child)
            break;

        if(has_child_trie_node(nodes[i]))
            break;

        destruct_trie_node(nodes[i]);
        nodes[i-1]->children[index] = NULL;
    }

    return del;
}

hash_table* search_trie(trie* this,const char* word)
{
    trie_node* tmp = this->root;
    int index = 0;
    for(int i = 0; word[i] != '\0'; i++)
    {
        index = (int) word[i] - 'A';
        if (index < 0 || index >= RE_SIZE)
        {
            if (!tmp->is_resize)
                return NULL;

            index = RE_SIZE - 1 + index;
        }
        if(tmp->children[index] == NULL || tmp->children[index]->value != word[i])
            return NULL;
        tmp = tmp->children[index];
    }
    if(!tmp->is_leaf)
        return NULL;

    return tmp->table;
}

void print_trie(trie* this)
{
    if(this->root->is_resize)
    {
        for(int i = 0; i < RE_SIZE; i++)
        {
            if(this->root->children != NULL && this->root->children[i] != NULL)
                print_trie_node(this->root->children[i]);
        }
    }
    else
    {
        for(int i = 0; i < DEFAULT_SIZE; i++)
        {
            if(this->root->children != NULL && this->root->children[i] != NULL)
                print_trie_node(this->root->children[i]);
        }
    }
}

void destruct_trie(trie* this)
{
    destruct_trie_node(this->root);
    this->root = NULL;
}
