#include "rb_tree.h"
#include <stdio.h>

void test_rb_tree() {
    RB_TREE *tree = create_rb_tree();
    int size;
    key_t key;
    //printf("input size:");
    scanf("%d", &size);
    printf("\n");
    while(size > 0) {
        scanf("%d", &key);
        RB_NODE *node = create_node(key);
        rb_insert(tree, node);
        --size;
    }
    printf("\n");
    draw_rb_tree(tree);

    destroy_rb_tree(tree);
}

int main() {
    test_rb_tree();
}
