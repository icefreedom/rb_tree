#ifndef __RB_NODE_H__
#define __RB_NODE_H__
typedef enum COLOR { RED, BLACK} COLOR;
typedef int key_t;
typedef struct RB_NODE {
    COLOR c;
    key_t key;
    RB_NODE *parent;
    RB_NODE *left;
    RB_NODE *right;
} RB_NODE;

typedef struct RB_TREE {
    RB_NODE *root;
} RB_TREE;

void draw_rb_tree(RB_TREE *tree) ;
void rb_insert(RB_TREE * tree, RB_NODE *node) ;
RB_NODE* create_node(key_t key) ;
RB_TREE* create_rb_tree() ;
void destroy_rb_tree(RB_TREE *tree) ;
#endif
