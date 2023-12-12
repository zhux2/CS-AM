#include <am.h>
#include <klib.h>

#define LSB(x) (x & (1u))

uint64_t mul(uint32_t x, uint32_t y) {
    uint64_t p = 0;
    while (y) {
        if (y & 1) p += x;
        y >>= 1;
        p <<= 1;
    }
    return p;
}

uint32_t modu(uint32_t a, uint32_t b) {
    uint32_t bit = 1;
    while (b < a && bit && !(b & (1UL << 31))) {
        b <<= 1;
        bit <<= 1;
    }
    while (bit) {
        if (a >= b) {
            a -= b;
        }
        bit >>= 1;
        b >>= 1;
    }
    return a;
}

uint32_t divu(uint32_t a, uint32_t b) {
    uint32_t bit = 1;
    uint32_t res = 0;
    while (b < a && bit && !(b & (1UL << 31))) {
        b <<= 1;
        bit <<= 1;
    }
    while (bit) {
        if (a >= b) {
            a -= b;
            res |= bit;
        }
        bit >>= 1;
        b >>= 1;
    }
    return res;
}