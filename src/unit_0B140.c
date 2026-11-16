#include "common.h"
#include "include_asm.h"
typedef struct { s32 state; u8 pad[0x50]; } SlotC_;
extern SlotC_ D_800F03F8[];
extern u8 D_800F0548[];
void func_800D7150(void);
void *memset(void *dst, s32 value, u32 size);
typedef struct { u16 button; s8 stick_x; s8 stick_y; u8 errnum; u8 pad; } Pad_;
typedef struct { u8 buf[8]; s32 st; s32 mode; } PakSt_;
typedef struct { s32 keys[6]; u8 pad[0x54 - 0x18]; } KeySet_;
typedef struct { u8 c[0x54]; } Row6B_;
typedef struct { u32 w0; u32 w1; } GfxWAA_;
typedef union { GfxWAA_ words; s64 force; } GfxAA_;
typedef struct { GfxAA_ *f0; GfxAA_ *f4; u8 p8[0x318]; } PoolAA_;
typedef struct { f32 x; f32 y; f32 z; u8 pC[8]; u8 type; } EntAA_;
extern PoolAA_ D_8014C740[];
extern s32 D_8014C540;
extern GfxAA_ *D_8014EC78;
void GfxPool_Grow(PoolAA_ *, u32);
extern s32 D_80101E90;
extern u8 D_80101E94;
extern u8 D_80101E95;
extern u8 D_80101E96;
extern EntAA_ D_80101EA8[];
extern f32 D_800F02D0;
extern u8 D_800F0318[];
extern u8 D_800F0338[];
void func_8006B1AC(void);
void func_8007AFD4(s32, f32 (*)[4], f32 (*)[4]);
s32 func_80070BA4(f32 (*)[4]);
extern Row6B_ D_800F043C[];
extern KeySet_ D_800F0404[];
void func_800D63C0(Pad_ *, s32);
s32 func_800D6FA0(s32);
void func_800D7030(s32, s32);
void func_800D66D0(PakSt_ *, s32);
extern void func_800D6420(Pad_ *);
extern u8 D_800F0403;
extern u8 D_800F0390;
s32 func_8006C0EC(s32,s32);
extern u8 D_800F03EB;
extern s32 D_80145568;
extern s32 D_8014556C;
extern s32 D_80145570;
extern s32 D_80145574;
extern s32 D_80145578;
extern s32 D_8014557C;
extern s32 D_80145580;
extern s32 D_80145584;
extern s32 D_80145588;
extern s32 D_8014558C;
extern s32 D_80145590;
extern s32 D_80145594;
extern s32 D_801455A4;
extern s32 D_801455B0;

s32 Memory_Free(s32);
extern s32 D_80102208;
extern s32 D_800F0414;
extern u8 D_800F03F4;
s32 func_800D7130();

INCLUDE_ASM("asm/nonmatchings/unit_0B140", func_8006A540);

s32 func_8006AA34(void) {
    s32 *var_s0;
    s32 var_s1;

    var_s1 = 0;
    var_s0 = &D_80102208;
    do {
        var_s1 += 1;
        Memory_Free(*var_s0);
        *var_s0 = 0;
        var_s0 += 1;
    } while (var_s1 < 3);
    return 1;
}

s32 func_8006AA84(s32 cam, f32 *pos) {
    f32 m[3][4];
    f32 o[3][4];
    f32 d[2];
    s32 i;
    s32 j;
    s32 n = 0;
    s32 mtx;
    EntAA_ *e;
    m[0][0] = m[0][1] = m[0][2] = m[0][3] = m[1][0] = m[1][1] = m[1][2] = m[1][3] = m[2][0] = m[2][1] = m[2][2] = m[2][3] = 0.0f;
    m[0][0] = m[1][1] = m[2][2] = 1.0f;
    func_8006B1AC();
    if ((u32) (D_8014EC78 + (9)) >= (u32) D_8014C740[D_8014C540].f4) { D_8014C740[D_8014C540].f0 = D_8014EC78; GfxPool_Grow(&D_8014C740[D_8014C540], (9)); D_8014EC78 = D_8014C740[D_8014C540].f0; };
    { GfxAA_ *_g = (GfxAA_ *) (D_8014EC78++); _g->words.w0 = (0xDB040004); _g->words.w1 = (u32) ( 1); };
    { GfxAA_ *_g = (GfxAA_ *) (D_8014EC78++); _g->words.w0 = (0xDB04000C); _g->words.w1 = (u32) ( 1); };
    { GfxAA_ *_g = (GfxAA_ *) (D_8014EC78++); _g->words.w0 = (0xDB040014); _g->words.w1 = (u32) ( 0xFFFF); };
    { GfxAA_ *_g = (GfxAA_ *) (D_8014EC78++); _g->words.w0 = (0xDB04001C); _g->words.w1 = (u32) ( 0xFFFF); };
    { GfxAA_ *_g = (GfxAA_ *) (D_8014EC78++); _g->words.w0 = (0xDE000000); _g->words.w1 = (u32) ( D_800F0338); };
    { GfxAA_ *_g = (GfxAA_ *) (D_8014EC78++); _g->words.w0 = (0xE7000000); _g->words.w1 = (u32) ( 0); };
    { GfxAA_ *_g = (GfxAA_ *) (D_8014EC78++); _g->words.w0 = (0xFA000000); _g->words.w1 = (u32) ( (D_80101E94 << 24) | (D_80101E95 << 16) | (D_80101E96 << 8) | 0xC0); };
    { GfxAA_ *_g = (GfxAA_ *) (D_8014EC78++); _g->words.w0 = (0xE2001D00); _g->words.w1 = (u32) ( 4); };
    { GfxAA_ *_g = (GfxAA_ *) (D_8014EC78++); _g->words.w0 = (0xEE000000); _g->words.w1 = (u32) ( 0xFFFC0000); };
    for (i = 0; i < 3; i++) {
        if ((u32) (D_8014EC78 + (7)) >= (u32) D_8014C740[D_8014C540].f4) { D_8014C740[D_8014C540].f0 = D_8014EC78; GfxPool_Grow(&D_8014C740[D_8014C540], (7)); D_8014EC78 = D_8014C740[D_8014C540].f0; };
        { GfxAA_ *_g = (GfxAA_ *) (D_8014EC78++); _g->words.w0 = (0xFD900000); _g->words.w1 = (u32) ( (&D_80102208)[i]); };
        { GfxAA_ *_g = (GfxAA_ *) (D_8014EC78++); _g->words.w0 = (0xF5900000); _g->words.w1 = (u32) ( 0x07000000); };
        { GfxAA_ *_g = (GfxAA_ *) (D_8014EC78++); _g->words.w0 = (0xE6000000); _g->words.w1 = (u32) ( 0); };
        { GfxAA_ *_g = (GfxAA_ *) (D_8014EC78++); _g->words.w0 = (0xF3000000); _g->words.w1 = (u32) ( 0x070FF400); };
        { GfxAA_ *_g = (GfxAA_ *) (D_8014EC78++); _g->words.w0 = (0xE7000000); _g->words.w1 = (u32) ( 0); };
        { GfxAA_ *_g = (GfxAA_ *) (D_8014EC78++); _g->words.w0 = (0xF5800400); _g->words.w1 = (u32) ( 0); };
        { GfxAA_ *_g = (GfxAA_ *) (D_8014EC78++); _g->words.w0 = (0xF2000000); _g->words.w1 = (u32) ( 0x0007C07C); };
        for (j = 0, e = D_80101EA8; j < D_80101E90; e++, j++) {
            if (e->type == i) {
                d[0] = pos[0] - e->x;
                d[1] = pos[1] - e->y;
                if (d[0] * d[0] + d[1] * d[1] < 1.296e9f) {
                    m[0][3] = e->x;
                    m[1][3] = e->y;
                    m[2][3] = e->z;
                    func_8007AFD4(cam, m, o);
                    o[0][3] *= D_800F02D0;
                    o[1][3] *= D_800F02D0;
                    o[2][3] *= D_800F02D0;
                    mtx = func_80070BA4(o);
                    if ((u32) (D_8014EC78 + (2)) >= (u32) D_8014C740[D_8014C540].f4) { D_8014C740[D_8014C540].f0 = D_8014EC78; GfxPool_Grow(&D_8014C740[D_8014C540], (2)); D_8014EC78 = D_8014C740[D_8014C540].f0; };
                    { GfxAA_ *_g = (GfxAA_ *) (D_8014EC78++); _g->words.w0 = (0xDA380003); _g->words.w1 = (u32) ( mtx); };
                    { GfxAA_ *_g = (GfxAA_ *) (D_8014EC78++); _g->words.w0 = (0xDE000000); _g->words.w1 = (u32) ( D_800F0318); };
                    n += 2;
                }
            }
        }
    }
    if ((u32) (D_8014EC78 + (7)) >= (u32) D_8014C740[D_8014C540].f4) { D_8014C740[D_8014C540].f0 = D_8014EC78; GfxPool_Grow(&D_8014C740[D_8014C540], (7)); D_8014EC78 = D_8014C740[D_8014C540].f0; };
    { GfxAA_ *_g = (GfxAA_ *) (D_8014EC78++); _g->words.w0 = (0xDB040004); _g->words.w1 = (u32) ( 3); };
    { GfxAA_ *_g = (GfxAA_ *) (D_8014EC78++); _g->words.w0 = (0xDB04000C); _g->words.w1 = (u32) ( 3); };
    { GfxAA_ *_g = (GfxAA_ *) (D_8014EC78++); _g->words.w0 = (0xDB040014); _g->words.w1 = (u32) ( 0xFFFD); };
    { GfxAA_ *_g = (GfxAA_ *) (D_8014EC78++); _g->words.w0 = (0xDB04001C); _g->words.w1 = (u32) ( 0xFFFD); };
    { GfxAA_ *_g = (GfxAA_ *) (D_8014EC78++); _g->words.w0 = (0xE7000000); _g->words.w1 = (u32) ( 0); };
    { GfxAA_ *_g = (GfxAA_ *) (D_8014EC78++); _g->words.w0 = (0xE3001801); _g->words.w1 = (u32) ( 0); };
    { GfxAA_ *_g = (GfxAA_ *) (D_8014EC78++); _g->words.w0 = (0xE2001D00); _g->words.w1 = (u32) ( 0); };
    return n;
}

INCLUDE_ASM("asm/nonmatchings/unit_0B140", func_8006B1AC);

INCLUDE_ASM("asm/nonmatchings/unit_0B140", func_8006B2A0);

INCLUDE_ASM("asm/nonmatchings/unit_0B140", func_8006B4F8);

s32 func_8006B630(void) {
    D_80145568 = 0;
    D_8014556C = 0;
    D_80145570 = 0;
    D_801455A4 = 0;
    D_80145574 = 0;
    D_80145578 = 0;
    D_8014557C = 0;
    D_80145580 = 0;
    D_80145584 = 0;
    D_80145588 = 0;
    D_801455B0 = 0;
    D_8014558C = 0;
    D_80145590 = 0;
    D_80145594 = 0;
    D_80145584 = 0;
    D_80145588 = 0;
    return 1;
}

s32 func_8006B6B8(u8 ch, s32 key) {
    Pad_ pad[4];
    u16 mask;

    func_800D6420(pad);
    switch (key) {
    case 14:
        if (pad[ch].stick_y > 0x28) {
            return 1;
        }
        mask = 0x800;
        break;
    case 15:
        if (pad[ch].stick_y < -0x28) {
            return 1;
        }
        mask = 0x400;
        break;
    case 16:
        if (pad[ch].stick_x < -0x28) {
            return 1;
        }
        mask = 0x200;
        break;
    case 17:
        if (pad[ch].stick_x > 0x28) {
            return 1;
        }
        mask = 0x100;
        break;
    case 18:
        mask = 0x8000;
        break;
    case 19:
        mask = 0x4000;
        break;
    case 20:
        mask = 8;
        break;
    case 21:
        mask = 4;
        break;
    case 22:
        mask = 0x2000;
        break;
    case 23:
        mask = 0x10;
        break;
    case 24:
        mask = 2;
        break;
    case 25:
        mask = 1;
        break;
    case 26:
        mask = 0x1000;
        break;
    case 27:
        mask = 0x9000;
        break;
    default:
        return 0;
    }
    return (pad[ch].button & mask) != 0;
}

void func_8006B838(void) {
    s32 *var_a0;
    s32 *var_a1;
    s32 *var_a2;
    s32 *var_a3;
    s32 temp_v1;
    s32 temp_v1_2;
    s32 var_t0;

    var_t0 = 0;
    var_a0 = &D_800F0414;
    var_a3 = (u8 *)&D_800F0414 + 4;
    var_a2 = (u8 *)&D_800F0414 - 0xC;
    var_a1 = (u8 *)&D_800F0414 - 0x10;
    do {
        temp_v1 = *var_a1;
        var_t0 += 1;
        *var_a1 = *var_a2;
        *var_a2 = temp_v1;
        temp_v1_2 = *var_a3;
        var_a2 += 21;
        var_a1 += 21;
        *var_a3 = *var_a0;
        *var_a0 = temp_v1_2;
        var_a0 += 21;
        var_a3 += 21;
    } while (var_t0 < 4);
}

s32 func_8006B894(s32 key) {
    Pad_ pad[4];
    u16 mask;

    func_800D6420(pad);
    switch (key) {
    case 14:
        if (pad[0].stick_y > 0x28 || pad[1].stick_y > 0x28 || pad[2].stick_y > 0x28 || pad[3].stick_y > 0x28) {
            return 1;
        }
        mask = 0x800;
        break;
    case 15:
        if (pad[0].stick_y < -0x28 || pad[1].stick_y < -0x28 || pad[2].stick_y < -0x28 || pad[3].stick_y < -0x28) {
            return 1;
        }
        mask = 0x400;
        break;
    case 16:
        if (pad[0].stick_x < -0x28 || pad[1].stick_x < -0x28 || pad[2].stick_x < -0x28 || pad[3].stick_x < -0x28) {
            return 1;
        }
        mask = 0x200;
        break;
    case 17:
        if (pad[0].stick_x > 0x28 || pad[1].stick_x > 0x28 || pad[2].stick_x > 0x28 || pad[3].stick_x > 0x28) {
            return 1;
        }
        mask = 0x100;
        break;
    case 18:
        mask = 0x8000;
        break;
    case 19:
        mask = 0x4000;
        break;
    case 20:
        mask = 8;
        break;
    case 21:
        mask = 4;
        break;
    case 22:
        mask = 0x2000;
        break;
    case 23:
        mask = 0x10;
        break;
    case 24:
        mask = 2;
        break;
    case 25:
        mask = 1;
        break;
    case 26:
        mask = 0x1000;
        break;
    case 27:
        mask = 0x9000;
        break;
    default:
        return 0;
    }
    if ((pad[0].button & mask) || (pad[1].button & mask) || (pad[2].button & mask) || (pad[3].button & mask)) {
        return 1;
    }
    return 0;
}

INCLUDE_ASM("asm/nonmatchings/unit_0B140", func_8006BA98);

u8 func_8006BD78(u8 a, s32 b) {
    return D_800F043C[a].c[b];
}

INCLUDE_ASM("asm/nonmatchings/unit_0B140", func_8006BDA8);

s32 func_8006BE34(u8 ch, s32 slot) {
    Pad_ pad;
    s32 mask;

    func_800D63C0(&pad, ch);
    mask = 0;
    if (mask == 0 && pad.stick_x > 0x28) mask = 0x10000;
    if (mask == 0 && pad.stick_x < -0x28) mask = 0x20000;
    if (mask == 0 && pad.stick_y > 0x28) mask = 0x80000;
    if (mask == 0 && pad.stick_y < -0x28) mask = 0x40000;
    if (mask == 0 && (pad.button & 0x8000)) mask = 0x8000;
    if (mask == 0 && (pad.button & 0x4000)) mask = 0x4000;
    if (mask == 0 && (pad.button & 0x20)) mask = 0x20;
    if (mask == 0 && (pad.button & 0x10)) mask = 0x10;
    if (mask == 0 && (pad.button & 0x2000)) mask = 0x2000;
    if (mask == 0 && (pad.button & 0x800)) mask = 0x800;
    if (mask == 0 && (pad.button & 0x200)) mask = 0x200;
    if (mask == 0 && (pad.button & 0x100)) mask = 0x100;
    if (mask == 0 && (pad.button & 0x400)) mask = 0x400;
    if (mask == 0 && (pad.button & 8)) mask = 8;
    if (mask == 0 && (pad.button & 2)) mask = 2;
    if (mask == 0 && (pad.button & 1)) mask = 1;
    if (mask == 0 && (pad.button & 4)) mask = 4;
    if (mask != 0) {
        if (slot == 0 || slot == 5) {
            D_800F0404[ch].keys[0] = mask;
            D_800F0404[ch].keys[5] = mask;
        } else if (slot == 1 || slot == 4) {
            D_800F0404[ch].keys[1] = mask;
            D_800F0404[ch].keys[4] = mask;
        } else {
            D_800F0404[ch].keys[slot] = mask;
        }
        return 1;
    }
    return 0;
}

u8 func_8006C0A0(void) {
    return D_800F03EB;
}

s32 func_8006C0AC(s32 arg0) {
    return *(s32 *)(&D_800F0390 + ((arg0 & 0xFF) * 4));
}

u8 func_8006C0C4(s32 arg0) {
    return *(u8 *)(&D_800F0403 + ((arg0 & 0xFF) * 0x54));
}

INCLUDE_ASM("asm/nonmatchings/unit_0B140", func_8006C0EC);

s32 func_8006C218(s32 arg0) {
    func_8006C0EC(arg0 & 0xFF, 0);
    return 1;
}

s32 func_8006C23C(u8 i) {
    u8 unused[16];

    return D_800F03F8[i].state == 2;
}

INCLUDE_ASM("asm/nonmatchings/unit_0B140", func_8006C274);

s32 func_8006C334(void) {
    return 0;
}

s32 func_8006C33C(void) {
    return D_800F03F4 & 1;
}

INCLUDE_ASM("asm/nonmatchings/unit_0B140", func_8006C34C);

INCLUDE_ASM("asm/nonmatchings/unit_0B140", func_8006C3D4);

s32 func_8006C468(void) {
    func_800D7130();
    return 1;
}

s32 func_8006C488(void) {
    func_800D7150();
    memset(D_800F0548, 0, 2);
    return 1;
}


INCLUDE_ASM("asm/nonmatchings/unit_0B140", func_8006C4BC);

s32 func_8006C520(void) {
    PakSt_ s;
    s32 i;

    for (i = 0; i < 4; i++) {
        s.st = func_800D6FA0(i);
        if (s.st == 0) {
            s.mode = 2;
            func_800D7030(i, 2);
        } else if (s.st == 11) {
            func_800D66D0(&s, i);
            if (s.st == 10) {
                s.mode = 1;
            }
        }
        D_800F03F8[i].state = s.mode;
    }
    return 1;
}

