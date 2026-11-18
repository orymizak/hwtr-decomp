#include "common.h"
#include "include_asm.h"
#include "car.h"
extern u8 D_800F3B3C[];
extern u16 D_800F3B34[];
extern u16 D_800F3B38[];
typedef struct { u8 a[32]; } E32_;
typedef struct { E32_ e[7]; u8 pad[16]; } E240_;
typedef struct { s32 t; u8 a; u8 b; u8 c; u8 d; s32 pad; } E0Cb_;
typedef struct { u8 p0[8]; s16 h8; u8 pA[2]; u8 bC; u8 bD; u8 pE[2]; } El10_;
typedef struct { u8 on; u8 p1[0x27]; } Blk28_;
typedef struct { s32 t; s32 w4; s32 w8; u8 pC[4]; El10_ el[6]; Blk28_ blk[3]; u8 pE8; u8 flag; u8 bEA; u8 bEB; u8 pEC[4]; } HudF0_;
extern u8 D_800F3B43;
extern char D_80100A94[];
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
Car *func_800960D8(u8);
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

void func_800A9604(u8 idx, u8 flag) {
    u8 buf[80];
    Car *p;
    u8 *c;
    s32 x;
    s32 y;
    s32 laps;

    p = func_800960D8((&D_800F3B40)[idx]);
    if (p->flags & 0x4000) {
        return;
    }
    laps = p->lapsDone;
    D_801416E0(0xFF, 0xFF, 0xFF);
    if (flag == 0) {
        x = 0x103;
        y = 0xD7;
    } else {
        x = 0x103;
        y = 0xD7;
        if (idx == 0) {
            y = 0x6B;
        }
    }
    D_801416DC(1, 0, x, y, 2);
    if (laps == 0) {
        D_801416E0(0xFF, 0xFF, 0);
    } else {
        D_801416E0(0, 0xFF, 0);
    }
    sprintf(buf, D_80100A94, laps + 1, D_800F3B43);
    if (flag == 0) {
        x = 0x108;
        y = 0xD4;
    } else {
        x = 0x108;
        y = 0xD4;
        if (idx == 0) {
            y = 0x68;
        }
    }
    for (c = buf; *c != 0; c++) {
        D_801416DC(1, *c, x, y, 1);
        if (*c < '0' || *c > '9') {
            x += 9;
        } else {
            x += 13;
        }
    }
}

void func_800A978C(u8 idx, u8 flag) {
    u8 buf[80];
    Car *p;
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
    t = D_800F3B30 - (((s32 (*)(void))func_800A76D8)() - p->unk440);
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
    Car *p;
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
    t = ((s32 (*)(void))func_800A76D8)() - p->lapStart[p->lap - 1];
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

extern char D_80100A90[];
u32 strlen(const char *);

/* Draws car idx's unk4B0 counter right-aligned on the HUD. */
void func_800A9EDC(u8 idx, u8 flag) {
    u8 buf[80];
    u8 *c;
    s32 x;
    s32 y;
    s8 i;

    D_801416E0(0xFF, 0xFF, 0xFF);
    if (flag == 0) {
        x = 0xCB;
        y = 0x23;
    } else {
        x = 0xCB;
        y = 0x8D;
        if (idx == 0) {
            y = 0x23;
        }
    }
    D_801416DC(1, 1, x, y, 2);
    D_801416E0(0xFF, 0, 0);
    sprintf(buf, D_80100A90, func_800960D8((&D_800F3B40)[idx])->unk4B0);
    if (flag == 0) {
        x = 0x11E;
        y = 0x20;
    } else {
        x = 0x11E;
        y = 0x8A;
        if (idx == 0) {
            y = 0x20;
        }
    }
    i = strlen(buf) - 1;
    for (c = &buf[i]; i >= 0; i--, c--) {
        D_801416DC(1, *c, x, y, 1);
        if (*c < '0' || *c > '9') {
            x -= 7;
        } else {
            x -= 13;
        }
    }
}

INCLUDE_ASM("asm/nonmatchings/unit_49CF0", func_800AA054);

INCLUDE_ASM("asm/nonmatchings/unit_49CF0", func_800AA214);

extern u8 D_800F3B3F;
/* Draws the HUD elements enabled in D_800F3B34[idx] for player idx. */
s32 func_800AA80C(u8 idx) {
    u8 two;
    u8 done;
    Car *p;
    u16 x;
    u16 y;

    two = D_800F3B42 == 2;
    D_801416E0(0xFF, 0xFF, 0xFF);
    done = 0;
    if (D_800F3B34[idx] & 1) {
        func_800A93B4(idx, two);
    }
    if (D_800F3B34[idx] & 0x10) {
        p = func_800960D8((&D_800F3B40)[idx]);
        D_801416E0(0xFF, 0xFF, 0xFF);
        if (p->unk6DC != 0) {
            if ((u8)two == 0) {
                x = 0x102;
                y = 0xB2;
            } else {
                x = 0x102;
                y = 0xB2;
                if (idx == 0) {
                    y = 0x46;
                }
            }
            D_801416DC(1, (u8)(p->unk6DC + 10), x, y, 2);
        }
    }
    if (D_800F3B34[idx] & 2) {
        if (D_800F3B3F != 0) {
            func_800A978C(idx, two);
        } else {
            func_800A99D0(idx, two);
        }
        done = func_800A9C00(idx, two);
    }
    if (D_800F3B34[idx] & 0x200) {
        func_800A978C(idx, two);
        func_800A9EDC(idx, two);
    }
    if (D_800F3B34[idx] & 8) {
        func_800A9604(idx, two);
    }
    if (((D_800F3B34[idx] & 4) != 0) & (done == 0)) {
        func_800AA054(idx, two);
    }
    if (D_800F3B34[idx] & 0x20) {
        func_800AA214(idx, two);
    }
    return 1;
}

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

void func_800ABC80(u8 i, s32 v, s8 id) {
    ((s32 *)&D_800F3B10)[i] = v;
    (&D_800F3B40)[i] = id;
}

INCLUDE_ASM("asm/nonmatchings/unit_49CF0", func_800ABCA4);

void func_800ABEA8(s8 arg0, s32 arg1) {
    *(u8 *)(&D_8011F68F + (arg1 & 0xFF)) = arg0;
}

void func_800ABEBC(s8 arg0) {
    D_800F3B45 = arg0;
}

void func_800ABEC8(u8 i, s32 v, s8 b) {
    D_800F3934[i].bEB = b;
    D_800F3934[i].bEA = 1;
    D_800F3934[i].w4 = v;
    D_800F3934[i].w8 = func_800A76D8(i);
}

INCLUDE_ASM("asm/nonmatchings/unit_49CF0", func_800ABF2C);

void func_800AC6EC(u8 i, s32 n) {
    if (i < D_800F3B42) {
        D_800F3B18[i].t = ((s32 (*)(void))func_800A76D8)();
        D_800F3B18[i].a = func_800960D8((&D_800F3B40)[i])->n6CC;
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

