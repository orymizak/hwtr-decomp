#include "common.h"
#include "include_asm.h"
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

INCLUDE_ASM("asm/nonmatchings/unit_4FB80", func_800AF180);
