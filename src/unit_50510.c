#include "common.h"
#include "ultra64.h"
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
typedef struct { s32 d[0x88C / 4]; } Big88C_;
typedef struct { s32 d[0x140 / 4]; } Blk140_;
extern u8 *D_800F5F08[];
typedef struct { u8 a; u8 pad; s16 x; s16 y; s16 x2; s16 y2; } E0A7_;
u8 func_800B2C94(P16b_, P16b_);
f32 func_800B12C4(s32 ms);
void *Memory_Alloc(s32);
void Memory_Free(void *);
extern u8 D_80123D28[];
extern u8 D_80123D24[];
extern s32 D_80138098[];
extern s32 (*D_80140FE8[])(s32);
extern s32 (*sysTimeGetMillis)(void);
void func_800B2440(u8 *, f32);
void func_800B2540(u8 *, f32);
void func_800B2640(u8 *, f32);
void func_800B271C(s32, f32);
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

extern s32 (*sysRandRange)(s32);
s32 func_800B073C();
s32 func_800B0A5C();
extern s32 (*D_80141040)(s32);
void func_800B1F00();
void func_800B1FE4();
s32 func_800B2330(s32);
s32 func_800C9130();

s32 func_800AF910(void) {
    u32 i;
    u32 j;
    s32 s;
    s32 ok;

    for (i = 0; i < 4; i++) {
        memset(&D_801206D0 + i * 0x88C, 0, 0x88C);
        D_80123D28[i] = 0;
    }
    memset(D_80123D24, 0, 4);
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            memset(&D_80122900[i][j], 0, 0x140);
        }
    }
    for (i = 0; i < 4; i++) {
        D_80138098[i] = 0;
        D_80123D00[i] = 0;
    }
    for (i = 0; i < 4; i++) {
        D_80140FE8[0](i);
        s = D_80140FE8[1](i);
        D_80123D10[i] = s;
        if (s != 4) {
            D_80123D24[i] = 1;
        } else {
            D_80123D24[i] = 0;
        }
        if (D_80123D10[i] != 1) {
            D_80123D20[i] = 1;
        } else {
            D_80123D20[i] = 0;
        }
    }
    if (D_80123D20[0] || D_80123D20[1] || D_80123D20[2] || D_80123D20[3]) {
        ok = 0;
        if (!(D_80123D20[0] && D_80123D10[0]) && !(D_80123D20[1] && D_80123D10[1]) &&
            !(D_80123D20[2] && D_80123D10[2]) && !(D_80123D20[3] && D_80123D10[3])) {
            ok = 1;
        }
        return ok;
    }
    return 0;
}

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


void func_800B0624(void);

s32 func_800AFC40(u32 i) {
    u8 *buf;
    u32 sum;
    u16 j;
    u8 ok;
    u8 *p;
    u32 n;

    if (i >= 4) {
        return 0;
    }
    buf = Memory_Alloc(0xDA0);
    memset(buf, 0, 0xDA0);
    *(s32 *)buf = ((s32 (*)(void))func_800B0624)();
    sum = 0;
    p = buf;
    for (j = 0, n = 0xD90; j < n; j++) {
        sum += *p++;
    }
    *(u32 *)(buf + 0xD90) = sum;
    ok = ((u8 (*)(u32, u8 *))D_80140FE8[3])(i, buf);
    Memory_Free(buf);
    if (ok) {
        D_80123D10[i] &= ~4;
    }
    return ok;
}

INCLUDE_ASM("asm/nonmatchings/unit_50510", func_800AFD1C);

void func_800B0624(void);
s32 func_800B0024(void) {
    u8 *buf;
    u32 i;
    u32 j;
    u16 k;
    s32 sum;
    s32 done;
    s32 *id;
    u8 *c;
    u16 n;

    done = 0;
    buf = Memory_Alloc(0xDA0);
    for (i = 0; i < 4; i++) {
        if (D_80123D20[i] != 0 && (D_80123D24[i] != 0 || (done ^ 1))) {
            *(Big88C_ *)buf = *(Big88C_ *)(&D_801206D0 + i * 0x88C);
            id = &D_80138098[i];
            if (*id == 0) {
                *id = ((s32 (*)(void))func_800B0624)();
            }
            *(s32 *)buf = *id;
            buf[0x88C] = D_80123D00[i];
            for (j = 0; j < (u32)D_80123D00[i]; j++) {
                *(Blk140_ *)(buf + 0x890 + j * 0x140) = *(Blk140_ *)&D_80122900[i][j];
            }
            sum = 0;
            c = buf;
            n = 0xD90;
            for (k = 0; k < n; k++) {
                sum += *c++;
            }
            *(s32 *)(buf + 0xD90) = sum;
            if (((s32 (*)(s32, u8 *))D_80140FE8[3])(i, buf) == 0) {
                D_80123D10[i] = ((u32 (*)(u32))D_80140FE8[1])(i);
                Memory_Free(buf);
                return 0;
            }
            if (D_80123D24[i] == 0) {
                done = 1;
            }
        }
    }
    Memory_Free(buf);
    return 1;
}

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

extern s32 D_80122904[][4][0x50];
extern Big88C_ D_80120F54[];

E140_ *func_800B0364(u32 i) {
    u32 k;
    u32 m;

    if (i >= 2) {
        return NULL;
    }
    for (k = 0; k < 4; k++) {
        for (m = 0; m < 4; m++) {
            if (D_80123D28[k] && m < (u32)D_80123D00[k] && D_80122904[k][m][0] == D_80120F54[k].d[i]) {
                return &D_80122900[k][m];
            }
        }
    }
    return NULL;
}

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

s32 func_800B0548(u32 i, u32 j) {
    u32 k;

    if (i >= 4 || j >= (u32)D_80123D00[i]) {
        return 0;
    }
    for (k = j; k < 3; k++) {
        *(Blk140_ *)&D_80122900[i][k] = *(Blk140_ *)&D_80122900[i][k + 1];
    }
    D_80123D00[i]--;
    return 1;
}

void func_800B0614(void) {

}

void func_800B061C(void) {

}

void func_800B0624(void) {
    do {

    } while (sysRandRange(0x17D78400) == 0);
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

void func_800B10F4(char *s) {
    u8 i;
    u8 j;

    for (i = 0; i < D_800F6040; i++) {
        if (strcmp(s, D_80123D30[i]) == 0) {
            for (j = i; j < D_800F6040; j++) {
                D_80123D30[j] = D_80123D30[j + 1];
            }
            D_80123D30[j] = &D_80100BB0;
            D_800F6040--;
            return;
        }
    }
}

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

f32 func_800B12C4(s32 ms) {
    f32 t;
    t = ms / 1000.0f;
    t = -t * 750.0f;
    return t + 375.0f;
}


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
                w += func_800C8EE4(t[0x28], (*(E0A5_ **)(t + 0x1C))[i].a, (*(E0A5_ **)(t + 0x1C))[i + 1].a);
            }
        }
        if (t[0x28] == 2) {
            w += 0x23;
        } else {
            w += func_800C902C(t[0x28], (*(E0A5_ **)(t + 0x1C))[i].a);
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
                x += func_800C8EE4(t[0x28], (*(E0A5_ **)(t + 0x1C))[i - 1].a, (*(E0A5_ **)(t + 0x1C))[i].a);
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

void func_800B1824(void) {
    u32 k;
    u32 i;

    for (k = 0; k < 33; k++) {
        for (i = 0; i < D_800F5F08[k][0]; i++) {
            func_800B133C(*(u8 **)(D_800F5F08[k] + 4) + i * 0x2C);
        }
        for (i = 0; i < D_800F5F08[k][8]; i++) {
            func_800B14F8(*(u8 **)(D_800F5F08[k] + 0xC) + i * 0x18);
        }
        for (i = 0; i < D_800F5F08[k][0x10]; i++) {
            func_800B1694(*(u8 **)(D_800F5F08[k] + 0x14) + i * 0x18);
        }
    }
}

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


void func_800B1A3C(u8 *t) {
    u32 i;

    if (t[0x26] == 0) {
        for (i = 0; i < t[0x18]; i++) {
            (*(E0A5_ **)(t + 0x1C))[i].x2 = (*(E0A5_ **)(t + 0x1C))[i].x + 640;
            (*(E0A5_ **)(t + 0x1C))[i].y2 = (*(E0A5_ **)(t + 0x1C))[i].y;
            (*(E0A5_ **)(t + 0x1C))[i].x = (*(E0A5_ **)(t + 0x1C))[i].x2;
        }
    } else if (t[0x26] == 2) {
        for (i = 0; i < t[0x18]; i++) {
            (*(E0A5_ **)(t + 0x1C))[i].x2 = (*(E0A5_ **)(t + 0x1C))[i].x + 640;
            (*(E0A5_ **)(t + 0x1C))[i].y2 = (*(E0A5_ **)(t + 0x1C))[i].y;
        }
    } else if (t[0x26] == 1) {
        for (i = 0; i < t[0x18]; i++) {
            (*(E0A5_ **)(t + 0x1C))[i].x2 = (*(E0A5_ **)(t + 0x1C))[i].x - 640;
            (*(E0A5_ **)(t + 0x1C))[i].y2 = (*(E0A5_ **)(t + 0x1C))[i].y;
        }
    }
}

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


void func_800B1694(u8 *t);

void func_800B1F00(t)
    u8 *t;
{
    u32 i;

    for (i = 0; i < t[0]; i++) {
        func_800B133C(*(u8 **)(t + 4) + i * 0x2C);
        func_800B192C(*(u8 **)(t + 4) + i * 0x2C);
    }
    for (i = 0; i < t[8]; i++) {
        func_800B14F8(*(u8 **)(t + 0xC) + i * 0x18);
        func_800B1B60(*(u8 **)(t + 0xC) + i * 0x18);
    }
    for (i = 0; i < t[0x10]; i++) {
        func_800B1694(*(u8 **)(t + 0x14) + i * 0x18);
        func_800B1D30(*(u8 **)(t + 0x14) + i * 0x18);
    }
}

typedef struct { u8 d[0x2C]; } E2Cc_;

void func_800B1FE4(t)
    u8 *t;
{
    u32 i;

    for (i = 0; i < t[0]; i++) {
        func_800B1A3C((u8 *)&(*(E2Cc_ **)(t + 4))[i]);
    }
    for (i = 0; i < t[8]; i++) {
        func_800B1C70((T0A2_ *)&(*(OSMesgQueue **)(t + 0xC))[i]);
    }
    for (i = 0; i < t[0x10]; i++) {
        func_800B1E40((T0A2_ *)&(*(OSMesgQueue **)(t + 0x14))[i]);
    }
}

void func_800B20A4(void) {
    u32 k;
    u32 i;

    for (k = 0; k < 33; k++) {
        for (i = 0; i < D_800F5F08[k][8]; i++) {
            func_800B1B60(*(u8 **)(D_800F5F08[k] + 0xC) + i * 0x18);
        }
        for (i = 0; i < D_800F5F08[k][0x10]; i++) {
            func_800B1D30(*(u8 **)(D_800F5F08[k] + 0x14) + i * 0x18);
        }
    }
}

s32 func_800B2170(s32 arg0) {
    u8 *t;
    s32 i;
    f32 off;
    Src10_ *s;
    Dst34_ *d;

    t = (u8 *) arg0;
    for (i = 0; i < t[0x20]; i++) {
        off = 500.0f - func_800B1294((*(Src10_ **) (t + 0x24))[i].a);
        (*(Dst34_ **) (t + 0x28))[i].q[0] = func_800B1294((*(Src10_ **) (t + 0x24))[i].a);
        (*(Dst34_ **) (t + 0x28))[i].q[1] = func_800B12C4((*(Src10_ **) (t + 0x24))[i].b);
        (*(Dst34_ **) (t + 0x28))[i].q[2] = func_800B1308((*(Src10_ **) (t + 0x24))[i].c);
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

s32 func_800B2330(s32 arg0) {
    u8 *t;
    s32 i;

    t = (u8 *) arg0;
    for (i = 0; i < t[0x20]; i++) {
        switch ((*(Src10_ **) (t + 0x24))[i].kind) {
            case 0:
                break;
            case 2:
                (*(Dst34_ **) (t + 0x28))[i].q[0] = (*(Dst34_ **) (t + 0x28))[i].p[0] + 1000.0f;
                (*(Dst34_ **) (t + 0x28))[i].q[1] = (*(Dst34_ **) (t + 0x28))[i].p[1];
                (*(Dst34_ **) (t + 0x28))[i].q[2] = (*(Dst34_ **) (t + 0x28))[i].p[2];
                (*(Dst34_ **) (t + 0x28))[i].w[0] = 0.0f;
                (*(Dst34_ **) (t + 0x28))[i].w[1] = 0.0f;
                (*(Dst34_ **) (t + 0x28))[i].w[2] = 0.0f;
                break;
        }
        (*(Dst34_ **) (t + 0x28))[i].v[0] = 0.0f;
        (*(Dst34_ **) (t + 0x28))[i].v[1] = 0.0f;
        (*(Dst34_ **) (t + 0x28))[i].v[2] = 0.0f;
        (*(Dst34_ **) (t + 0x28))[i].e = (*(Src10_ **) (t + 0x24))[i].d;
        (*(Dst34_ **) (t + 0x28))[i].f = (*(Src10_ **) (t + 0x24))[i].d;
    }
}

void func_800B2440(u8 *t, f32 s) {
    u8 i;
    s32 dx;
    s32 dy;

    for (i = 0; i < t[0x18]; i++) {
        dx = (*(E0A7_ **)(t + 0x1C))[i].x2 - (*(E0A7_ **)(t + 0x1C))[i].x;
        dx *= s;
        dy = (*(E0A7_ **)(t + 0x1C))[i].y2 - (*(E0A7_ **)(t + 0x1C))[i].y;
        dy *= s;
        if (dx > 0) {
            (*(E0A7_ **)(t + 0x1C))[i].x += dx;
        } else {
            (*(E0A7_ **)(t + 0x1C))[i].x = (*(E0A7_ **)(t + 0x1C))[i].x2;
        }
        if (dy > 0) {
            (*(E0A7_ **)(t + 0x1C))[i].y += dy;
        } else {
            (*(E0A7_ **)(t + 0x1C))[i].y = (*(E0A7_ **)(t + 0x1C))[i].y2;
        }
    }
}

void func_800B2540(u8 *t, f32 s) {
    u8 i;
    s32 dx;
    s32 dy;

    for (i = 0; i < t[2]; i++) {
        dx = (*(E0A7_ **)(t + 4))[i].x2 - (*(E0A7_ **)(t + 4))[i].x;
        dx *= s;
        dy = (*(E0A7_ **)(t + 4))[i].y2 - (*(E0A7_ **)(t + 4))[i].y;
        dy *= s;
        if (dx > 0) {
            (*(E0A7_ **)(t + 4))[i].x += dx;
        } else {
            (*(E0A7_ **)(t + 4))[i].x = (*(E0A7_ **)(t + 4))[i].x2;
        }
        if (dy > 0) {
            (*(E0A7_ **)(t + 4))[i].y += dy;
        } else {
            (*(E0A7_ **)(t + 4))[i].y = (*(E0A7_ **)(t + 4))[i].y2;
        }
    }
}

void func_800B2640(u8 *t, f32 s) {
    s32 i;
    s32 dx;
    s32 dy;

    for (i = 0; i < t[2]; i++) {
        dx = (*(E0A7_ **)(t + 4))[i].x2 - (*(E0A7_ **)(t + 4))[i].x;
        dx *= s;
        dy = (*(E0A7_ **)(t + 4))[i].y2 - (*(E0A7_ **)(t + 4))[i].y;
        dy *= s;
        if (dx > 0) {
            (*(E0A7_ **)(t + 4))[i].x += dx;
        } else {
            (*(E0A7_ **)(t + 4))[i].x = (*(E0A7_ **)(t + 4))[i].x2;
        }
        if (dy > 0) {
            (*(E0A7_ **)(t + 4))[i].y += dy;
        } else {
            (*(E0A7_ **)(t + 4))[i].y = (*(E0A7_ **)(t + 4))[i].y2;
        }
    }
}

INCLUDE_ASM("asm/nonmatchings/unit_50510", func_800B271C);

void func_800B2844(u8 *t, u8 i) {
    f32 dt;
    s32 now;
    u32 d;
    s32 k;

    if (D_80123DB0 == NULL) {
        dt = 0.15f;
        D_80123DB0[i] = sysTimeGetMillis();
    } else {
        now = sysTimeGetMillis();
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

void func_800B2A20(u8 *menu) {
    u8 i;

    for (i = 0; i < menu[0x20]; i++) {
        func_800B1250(*(s32 *)(*(u8 **)(menu + 0x24) + i * 0x10));
    }
}

void func_800B2A84(void) {
    u8 i;

    for (i = 0; i < 32; i++) {
        D_80141040(i);
    }
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


void func_800B2BAC(u8 *t) {
    u8 i;

    for (i = 0; i < t[0]; i++) {
        func_800B133C(*(u8 **)(t + 4) + i * 0x2C);
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

/* Returns 1 when the two points are at most 10 apart on both axes. */
u8 func_800B2C94(P16b_ a, P16b_ b) {
    if ((u32)(a.x - b.x + 10) >= 21 || (u32)(a.y - b.y + 10) >= 21) {
        return 0;
    }
    return 1;
}

s32 func_800B2CE0(u8 *t) {
    u8 i;

    for (i = 0; i < t[0x18]; i++) {
        if (!func_800B2C94(*(P16b_ *)&(*(E0A5_ **)(t + 0x1C))[i].x, *(P16b_ *)&(*(E0A5_ **)(t + 0x1C))[i].x2)) {
            return 0;
        }
    }
    return 1;
}

s32 func_800B2DA0(u8 *t) {
    u8 i;

    for (i = 0; i < t[2]; i++) {
        if (!func_800B2C94(*(P16b_ *)&(*(E0A5_ **)(t + 4))[i].x, *(P16b_ *)&(*(E0A5_ **)(t + 4))[i].x2)) {
            return 0;
        }
    }
    return 1;
}

s32 func_800B2E60(u8 *t) {
    u8 i;

    for (i = 0; i < t[2]; i++) {
        if (!func_800B2C94(*(P16b_ *)&(*(E0A5_ **)(t + 4))[i].x, *(P16b_ *)&(*(E0A5_ **)(t + 4))[i].x2)) {
            return 0;
        }
    }
    return 1;
}

void func_800B2F20(s32 a) {
    s32 x[4];
}
