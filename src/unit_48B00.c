#include "common.h"
#include "include_asm.h"



INCLUDE_ASM("asm/nonmatchings/unit_48B00", func_800A7F00);

INCLUDE_ASM("asm/nonmatchings/unit_48B00", func_800A7F5C);

INCLUDE_ASM("asm/nonmatchings/unit_48B00", func_800A7F98);

INCLUDE_ASM("asm/nonmatchings/unit_48B00", func_800A8050);

s32 func_800A81B4(void *arg0, s32 arg1) {
    return *(s32 *)((u8 *)arg0 + 0x8) + (*(u16 *)((u8 *)arg0 + 0x0) * (arg1 & 0xFFFF));
}

u16 func_800A81D4(void *arg0) {
    return *(u16 *)((u8 *)arg0 + 0x4);
}

INCLUDE_ASM("asm/nonmatchings/unit_48B00", func_800A81DC);

INCLUDE_ASM("asm/nonmatchings/unit_48B00", func_800A8218);

INCLUDE_ASM("asm/nonmatchings/unit_48B00", func_800A8268);

INCLUDE_ASM("asm/nonmatchings/unit_48B00", func_800A82F8);
