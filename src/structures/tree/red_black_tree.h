#include "red_black_tree_node.h"
#include <stdlib.h>

typedef struct RBTree
{
    RBTreeNode* root;
    size_t size;
}RBTree;

void red_black_tree_init(RBTree* this);
void red_black_tree_destroy(RBTree* this);

void red_black_tree_insert(RBTree* this,int code, const unsigned char* value,size_t value_size);

RBTreeNode* red_black_tree_find_node_by_value(unsigned char* value, RBTreeNode* subRoot, size_t value_size);

RBTreeNode* red_black_tree_remove(RBTree* this, RBTreeNode* node);
RBTreeNode* red_black_tree_find_by_code(RBTree* this, int code);
size_t red_black_tree_get_size(RBTree* this);
void red_black_tree_postorder_get_nodes(RBTreeNode* node,RBTreeNode** nodeArray, size_t* size);

unsigned char *red_black_tree_serialize(RBTree *this, size_t *size);
void red_black_tree_deserialize(RBTree *this, unsigned char *buffer, size_t size);

