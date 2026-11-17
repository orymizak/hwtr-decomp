#include "common.h"
#include "include_asm.h"
typedef struct { s32 a; s32 b; s32 c; } V12_;
typedef struct { u8 busy; u8 pad[3]; V12_ v; u8 pad2[0x28]; } Q817_;
typedef struct { u8 busy; u8 pad[3]; V12_ v; f32 c; f32 d; u16 h18; u16 h1A; u8 pad2[0x18]; } QAD_;
typedef struct { V12_ pos; f32 c; f32 d; u8 p14[4]; s32 x18; f32 x1C; u8 p20[0x30]; s32 x50; V12_ p54; V12_ p60; f32 f6C; f32 f70; f32 f74; f32 f78; u16 id; u8 p7E[0xA]; } TrAD_;
void *memset(void *, s32, u32);
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

s32 func_80080AD0(TrAD_ *t, u8 i) {
    QAD_ q;
    u8 n;

    if (D_80114800 == 0) {
        return 0;
    }
    memset(t, 0, sizeof(TrAD_));
    t->id = D_80114814[i];
    t->x50 = -1;
    n = func_80080730((Q817_ *)&q, t->id);
    if (n == 0) {
        return 0;
    }
    if (q.busy != 0) {
        return 0;
    }
    t->pos = q.v;
    t->c = q.c;
    t->d = q.d;
    t->x18 = q.h18;
    t->x1C = q.h1A * 0.1f;
    t->id += n;
    t->p54 = t->pos;
    t->p60 = t->pos;
    t->f6C = t->c;
    t->f70 = t->d;
    t->f74 = t->c;
    t->f78 = t->d;
    return 1;
}

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
