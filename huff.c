#include "bitreader.h"
#include "bitwriter.h"
#include "node.h"
#include "pq.h"
#define OPTIONS "i:o:h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

typedef struct Code {
    uint64_t code;
    uint8_t code_length;
} xCode;

uint32_t fill_histogram(const char *fin, uint32_t *histogram) {
    for (int index = 0; index < 256; ++index) {
        histogram[index] = 0;
    }
    ++histogram[0x00];
    ++histogram[0xff];
    BitReader *br = bit_read_open(fin);
    uint32_t file_size = 0;
    uint8_t byte_size;
    while (!bit_read(br)) {
        byte_size = bit_read_uint8(br);
        if (bit_read(br)) {
            break;
        }

        file_size = 1 + file_size;
        histogram[byte_size] = 1 + histogram[byte_size];
    }
    bit_read_close(&br);
    return file_size;
}

Node *create_tree(uint32_t *histogram, uint16_t *num_leaves) {
    PriorityQueue *priorityqueue = pq_create();
    for (int index = 0; index < 256; index++) {
        if (histogram[index] > 0) {
            Node *newestnode = node_create((uint8_t) index, histogram[index]);
            enqueue(priorityqueue, newestnode);
            (*num_leaves)++;
        }
    }
    while (!pq_size_is_1(priorityqueue)) {
        Node *left = dequeue(priorityqueue);
        Node *right = dequeue(priorityqueue);
        Node *newestnode = node_create(0, left->weight + right->weight);
        newestnode->right = right;
        newestnode->left = left;
        enqueue(priorityqueue, newestnode);
    }
    Node *xxx = dequeue(priorityqueue);
    pq_free(&priorityqueue);

    return xxx;
}
void huff_write_tree(BitWriter *outbuf, Node *node) {

    if (node->left == NULL) {
        bit_write_bit(outbuf, 1);
        bit_write_uint8(outbuf, node->symbol);
    } else {
        huff_write_tree(outbuf, node->left);
        huff_write_tree(outbuf, node->right);
        bit_write_bit(outbuf, 0);
    }
}
void fill_code_table(xCode *code_table, Node *node, uint64_t code, uint8_t code_length) {

    if (node->left != NULL && node->right != NULL) {
        fill_code_table(code_table, node->left, code, code_length + 1);
        code |= 1 << code_length;
        fill_code_table(code_table, node->right, code, code_length + 1);
    } else {
        code_table[node->symbol].code = code;
        code_table[node->symbol].code_length = code_length;
    }
}

void huff_compress_file(BitWriter *outbuf, FILE *fin, uint32_t file_size, uint16_t num_leaves,
    Node *code_tree, xCode *code_table) {
    bit_write_uint8(outbuf, 'H');
    bit_write_uint8(outbuf, 'C');
    bit_write_uint32(outbuf, file_size);
    bit_write_uint16(outbuf, num_leaves);
    huff_write_tree(outbuf, code_tree);

    while (true) {
        int b = fgetc(fin);
        if (b == EOF) {
            break;
        }
        uint64_t code = code_table[b].code;
        uint8_t code_length = code_table[b].code_length;
        for (uint8_t index = 0; index < code_length; index++) {
            bit_write_bit(outbuf, code & 1);
            code >>= 1;
        }
    }
}

int main(int argc, char *argv[]) {
    int opt;
    char *fileinput = NULL;
    char *fileoutput = NULL;
    while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
        switch (opt) {
        case 'i': fileinput = optarg; break;
        case 'o': fileoutput = optarg; break;
        case 'h':
        default: printf("Wrong use"); return EXIT_FAILURE;
        }
    }

    if (!fileoutput || !fileinput) {
        fprintf(stderr, "Error");
        return 1;
    }
    uint32_t histogram[256] = { 0 };
    uint32_t file_size = fill_histogram(fileinput, histogram);
    uint16_t num_leaves = 0;
    Node *code_tree = create_tree(histogram, &num_leaves);
    FILE *xfile = fopen(fileinput, "rb");
    BitWriter *outbuf = bit_write_open(fileoutput);

    xCode code_table[256];
    memset(code_table, 0, sizeof(code_table));
    fill_code_table(code_table, code_tree, 0, 0);
    fseek(xfile, 0, SEEK_SET);
    huff_compress_file(outbuf, xfile, file_size, num_leaves, code_tree, code_table);

    bit_write_close(&outbuf);
    fclose(xfile);
    node_free(&code_tree);

    return 0;
}
