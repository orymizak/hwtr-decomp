#include "common.h"
#include "include_asm.h"
s32 func_800CD9EC();
void *func_800D2444();

s32 func_800D23FC();

void func_800CD9A0(void) {
    func_800D23FC();
}

s32 func_800CD9BC(void) {
    void *temp_s0;

    temp_s0 = func_800D2444();
    return (*(s32 *)((u8 *)(temp_s0) + 0x8)) - func_800CD9EC();
}

INCLUDE_ASM("asm/nonmatchings/unit_6E5A0", func_800CD9EC);
