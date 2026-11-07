#include "common.h"
#include "include_asm.h"
extern u8 D_80114838;

s32 func_80081840();

INCLUDE_ASM("asm/nonmatchings/unit_22440", func_80081840);

void func_80084B80(void) {
    func_80081840();
}

INCLUDE_ASM("asm/nonmatchings/unit_22440", func_80084B9C);

INCLUDE_ASM("asm/nonmatchings/unit_22440", func_80084FDC);

INCLUDE_ASM("asm/nonmatchings/unit_22440", func_800853B4);

INCLUDE_ASM("asm/nonmatchings/unit_22440", func_800853F4);

INCLUDE_ASM("asm/nonmatchings/unit_22440", func_80085780);

s32 func_800858E8(void) {
    return 1;
}

INCLUDE_ASM("asm/nonmatchings/unit_22440", func_800858F0);

void *func_8008637C(u8 *arg0) {
    return (*arg0 * 0x2A8) + &D_80114838;
}

INCLUDE_ASM("asm/nonmatchings/unit_22440", func_800863A4);
