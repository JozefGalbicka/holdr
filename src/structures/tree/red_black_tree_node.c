#include "red_black_tree_node.h"
#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>


#define EMPTY 0

void red_black_tree_node_init(RBTreeNode* this)
{
    this->value_size = EMPTY;
    this->value = NULL;
    this->nodeColour = black;
    this->code = EMPTY;
    this->parent = NULL;
    this->left = NULL;
    this->right = NULL;
}


void red_black_tree_node_destroy(RBTreeNode* this)
{
    if(this->value != NULL)
        free(this->value);

    this->value = NULL;
    this->value_size = EMPTY;
    this->nodeColour = black;
    this->code = EMPTY;
    this->parent = NULL;
    this->left = NULL;
    this->right = NULL;
}


void red_black_tree_node_set_left_son(RBTreeNode* this, RBTreeNode* left)
{
    this->left = left;
}

void red_black_tree_node_set_right_son(RBTreeNode* this,RBTreeNode* right)
{
    this->right = right;
}

void red_black_tree_node_set_parent(RBTreeNode* this,RBTreeNode* parent)
{
    this->parent = parent;
}

RBTreeNode* red_black_tree_node_get_left_son(RBTreeNode* this)
{
    return this->left;
}

RBTreeNode* red_black_tree_node_get_right_son(RBTreeNode* this)
{
    return this->right;
}

RBTreeNode* red_black_tree_node_get_parent(RBTreeNode* this)
{
    return this->parent;
}

RBTreeNode* red_black_tree_get_uncle(RBTreeNode* this)
{
    if(this->parent == NULL)
        return NULL;

    if(this->parent->parent == NULL)
        return NULL;

    return ((this->parent->parent->right == this)?(this->parent->parent->left):(this->parent->parent->right));
}

void red_black_tree_node_recolour(RBTreeNode* this)
{
    this->nodeColour = (this->nodeColour == red) ? black : red;
}

void red_black_tree_node_set_colour(RBTreeNode* this, Colour nodeColour)
{
    this->nodeColour = nodeColour;
}

void red_black_tree_node_set_code(RBTreeNode* this, int code)
{
    this->code = code;
}

void red_black_tree_node_set_value(RBTreeNode* this, const unsigned char* value, size_t value_size)
{
    if(this->value != NULL)
        free(this->value);

    this->value = calloc(value_size,sizeof(unsigned char));
    for(size_t i = 0; i < value_size; i++)
    {
        this->value[i] = value[i];
    }
    this->value_size = value_size;
}

Colour red_black_tree_node_get_colour(RBTreeNode* this)
{
    return this->nodeColour;
}

int red_black_tree_node_get_code(RBTreeNode* this)
{
    return this->code;
}

unsigned char* red_black_tree_node_get_value(RBTreeNode* this)
{
    return this->value;
}

size_t red_black_tree_node_get_value_size(RBTreeNode* this)
{
    return this->value_size;
}

unsigned char *red_black_tree_node_serialize(RBTreeNode *this, size_t *size)
{
    uint32_t code = htonl(this->code);
    uint32_t value_size = htonl((uint32_t)this->value_size);
    size_t buffer_size = sizeof(code) + sizeof(value_size) + this->value_size;

    unsigned char *buffer = malloc(buffer_size);

    memcpy(buffer, &code, sizeof(code));
    memcpy(buffer + sizeof(code), &value_size, sizeof(value_size));
    memcpy(buffer + sizeof(code) + sizeof(value_size), this->value, this->value_size);

    *size = buffer_size;
    return buffer;
}

unsigned char *red_black_tree_node_deserialize(unsigned char *buffer, int *code, size_t *value_size, unsigned char** value)
{
    uint32_t code_l;
    uint32_t value_size_l;

    memcpy(&code_l, buffer, sizeof(code_l));
    *code = ntohl(code_l);

    memcpy(&value_size_l, buffer + sizeof(code_l), sizeof(value_size_l));
    *value_size = ntohl(value_size_l);

    unsigned char *value_l = malloc(*value_size);

    memcpy(value_l, buffer + sizeof(code_l) + sizeof(value_size_l), *value_size);
    *value = value_l;

    return (buffer + sizeof(code_l) + sizeof(value_size_l) + *value_size - 1);
}