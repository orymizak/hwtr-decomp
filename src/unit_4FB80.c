#include "common.h"
#include "include_asm.h"
void func_80098584(u8 *, s32);
void func_800AF8FC(u8 *, s32);
void func_80088004(u8 *, s32);
void func_800A6D98(u8 *, s32);
void func_80090FA8(u8 *, s32);
extern s32 D_8011F6A4;
extern u8 D_8011F6B0[];
void *memset(void *dst, s32 value, u32 size);

extern s32 D_8011F6A0;

s32 func_800AEF80(void) {
    D_8011F6A0 = 0;
    D_8011F6A4 = 0;
    memset(D_8011F6B0, 0, 0x1000);
    return 1;
}


s32 func_800AEFBC(void) {
    D_8011F6A0 = 0;
    D_8011F6A4 = 0;
    memset(D_8011F6B0, 0, 0x1000);
    return 1;
}


INCLUDE_ASM("asm/nonmatchings/unit_4FB80", func_800AEFF8);

s32 func_800AF174(void) {
    return D_8011F6A0;
}

void func_800AF180(u32 n) {
    u8 *h = D_8011F6B0;
    u8 *q;
    u32 i;

    for (i = 0; i < n; i++) {
        h += ((s32 *)h)[5] + 0x18;
    }
    q = h + 0x18;
    func_80098584(q, ((s32 *)h)[0]);
    q += ((s32 *)h)[0];
    func_800AF8FC(q, ((s32 *)h)[1]);
    q += ((s32 *)h)[1];
    func_80088004(q, ((s32 *)h)[2]);
    q += ((s32 *)h)[2];
    func_800A6D98(q, ((s32 *)h)[3]);
    func_80090FA8(q + ((s32 *)h)[3], ((s32 *)h)[4]);
}

