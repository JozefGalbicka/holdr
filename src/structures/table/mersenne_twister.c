
#include "mersenne_twister.h"

// Parameters for  Mersenne Twister

#define SIZE_MT 624
#define EMPTY 0
#define INIT_VALUE 1812433253
#define RES_MASK 0xffffffffU
#define LOW_BIT 0x0U
#define HIGH_BIT 0x9908b0dfU
#define BORDER 397
#define CONST1 0x80000000U
#define CONST2 0x7fffffffU
#define CONST3 0x9d2c5680U
#define CONST4 0xefc60000U

static uint32_t mt[SIZE_MT];
static int mt_index = SIZE_MT;

void initialize_mt(uint32_t seed)
{
    mt[EMPTY] = seed;

    for(unsigned int i = 1; i < SIZE_MT; i++)
        mt[i] = (INIT_VALUE * (mt[i-1] ^ (mt[i-1] >> 30)) + i) & RES_MASK;
}

uint32_t extract_mt()
{
    uint32_t y;
    const uint32_t mag[2] = {LOW_BIT, HIGH_BIT};

    if (mt_index >= SIZE_MT)
    {
        for (unsigned int i = 0; i < SIZE_MT-1; i++)
        {
            y = (mt[i] & CONST1) | (mt[(i + 1) % SIZE_MT] & CONST2);
            mt[i] = mt[(i + BORDER) % SIZE_MT] ^ (y >> 1) ^mag[y & 0x1];    //if lowest bit == 0 0x0 if 1 = 0x990.......
        }

        y = (mt[SIZE_MT - 1] & CONST1) | (mt[0] & CONST2);
        mt[SIZE_MT - 1] = mt[(BORDER - 1)] ^ (y >> 1) ^ mag[y & 0x1];

        mt_index = 0;
    }


    y = mt[mt_index++];

    y ^= (y >> 11);
    y ^= (y << 7) & CONST3;
    y ^= (y << 15) & CONST4;
    y ^= (y >> 18);

    return y;
}




