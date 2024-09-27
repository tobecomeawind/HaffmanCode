#include <stdio.h>
#include <stdint.h>

#ifndef TREE_H
#define TREE_H

#define NODEWP 0b11111000 //node without params
#define RIGHT  0b00000001 //is node->right
#define LEFT   0b00000010 //is node->left
#define isLEAF 0b00000100 //is node->leaf
#define END    0b00000000 //end of serialization

#define True 1

typedef struct node{

	leaf* lp;

	int count;

	struct node* left;
	struct node* right;

} node;

int   compare_alph (leaf* , leaf*);

void  create_table (node* , int   , uint32_t);
node* init_node    (leaf* , node* , node*);
node* create_tree  (leaf**, int);
void  encode       (char* , node* , leaf**, int);
void  destruct_tree(node*);
#endif
