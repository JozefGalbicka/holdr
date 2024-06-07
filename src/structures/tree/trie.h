#ifndef TRIE_H
#define TRIE_H

#include <malloc.h>
#include <string.h>
#include "trie_node.h"

#define DEFAULT_SIZE 58
#define RE_SIZE 256

typedef struct trie
{
    trie_node* root;
}trie;

void init_trie(trie* this);
hash_table* insert_trie(trie* this,const char* word);
hash_table* delete_trie(trie* this,const char* word);
hash_table* search_trie(trie* this,const char* word);
void print_trie(trie* this);
void destruct_trie(trie* this);

#endif