#include "common.h"
#include "include_asm.h"



u8 __osContAddressCrc(u16 addr) {
    u32 temp = 0;
    u32 i;

    for (i = 0x400; i; i >>= 1) {
        temp <<= 1;
        if ((u16)addr & i) {
            if (temp & 0x20) {
                temp ^= 0x14;
            } else {
                ++temp;
            }
        } else {
            if (temp & 0x20) {
                temp ^= 0x15;
            }
        }
    }
    for (i = 5; i != 0; i--) {
        temp <<= 1;
        if (temp & 0x20) {
            temp ^= 0x15;
        }
    }
    return temp & 0x1F;
}

u8 __osContDataCrc(u8 *data) {
    u32 temp = 0;
    u32 i, j;

    for (i = 32; i != 0; i--) {
        for (j = 0x80; j != 0; j >>= 1) {
            temp <<= 1;
            if ((*data & j) != 0) {
                if ((temp & 0x100) != 0) {
                    temp ^= 0x84;
                } else {
                    temp++;
                }
            } else if (temp & 0x100) {
                temp ^= 0x85;
            }
        }
        data++;
    }
    do {
        temp <<= 1;
        if ((temp & 0x100) != 0) {
            temp ^= 0x85;
        }
        i++;
    } while (i < 8U);
    return temp;
}
