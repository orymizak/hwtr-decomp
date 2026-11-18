#include "common.h"
#include "ultra64.h"
#include "include_asm.h"

extern s32 D_80127BA0;
extern s32 D_80127BA4;
extern s32 D_800FAEC0;
extern s32 D_80127BB0;
extern s32 D_80127BB4;
extern s32 D_80127BB8;
extern u32 D_80127BBC;

void func_800D23B0(s32 arg0, s32 arg1) {
    func_800D2450((void *) arg0, 0U, arg1);
    alHeapInit(&(*(u8 *)&D_80127BA0), (u32) arg0, (u32) arg1);
}

void func_800D23FC(s32 arg0) {
    alHeapDBAlloc(0, 0, &(*(u8 *)&D_80127BA0), 1, arg0);
}

s32 func_800D242C(void) {
    return D_80127BA4 - D_80127BA0;
}

s32 *func_800D2444(void) {
    return &D_80127BA0;
}

void func_800D2450(void *arg0, u8 arg1, s32 arg2) {
    u8 unused[1];
    u8 *p = arg0;
    s32 i;

    for (i = arg2 - 1; i != -1; i--) {
        *p++ = arg1;
    }
}

void func_800D247C(u8 *dst, u8 *src, s32 n) {
    if (src < dst) {
        dst += n;
        src += n;
        for (n--; n != -1; n--) {
            *--dst = *--src;
        }
    } else {
        for (n--; n != -1; n--) {
            *dst++ = *src++;
        }
    }
}

INCLUDE_ASM("asm/nonmatchings/unit_72FB0", func_800D24F0);

s32 func_800D2594(u32 arg0) {
    if ((u32) (D_80127BBC + 0xB8) < arg0) {
        if (D_800FAEC0 != 0) {
            D_800FAEC0 = 0;
            return D_80127BB4;
        }
        goto block_7;
    }
    if (arg0 < (u32) D_80127BBC) {
        if (D_800FAEC0 != 0) {
            D_800FAEC0 = 0;
            return D_80127BB8;
        }
        goto block_7;
    }
    D_800FAEC0 = 1;
block_7:
    return D_80127BB0;
}
