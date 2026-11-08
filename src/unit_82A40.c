#include "common.h"
#include "include_asm.h"
typedef struct { s32 status; void *queue; s32 channel; u8 id[32]; u8 label[32]; s32 version; s32 dir_size; s32 inode_table; s32 minode_table; s32 dir_table; s32 inode_start_page; u8 banks; u8 activebank; } PfsB_;
s32 __osContRamWrite(void *, s32, u16, u8 *, s32);



s32 __osPfsSelectBank(PfsB_ *pfs, u8 bank) {
    u8 temp[32];
    s32 i;
    s32 ret = 0;

    for (i = 0; i < 32; i++) {
        temp[i] = bank;
    }
    ret = __osContRamWrite(pfs->queue, pfs->channel, 0x400, temp, 0);
    if (ret == 0) {
        pfs->activebank = bank;
    }
    return ret;
}
