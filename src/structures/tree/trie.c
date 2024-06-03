#include "trie.h"

void init_trie(trie* this)
{
    this->root = calloc(1,sizeof(trie_node));
    init_trie_node(this->root,'\0');
}

void insert_trie(trie* this, const char* word)
{
    trie_node* tmp = this->root;
    int index = 0;

    for(int i = 0; word[i] != '\0'; i++)
    {
        index = (int) word[i] - 'A';
        if (index < 0)
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
}

void delete_trie(trie* this, const char* word)
{
    trie_node* tmp = this->root;
    trie_node* nodes[strlen(word)+1];
    int ind[strlen(word)];
    bzero(ind,strlen(word));
    int index = 0;

    for(int i = 0; word[i] != '\0'; i++)
    {
        index = (int) word[i] - 'A';
        if (index < 0)
        {
            if (!tmp->is_resize)
                return;
            index = RE_SIZE - 1 + index;
        }

        if(tmp->children[index] == NULL || tmp->children[index]->value != word[i])
            return;

        ind[i] = index;
        nodes[i] = tmp;
        tmp = tmp->children[index];
    }

    if(!tmp->is_leaf)
        return;

    tmp->is_leaf = false;

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
}

bool search_trie(trie* this,const char* word)
{
    trie_node* tmp = this->root;
    int index = 0;
    for(int i = 0; word[i] != '\0'; i++)
    {
        index = (int) word[i] - 'A';
        if (index < 0)
        {
            if (!tmp->is_resize)
                return false;

            index = RE_SIZE - 1 + index;
        }
        if(tmp->children[index] == NULL || tmp->children[index]->value != word[i])
            return false;
        tmp = tmp->children[index];
    }
    if(!tmp->is_leaf)
        return false;

    return true;
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
