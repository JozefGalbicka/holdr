#include <stdlib.h>

typedef enum Colour
{
    red,
    black,
}Colour;

typedef struct RBTreeNode
{
    size_t value_size;
    struct RBTreeNode* parent;
    struct RBTreeNode* left;
    struct RBTreeNode* right;
    unsigned char* value;
    int code;
    Colour nodeColour;
}RBTreeNode;

void red_black_tree_node_init(RBTreeNode* this);
void red_black_tree_node_destroy(RBTreeNode* this);

void red_black_tree_node_set_left_son(RBTreeNode* this, RBTreeNode* left);
void red_black_tree_node_set_right_son(RBTreeNode* this,RBTreeNode* right);
void red_black_tree_node_set_parent(RBTreeNode* this,RBTreeNode* parent);

RBTreeNode* red_black_tree_node_get_left_son(RBTreeNode* this);
RBTreeNode* red_black_tree_node_get_right_son(RBTreeNode* this);
RBTreeNode* red_black_tree_node_get_parent(RBTreeNode* this);
RBTreeNode* red_black_tree_get_uncle(RBTreeNode* this);

void red_black_tree_node_recolour(RBTreeNode* this);

void red_black_tree_node_set_colour(RBTreeNode* this, Colour nodeColour);
void red_black_tree_node_set_code(RBTreeNode* this, int code);
void red_black_tree_node_set_value(RBTreeNode* this, const unsigned char* value, size_t value_size);

Colour red_black_tree_node_get_colour(RBTreeNode* this);
int red_black_tree_node_get_code(RBTreeNode* this);
unsigned char* red_black_tree_node_get_value(RBTreeNode* this);
size_t red_black_tree_node_get_value_size(RBTreeNode* this);

unsigned char *red_black_tree_node_serialize(RBTreeNode *this, size_t *size);
unsigned char *red_black_tree_node_deserialize(unsigned char *buffer, int *code, size_t *value_size, unsigned char** value);
