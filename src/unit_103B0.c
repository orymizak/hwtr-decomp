#include "common.h"
#include "include_asm.h"
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

INCLUDE_ASM("asm/nonmatchings/unit_103B0", func_8006FDB8);

INCLUDE_ASM("asm/nonmatchings/unit_103B0", func_8006FE2C);

INCLUDE_ASM("asm/nonmatchings/unit_103B0", func_8006FF00);

INCLUDE_ASM("asm/nonmatchings/unit_103B0", func_8006FF90);

INCLUDE_ASM("asm/nonmatchings/unit_103B0", func_80070148);

INCLUDE_ASM("asm/nonmatchings/unit_103B0", func_800702C4);

INCLUDE_ASM("asm/nonmatchings/unit_103B0", func_800704DC);

INCLUDE_ASM("asm/nonmatchings/unit_103B0", func_80070780);

INCLUDE_ASM("asm/nonmatchings/unit_103B0", func_80070860);

INCLUDE_ASM("asm/nonmatchings/unit_103B0", func_800709CC);

INCLUDE_ASM("asm/nonmatchings/unit_103B0", func_80070BA4);

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
