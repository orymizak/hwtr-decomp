#include "common.h"
#include "include_asm.h"
extern u8 D_800F3B3C[];
extern u16 D_800F3B34[];
extern u16 D_800F3B38[];
typedef struct { u8 a[32]; } E32_;
typedef struct { E32_ e[7]; u8 pad[16]; } E240_;
typedef struct { s32 t; u8 a; u8 b; u8 c; u8 d; s32 pad; } E0Cb_;
typedef struct { u8 p0[8]; s16 h8; u8 pA[2]; u8 bC; u8 bD; u8 pE[2]; } El10_;
typedef struct { u8 on; u8 p1[0x27]; } Blk28_;
typedef struct { s32 t; u8 p4[0xC]; El10_ el[6]; Blk28_ blk[3]; u8 pE8; u8 flag; u8 pEA[6]; } HudF0_;
extern HudF0_ D_800F3934[];
void func_800AAA20(Blk28_ *, El10_ *, s32, s32);
void func_800AABE4(Blk28_ *, El10_ *, s32, s32);
extern s32 D_800F3B30;
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

void func_800A978C(u8 idx, u8 flag) {
    u8 buf[80];
    u8 *p;
    u8 *c;
    s32 x;
    s32 color;
    s32 t;

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
    t = D_800F3B30 - (((s32 (*)(void))func_800A76D8)() - *(s32 *)(p + 0x440));
    if (t < 0) {
        t = 0;
    }
    sprintf(buf, D_80100A9C, t / 60000, (u32)(t % 60000) / 1000, ((u32)(t % 60000) % 1000) / 100, (((u32)(t % 60000) % 1000) % 100) / 10);
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

void func_800AB8AC(u8 p) {
    El10_ *e0;
    El10_ *e1;
    El10_ *e2;
    El10_ *e3;
    El10_ *e4;
    El10_ *e5;

    e0 = &D_800F3934[p].el[0];
    e1 = e0 + 1;
    e2 = e0 + 2;
    e3 = e0 + 3;
    e4 = e0 + 4;
    e5 = e0 + 5;
    if (D_800F3934[p].flag == 0) {
        return;
    }
    if (e0->bD && e2->bD && e4->bD && e1->bD && e3->bD && e5->bD) {
        D_800F3934[p].flag = 0;
        return;
    }
    if (D_800F3934[p].t == 0) {
        if (e0->bC || e2->bC || e4->bC || e1->bC || e3->bC || e5->bC) {
            D_800F3934[p].t = ((s32 (*)(void))func_800A76D8)();
            e0->h8 = 0;
            e2->h8 = 0;
            e4->h8 = 0;
            e1->h8 = 0;
            e3->h8 = 0;
            e5->h8 = 0;
        }
    } else if ((u32)(((s32 (*)(void))func_800A76D8)() - D_800F3934[p].t) > 1000) {
        e0->h8 = -20;
        e2->h8 = -20;
        e4->h8 = -20;
        e1->bD = 1;
        e3->bD = 1;
        e5->bD = 1;
    }
    D_801416E0(0xFF, 0xFF, 0);
    if (D_800F3934[p].blk[0].on) {
        if (!e0->bD && !e1->bD) {
            func_800AAA20(&D_800F3934[p].blk[0], e0, 2, 1);
            func_800AABE4(&D_800F3934[p].blk[0], e1, 2, 0);
        } else {
            func_800AAA20(&D_800F3934[p].blk[0], e0, 1, 0);
        }
    }
    if (D_800F3934[p].blk[1].on) {
        if (!e0->bD && !e1->bD) {
            func_800AAA20(&D_800F3934[p].blk[1], e2, 2, 1);
            func_800AABE4(&D_800F3934[p].blk[1], e3, 2, 0);
        } else {
            func_800AAA20(&D_800F3934[p].blk[1], e2, 1, 0);
        }
    }
    if (D_800F3934[p].blk[2].on) {
        if (!e0->bD && !e1->bD) {
            func_800AAA20(&D_800F3934[p].blk[2], e4, 2, 1);
            func_800AABE4(&D_800F3934[p].blk[2], e5, 2, 0);
        } else {
            func_800AAA20(&D_800F3934[p].blk[2], e4, 1, 0);
        }
    }
}

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

