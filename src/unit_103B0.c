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

INCLUDE_ASM("asm/nonmatchings/unit_103B0", func_8006F8C4);

INCLUDE_ASM("asm/nonmatchings/unit_103B0", func_8006FA7C);

INCLUDE_ASM("asm/nonmatchings/unit_103B0", func_8006FBF0);

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


INCLUDE_ASM("asm/nonmatchings/unit_103B0", func_8006FF90);

INCLUDE_ASM("asm/nonmatchings/unit_103B0", func_80070148);

INCLUDE_ASM("asm/nonmatchings/unit_103B0", func_800702C4);

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

INCLUDE_ASM("asm/nonmatchings/unit_103B0", func_80070860);

INCLUDE_ASM("asm/nonmatchings/unit_103B0", func_800709CC);

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
