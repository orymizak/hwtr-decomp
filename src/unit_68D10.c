#include "common.h"
#include "include_asm.h"

extern s32 (*D_801455A0)(s32);
extern s32 D_80124D48;
extern s32 D_80124D51;
extern s32 D_80124D40;
extern s32 (*D_80141048)();
extern s32 D_80124D44;

INCLUDE_ASM("asm/nonmatchings/unit_68D10", func_800C8110);

s32 func_800C8134(s32 arg0) {
    return D_801455A0(arg0 & 0xFF) & 0xFF;
}

s32 *func_800C815C(void) {
    return &D_80124D48;
}

s32 *func_800C8168(void) {
    return &D_80124D51;
}

INCLUDE_ASM("asm/nonmatchings/unit_68D10", func_800C8174);

INCLUDE_ASM("asm/nonmatchings/unit_68D10", func_800C81A0);

INCLUDE_ASM("asm/nonmatchings/unit_68D10", func_800C81CC);

INCLUDE_ASM("asm/nonmatchings/unit_68D10", func_800C8214);

INCLUDE_ASM("asm/nonmatchings/unit_68D10", func_800C82B4);

INCLUDE_ASM("asm/nonmatchings/unit_68D10", func_800C8320);

INCLUDE_ASM("asm/nonmatchings/unit_68D10", func_800C838C);

INCLUDE_ASM("asm/nonmatchings/unit_68D10", func_800C83F8);

INCLUDE_ASM("asm/nonmatchings/unit_68D10", func_800C8464);

INCLUDE_ASM("asm/nonmatchings/unit_68D10", func_800C84B0);

void func_800C84FC(void) {
    D_80124D40 = D_80141048();
}

void func_800C8528(void) {
    D_80124D44 = D_80141048();
}

INCLUDE_ASM("asm/nonmatchings/unit_68D10", func_800C8554);

INCLUDE_ASM("asm/nonmatchings/unit_68D10", func_800C891C);

INCLUDE_ASM("asm/nonmatchings/unit_68D10", func_800C8CE4);

INCLUDE_ASM("asm/nonmatchings/unit_68D10", func_800C8D40);
