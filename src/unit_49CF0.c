#include "common.h"
#include "include_asm.h"
extern u8 D_800F3B3C[];
extern u16 D_800F3B34[];
extern u16 D_800F3B38[];
typedef struct { u8 a[32]; } E32_;
typedef struct { E32_ e[7]; u8 pad[16]; } E240_;
typedef struct { s32 t; u8 a; u8 b; u8 c; u8 d; s32 pad; } E0Cb_;
extern char D_80100A9C[];
extern void (*D_801416DC)(s32, s32, u16, u16, s32);
extern void (*D_801416E0)(s32, s32, s32);
s32 sprintf(char *, const char *, ...);
extern E0Cb_ D_800F3B18[];
extern u8 D_800F3B42;
u8 *func_800960D8(u8);
extern E240_ D_800F3951[];
extern E240_ D_800F3961[];
extern s32 (*D_801416D4)(s32);
s32 func_800A76D8(s32);
extern u8 D_800F3938;
extern u8 D_800F393C;
extern u8 D_800F3A1E;
extern u8 D_800F3A1F;
extern u8 D_8011F68F;
extern u8 D_800F3B10;
extern u8 D_800F3B40;

extern s8 D_800F3B45;

INCLUDE_ASM("asm/nonmatchings/unit_49CF0", func_800A90F0);

s32 func_800A9370(void) {
    s32 (**fp)(s32) = &D_801416D4;
    s32 a = (*fp)(0) & 1;

    return a & (*fp)(1);
}


INCLUDE_ASM("asm/nonmatchings/unit_49CF0", func_800A93B4);

INCLUDE_ASM("asm/nonmatchings/unit_49CF0", func_800A9604);

INCLUDE_ASM("asm/nonmatchings/unit_49CF0", func_800A978C);

void func_800A99D0(u8 idx, u8 flag) {
    u8 buf[80];
    u8 *p;
    u8 *c;
    s32 x;
    s32 color;
    u32 t;

    p = func_800960D8((&D_800F3B40)[idx]);
    D_801416E0(0xFF, 0xFF, 0xFF);
    if (flag == 0) {
        x = 0xCB;
        color = 0x15;
    } else {
        x = 0xCB;
        color = (-(idx != 0) & 0x7F) | 0x15;
    }
    D_801416DC(1, 1, x, color, 2);
    D_801416E0(0, 0xFF, 0);
    t = ((s32 (*)(void))func_800A76D8)() - *(s32 *)(p + (p[0x478] << 2) + 0x44C);
    sprintf(buf, D_80100A9C, t / 60000, (t % 60000) / 1000, ((t % 60000) % 1000) / 100, (((t % 60000) % 1000) % 100) / 10);
    if (flag == 0) {
        x = 0xCF;
        color = 0x12;
    } else {
        x = 0xCF;
        color = 0x7C;
        if (idx == 0) {
            color = 0x12;
        }
    }
    for (c = buf; *c != 0; c++) {
        D_801416DC(1, *c, x, color, 1);
        if (*c < '0' || *c > '9') {
            x += 7;
        } else {
            x += 13;
        }
    }
}

INCLUDE_ASM("asm/nonmatchings/unit_49CF0", func_800A9C00);

INCLUDE_ASM("asm/nonmatchings/unit_49CF0", func_800A9EDC);

INCLUDE_ASM("asm/nonmatchings/unit_49CF0", func_800AA054);

INCLUDE_ASM("asm/nonmatchings/unit_49CF0", func_800AA214);

INCLUDE_ASM("asm/nonmatchings/unit_49CF0", func_800AA80C);

INCLUDE_ASM("asm/nonmatchings/unit_49CF0", func_800AAA20);

INCLUDE_ASM("asm/nonmatchings/unit_49CF0", func_800AABE4);

INCLUDE_ASM("asm/nonmatchings/unit_49CF0", func_800AAD9C);

INCLUDE_ASM("asm/nonmatchings/unit_49CF0", func_800AB38C);

INCLUDE_ASM("asm/nonmatchings/unit_49CF0", func_800AB700);

INCLUDE_ASM("asm/nonmatchings/unit_49CF0", func_800AB8AC);

void func_800ABC80(s32 arg0, s32 arg1, s8 arg2) {
    s32 temp_a0;

    temp_a0 = arg0 & 0xFF;
    *(s32 *)(&D_800F3B10 + (temp_a0 * 4)) = arg1;
    *(u8 *)(&D_800F3B40 + temp_a0) = arg2;
}

INCLUDE_ASM("asm/nonmatchings/unit_49CF0", func_800ABCA4);

void func_800ABEA8(s8 arg0, s32 arg1) {
    *(u8 *)(&D_8011F68F + (arg1 & 0xFF)) = arg0;
}

void func_800ABEBC(s8 arg0) {
    D_800F3B45 = arg0;
}

void func_800ABEC8(s32 arg0, s32 arg1, s8 arg2) {
    s32 temp_a0;
    s32 temp_s0;

    temp_a0 = arg0 & 0xFF;
    temp_s0 = temp_a0 * 0xF0;
    *(u8 *)(&D_800F3A1F + temp_s0) = arg2;
    *(u8 *)(&D_800F3A1E + temp_s0) = 1;
    *(s32 *)(&D_800F3938 + temp_s0) = arg1;
    *(s32 *)(&D_800F393C + temp_s0) = func_800A76D8(temp_a0);
}

INCLUDE_ASM("asm/nonmatchings/unit_49CF0", func_800ABF2C);

void func_800AC6EC(u8 i, s32 n) {
    if (i < D_800F3B42) {
        D_800F3B18[i].t = ((s32 (*)(void))func_800A76D8)();
        D_800F3B18[i].a = func_800960D8((&D_800F3B40)[i])[0x6CC];
        D_800F3B18[i].b = n;
        if (D_800F3B18[i].a + (u8)n >= 11) {
            D_800F3B18[i].b = 10 - D_800F3B18[i].a;
        }
        D_800F3B18[i].c = 9;
        D_800F3B18[i].d = 1;
    }
}


void func_800AC7C8(u8 i, u8 v) {
    u8 j;

    if (D_800F3B3C[i] == v) {
        return;
    }
    if (v != 0) {
        if (D_800F3B34[i] != D_800F3B38[i]) {
            D_800F3B34[i] = D_800F3B38[i];
        } else {
            D_800F3B34[i] = 0;
        }
    }
    D_800F3B3C[i] = v;
    for (j = 0; j < 3; j++) {
        D_800F3951[i].e[j].a[0] = 1;
        D_800F3961[i].e[j].a[0] = 1;
    }
}

