#include "common.h"
#include "include_asm.h"



INCLUDE_ASM("asm/nonmatchings/unit_48FA0", func_800A83A0);

INCLUDE_ASM("asm/nonmatchings/unit_48FA0", func_800A8670);

INCLUDE_ASM("asm/nonmatchings/unit_48FA0", func_800A8710);

INCLUDE_ASM("asm/nonmatchings/unit_48FA0", func_800A8BA4);

INCLUDE_ASM("asm/nonmatchings/unit_48FA0", func_800A8C94);

void func_800A8E14(void *arg0) {
    f32 temp_ft0;
    f32 temp_ft1;
    f32 temp_fv0;
    f32 temp_fv1;
    f32 temp_fv1_2;

    temp_ft1 = *(f32 *)((u8 *)arg0 + 0xC);
    temp_ft0 = *(f32 *)((u8 *)arg0 + 0x0);
    temp_fv1 = *(f32 *)((u8 *)arg0 + 0x4);
    temp_fv0 = *(f32 *)((u8 *)arg0 + 0x8);
    temp_fv1_2 = 1.0f / sqrtf((temp_ft1 * temp_ft1) + (temp_ft0 * temp_ft0) + ((temp_fv1 * temp_fv1) + (temp_fv0 * temp_fv0)));
    *(f32 *)((u8 *)arg0 + 0xC) = (f32) (*(f32 *)((u8 *)arg0 + 0xC) * temp_fv1_2);
    *(f32 *)((u8 *)arg0 + 0x0) = (f32) (*(f32 *)((u8 *)arg0 + 0x0) * temp_fv1_2);
    *(f32 *)((u8 *)arg0 + 0x4) = (f32) (*(f32 *)((u8 *)arg0 + 0x4) * temp_fv1_2);
    *(f32 *)((u8 *)arg0 + 0x8) = (f32) (*(f32 *)((u8 *)arg0 + 0x8) * temp_fv1_2);
}

INCLUDE_ASM("asm/nonmatchings/unit_48FA0", func_800A8EBC);

INCLUDE_ASM("asm/nonmatchings/unit_48FA0", func_800A9068);
