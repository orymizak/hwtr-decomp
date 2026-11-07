#include "common.h"
#include "include_asm.h"

extern s32 D_80140E44;
s32 func_800D6460();
s32 func_800D69E0();
s32 func_800D7170();
s32 func_800D7570();

void func_800D61F0(void) {
    D_80140E44 = 0x80;
}

INCLUDE_ASM("asm/nonmatchings/unit_76DF0", func_800D6200);

INCLUDE_ASM("asm/nonmatchings/unit_76DF0", func_800D6240);

s32 func_800D6300(void) {
    s32 temp_s0;

    temp_s0 = func_800D7170();
    func_800D7570();
    func_800D6460();
    func_800D69E0();
    return temp_s0 & 0xFF;
}
