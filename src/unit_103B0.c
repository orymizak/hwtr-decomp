#include "common.h"
#include "include_asm.h"
typedef struct { s8 a; s8 b; s8 c; u8 pad[0xD]; } Ent6F_;
extern Ent6F_ D_800F1DF0[];
extern u8 D_800F20F8;
extern u8 D_800F20F4;
void func_800704DC(void);
typedef struct { s32 f0; s32 f4; s32 f8; s32 fC; u8 p10[0x78]; s32 f88; s32 f8C; } S_D_8014EB58;
extern S_D_8014EB58 D_8014EB58[];
extern s32 D_8014108C;
typedef struct { s32 f0; s32 f4; u8 p8[0x208]; s32 f210; s32 f214; u8 p218[0xFC]; s32 f314; s32 f318; u8 p31C[0x4]; } S_D_8014C740;
typedef struct { u32 w0; u32 w1; } Gfx2_;
typedef struct Task6F_ { struct Task6F_ *next; s32 w4; s32 w8; s32 wC; s32 w10; s32 w14; s32 w18; s32 w1C; s32 w20; s32 w24; s32 w28; s32 w2C; s32 w30; s32 w34; s32 w38; s32 w3C; Gfx2_ *dl; s32 dlsize; s32 w48; s32 w4C; s32 w50; void *msg; } Task6F_;
typedef struct { u32 w0; u32 w1; } G9_;
typedef struct { u8 r; u8 g; u8 b; u8 pad3; u8 r2; u8 g2; u8 b2; u8 pad[9]; } L16_;
extern Task6F_ D_8014C1D0[];
extern s16 D_8010A226;
extern u8 D_8010A228[];
extern u8 D_8010C3E0[];
extern u8 D_8014EC80[];
extern u8 D_800E7AA0[];
extern u8 D_800E7B70[];
extern u8 D_8014CD80[];
extern u8 D_80138E40[];
extern u8 D_80140E40[];
extern u8 D_80143850[];
void func_800D61C0(void);
void func_8006F7B0(void *);
void osCreateThread(void *, s32, void (*)(void *), void *, void *, s32);
void osStartThread(void *);
void func_8006F8C4(void);
extern L16_ D_800F1DE8[];
void MtxPool_Grow(void *, u32);
void guLookAtReflect(void *, void *, f32, f32, f32, f32, f32, f32, f32, f32, f32);
void GfxPool_Grow(void *, u32);
extern Task6F_ *D_8010A220;
extern s32 *D_80141690;
extern u16 D_800F1CBC;
extern s32 D_8014C648;
extern volatile s32 D_80145484;
extern s32 D_801380B0;
extern s32 D_801380A8;
extern u8 D_800FB9C0[];
extern u16 D_800F20FA;
extern u16 D_800F20FC;
extern u8 D_800F1CC0;
extern s32 D_800F1CC8;
extern s16 D_8010A224;
extern u8 D_8014D33C;
extern u8 D_F1CF8;
void osViBlack(u8);
void osViSetMode(void *);
void osViSetSpecialFeatures(u32);
u32 osSetIntMask(u32);
void osWritebackDCacheAll(void);
s32 osSendMesg(void *, void *, s32);
void func_800D5ED0(void);
void func_800D60B0(void *, s32);
void func_800D5FB0(void *);
void func_800D5FF0(void);
extern S_D_8014C740 D_8014C740[];
extern u8 D_800F20F7;
extern s32 D_8014C540;
extern s32 D_8014EC78;
f32 cosf(f32);
f32 sinf(f32);
extern f32 D_800F2114;
extern f32 D_800F2118;
extern f32 D_800F211C;
extern f32 D_800F2120;
extern f32 D_800F2124;
extern f32 D_800F2128;
extern f32 D_800F212C;
extern f32 D_800F2130;
extern f32 D_800F2134;

s32 func_800D6010();

INCLUDE_ASM("asm/nonmatchings/unit_103B0", func_8006F7B0);

void func_8006F8C4(void) {
    u32 i;

    D_8010A224 = 8;
    D_8010A226 = 4;
    D_80145484 = 0;
    func_800D61C0();
    osCreateThread(D_8010A228, 5, func_8006F7B0, 0, D_8010C3E0, 0x3C);
    osStartThread(D_8010A228);
    for (i = 0; i < 10; i++) {
        D_8014C1D0[i].next = &D_8014C1D0[i + 1];
        D_8014C1D0[i].w50 = (s32) D_8014EC80;
        D_8014C1D0[i].w10 = 1;
        D_8014C1D0[i].w14 = 0;
        D_8014C1D0[i].w18 = (s32) D_800E7AA0;
        D_8014C1D0[i].w1C = D_800E7B70 - D_800E7AA0;
        D_8014C1D0[i].w24 = 0x1000;
        D_8014C1D0[i].w2C = 0x800;
        D_8014C1D0[i].w30 = (s32) D_8014CD80;
        D_8014C1D0[i].w34 = 0x400;
        D_8014C1D0[i].w38 = (s32) D_80138E40;
        D_8014C1D0[i].w3C = (s32) D_80140E40;
        D_8014C1D0[i].w48 = (s32) D_80143850;
        D_8014C1D0[i].w4C = 0xC00;
        D_8014C1D0[i].w50 = (s32) D_8014EC80;
    }
    D_8014C1D0[9].next = &D_8014C1D0[0];
    D_8010A220 = &D_8014C1D0[0];
}

INCLUDE_ASM("asm/nonmatchings/unit_103B0", func_8006FA7C);

s32 func_8006FBF0(void) {
    Gfx2_ dl[0x101];
    Task6F_ *t;
    u16 f;
    u32 mask;

    osViBlack(1);
    osViSetMode(&D_800FB9C0[D_801380B0 * 0x50]);
    osViSetSpecialFeatures(0x5A);
    osViBlack(0);
    D_800F20FA = 640;
    D_800F20FC = 480;
    func_800D5ED0();
    func_800D60B0(&D_800F1CC0, 2);
    D_801380A8 = 0x8023E000;
    func_800D5FB0(func_800D5FF0);
    D_80141690 = &D_800F1CC8;
    func_8006F8C4();
    t = D_8010A220;
    f = D_800F1CBC;
    dl[0].w0 = 0xDE000000;
    dl[0].w1 = (u32)&D_F1CF8;
    dl[1].w0 = 0xE9000000;
    dl[1].w1 = 0;
    dl[2].w0 = 0xDF000000;
    dl[2].w1 = 0;
    t->dl = dl;
    t->dlsize = 0x18;
    t->w14 = 0;
    t->w20 = D_80141690[0];
    t->w28 = D_80141690[1];
    t->w8 = 0;
    t->wC = D_8014C648;
    if (f & 4) {
        D_800F1CBC = f ^ 4;
        t->w14 |= 2;
    }
    D_800F1CBC = 0;
    D_8010A220->msg = &D_8010A224;
    mask = osSetIntMask(1);
    D_80145484++;
    osSetIntMask(mask);
    osWritebackDCacheAll();
    osSendMesg(&D_8014D33C, D_8010A220, 1);
    D_8010A220 = D_8010A220->next;
    ((void (*)(void))func_800D6010)();
    return 1;
}

void func_8006FDB8(s32 a) {
    u8 *p;

    if (a == 0) {
        a = 1;
    }
    p = &D_800F20F7;
    if ((*p != a) | (D_800F20F8 == 1)) {
        *p = a;
        D_800F20F8 = 1;
        if (D_800F20F4 == 1) {
            func_800704DC();
        }
    }
}


INCLUDE_ASM("asm/nonmatchings/unit_103B0", func_8006FE2C);

void func_8006FF00(s32 i, u8 a, u8 b, u8 c) {
    if (D_800F1DF0[i].a != a) {
        D_800F1DF0[i].a = a;
        D_800F20F8 = 1;
    }
    if (D_800F1DF0[i].b != b) {
        D_800F1DF0[i].b = b;
        D_800F20F8 = 1;
    }
    if (D_800F1DF0[i].c != c) {
        D_800F1DF0[i].c = c;
        D_800F20F8 = 1;
    }
}


void func_80070860(s32 arg0);
void func_8006FF90(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    if ((D_800F20F4 == 1) & (D_800F20F8 == 0)) {
        D_800F1DE8[arg0].r = arg1;
        D_800F1DE8[arg0].g = arg2;
        D_800F1DE8[arg0].b = arg3;
        D_800F1DE8[arg0].r2 = arg1;
        D_800F1DE8[arg0].g2 = arg2;
        D_800F1DE8[arg0].b2 = arg3;
        ((void (*)(s32 arg0))func_80070860)(arg0);
    }
    if (D_800F1DE8[arg0].r != (u8) arg1) {
        D_800F1DE8[arg0].r = arg1;
        D_800F20F8 = 1;
    }
    if (D_800F1DE8[arg0].g != (u8) arg2) {
        D_800F1DE8[arg0].g = arg2;
        D_800F20F8 = 1;
    }
    if (D_800F1DE8[arg0].b != (u8) arg3) {
        D_800F1DE8[arg0].b = arg3;
        D_800F20F8 = 1;
    }
    if (D_800F1DE8[arg0].r2 != (u8) arg1) {
        D_800F1DE8[arg0].r2 = arg1;
        D_800F20F8 = 1;
    }
    if (D_800F1DE8[arg0].g2 != (u8) arg2) {
        D_800F1DE8[arg0].g2 = arg2;
        D_800F20F8 = 1;
    }
    if (D_800F1DE8[arg0].b2 != (u8) arg3) {
        D_800F1DE8[arg0].b2 = arg3;
        D_800F20F8 = 1;
    }
}

INCLUDE_ASM("asm/nonmatchings/unit_103B0", func_80070148);

void func_800702C4(s32 a, s32 b, s32 c) {
    s32 w;
    s32 g;

    w = (a << 24) + ((b & 0xFF) << 16) + ((c & 0xFF) << 8);
    if ((u32) (D_8014EC78 + 0x60) >= (u32) D_8014C740[D_8014C540].f4) {
        D_8014C740[D_8014C540].f0 = D_8014EC78;
        GfxPool_Grow(&D_8014C740[D_8014C540], 0xCU);
        D_8014EC78 = D_8014C740[D_8014C540].f0;
    }
    g = D_8014EC78;
    D_8014EC78 = g + 8;
    ((s32 *)g)[0] = 0xDB0A0000;
    ((s32 *)g)[1] = w;
    D_8014EC78 = g + 16;
    ((s32 *)g)[2] = 0xDB0A0004;
    ((s32 *)g)[3] = w;
    D_8014EC78 = g + 24;
    ((s32 *)g)[4] = 0xDB0A0018;
    ((s32 *)g)[5] = w;
    D_8014EC78 = g + 32;
    ((s32 *)g)[6] = 0xDB0A001C;
    ((s32 *)g)[7] = w;
    D_8014EC78 = g + 40;
    ((s32 *)g)[8] = 0xDB0A0030;
    ((s32 *)g)[9] = w;
    D_8014EC78 = g + 48;
    ((s32 *)g)[10] = 0xDB0A0034;
    ((s32 *)g)[11] = w;
    D_8014EC78 = g + 56;
    ((s32 *)g)[12] = 0xDB0A0048;
    ((s32 *)g)[13] = w;
    D_8014EC78 = g + 64;
    ((s32 *)g)[14] = 0xDB0A004C;
    ((s32 *)g)[15] = w;
    D_8014EC78 = g + 72;
    ((s32 *)g)[16] = 0xDB0A0060;
    ((s32 *)g)[17] = w;
    D_8014EC78 = g + 80;
    ((s32 *)g)[18] = 0xDB0A0064;
    ((s32 *)g)[19] = w;
    D_8014EC78 = g + 88;
    ((s32 *)g)[20] = 0xDB0A0078;
    ((s32 *)g)[21] = w;
    D_8014EC78 = g + 96;
    ((s32 *)g)[22] = 0xDB0A007C;
    ((s32 *)g)[23] = w;
}

INCLUDE_ASM("asm/nonmatchings/unit_103B0", func_800704DC);

void func_80070780(void) {
    s32 temp_v1;
    u8 temp_s0;

    temp_s0 = D_800F20F7;
    if ((u32) (D_8014EC78 + 8) >= (u32) D_8014C740[D_8014C540].f4) {
        D_8014C740[D_8014C540].f0 = D_8014EC78;
        GfxPool_Grow(&D_8014C740[D_8014C540], 1U);
        D_8014EC78 = D_8014C740[D_8014C540].f0;
    }
    temp_v1 = D_8014EC78;
    D_8014EC78 = temp_v1 + 8;
    (*(s32 *)((u8 *)(temp_v1) + 0x0)) = 0xDB020000;
    (*(s32 *)((u8 *)(temp_v1) + 0x4)) = (s32) (temp_s0 * 0x18);
}

void func_80070860(s32 arg0) {
    s32 g;

    if ((u32) (D_8014EC78 + 0x10) >= (u32) D_8014C740[D_8014C540].f4) {
        D_8014C740[D_8014C540].f0 = D_8014EC78;
        GfxPool_Grow(&D_8014C740[D_8014C540], 2U);
        D_8014EC78 = D_8014C740[D_8014C540].f0;
    }
    g = D_8014EC78;
    D_8014EC78 = g + 8;
    ((G9_ *)g)[0].w0 = (arg0 & 0xFFFF) | 0xDB0A0000;
    ((G9_ *)g)[0].w1 = (D_800F1DE8[arg0].r << 24) + (D_800F1DE8[arg0].g << 16) + (D_800F1DE8[arg0].b << 8);
    D_8014EC78 = g + 16;
    ((G9_ *)g)[1].w0 = ((arg0 + 4) & 0xFFFF) | 0xDB0A0000;
    ((G9_ *)g)[1].w1 = (D_800F1DE8[arg0].r << 24) + (D_800F1DE8[arg0].g << 16) + (D_800F1DE8[arg0].b << 8);
}

void func_80070BA4(s32 arg0);
void func_800709CC(void) {
    s32 m;
    s32 g;
    s32 l;

    m = ((s32 (*)(void)) func_80070BA4)();
    if ((u32) (D_8014108C + 0x40) >= (u32) D_8014EB58[D_8014C540].f4) {
        D_8014EB58[D_8014C540].f0 = D_8014108C;
        MtxPool_Grow(&D_8014EB58[D_8014C540], 1U);
        D_8014108C = D_8014EB58[D_8014C540].f0;
    }
    if ((u32) (D_8014EC78 + 8) >= (u32) D_8014C740[D_8014C540].f4) {
        D_8014C740[D_8014C540].f0 = D_8014EC78;
        GfxPool_Grow(&D_8014C740[D_8014C540], 1U);
        D_8014EC78 = D_8014C740[D_8014C540].f0;
    }
    guLookAtReflect((void *) m, (void *) D_8014108C, 50.0f, 0.0f, 400.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);
    g = D_8014EC78;
    l = D_8014108C;
    D_8014EC78 = g + 8;
    ((G9_ *)g)[0].w0 = 0xDC08000A;
    ((G9_ *)g)[0].w1 = l;
    D_8014EC78 = g + 16;
    ((G9_ *)g)[1].w0 = 0xDC08030A;
    ((G9_ *)g)[1].w1 = l + 0x10;
    D_8014108C = l + 0x40;
}

void func_80070BA4(s32 arg0) {
    s32 temp_a1;

    if ((u32) (D_8014108C + 0x40) >= (u32) D_8014EB58[D_8014C540].f4) {
        D_8014EB58[D_8014C540].f0 = D_8014108C;
        MtxPool_Grow(&D_8014EB58[D_8014C540], 1U);
        D_8014108C = D_8014EB58[D_8014C540].f0;
    }
    temp_a1 = D_8014108C;
    D_8014108C = temp_a1 + 0x40;
    func_80070C5C((void *) arg0, (void *) temp_a1);
}

INCLUDE_ASM("asm/nonmatchings/unit_103B0", func_80070C5C);

void func_80070E58(void) {
    register f32 temp_fs1_2;
    register f32 temp_fs0_3;
    register f32 temp_fv0;
    register f32 temp_fs1;

    temp_fs1 = D_800F2120 * 0.5f;
    D_800F2128 = 1.0f / cosf(temp_fs1);
    D_800F212C = -((sinf(temp_fs1)) / cosf(temp_fs1));
    temp_fs1_2 = temp_fs1 / D_800F2124;
    temp_fs0_3 = (sinf(temp_fs1_2)) / cosf(temp_fs1_2);
    temp_fv0 = D_800F211C * D_800F2118;
    D_800F2130 = -temp_fs0_3;
    D_800F2114 = temp_fv0;
    D_800F2134 = D_800F212C * temp_fv0;
}

INCLUDE_ASM("asm/nonmatchings/unit_103B0", func_80070F38);

INCLUDE_ASM("asm/nonmatchings/unit_103B0", func_8007126C);

INCLUDE_ASM("asm/nonmatchings/unit_103B0", func_80071530);

INCLUDE_ASM("asm/nonmatchings/unit_103B0", func_800717C8);

void func_80071924(void) {
    func_800D6010();
}

INCLUDE_ASM("asm/nonmatchings/unit_103B0", func_80071940);

INCLUDE_ASM("asm/nonmatchings/unit_103B0", func_80071A40);
