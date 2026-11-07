#include "common.h"
#include "include_asm.h"

extern s32 (*D_801416A4)();
extern s32 (*D_801416A8)();
extern s32 D_801345C8;

INCLUDE_ASM("asm/nonmatchings/unit_4F640", func_800AEA40);

s32 func_800AEAFC(void) {
    return 1;
}

void func_800AEB04(void) {
    D_801416A4();
}

INCLUDE_ASM("asm/nonmatchings/unit_4F640", func_800AEB28);

void func_800AEDFC(void) {
    D_801416A8();
}

INCLUDE_ASM("asm/nonmatchings/unit_4F640", func_800AEE20);

s32 func_800AEF70(void) {
    return D_801345C8;
}
