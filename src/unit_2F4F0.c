#include "common.h"
#include "include_asm.h"
typedef struct { s32 x; s32 y; s32 z; } Pos_;
extern u8 D_80118558[];
extern u8 D_801185AC;
extern u8 D_801185A4;
extern u8 D_801185A2;



INCLUDE_ASM("asm/nonmatchings/unit_2F4F0", func_8008E8F0);

s32 func_8008EA64(void) {
    return 1;
}

INCLUDE_ASM("asm/nonmatchings/unit_2F4F0", func_8008EA6C);

INCLUDE_ASM("asm/nonmatchings/unit_2F4F0", func_8009013C);

void func_80090410(u8 i, Pos_ *a1, f32 *a2, f32 *a3, f32 *a4, f32 *a5, s8 *a6, u8 *a7) {
    u8 *p = D_80118558 + i * 0x64;

    *a1 = *(Pos_ *)(p + 4);
    a2[0] = *(f32 *)(p + 0x1C);
    a2[1] = *(f32 *)(p + 0x28);
    a2[2] = *(f32 *)(p + 0x34);
    a3[0] = *(f32 *)(p + 0x20);
    a3[1] = *(f32 *)(p + 0x2C);
    a3[2] = *(f32 *)(p + 0x38);
    a4[0] = *(f32 *)(p + 0x24);
    a4[1] = *(f32 *)(p + 0x30);
    a4[2] = *(f32 *)(p + 0x3C);
    *a5 = *(f32 *)(p + 0x40);
    *a6 = *(s32 *)(p + 0x44) == 0;
    *a7 = p[0x49];
}

INCLUDE_ASM("asm/nonmatchings/unit_2F4F0", func_800904C4);

void func_80090550(s32 arg0, s8 arg1) {
    *(u8 *)(&D_801185A2 + ((arg0 & 0xFF) * 0x64)) = arg1;
}

INCLUDE_ASM("asm/nonmatchings/unit_2F4F0", func_80090578);

void func_800908A0(void) {

}

INCLUDE_ASM("asm/nonmatchings/unit_2F4F0", func_800908A8);

INCLUDE_ASM("asm/nonmatchings/unit_2F4F0", func_800908E8);

INCLUDE_ASM("asm/nonmatchings/unit_2F4F0", func_80090934);

INCLUDE_ASM("asm/nonmatchings/unit_2F4F0", func_8009099C);

INCLUDE_ASM("asm/nonmatchings/unit_2F4F0", func_80090D80);

INCLUDE_ASM("asm/nonmatchings/unit_2F4F0", func_80090FA8);

void func_800911B4(s32 arg0) {
    *(u8 *)(&D_801185A4 + ((arg0 & 0xFF) * 0x64)) = 1;
}

void func_800911E0(s32 arg0) {
    *(s32 *)(&D_801185AC + ((arg0 & 0xFF) * 0x64)) = 0xFA;
}
