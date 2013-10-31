#include <stdio.h>
#include <stdlib.h>
#include "binary_tree_draw.h"
typedef enum COLOR {
    RED, BLACK
} COLOR;
typedef struct RBNode {
    int key;
    COLOR color;
    RBNode *left;
    RBNode *right;
    RBNode *parent;
} RBNode;

void* lchild(void *node) {
    if(node) {
        return ((RBNode*)node)->left;
    }
    return NULL;
}
void *rchild(void *node) {
    if(node) {
        return ((RBNode*)node)->right;
    }
    return NULL;
}

int isRed(void *node) {
    if(node) {
        return ((RBNode*)node)->color == RED;
    }
    return 0;
}
int key(void *node) {
    if(node) {
        return ((RBNode*)node)->key;
    }
    return 0;
}
int isTwisted(RBNode *node) {
    if(node->color == RED) {
        if(node->parent != NULL) {
            RBNode *p = node->parent;
            if(p->color == RED && p->parent != NULL) {
                return 1;
            }
        }
    }
    return 0;
}

// return root node
RBNode* twist(RBNode *node) {
    RBNode* p = node->parent;
    RBNode* g = p->parent;
    RBNode* gg = g->parent;
    RBNode *x, *y, *z, *T1, *T2, *T3, *T4;
    if(g->left == p && p->left == node) {
        z = g;
        y = p;
        x = node;
        T1 = x->left;
        T2 = x->right;
        T3 = y->right;
        T4 = z->right;
    } else if(g->right == p && p->right == node) {
        x = g;
        y = p;
        z = node;
        T1 = x->left;
        T2 = y->left;
        T3 = z->left;
        T4 = z->right;
    } else if(g->left == p && p->right == node) {
        z = g;
        x = p;
        y = node;
        T1 = x->left;
        T2 = y->left;
        T3 = y->right;
        T4 = z->right;
    } else {
        x = g;
        z = p;
        y = node;
        T1 = x->left;
        T2 = y->left;
        T3 = y->right;
        T4 = z->right;
    }

    y->color = RED;
    y->left = x;
    y->right = z;
    y->parent = gg;
    if(gg) {
        if(gg->left == g)
            gg->left = y;
        else
            gg->right = y;
    }

    x->color = BLACK;
    x->left = T1;
    x->right = T2;
    x->parent = y;

    z->color = BLACK;
    z->left = T3;
    z->right = T4;
    z->parent = y;
    
    if(T1) T1->parent = x;
    if(T2) T2->parent = x;
    if(T3) T3->parent = z;
    if(T4) T4->parent = z;

    return y;
}

RBNode* createNode(int key) {
    RBNode * node = (RBNode*) malloc(sizeof(RBNode));
    node->key = key;
    node->color = RED;
    node->left = NULL;
    node->right = NULL;
    node->parent = NULL;
    return node;
}
RBNode* insertNode(RBNode *root, int key, int *twistCount) {
    if(root == NULL) {
        root = createNode(key);
        root->color = BLACK;
        return root;
    }

    RBNode *node = createNode(key);
    RBNode *p = root, *pre = p;
    while(p) {
        if(p->key > key) {
            pre = p;
            p = p->left;
        } else {
            pre = p;
            p = p->right;
        }
    }
    if(pre->key > key) {
        pre->left = node;
        node->parent = pre;
    } else {
        pre->right = node;
        node->parent = pre;
    }

    while(isTwisted(node)) {
        if(twistCount) *twistCount += 1;
        node = twist(node);
        if(node->parent == NULL)
        {
            root = node;
            root->color = BLACK;
        }
    }
    return root;
}

int main() {
    int c, n;
    printf("input count:");
    scanf("%d", &c);
    int i, twistCount = 0;
    RBNode * root = NULL;
    for(i = 0; i < c; ++i) {
        scanf("%d", &n);
        root = insertNode(root, n, &twistCount);
        draw_rb_tree(root);
        printf("\ntwist count:%d\n", twistCount);
    }
    return 0;
}


