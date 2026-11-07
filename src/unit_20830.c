#include "common.h"
#include "include_asm.h"
s32 func_8007FD40(s32);
s32 func_80080070(s32);
extern s32 D_800F2690;

extern s32 D_8010C410;
extern s32 D_8010C414;

INCLUDE_ASM("asm/nonmatchings/unit_20830", func_8007FC30);

INCLUDE_ASM("asm/nonmatchings/unit_20830", func_8007FD04);

INCLUDE_ASM("asm/nonmatchings/unit_20830", func_8007FD40);

INCLUDE_ASM("asm/nonmatchings/unit_20830", func_80080070);

INCLUDE_ASM("asm/nonmatchings/unit_20830", func_800802C0);

void func_80080360(void) {
    D_8010C410 = 0;
    D_8010C414 = 0;
}

INCLUDE_ASM("asm/nonmatchings/unit_20830", func_80080374);

INCLUDE_ASM("asm/nonmatchings/unit_20830", func_80080460);

void func_800804B4(void) {
    func_8007FD40(*(s32 *)(&(*(u8 *)&D_8010C410) + (D_800F2690 * 4)));
    func_80080070(*(s32 *)(&(*(u8 *)&D_8010C410) + (D_800F2690 * 4)));
}

INCLUDE_ASM("asm/nonmatchings/unit_20830", func_80080500);
