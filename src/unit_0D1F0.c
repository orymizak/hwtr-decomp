#include "common.h"
#include "include_asm.h"
typedef struct { s32 f0; s32 f4; u8 p8[0x548]; } S_D_801455C0;
extern S_D_801455C0 D_801455C0[];
s32 func_8007F5C4(s32,s32);
s32 func_8006DD60(s32 *,s32,s32);
extern s32 D_8014D980;

s32 func_800757E8(s32);
s32 func_8006CF10(s32 *, s32);
s32 func_8006D15C(s32 *, s32, s32);
s32 Memory_Free(s32);
extern s32 D_800F05E0;
s32 func_8007F57C(s32);
extern s32 D_800F05E4;

INCLUDE_ASM("asm/nonmatchings/unit_0D1F0", func_8006C5F0);

INCLUDE_ASM("asm/nonmatchings/unit_0D1F0", func_8006CA88);

INCLUDE_ASM("asm/nonmatchings/unit_0D1F0", func_8006CF10);

INCLUDE_ASM("asm/nonmatchings/unit_0D1F0", func_8006D15C);

INCLUDE_ASM("asm/nonmatchings/unit_0D1F0", func_8006D608);

INCLUDE_ASM("asm/nonmatchings/unit_0D1F0", func_8006D6EC);

void func_8006DA40(void *arg0) {
    s32 temp_a0;

    temp_a0 = *(s32 *)((u8 *)arg0 + 0x4);
    if (temp_a0 != -1) {
        func_800757E8(temp_a0);
    }
    *(s32 *)((u8 *)arg0 + 0x0) = 0;
    *(s32 *)((u8 *)arg0 + 0x4) = -1;
}

void func_8006DA88(void) {
    u32 var_a0;

    var_a0 = 0;
    do {
        D_801455C0[var_a0].f0 = 0;
        D_801455C0[var_a0].f4 = -1;
        var_a0 += 1;
    } while (var_a0 < 8U);
}

void func_8006DAC4(s32 arg0) {
    void *var_s0;
    u32 var_s1;

    var_s1 = 0;
    var_s0 = &(*(u8 *)&D_801455C0);
    do {
        ((s32 (*)(void *, s32))func_8006CF10)(var_s0, arg0);
        var_s1 += 1;
        var_s0 += 0x550;
    } while (var_s1 < 8U);
}

void func_8006DB1C(s32 arg0, s32 arg1) {
    void *var_s0;
    u32 var_s1;

    var_s1 = 0;
    var_s0 = &(*(u8 *)&D_801455C0);
    do {
        ((s32 (*)(void *, s32, s32))func_8006D15C)(var_s0, arg0, arg1 & 0xFF);
        var_s1 += 1;
        var_s0 += 0x550;
    } while (var_s1 < 8U);
}

INCLUDE_ASM("asm/nonmatchings/unit_0D1F0", func_8006DB84);

INCLUDE_ASM("asm/nonmatchings/unit_0D1F0", func_8006DC28);

void func_8006DC90(void) {
    func_8006DD60(&D_8014D980, 0x140, 0xF0);
}

void func_8006DCB8(void) {
    if (D_800F05E0 != 0) {
        Memory_Free(D_800F05E0);
        D_800F05E0 = 0;
    }
}

void func_8006DCE8(s32 arg0) {
    if (D_800F05E0 != 0) {
        Memory_Free(D_800F05E0);
        D_800F05E0 = 0;
    }
    D_800F05E0 = func_8007F57C(arg0);
}

INCLUDE_ASM("asm/nonmatchings/unit_0D1F0", func_8006DD34);

INCLUDE_ASM("asm/nonmatchings/unit_0D1F0", func_8006DD60);

INCLUDE_ASM("asm/nonmatchings/unit_0D1F0", func_8006DEF4);

INCLUDE_ASM("asm/nonmatchings/unit_0D1F0", func_8006DF34);

void func_8006DFBC(void) {
    if (D_800F05E0 != 0) {
        Memory_Free(D_800F05E0);
        D_800F05E0 = 0;
    }
    D_800F05E0 = func_8007F5C4(0x20, 0x20);
    D_800F05E4 = 1;
}

void func_8006E00C(s32 arg0) {
    if (D_800F05E0 != 0) {
        Memory_Free(D_800F05E0);
        D_800F05E0 = 0;
    }
    D_800F05E0 = func_8007F57C(arg0);
    D_800F05E4 = 0;
}
