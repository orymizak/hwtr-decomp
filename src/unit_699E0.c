#include "common.h"
#include "include_asm.h"
void (*D_80141004)(s32);



INCLUDE_ASM("asm/nonmatchings/unit_699E0", func_800C8DE0);

INCLUDE_ASM("asm/nonmatchings/unit_699E0", func_800C8EE4);

INCLUDE_ASM("asm/nonmatchings/unit_699E0", func_800C902C);

INCLUDE_ASM("asm/nonmatchings/unit_699E0", func_800C90DC);

void func_800C9130(void) {
    void (**fp)(s32) = &D_80141004;

    (*fp)(0);
    (*fp)(1);
}


INCLUDE_ASM("asm/nonmatchings/unit_699E0", func_800C916C);

INCLUDE_ASM("asm/nonmatchings/unit_699E0", func_800C9200);
