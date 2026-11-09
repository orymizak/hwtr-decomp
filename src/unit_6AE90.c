#include "common.h"
#include "include_asm.h"
typedef struct { s32 a; s32 b; u8 pad[0x10]; } E18c_;
typedef struct { u8 pad[8]; u8 n1; u8 p1[3]; E18c_ *a1; u8 n2; u8 p2[3]; E18c_ *a2; } TCB_;
typedef struct { u8 a; u8 pad; u16 x; u16 y; u16 x2; u16 y2; } E0A5_;
void func_800CA478(u8 *t);
extern s32 (*D_801416D8)(s32, u8);
s8 func_80086F80(u8 font, u8 a, u8 b);
void func_800CA290(u8 *t);
void func_800CA660(E18c_ *);
void func_800CA7A8(E18c_ *);
void func_800CAE1C(E18c_ *);
void func_800CAF28(E18c_ *);

extern s32 D_80125580;
extern s8 D_80124D70;
extern s8 D_80124D71;
extern s32 (*D_801416A8)();
extern s32 (*D_801416B0)();

void func_800CA290(u8 *t) {
    u8 i;
    s32 w;
    s32 x;
    s32 y;

    if (*(s16 *)(t + 0xA) > 0x140) {
        *(s16 *)(t + 0xA) = 0x140;
    }
    i = 0;
    w = 0;
    x = (*(s16 *)(t + 8) + *(s16 *)(t + 0xA)) / 2;
    y = *(s16 *)(t + 0xC);
    for (; i < t[2] - 1; i++) {
        w += D_801416D8(0, (*(E0A5_ **)(t + 4))[i].a) + func_80086F80(0, (*(E0A5_ **)(t + 4))[i].a, (*(E0A5_ **)(t + 4))[i + 1].a);
    }
    w += D_801416D8(0, (*(E0A5_ **)(t + 4))[i].a);
    w /= 2;
    x -= w;
    (*(E0A5_ **)(t + 4))[0].x2 = x;
    (*(E0A5_ **)(t + 4))[0].y2 = y;
    for (i = 1; i < t[2]; i++) {
        x += D_801416D8(0, (*(E0A5_ **)(t + 4))[i - 1].a) + func_80086F80(0, (*(E0A5_ **)(t + 4))[i - 1].a, (*(E0A5_ **)(t + 4))[i].a);
        (*(E0A5_ **)(t + 4))[i].x2 = x;
        (*(E0A5_ **)(t + 4))[i].y2 = y;
    }
}

void func_800CA478(u8 *t) {
    u8 i;
    s32 w;
    s32 x;
    s32 y;

    if (*(s16 *)(t + 0xA) > 0x140) {
        *(s16 *)(t + 0xA) = 0x140;
    }
    i = 0;
    w = 0;
    x = (*(s16 *)(t + 8) + *(s16 *)(t + 0xA)) / 2;
    y = *(s16 *)(t + 0xC);
    for (; i < t[2] - 1; i++) {
        w += D_801416D8(0, (*(E0A5_ **)(t + 4))[i].a) + func_80086F80(0, (*(E0A5_ **)(t + 4))[i].a, (*(E0A5_ **)(t + 4))[i + 1].a);
    }
    w += D_801416D8(0, (*(E0A5_ **)(t + 4))[i].a);
    w /= 2;
    x -= w;
    (*(E0A5_ **)(t + 4))[0].x2 = x;
    (*(E0A5_ **)(t + 4))[0].y2 = y;
    for (i = 1; i < t[2]; i++) {
        x += D_801416D8(0, (*(E0A5_ **)(t + 4))[i - 1].a) + func_80086F80(0, (*(E0A5_ **)(t + 4))[i - 1].a, (*(E0A5_ **)(t + 4))[i].a);
        (*(E0A5_ **)(t + 4))[i].x2 = x;
        (*(E0A5_ **)(t + 4))[i].y2 = y;
    }
}

INCLUDE_ASM("asm/nonmatchings/unit_6AE90", func_800CA660);

INCLUDE_ASM("asm/nonmatchings/unit_6AE90", func_800CA7A8);

void func_800CA8F0(TCB_ *t) {
    u8 i;

    for (i = 0; i < t->n1; i++) {
        func_800CA290(&t->a1[i]);
        func_800CA660(&t->a1[i]);
    }
    for (i = 0; i < t->n2; i++) {
        func_800CA478(&t->a2[i]);
        func_800CA7A8(&t->a2[i]);
    }
}


INCLUDE_ASM("asm/nonmatchings/unit_6AE90", func_800CA9B8);

INCLUDE_ASM("asm/nonmatchings/unit_6AE90", func_800CAB30);

INCLUDE_ASM("asm/nonmatchings/unit_6AE90", func_800CACA8);

void func_800CAE10(void) {
    D_80125580 = 0;
}

INCLUDE_ASM("asm/nonmatchings/unit_6AE90", func_800CAE1C);

INCLUDE_ASM("asm/nonmatchings/unit_6AE90", func_800CAF28);

void func_800CB034(TCB_ *t) {
    u8 i;

    for (i = 0; i < t->n1; i++) {
        func_800CAE1C(&t->a1[i]);
    }
    for (i = 0; i < t->n2; i++) {
        func_800CAF28(&t->a2[i]);
    }
}


void func_800CB0DC(TCB_ *t) {
    u8 i;

    for (i = 0; i < t->n1; i++) {
        t->a1[i].b = 0;
    }
    for (i = 0; i < t->n2; i++) {
        t->a2[i].b = 0;
    }
}


INCLUDE_ASM("asm/nonmatchings/unit_6AE90", func_800CB168);

void func_800CB268(void) {
  int new_var;
  new_var = 0x80;
  D_80124D70 = new_var;
  D_80124D71 = 0;
}

void func_800CB280(void) {
    D_80124D70 = 0;
}

void func_800CB28C(void) {
    D_801416B0();
    D_801416A8();
}

INCLUDE_ASM("asm/nonmatchings/unit_6AE90", func_800CB2C0);

INCLUDE_ASM("asm/nonmatchings/unit_6AE90", func_800CB4D4);
