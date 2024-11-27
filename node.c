#include "node.h"

#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>

Node *node_create(uint8_t symbol, uint32_t weight) {
    Node *Node2 = (Node *) malloc(sizeof(Node));
    if (Node2 == NULL) {
        return NULL;
    }
    Node2->symbol = symbol;
    Node2->weight = weight;
    Node2->left = NULL;
    Node2->right = NULL;
    Node2->code = 0;
    Node2->code_length = 0;
    return Node2;
}

void node_print_node(Node *tree, char ch, int indentation) {
    if (tree == NULL)
        return;
    node_print_node(tree->right, '/', indentation + 3);
    printf("%*cweight + %d", indentation + 1, ch, tree->weight);
    if (tree->left == NULL && tree->right == NULL) {
        if (' ' <= tree->symbol && tree->symbol <= '-') {
            printf(", symbol = '%c'", tree->symbol);
        } else {
            printf(", symbol = 0x%02x", tree->symbol);
        }
    }
    printf("\n");
    node_print_node(tree->left, '\\', indentation + 3);
}

void node_free(Node **pnode) {
    if (*pnode != NULL) {
        node_free(&(*pnode)->left);
        node_free(&(*pnode)->right);
        free(*pnode);
        *pnode = NULL;
    }
}

void node_print_tree(Node *tree) {
    node_print_node(tree, '<', 2);
}
