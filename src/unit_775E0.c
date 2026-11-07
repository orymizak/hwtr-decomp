#include "common.h"
#include "include_asm.h"

s32 func_800D74F0(s32 *);
extern s32 D_800FB210;
extern u8 D_80140F6F;

INCLUDE_ASM("asm/nonmatchings/unit_775E0", func_800D69E0);

void func_800D6A40(void) {
    func_800D74F0(&D_800FB210);
}

INCLUDE_ASM("asm/nonmatchings/unit_775E0", func_800D6A60);

INCLUDE_ASM("asm/nonmatchings/unit_775E0", func_800D6C4C);

INCLUDE_ASM("asm/nonmatchings/unit_775E0", func_800D6DCC);

INCLUDE_ASM("asm/nonmatchings/unit_775E0", func_800D6E14);

s32 func_800D6E5C(void) {
    u32 var_a0;
    u8 *var_v1;

    var_a0 = 0;
    var_v1 = &D_80140F6F;
    do {
        var_a0 += 1;
        *var_v1 &= 0x7F;
        var_v1 += 0xA;
    } while (var_a0 < 4U);
    return 0;
}

INCLUDE_ASM("asm/nonmatchings/unit_775E0", func_800D6E8C);

INCLUDE_ASM("asm/nonmatchings/unit_775E0", func_800D6F18);

INCLUDE_ASM("asm/nonmatchings/unit_775E0", func_800D6F40);
