#include "common.h"
#include "include_asm.h"
s32 func_80099434(void *);
s32 func_8009951C(void *);
extern u8 D_80118634;
extern u8 D_80118630;

extern s32 D_8011B318;

INCLUDE_ASM("asm/nonmatchings/unit_31E10", func_80091210);

INCLUDE_ASM("asm/nonmatchings/unit_31E10", func_80091B20);

INCLUDE_ASM("asm/nonmatchings/unit_31E10", func_80091BA0);

INCLUDE_ASM("asm/nonmatchings/unit_31E10", func_80091DC4);

INCLUDE_ASM("asm/nonmatchings/unit_31E10", func_80091E30);

INCLUDE_ASM("asm/nonmatchings/unit_31E10", func_80092534);

INCLUDE_ASM("asm/nonmatchings/unit_31E10", func_80093164);

INCLUDE_ASM("asm/nonmatchings/unit_31E10", func_800937E8);

void func_80093990(void) {

}

INCLUDE_ASM("asm/nonmatchings/unit_31E10", func_80093998);

INCLUDE_ASM("asm/nonmatchings/unit_31E10", func_80093B74);

INCLUDE_ASM("asm/nonmatchings/unit_31E10", func_80093D34);

INCLUDE_ASM("asm/nonmatchings/unit_31E10", func_80094234);

INCLUDE_ASM("asm/nonmatchings/unit_31E10", func_800946DC);

INCLUDE_ASM("asm/nonmatchings/unit_31E10", func_800949E4);

INCLUDE_ASM("asm/nonmatchings/unit_31E10", func_80095804);

INCLUDE_ASM("asm/nonmatchings/unit_31E10", func_80095970);

INCLUDE_ASM("asm/nonmatchings/unit_31E10", func_80095C60);

INCLUDE_ASM("asm/nonmatchings/unit_31E10", func_80095D5C);

INCLUDE_ASM("asm/nonmatchings/unit_31E10", func_80095FE4);

void *func_800960D8(s32 arg0) {
    return ((arg0 & 0xFF) * 0x77C) + &D_80118630;
}

s32 func_80096100(void) {
    return D_8011B318;
}

s32 func_8009610C(s32 arg0) {
    return *(s32 *)(&D_80118634 + ((arg0 & 0xFF) * 0x77C)) & 1;
}

void func_80096138(s32 arg0, s32 arg1) {
    void *temp_s0;

    temp_s0 = ((arg0 & 0xFF) * 0x77C) + &D_80118630;
    if ((*(s32 *)((u8 *)(temp_s0) + 0x6D8)) == 2) {
        func_8009951C(temp_s0);
    }
    (*(s32 *)((u8 *)(temp_s0) + 0x6D8)) = arg1;
    if (arg1 == 2) {
        func_80099434(temp_s0);
    }
}

INCLUDE_ASM("asm/nonmatchings/unit_31E10", func_800961B0);

INCLUDE_ASM("asm/nonmatchings/unit_31E10", func_80096954);

INCLUDE_ASM("asm/nonmatchings/unit_31E10", func_8009751C);

INCLUDE_ASM("asm/nonmatchings/unit_31E10", func_800977AC);

INCLUDE_ASM("asm/nonmatchings/unit_31E10", func_800979F4);

INCLUDE_ASM("asm/nonmatchings/unit_31E10", func_80097B00);

INCLUDE_ASM("asm/nonmatchings/unit_31E10", func_80098300);

INCLUDE_ASM("asm/nonmatchings/unit_31E10", func_80098584);

INCLUDE_ASM("asm/nonmatchings/unit_31E10", func_8009887C);

INCLUDE_ASM("asm/nonmatchings/unit_31E10", func_80098BC0);

s32 func_80098BEC(void) {
    return 1;
}

INCLUDE_ASM("asm/nonmatchings/unit_31E10", func_80098BF4);

INCLUDE_ASM("asm/nonmatchings/unit_31E10", func_80098D84);

INCLUDE_ASM("asm/nonmatchings/unit_31E10", func_80098E84);
