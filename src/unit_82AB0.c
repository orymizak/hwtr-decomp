#include "common.h"
#include "include_asm.h"
typedef struct { s32 status; void *queue; s32 channel; u8 id[32]; u8 label[32]; s32 version; s32 dir_size; s32 inode_table; s32 minode_table; s32 dir_table; s32 inode_start_page; u8 banks; u8 activebank; } PfsC_;
s32 func_800E1E40(PfsC_ *, u8);
s32 func_800E2940(void *, s32, u16, u8 *);
s32 func_800E40D0(void *, void *, s32);



u16 func_800E1EB0(u8 *ptr, s32 length) {
    s32 i;
    u32 sum;
    u8 *tmp;

    sum = 0;
    tmp = ptr;
    for (i = 0; i < length; i++) {
        sum += *tmp;
        tmp++;
    }
    return sum & 0xFFFF;
}

s32 func_800E1EE4(void *arg0, void *arg1, void *arg2) {
    u16 temp_v1;
    void *temp_a3;
    void *var_a0;

    var_a0 = arg0;
    temp_a3 = var_a0 + 0x1C;
    (*(s16 *)arg2) = 0U;
    (*(s16 *)arg1) = 0U;
    do {
        temp_v1 = (*(s16 *)var_a0);
        (*(s16 *)arg1) = (u16) ((*(s16 *)arg1) + temp_v1);
        var_a0 += 2;
        (*(s16 *)arg2) = (u16) ((*(s16 *)arg2) + ~temp_v1);
    } while ((u32) var_a0 < (u32) temp_a3);
    return 0;
}

INCLUDE_ASM("asm/nonmatchings/unit_82AB0", func_800E1F28);

INCLUDE_ASM("asm/nonmatchings/unit_82AB0", func_800E2214);

INCLUDE_ASM("asm/nonmatchings/unit_82AB0", func_800E2378);

s32 func_800E2544(PfsC_ *pfs) {
    u8 temp[32];
    s32 ret;

    if (pfs->activebank != 0) {
        ret = func_800E1E40(pfs, 0);
        if (ret == 2) {
            ret = func_800E1E40(pfs, 0);
        }
        if (ret != 0) {
            return ret;
        }
    }
    ret = func_800E2940(pfs->queue, pfs->channel, 1, temp);
    if (ret != 0) {
        if (ret != 2) {
            return ret;
        }
        ret = func_800E2940(pfs->queue, pfs->channel, 1, temp);
        if (ret != 0) {
            return ret;
        }
    }
    if (func_800E40D0(pfs->id, temp, 32) != 0) {
        return 2;
    }
    return 0;
}

INCLUDE_ASM("asm/nonmatchings/unit_82AB0", func_800E25FC);
