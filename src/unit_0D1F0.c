#include "common.h"
#include "include_asm.h"
s32 func_8007F5C4(s32,s32);
s32 func_8006DD60(s32 *,s32,s32);
extern s32 D_8014D980;

s32 func_800757E8(s32);
s32 func_8006CF10(s32 *, s32);
extern s32 D_801455C0;
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

INCLUDE_ASM("asm/nonmatchings/unit_0D1F0", func_8006DA88);

INCLUDE_ASM("asm/nonmatchings/unit_0D1F0", func_8006DAC4);

INCLUDE_ASM("asm/nonmatchings/unit_0D1F0", func_8006DB1C);

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
