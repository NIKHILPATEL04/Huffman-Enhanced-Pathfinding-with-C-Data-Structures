#include "bitreader.h"

#include <assert.h>
#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

struct BitReader {
    FILE *underlying_stream;
    uint8_t byte;
    uint8_t bit_position;
};
void bit_read_close(BitReader **pbuf) {
    if (*pbuf != NULL && pbuf != NULL) {
        fseek(((*pbuf)->underlying_stream), 0L, SEEK_SET);
        if (fclose((*pbuf)->underlying_stream) != 0) {
            printf("Error\n");
            exit(EXIT_FAILURE);
        }
        free(*pbuf);
        *pbuf = NULL;
    } else {
        printf("Error\n");
        exit(EXIT_FAILURE);
    }
}
BitReader *bit_read_open(const char *filename) {
    BitReader *rfunction_pointer = (BitReader *) malloc(sizeof(BitReader));
    if (rfunction_pointer == NULL) {
        return NULL;
    }

    rfunction_pointer->underlying_stream = fopen(filename, "rb");
    if (rfunction_pointer->underlying_stream == NULL) {
        free(rfunction_pointer);
        return NULL;
    }
    rfunction_pointer->bit_position = 8;
    rfunction_pointer->byte = 0;

    return rfunction_pointer;
}

uint8_t bit_read_bit(BitReader *buf) {
    if (buf == NULL) {
        printf("Fatal error\n");
        exit(EXIT_FAILURE);
    }

    if (buf->bit_position > 7) {
        int freader = fgetc(buf->underlying_stream);
        if (freader == EOF) {
            return (uint8_t) freader;
        }
        buf->bit_position = 0;
        buf->byte = (uint8_t) freader;
    }
    uint8_t x = (buf->byte >> buf->bit_position) & 1;
    buf->bit_position += 1;

    return x;
}

uint8_t bit_read_uint8(BitReader *buf) {
    if (buf == NULL) {
        printf("ERROR: Buffer null\n");
        exit(EXIT_FAILURE);
    }

    uint8_t byte = 0x00;
    for (int unit = 0; unit < 8; unit++) {
        uint8_t position = bit_read_bit(buf);
        byte |= (position << unit);
    }

    return byte;
}

uint32_t bit_read_uint32(BitReader *buf) {
    if (buf == NULL) {
        printf("Error :Buffer null\n");
        exit(EXIT_FAILURE);
    }

    uint32_t word = 0x00000000;
    for (int unit = 0; unit < 32; unit++) {
        uint32_t position = bit_read_bit(buf);
        word |= (position << unit);
    }

    return word;
}

uint16_t bit_read_uint16(BitReader *buf) {
    if (buf == NULL) {
        printf("ERROR: Buffer null\n");
        exit(EXIT_FAILURE);
    }

    uint16_t word = 0x0000;
    for (int unit = 0; unit < 16; unit++) {
        uint16_t position = bit_read_bit(buf);
        word |= (position << unit);
    }

    return word;
}

int bit_read(BitReader *bf) {
    if (bf == NULL) {
        return 1;
    }
    return feof(bf->underlying_stream);
}
