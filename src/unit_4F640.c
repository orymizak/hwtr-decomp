#include "common.h"
#include "include_asm.h"
typedef struct { s32 w; u8 p[16]; } E14t_;
typedef struct { u8 pad[0x18]; u8 n; u8 p2[3]; s32 w1C; u8 p3[0x10]; E14t_ e[1]; } TAE_;
extern s32 D_801415B8;
extern u8 D_800F3B80;
extern u8 D_800F3B81;

extern s32 (*D_801416A4)();
extern s32 (*D_801416A8)();
extern s32 D_801345C8;

s32 func_800AEA40(TAE_ *t) {
    u8 i;

    D_801345C8 = 0;
    for (i = 0; i < t->n; i++) {
        if (t->e[i].w == 1) {
            D_801345C8++;
        }
    }
    if (D_801345C8 == 0) {
        D_801345C8 = 1;
    }
    D_801415B8 = t->n;
    if (t->w1C & 0x200) {
        D_800F3B81 = 1;
    } else {
        D_800F3B81 = 0;
    }
    D_800F3B80 = 0;
    return 1;
}

s32 func_800AEAFC(void) {
    return 1;
}

void func_800AEB04(void) {
    D_801416A4();
}

INCLUDE_ASM("asm/nonmatchings/unit_4F640", func_800AEB28);

void func_800AEDFC(void) {
    D_801416A8();
}

INCLUDE_ASM("asm/nonmatchings/unit_4F640", func_800AEE20);

s32 func_800AEF70(void) {
    return D_801345C8;
}
