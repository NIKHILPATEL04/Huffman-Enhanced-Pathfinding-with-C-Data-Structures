#include "bitreader.h"
#include "node.h"
#include "pq.h"

#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#define STACK   64
#define OPTIONS "i:o:h:"

typedef struct {
    Node *xxStack[64];
    int item_top;
} xStack;

void stack_push(xStack *xxStack, Node *node) {
    if (xxStack->item_top >= STACK - 1) {
        fprintf(stderr, "Error:\n");
        exit(EXIT_FAILURE);
    }
    xxStack->xxStack[++xxStack->item_top] = node;
}

Node *stack_pop(xStack *xxStack) {
    if (xxStack->item_top == -1) {
        fprintf(stderr, "Error:\n");
        exit(EXIT_FAILURE);
    }
    return xxStack->xxStack[xxStack->item_top--];
}

void dehuff_decompress_file(FILE *fout, BitReader *inbuf) {
    uint8_t type1 = bit_read_uint8(inbuf);
    uint8_t type2 = bit_read_uint8(inbuf);
    uint32_t filesize = bit_read_uint32(inbuf);
    uint16_t num_leaves = bit_read_uint16(inbuf);
    xStack xxStack;
    xxStack.item_top = -1;

    uint16_t num_nodes = 2 * num_leaves - 1;
    Node *xNode;

    if (type2 != 'C' || type1 != 'H') {
        fprintf(stderr, "Invalid\n");
        exit(EXIT_FAILURE);
    }

    for (uint32_t index = 0; index < num_nodes; index++) {
        uint8_t bit = bit_read_bit(inbuf);
        if (bit == 1) {
            uint8_t symbol = bit_read_uint8(inbuf);
            xNode = node_create(symbol, 0);
        } else {
            xNode = node_create(0, 0);
            xNode->right = stack_pop(&xxStack);
            xNode->left = stack_pop(&xxStack);
        }
        stack_push(&xxStack, xNode);
    }
    Node *code_tree = stack_pop(&xxStack);
    for (uint32_t index = 0; index < filesize; index++) {
        xNode = code_tree;
        while (1) {
            uint8_t bit = bit_read_bit(inbuf);
            if (bit == 0) {
                xNode = xNode->left;
            } else {
                xNode = xNode->right;
            }
            if ((xNode->left == NULL) && (xNode->right == NULL)) {
                break;
            }
        }
        fputc(xNode->symbol, fout);
    }
}

int main(int argc, char *argv[]) {
    int dehuffer;
    char *input_file = NULL;
    char *output_file = NULL;

    while ((dehuffer = getopt(argc, argv, OPTIONS)) != -1) {
        switch (dehuffer) {
        case 'i': input_file = optarg; break;
        case 'o': output_file = optarg; break;
        case 'h': return 0;
        default: return 1;
        }
    }

    FILE *fout = fopen(output_file, "w");
    if (!fout) {
        fprintf(stderr, "Fata Error:\n");
        return 1;
    }

    if (!output_file || !input_file) {
        fprintf(stderr, "Fatal Error\n");
        return 1;
    }

    BitReader *inbuf = bit_read_open(input_file);
    if (!inbuf) {
        fprintf(stderr, "Fatal Error:\n");
        fclose(fout);
        return 1;
    }

    dehuff_decompress_file(fout, inbuf);

    bit_read_close(&inbuf);
    fclose(fout);

    return 0;
}
