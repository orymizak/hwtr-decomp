#include "common.h"
#include "include_asm.h"
s32 func_800D7208(s32,s32);

s32 osStartThread(s32 *);
extern s32 D_8012FE90;

void func_800D7130(void) {
    func_800D7208(0x304, 0);
}

void func_800D7150(void) {
    func_800D7208(0x305, 0);
}

INCLUDE_ASM("asm/nonmatchings/unit_77D30", func_800D7170);

INCLUDE_ASM("asm/nonmatchings/unit_77D30", func_800D7208);

void func_800D726C(void) {
    func_800D7208(0x7F00, 0);
}

void func_800D728C(void) {
    osStartThread(&D_8012FE90);
}

INCLUDE_ASM("asm/nonmatchings/unit_77D30", func_800D72AC);

INCLUDE_ASM("asm/nonmatchings/unit_77D30", func_800D7450);

INCLUDE_ASM("asm/nonmatchings/unit_77D30", func_800D74F0);
