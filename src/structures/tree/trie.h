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
void insert_trie(trie* this, const char* word);
void delete_trie(trie* this,const char* word);
bool search_trie(trie* this,const char* word);
void print_trie(trie* this);
void destruct_trie(trie* this);