#include "common.h"
#include "include_asm.h"

extern s32 D_8010C410;
extern s32 D_8010C414;

INCLUDE_ASM("asm/nonmatchings/auto_test", func_8007FC30);

INCLUDE_ASM("asm/nonmatchings/auto_test", func_8007FD04);

INCLUDE_ASM("asm/nonmatchings/auto_test", func_8007FD40);

INCLUDE_ASM("asm/nonmatchings/auto_test", func_80080070);

INCLUDE_ASM("asm/nonmatchings/auto_test", func_800802C0);

void func_80080360(void) {
    D_8010C410 = 0;
    D_8010C414 = 0;
}

INCLUDE_ASM("asm/nonmatchings/auto_test", func_80080374);

INCLUDE_ASM("asm/nonmatchings/auto_test", func_80080460);

INCLUDE_ASM("asm/nonmatchings/auto_test", func_800804B4);

INCLUDE_ASM("asm/nonmatchings/auto_test", func_80080500);
