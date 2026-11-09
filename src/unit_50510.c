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
typedef struct { s32 pad0; s16 a; s16 b; s16 c; u8 kind; u8 padB; u16 d; u16 padE; } Src10_;
typedef struct { f32 p[3]; f32 q[3]; f32 v[3]; f32 w[3]; u16 e; u16 f; } Dst34_;
extern s32 (*D_80141048)(void);
void func_800B2440(s32, f32);
void func_800B2540(s32, f32);
void func_800B2640(s32, f32);
void func_800B271C(s32, f32);
f32 func_800B12C4(s32);
s32 func_800B2170(s32 arg0);
void func_800B133C(u8 *t);
s32 func_800C8EE4(s32, u8, u8);
s32 func_800C902C(s32, u8);
void func_800B192C(u8 *t);
extern u32 D_80123D10[];
extern u32 (*D_80140FEC)(u32);
s8 func_800B0FB0(char *s);
extern E140_ D_80122900[][4];
extern void (*D_8014103C)(u8, s32);
void func_800B1250(s32 x);
extern s32 D_80123D00[];
extern u8 D_801206D0;

extern s32 (*D_8014104C)(s32);
s32 func_800B073C();
s32 func_800B0A5C();
extern s32 (*D_80141040)(s32);
s32 func_800B1F00();
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

void func_800B133C(u8 *t) {
    s32 i;
    s32 w;
    s16 x;
    s32 y;
    s32 v;

    w = 0;
    if (t[0x18] != 0) {
        for (i = 0; i < t[0x18] - 1; i++) {
            if (t[0x28] == 2) {
                w += 0x23;
            } else {
                w += ((s32 (*)(u8, u8, u8))func_800C8EE4)(t[0x28], (*(E0A5_ **)(t + 0x1C))[i].a, (*(E0A5_ **)(t + 0x1C))[i + 1].a);
            }
        }
        if (t[0x28] == 2) {
            w += 0x23;
        } else {
            w += ((s32 (*)(u8, u8))func_800C902C)(t[0x28], (*(E0A5_ **)(t + 0x1C))[i].a);
        }
        w /= 2;
        if (t[0x27] == 0) {
            v = *(s16 *)(t + 0x20);
        } else {
            v = *(s16 *)(t + 0x20) - w;
        }
        y = *(s16 *)(t + 0x22);
        x = v;
        (*(E0A5_ **)(t + 0x1C))[0].x2 = x;
        (*(E0A5_ **)(t + 0x1C))[0].y2 = y;
        *(P16b_ *)&(*(E0A5_ **)(t + 0x1C))[0].x = *(P16b_ *)&(*(E0A5_ **)(t + 0x1C))[0].x2;
        for (i = 1; i < t[0x18]; i++) {
            if (t[0x28] == 2) {
                x += 0x23;
            } else {
                x += ((s32 (*)(u8, u8, u8))func_800C8EE4)(t[0x28], (*(E0A5_ **)(t + 0x1C))[i - 1].a, (*(E0A5_ **)(t + 0x1C))[i].a);
            }
            (*(E0A5_ **)(t + 0x1C))[i].x2 = x;
            (*(E0A5_ **)(t + 0x1C))[i].y2 = y;
            *(P16b_ *)&(*(E0A5_ **)(t + 0x1C))[i].x = *(P16b_ *)&(*(E0A5_ **)(t + 0x1C))[i].x2;
        }
    }
}

void func_800B14F8(u8 *t) {
    u8 i;
    s32 w;
    s32 x;
    s32 y;

    i = 0;
    w = 0;
    x = (*(s16 *)(t + 8) + *(s16 *)(t + 0xA)) / 2;
    y = *(s16 *)(t + 0xC);
    for (; i < t[2] - 1; i++) {
        w += func_800C8EE4(0, (*(E0A5_ **)(t + 4))[i].a, (*(E0A5_ **)(t + 4))[i + 1].a);
    }
    w += func_800C902C(0, (*(E0A5_ **)(t + 4))[i].a);
    w /= 2;
    x -= w;
    (*(E0A5_ **)(t + 4))[0].x2 = x;
    (*(E0A5_ **)(t + 4))[0].y2 = y;
    *(P16b_ *)&(*(E0A5_ **)(t + 4))[0].x = *(P16b_ *)&(*(E0A5_ **)(t + 4))[0].x2;
    for (i = 1; i < t[2]; i++) {
        x += func_800C8EE4(0, (*(E0A5_ **)(t + 4))[i - 1].a, (*(E0A5_ **)(t + 4))[i].a);
        (*(E0A5_ **)(t + 4))[i].x2 = x;
        (*(E0A5_ **)(t + 4))[i].y2 = y;
        *(P16b_ *)&(*(E0A5_ **)(t + 4))[i].x = *(P16b_ *)&(*(E0A5_ **)(t + 4))[i].x2;
    }
}

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

s32 func_800B2170(s32 arg0) {
    u8 *t;
    s32 i;
    f32 off;
    Src10_ *s;
    Dst34_ *d;

    t = (u8 *) arg0;
    for (i = 0; i < t[0x20]; i++) {
        off = 500.0f - ((f32 (*)(s32 arg0))func_800B1294)((*(Src10_ **) (t + 0x24))[i].a);
        (*(Dst34_ **) (t + 0x28))[i].q[0] = ((f32 (*)(s32 arg0))func_800B1294)((*(Src10_ **) (t + 0x24))[i].a);
        (*(Dst34_ **) (t + 0x28))[i].q[1] = func_800B12C4((*(Src10_ **) (t + 0x24))[i].b);
        (*(Dst34_ **) (t + 0x28))[i].q[2] = ((f32 (*)(s32 arg0))func_800B1308)((*(Src10_ **) (t + 0x24))[i].c);
        switch ((*(Src10_ **) (t + 0x24))[i].kind) {
            case 0:
                (*(Dst34_ **) (t + 0x28))[i].p[0] = (*(Dst34_ **) (t + 0x28))[i].q[0];
                goto common;
            case 2:
                (*(Dst34_ **) (t + 0x28))[i].p[0] = (*(Dst34_ **) (t + 0x28))[i].q[0] + off;
            common:
                (*(Dst34_ **) (t + 0x28))[i].p[1] = (*(Dst34_ **) (t + 0x28))[i].q[1];
                (*(Dst34_ **) (t + 0x28))[i].p[2] = (*(Dst34_ **) (t + 0x28))[i].q[2];
                (*(Dst34_ **) (t + 0x28))[i].v[0] = 0.0f;
                (*(Dst34_ **) (t + 0x28))[i].v[1] = 0.0f;
                (*(Dst34_ **) (t + 0x28))[i].v[2] = 0.0f;
                break;
        }
        (*(Dst34_ **) (t + 0x28))[i].w[0] = 0.0f;
        (*(Dst34_ **) (t + 0x28))[i].w[1] = 0.0f;
        (*(Dst34_ **) (t + 0x28))[i].w[2] = 0.0f;
        (*(Dst34_ **) (t + 0x28))[i].e = (*(Src10_ **) (t + 0x24))[i].d;
        (*(Dst34_ **) (t + 0x28))[i].f = (*(Src10_ **) (t + 0x24))[i].d;
    }
}

INCLUDE_ASM("asm/nonmatchings/unit_50510", func_800B2330);

INCLUDE_ASM("asm/nonmatchings/unit_50510", func_800B2440);

INCLUDE_ASM("asm/nonmatchings/unit_50510", func_800B2540);

INCLUDE_ASM("asm/nonmatchings/unit_50510", func_800B2640);

INCLUDE_ASM("asm/nonmatchings/unit_50510", func_800B271C);

void func_800B2844(u8 *t, u8 i) {
    f32 dt;
    s32 now;
    u32 d;
    s32 k;

    if (D_80123DB0 == NULL) {
        dt = 0.15f;
        D_80123DB0[i] = D_80141048();
    } else {
        now = D_80141048();
        d = now - D_80123DB0[i];
        if (d > 100) {
            dt = 0.1f;
        } else {
            dt = d / 100.0f;
        }
        D_80123DB0[i] = now;
    }
    for (k = 0; k < t[0]; k++) {
        func_800B2440(*(s32 *) (t + 4) + k * 0x2C, dt);
    }
    for (k = 0; k < t[8]; k++) {
        func_800B2540(*(s32 *) (t + 0xC) + k * 0x18, dt);
    }
    for (k = 0; k < t[0x10]; k++) {
        func_800B2640(*(s32 *) (t + 0x14) + k * 0x18, dt);
    }
    for (k = 0; k < t[0x20]; k++) {
        func_800B271C(*(s32 *) (t + 0x28) + k * 0x34, dt);
    }
}

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
