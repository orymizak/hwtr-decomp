#include "common.h"
#include "include_asm.h"
typedef struct { s32 a; s32 b; s32 c; } V12_;
typedef struct { u8 busy; u8 pad[3]; V12_ v; u8 pad2[0x28]; } Q817_;
extern u16 D_80114814[];
u8 func_80080730(Q817_ *, u16);

s32 Memory_Free(s32);
extern s32 D_80114800;
extern s32 D_8011480C;

INCLUDE_ASM("asm/nonmatchings/unit_21150", func_80080550);

s32 func_800806FC(void) {
    if (D_80114800 != 0) {
        Memory_Free(D_80114800);
    }
    D_80114800 = 0;
    return 1;
}

INCLUDE_ASM("asm/nonmatchings/unit_21150", func_80080730);

INCLUDE_ASM("asm/nonmatchings/unit_21150", func_80080AD0);

INCLUDE_ASM("asm/nonmatchings/unit_21150", func_80080C20);

INCLUDE_ASM("asm/nonmatchings/unit_21150", func_80081384);

s32 func_800817A8(u8 i, V12_ *out) {
    Q817_ q;

    if (D_80114800 == 0) {
        return 0;
    }
    if (func_80080730(&q, D_80114814[i]) == 0) {
        return 0;
    }
    if (q.busy != 0) {
        return 0;
    }
    *out = q.v;
    return 1;
}


s32 func_8008182C(void) {
    return D_8011480C;
}
