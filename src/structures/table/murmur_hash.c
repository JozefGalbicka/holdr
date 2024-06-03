// implementation of MurmurHash3 algorithm
#include "murmur_hash.h"

#define CONST_1 0xcc9e2d51
#define CONST_2 0x1b873593
#define SIZE4 4
#define CONST_3 0xe6546b64
#define CONST_4 0x85ebca6b
#define CONST_5 0xc2b2ae35

static inline uint32_t murmur_32_scramble(uint32_t k)
{
    k *= CONST_1;
    k = (k << 15) | (k >> 17);
    k *= CONST_2;
    return k;
}

uint32_t murmur3_32(const uint8_t* key, size_t len, uint32_t seed)
{
    uint32_t h = seed;
    uint32_t k;

    for (size_t i = len >> 2; i; i--)
    {
        memcpy(&k, key, SIZE4);
        key += SIZE4;
        h ^= murmur_32_scramble(k);
        h = (h << 13) | (h >> 19);
        h = h * 5 + CONST_3;
    }

    k = 0;
    for (size_t i = len & 3; i; i--)
    {
        k <<= 8;
        k |= key[i - 1];
    }

    h ^= murmur_32_scramble(k);
    h ^= len;
    h ^= h >> 16;
    h *= CONST_4;
    h ^= h >> 13;
    h *= CONST_5;
    h ^= h >> 16;
    return h;
}