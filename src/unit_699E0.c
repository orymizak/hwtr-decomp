#include "common.h"
#include "include_asm.h"
extern void (*D_80141004)(s32);
extern void (*D_80141000)(s32, void *);
extern u8 D_80101070;
extern u8 D_80101078;
void func_800C8DE0(void);



INCLUDE_ASM("asm/nonmatchings/unit_699E0", func_800C8DE0);

INCLUDE_ASM("asm/nonmatchings/unit_699E0", func_800C8EE4);

INCLUDE_ASM("asm/nonmatchings/unit_699E0", func_800C902C);

void func_800C90DC(void) {
    void (**fp)(s32, void *) = &D_80141000;

    (*fp)(0, &D_80101070);
    (*fp)(1, &D_80101078);
    func_800C8DE0();
}

void func_800C9130(void) {
    void (**fp)(s32) = &D_80141004;

    (*fp)(0);
    (*fp)(1);
}


INCLUDE_ASM("asm/nonmatchings/unit_699E0", func_800C916C);

INCLUDE_ASM("asm/nonmatchings/unit_699E0", func_800C9200);
