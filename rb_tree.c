#include "rb_tree.h"
#include <stdlib.h>
#include <stdio.h>
//create and init node
RB_NODE* create_node(key_t key) {
    RB_NODE *node = (RB_NODE*) malloc(sizeof(RB_NODE));
    node->c = RED;
    node->key = key;
    node->parent = NULL;
    node->left = NULL;
    node->right = NULL;
    return node;
}
RB_TREE* create_rb_tree() {
    RB_TREE * tree = (RB_TREE*) malloc(sizeof(RB_TREE));
    tree->root = NULL;
    return tree;
}
static void destroy_rb_tree_internal(RB_NODE *root) {
    if(root == NULL) return;
    if(root->left) destroy_rb_tree_internal(root->left);
    if(root->right) destroy_rb_tree_internal(root->right);
    free(root);
    root = NULL;
}
void destroy_rb_tree(RB_TREE *tree) {
    if(tree == NULL) return;
    destroy_rb_tree_internal(tree->root);
    free(tree);
    tree = NULL;
}
static RB_NODE *grand_node(RB_NODE *node) {
    if(node->parent == NULL) 
        return NULL;

    return node->parent->parent;
}

static RB_NODE *uncle_node(RB_NODE *node) {
    RB_NODE * grand = grand_node(node);
    if(node->parent == NULL || 
        grand == NULL) 
        return NULL;

    if(node->parent == grand->left)
        return grand->right;
    else
        return grand->left;
}
static void right_rotate(RB_TREE *tree, RB_NODE *node) {
    RB_NODE *left = node->left;
    RB_NODE *parent = node->parent;
    if(left == NULL) return;

    if(parent) {
        if(parent->left == node)
            parent->left = left;
        else
            parent->right = left;
    } else
        tree->root = left;
    
    node->parent = left;
    node->left = NULL;

    left->parent = parent;
    RB_NODE *right = left->right;
    left->right = node;
    
    
    node->left = right;
}

static void left_rotate(RB_TREE *tree, RB_NODE *node) {
    RB_NODE *right = node->right;
    RB_NODE *parent = node->parent;
    if(right == NULL)
        return;
    
    if(parent) {
        if(parent->left == node)
            parent->left = right;
        else
            parent->right = right;
    } else
        tree->root = right;
    
    node->parent = right;
    node->right = NULL;

    right->parent = parent;
    RB_NODE *left = right->left;
    right->left = node;
    
    
    node->right = left;

    
}

static void adjustNode(RB_TREE *tree, RB_NODE *node) {
    if(node->parent == NULL) 
        return;
    RB_NODE *parent = node->parent;
    RB_NODE *uncle, *grand;
    
    while(node->c == RED) {
        //parent is impossible to be null
        parent = node->parent;
        if(parent == NULL) {
            node->c = BLACK;
            return;
        }
        //situation 1
        if(parent->c == BLACK)
            return;
        //grand is impossible to be null here
        grand = grand_node(node); 
        //uncle maybe is null
        uncle = uncle_node(node);
        //situation 2: parent is RED, uncle is RED
        if(uncle != NULL && uncle->c == RED) {
            parent->c = BLACK;
            uncle->c = BLACK;
            grand->c = RED;
            node = grand;
        } 
        //situation 3: uncle is null or BLACK
        else if(uncle == NULL || uncle->c == BLACK) {
            //situation 3-1: node is the right child
            if(parent == grand->left && node == parent->right) {
                node = parent;
                left_rotate(tree, node);   
            } else if(parent == grand->right && node == parent->left) {
                node = parent;
                right_rotate(tree, node);
            }
            //situation 3-2: node is the left child
            else {
                parent->c = BLACK;
                grand->c = RED;
                if(parent == grand->left)
                    right_rotate(tree, grand);
                else 
                    left_rotate(tree, grand);
            }
        }
    }
    
}

void rb_insert(RB_TREE * tree, RB_NODE *node) {
    //check tree
    if(tree == NULL ) {
        return;
    }

    //check root node
    if(tree->root == NULL) {
        node->c = BLACK;
        tree->root = node;
        return;
    }

    //find the position
    RB_NODE * r = tree->root;
    RB_NODE * prev;
    while(r) {
        prev = r;
        if(r->key == node->key) {
            //update and return
            return;
        }
        if(r->key > node->key ) {
            r = r->left;
        } else if (r->key < node->key) {
            r = r->right;
        }
    }
    
    //insert
    node->c = RED;
    node->parent = prev;
    if(prev->key < node->key) {
        prev->right = node;
    } else {
        prev->left = node;
    }

    //adjust to match rb tree definition
    adjustNode(tree, node);
}
static void print_node(RB_NODE *node, char node_buffer[64]) {
    if(node == NULL) {
        node_buffer[0] = '\0';
        return;
    }
    int len;
    if(node->c == RED)
        len = sprintf(node_buffer, "    %d[fillcolor=%s];\n", node->key, "red");
    else
        len = sprintf(node_buffer, "    %d[fillcolor=%s];\n", node->key, "black");
    node_buffer[len] = '\0';
}

static void print_edge(RB_NODE * one, RB_NODE *two, char edge_buffer[256]) {
    int len = sprintf(edge_buffer, "    %d -> %d ;\n", one->key, two->key);
    edge_buffer[len] = '\0';
}
static int append_buffer(char **buffer, int *len, const char* content) {
    int use_len = sprintf(*buffer, "%s", content);
    if(use_len <= 0 ) return -1; //error
    *buffer += use_len;
    *len -= use_len;
    return 0;
}
static int print_rb_tree(RB_NODE *root, char** buffer, int *len) {
    if(root == NULL) return 0;
    char edge_buffer[256];
    int use_len;
    char node_buffer[64];
    print_node(root, node_buffer);
    if(append_buffer(buffer, len, node_buffer) != 0)
        return -1;
    if(root->left) {
        print_node(root->left, node_buffer);
        if(append_buffer(buffer, len, node_buffer) != 0)
            return -1;
        
        print_edge(root, root->left, edge_buffer);
        if(append_buffer(buffer, len, edge_buffer) != 0)
            return -1;
    }
    if(root->right) {
        print_node(root->right, node_buffer);
        if(append_buffer(buffer, len, node_buffer) != 0)
            return -1;
        
        print_edge(root, root->right, edge_buffer);
        if(append_buffer(buffer, len, edge_buffer) != 0)
            return -1;

    }

    if(root->left) {
        if(print_rb_tree(root->left, buffer, len) != 0)
            return -1;
    }
    if(root->right) {
        if(print_rb_tree(root->right, buffer, len) != 0)
            return -1;
    }
    
}

void draw_rb_tree(RB_TREE *tree) {
    char * edges_buffer = (char*) malloc(sizeof(char) * 8192);
    int len = 8192;
    char * buffer_p = edges_buffer;
    if(print_rb_tree(tree->root, &buffer_p, &len) != 0)
        return; //error
    len = 0;
    
    printf("digraph rbtree {\n" 
           "    node[shape=circle, fontsize=12, fontcolor=white, style=filled, color=skyblue];\n"
           "    edge[shape=solid, color=black];\n"
           "%s}\n", edges_buffer);

    free(edges_buffer);
}
