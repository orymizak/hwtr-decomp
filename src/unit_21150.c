#include "common.h"
#include "include_asm.h"

s32 Memory_Free(s32);
extern s32 D_80114800;
extern s32 D_8011480C;

INCLUDE_ASM("asm/nonmatchings/unit_21150", func_80080550);

s32 func_800806FC(void) {
    if (D_80114800 != 0) {
        Memory_Free(D_80114800);
    }
    D_80114800 = 0;
    return 1;
}

INCLUDE_ASM("asm/nonmatchings/unit_21150", func_80080730);

INCLUDE_ASM("asm/nonmatchings/unit_21150", func_80080AD0);

INCLUDE_ASM("asm/nonmatchings/unit_21150", func_80080C20);

INCLUDE_ASM("asm/nonmatchings/unit_21150", func_80081384);

INCLUDE_ASM("asm/nonmatchings/unit_21150", func_800817A8);

s32 func_8008182C(void) {
    return D_8011480C;
}
