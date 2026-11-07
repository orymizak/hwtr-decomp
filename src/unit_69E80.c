#include "common.h"
#include "include_asm.h"

s32 Memory_Alloc(s32);
s32 func_800C9280();
s32 func_800C96DC();
extern void *D_800F5F08;
s32 Memory_Free(s32);
extern s32 D_800F9F90;
extern s32 D_800F9F94;
extern s32 D_800F9F98;

INCLUDE_ASM("asm/nonmatchings/unit_69E80", func_800C9280);

INCLUDE_ASM("asm/nonmatchings/unit_69E80", func_800C9590);

INCLUDE_ASM("asm/nonmatchings/unit_69E80", func_800C96DC);

INCLUDE_ASM("asm/nonmatchings/unit_69E80", func_800C9848);

INCLUDE_ASM("asm/nonmatchings/unit_69E80", func_800C9978);

INCLUDE_ASM("asm/nonmatchings/unit_69E80", func_800C9A00);

INCLUDE_ASM("asm/nonmatchings/unit_69E80", func_800C9AF0);

void func_800C9B40(void) {
    if (D_800F9F90 != 0) {
        Memory_Free(D_800F9F98);
        D_800F9F98 = 0;
        D_800F9F94 = 0;
        D_800F9F90 = 0;
    }
}

INCLUDE_ASM("asm/nonmatchings/unit_69E80", func_800C9B84);

INCLUDE_ASM("asm/nonmatchings/unit_69E80", func_800C9D00);

INCLUDE_ASM("asm/nonmatchings/unit_69E80", func_800C9DA0);

INCLUDE_ASM("asm/nonmatchings/unit_69E80", func_800C9E70);

INCLUDE_ASM("asm/nonmatchings/unit_69E80", func_800C9ED0);

INCLUDE_ASM("asm/nonmatchings/unit_69E80", func_800C9F18);

INCLUDE_ASM("asm/nonmatchings/unit_69E80", func_800C9F78);
