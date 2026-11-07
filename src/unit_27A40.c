#include "common.h"
#include "include_asm.h"
s32 func_80060840();
s32 func_80061334();
s32 func_8006B630();
s32 func_8007EF30();



INCLUDE_ASM("asm/nonmatchings/unit_27A40", func_80086E40);

void func_80086F78(void) {

}

INCLUDE_ASM("asm/nonmatchings/unit_27A40", func_80086F80);

INCLUDE_ASM("asm/nonmatchings/unit_27A40", func_8008711C);

INCLUDE_ASM("asm/nonmatchings/unit_27A40", func_80087260);

INCLUDE_ASM("asm/nonmatchings/unit_27A40", func_8008744C);

INCLUDE_ASM("asm/nonmatchings/unit_27A40", func_8008745C);

INCLUDE_ASM("asm/nonmatchings/unit_27A40", func_800875A0);

s32 func_80087660(void) {
    s8 var_s0;
    register s32 temp_s1;

    temp_s1 = (func_80060840() & 0xFF) == 0;
    var_s0 = temp_s1;
    if (!(func_80061334() & 0xFF)) {
        var_s0 = temp_s1 + 1;
    }
    if (!(func_8006B630() & 0xFF)) {
        var_s0 += 1;
    }
    if (!(func_8007EF30() & 0xFF)) {
        var_s0 += 1;
    }
    return (var_s0 & 0xFF) == 0;
}

INCLUDE_ASM("asm/nonmatchings/unit_27A40", func_800876D4);

s32 func_8008770C(void) {
    return 0;
}
