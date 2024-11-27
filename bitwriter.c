#include "bitwriter.h"

#include <assert.h>
#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

struct BitWriter {
    FILE *underlying_stream;
    uint8_t byte;
    uint8_t bit_position;
};

BitWriter *bit_write_open(const char *filename) {
    BitWriter *wfunction_pointer = (BitWriter *) malloc(sizeof(BitWriter));
    if (wfunction_pointer == NULL) {
        return NULL;
    }

    wfunction_pointer->underlying_stream = fopen(filename, "wb");
    if (wfunction_pointer->underlying_stream == NULL) {
        free(wfunction_pointer);
        return NULL;
    }
    wfunction_pointer->bit_position = 0;
    wfunction_pointer->byte = 0;

    return wfunction_pointer;
}
void bit_write_bit(BitWriter *buf, uint8_t bit) {
    if (buf == NULL) {
        printf("NULL\n");
        return;
    }
    if (buf->bit_position > 7) {
        if (fputc(buf->byte, buf->underlying_stream) == EOF) {
            printf("Error\n");
            return;
        }
        buf->bit_position = 0;
        buf->byte = 0x00;
    }
    if (bit & 1) {
        buf->byte |= (1 << buf->bit_position);
    }

    buf->bit_position = buf->bit_position + 1;
}
void bit_write_close(BitWriter **pbuf) {
    if (*pbuf != NULL && pbuf != NULL) {
        if ((*pbuf)->bit_position > 0) {
            if (fputc((*pbuf)->byte, (*pbuf)->underlying_stream) == EOF) {
                printf("Error\n");
            }
        }
        fseek(((*pbuf)->underlying_stream), 0L, SEEK_SET);
        if (fclose((*pbuf)->underlying_stream) != 0) {
            printf("Error\n");
        }

        free(*pbuf);
        *pbuf = NULL;
    }
}

void bit_write_uint16(BitWriter *buf, uint16_t x) {
    for (int unit = 0; unit < 16; unit++) {
        uint8_t bit = (x >> unit) & 1;
        bit_write_bit(buf, bit);
    }
}

void bit_write_uint8(BitWriter *buf, uint8_t byte) {
    for (int unit = 0; unit < 8; unit++) {
        uint8_t bit = (byte >> unit) & 1;
        bit_write_bit(buf, bit);
    }
}
void bit_write_uint32(BitWriter *buf, uint32_t x) {
    for (int unit = 0; unit < 32; unit++) {
        uint8_t bit = (x >> unit) & 1;
        bit_write_bit(buf, bit);
    }
}
