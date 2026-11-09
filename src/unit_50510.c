#include "common.h"
#include "include_asm.h"
s32 strcmp(const char *, const char *);
typedef struct { u8 d[0x2C]; } E2C5_;
typedef struct { u8 n; u8 pad[3]; E2C5_ *arr; } T2C5_;
typedef struct { u8 pad[0x14]; u32 lo; u32 hi; } Bits5_;
extern s8 D_800F5F90[];
extern u8 D_80123D20[];
void func_800C90DC(void);
void func_800B1824(void);
void func_800B20A4(void);
extern s32 D_80123DB0[];
extern u8 D_800F6040;
extern void *D_80123D30[];
extern u8 D_80100BB0;
void func_800B11FC(void);
typedef struct { u8 d[0x140]; } E140_;
typedef struct { u8 a; u8 pad; u16 x; u16 y; u16 x2; u16 y2; } E0A2_;
typedef struct { u8 pad[2]; u8 n; u8 pad2; E0A2_ *arr; u8 pad3[0xB]; u8 mode; } T0A2_;
typedef struct { s16 x; s16 y; } P16b_;
typedef struct { u8 a; u8 pad; u16 x; u16 y; u16 x2; u16 y2; } E0A5_;
void func_800B192C(u8 *t);
extern u32 D_80123D10[];
extern u32 (*D_80140FEC)(u32);
s8 func_800B0FB0(char *s);
extern E140_ D_80122900[][4];
extern void (*D_8014103C)(u8, s32);
void func_800B1250(s32 x);
extern s32 D_80123D00[];
s32 func_800B133C(s32);
extern u8 D_801206D0;

extern s32 (*D_8014104C)(s32);
s32 func_800B073C();
s32 func_800B0A5C();
extern s32 (*D_80141040)(s32);
s32 func_800B1F00();
s32 func_800B2170(s32);
s32 func_800B1FE4();
s32 func_800B2330(s32);
s32 func_800C9130();

INCLUDE_ASM("asm/nonmatchings/unit_50510", func_800AF910);

s32 func_800AFB74(void) {
    return 1;
}

s32 func_800AFB7C(u32 i) {
    u32 s;

    if (i >= 4) {
        return 4;
    }
    if (D_80123D10[i] & 0x40) {
        return 8;
    }
    s = D_80140FEC(i);
    if (s == 0) {
        return 0;
    } else if (s & 2) {
        return 2;
    } else if (s == 4) {
        return 3;
    } else if (s & 1) {
        return 1;
    } else if (s & 0x20) {
        return 4;
    } else if (s & 8) {
        return 7;
    } else if (s & 0x10) {
        return 6;
    } else if (s & 0x40) {
        return 8;
    } else if (s & 0x80) {
        return 9;
    }
    return 4;
}


INCLUDE_ASM("asm/nonmatchings/unit_50510", func_800AFC40);

INCLUDE_ASM("asm/nonmatchings/unit_50510", func_800AFD1C);

INCLUDE_ASM("asm/nonmatchings/unit_50510", func_800B0024);

u8 *func_800B027C(void) {
    u8 i;

    for (i = 0; i < 4; i++) {
        if (D_80123D20[i] != 0) {
            return i * 0x88C + &D_801206D0;
        }
    }
    return &D_801206D0;
}


void *func_800B02E4(s32 arg0) {
    return (arg0 * 0x88C) + &D_801206D0;
}

E140_ *func_800B0310(u32 i, u32 j) {
    if (i >= 4 || j >= (u32)D_80123D00[i]) {
        return NULL;
    }
    return &D_80122900[i][j];
}

INCLUDE_ASM("asm/nonmatchings/unit_50510", func_800B0364);

s32 func_800B042C(u32 i) {
    s32 r;

    if (i < 4) {
        r = D_80123D00[i];
    } else {
        r = 0;
    }
    return r;
}

INCLUDE_ASM("asm/nonmatchings/unit_50510", func_800B0454);

INCLUDE_ASM("asm/nonmatchings/unit_50510", func_800B0548);

void func_800B0614(void) {

}

void func_800B061C(void) {

}

void func_800B0624(void) {
    do {

    } while (D_8014104C(0x17D78400) == 0);
}

INCLUDE_ASM("asm/nonmatchings/unit_50510", func_800B0654);

void func_800B06CC(Bits5_ *b, u32 mask) {
    u8 i;

    for (i = 0; i < 24; i++, mask >>= 1) {
        if (mask == 0) {
            return;
        }
        if (mask & 1) {
            s8 bit = D_800F5F90[i];

            if (bit >= 32) {
                b->hi |= 1 << (bit - 32);
            } else {
                b->lo |= 1 << bit;
            }
        }
    }
}

INCLUDE_ASM("asm/nonmatchings/unit_50510", func_800B073C);

INCLUDE_ASM("asm/nonmatchings/unit_50510", func_800B0A5C);

s32 func_800B0F74(void) {
    func_800B073C();
    return 1;
}

void func_800B0F94(void) {
    func_800B0A5C();
}

s8 func_800B0FB0(char *s) {
    u8 i;
    u8 n;

    for (i = 0; i < D_800F6040; i++) {
        if (strcmp(s, D_80123D30[i]) == 0) {
            return -1;
        }
    }
    n = D_800F6040;
    D_80123D30[n] = s;
    D_800F6040 = n + 1;
    return n;
}


s8 func_800B1064(char *s) {
    u8 i;

    for (i = 0; i < D_800F6040; i++) {
        if (strcmp(s, D_80123D30[i]) == 0) {
            return i;
        }
    }
    return -1;
}

INCLUDE_ASM("asm/nonmatchings/unit_50510", func_800B10F4);

void func_800B11FC(void) {
    u8 i;

    for (i = 0; i < D_800F6040; i++) {
        D_80123D30[i] = &D_80100BB0;
    }
    D_800F6040 = 0;
}


void func_800B1250(s32 x) {
    s8 r = func_800B0FB0(x);

    if (r >= 0) {
        D_8014103C(r, x);
    }
}

f32 func_800B1294(s32 arg0) {
    return (((f32) arg0 / 1000.0f) * 1000.0f) + -500.0f;
}

INCLUDE_ASM("asm/nonmatchings/unit_50510", func_800B12C4);

f32 func_800B1308(s32 arg0) {
    return (((f32) (arg0 + 0x1F4) / 1000.0f) * 1000.0f) + -500.0f;
}

INCLUDE_ASM("asm/nonmatchings/unit_50510", func_800B133C);

INCLUDE_ASM("asm/nonmatchings/unit_50510", func_800B14F8);

INCLUDE_ASM("asm/nonmatchings/unit_50510", func_800B1694);

INCLUDE_ASM("asm/nonmatchings/unit_50510", func_800B1824);

void func_800B192C(u8 *t) {
    u32 i;

    if (t[0x26] == 2) {
        for (i = 0; i < t[0x18]; i++) {
            (*(E0A5_ **)(t + 0x1C))[i].x = (*(E0A5_ **)(t + 0x1C))[i].x2 + 640;
            (*(E0A5_ **)(t + 0x1C))[i].y = (*(E0A5_ **)(t + 0x1C))[i].y2;
        }
    } else if (t[0x26] == 1) {
        for (i = 0; i < t[0x18]; i++) {
            (*(E0A5_ **)(t + 0x1C))[i].x = (*(E0A5_ **)(t + 0x1C))[i].x2 - 640;
            (*(E0A5_ **)(t + 0x1C))[i].y = (*(E0A5_ **)(t + 0x1C))[i].y2;
        }
    } else if (t[0x26] == 0) {
        for (i = 0; i < t[0x18]; i++) {
            *(P16b_ *)&(*(E0A5_ **)(t + 0x1C))[i].x = *(P16b_ *)&(*(E0A5_ **)(t + 0x1C))[i].x2;
        }
    }
}


INCLUDE_ASM("asm/nonmatchings/unit_50510", func_800B1A3C);

void func_800B1B60(u8 *t) {
    u32 i;

    if (t[0x13] == 2) {
        for (i = 0; i < t[2]; i++) {
            (*(E0A5_ **)(t + 4))[i].x = (*(E0A5_ **)(t + 4))[i].x2 + 640;
            (*(E0A5_ **)(t + 4))[i].y = (*(E0A5_ **)(t + 4))[i].y2;
        }
    } else if (t[0x13] == 1) {
        for (i = 0; i < t[2]; i++) {
            (*(E0A5_ **)(t + 4))[i].x = (*(E0A5_ **)(t + 4))[i].x2 - 640;
            (*(E0A5_ **)(t + 4))[i].y = (*(E0A5_ **)(t + 4))[i].y2;
        }
    } else if (t[0x13] == 0) {
        for (i = 0; i < t[2]; i++) {
            *(P16b_ *)&(*(E0A5_ **)(t + 4))[i].x = *(P16b_ *)&(*(E0A5_ **)(t + 4))[i].x2;
        }
    }
}


void func_800B1C70(T0A2_ *t) {
    u32 i;

    if (t->mode == 2) {
        for (i = 0; i < t->n; i++) {
            t->arr[i].x2 = t->arr[i].x + 640;
            t->arr[i].y2 = t->arr[i].y;
        }
    } else if (t->mode == 1) {
        for (i = 0; i < t->n; i++) {
            t->arr[i].x2 = t->arr[i].x - 640;
            t->arr[i].y2 = t->arr[i].y;
        }
    }
}


void func_800B1D30(u8 *t) {
    u32 i;

    if (t[0x13] == 2) {
        for (i = 0; i < t[2]; i++) {
            (*(E0A5_ **)(t + 4))[i].x = (*(E0A5_ **)(t + 4))[i].x2 + 640;
            (*(E0A5_ **)(t + 4))[i].y = (*(E0A5_ **)(t + 4))[i].y2;
        }
    } else if (t[0x13] == 1) {
        for (i = 0; i < t[2]; i++) {
            (*(E0A5_ **)(t + 4))[i].x = (*(E0A5_ **)(t + 4))[i].x2 - 640;
            (*(E0A5_ **)(t + 4))[i].y = (*(E0A5_ **)(t + 4))[i].y2;
        }
    } else if (t[0x13] == 0) {
        for (i = 0; i < t[2]; i++) {
            *(P16b_ *)&(*(E0A5_ **)(t + 4))[i].x = *(P16b_ *)&(*(E0A5_ **)(t + 4))[i].x2;
        }
    }
}


void func_800B1E40(T0A2_ *t) {
    u32 i;

    if (t->mode == 2) {
        for (i = 0; i < t->n; i++) {
            t->arr[i].x2 = t->arr[i].x + 640;
            t->arr[i].y2 = t->arr[i].y;
        }
    } else if (t->mode == 1) {
        for (i = 0; i < t->n; i++) {
            t->arr[i].x2 = t->arr[i].x - 640;
            t->arr[i].y2 = t->arr[i].y;
        }
    }
}


INCLUDE_ASM("asm/nonmatchings/unit_50510", func_800B1F00);

INCLUDE_ASM("asm/nonmatchings/unit_50510", func_800B1FE4);

INCLUDE_ASM("asm/nonmatchings/unit_50510", func_800B20A4);

INCLUDE_ASM("asm/nonmatchings/unit_50510", func_800B2170);

INCLUDE_ASM("asm/nonmatchings/unit_50510", func_800B2330);

INCLUDE_ASM("asm/nonmatchings/unit_50510", func_800B2440);

INCLUDE_ASM("asm/nonmatchings/unit_50510", func_800B2540);

INCLUDE_ASM("asm/nonmatchings/unit_50510", func_800B2640);

INCLUDE_ASM("asm/nonmatchings/unit_50510", func_800B271C);

INCLUDE_ASM("asm/nonmatchings/unit_50510", func_800B2844);

void func_800B2A20(void *arg0) {
    s32 temp_a0;
    s32 var_s0;

    var_s0 = 0;
    if ((*(u8 *)((u8 *)(arg0) + 0x20)) != 0) {
        do {
            temp_a0 = *(s32 *)(((var_s0 & 0xFF) * 0x10) + (*(s32 *)((u8 *)(arg0) + 0x24)));
            var_s0 += 1;
            func_800B1250(temp_a0);
        } while ((u32) (var_s0 & 0xFF) < (u8) (*(u8 *)((u8 *)(arg0) + 0x20)));
    }
}

void func_800B2A84(void) {
    s32 temp_a0;
    s32 var_s0;

    var_s0 = 0;
    do {
        temp_a0 = var_s0 & 0xFF;
        var_s0 += 1;
        D_80141040(temp_a0);
    } while ((u32) (var_s0 & 0xFF) < 0x20U);
    func_800B11FC();
}

void func_800B2AD0(s32 arg0) {
    func_800B1F00();
    func_800B2170(arg0);
}

void func_800B2AFC(s32 arg0) {
    func_800B1FE4();
    func_800B2330(arg0);
}

void func_800B2B28(T2C5_ *t) {
    u8 i;

    for (i = 0; i < t->n; i++) {
        func_800B133C((s32)&t->arr[i]);
        func_800B192C(&t->arr[i]);
    }
}


void func_800B2BAC(void *arg0) {
    u16 var_v0;
    s32 var_s0;

    var_s0 = 0;
    if ((*(u8 *)((u8 *)(arg0) + 0x0)) != 0) {
        var_v0 = 0 & 0xFF;
        do {
            var_v0 = var_s0 & 0xFF;
            func_800B133C((*(s32 *)((u8 *)(arg0) + 0x4)) + (var_v0 * 0x2C));
            var_s0 += 1;
        } while ((u32) (var_s0 & 0xFF) < (*(u8 *)((u8 *)(arg0) + 0x0)));
    }
}

void func_800B2C1C(void) {
    u8 i;

    func_800C90DC();
    func_800B1824();
    func_800B20A4();
    for (i = 0; i < 33; i++) {
        D_80123DB0[i] = 0;
    }
}

void func_800B2C78(void) {
    func_800C9130();
}

INCLUDE_ASM("asm/nonmatchings/unit_50510", func_800B2C94);

INCLUDE_ASM("asm/nonmatchings/unit_50510", func_800B2CE0);

INCLUDE_ASM("asm/nonmatchings/unit_50510", func_800B2DA0);

INCLUDE_ASM("asm/nonmatchings/unit_50510", func_800B2E60);

INCLUDE_ASM("asm/nonmatchings/unit_50510", func_800B2F20);
