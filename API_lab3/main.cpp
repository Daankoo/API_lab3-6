#include "header.h"

int main() {
    uint8_t a1[] = { 0xE1, 0x01 };  // '100001111'
    uint8_t a2[] = { 0xEE, 0x00 };  // '011101110'
    uint8_t a3[] = { 0xFF };        // '11111111'
    uint8_t a4[] = { 0xAA };        // '01010101'

    BitWriter w("test.bin");
    w.writeBits(a1, 9);
    w.writeBits(a2, 9);
    w.writeBits(a3, 8);
    w.writeBits(a4, 4);
    w.close();

    uint8_t b1[2], b2[1], b3[2], b4[1];

    BitReader r("test.bin");
    r.readBits(b1, 11);
    r.readBits(b2, 7);
    r.readBits(b3, 8);
    r.readBits(b4, 4);
    r.close();

    printf("b1 (11 bits): %02X %02X\n", b1[0], b1[1]);
    printf("b2  (7 bits): %02X\n", b2[0]);
    printf("b3  (8 bits): %02X\n", b3[0]);
    printf("b4  (4 bits): %02X\n", b4[0]);

    return 0;
}