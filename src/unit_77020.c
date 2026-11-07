#include "common.h"
#include "include_asm.h"

s32 func_800D7450(s32 *);
extern s32 D_800FB1E0;
s32 func_800D74F0(s32 *);
s32 func_800E1900(s32 *, void *, s32);
extern s32 D_8014EB40;
s32 func_800E16F0(s32, void *);
s32 func_800E0F50(s32, s32, u8, s32, s32, s32);
s32 func_800E0900(s32, u16, s32, s32, s32, s32, s32);
s32 func_800E1530(s32, u16, s32, s32, s32, s32);
extern u16 D_800FB1B0;
extern s32 D_800FB1B4;
s32 func_800E1390(s32, s32, s32);

INCLUDE_ASM("asm/nonmatchings/unit_77020", func_800D6420);

void func_800D6460(void) {
    func_800D7450(&D_800FB1E0);
}

void func_800D6480(void) {
    func_800D74F0(&D_800FB1E0);
}

s32 func_800D64A0(void *arg0) {
    s32 temp_v0;
    void *temp_a1;
    void *temp_s0;

    temp_s0 = *(s32 *)((u8 *)arg0 + 0xC);
    temp_a1 = *(s32 *)((u8 *)temp_s0 + 0x0);
    *(s32 *)((u8 *)temp_s0 + 0xC) = 0;
    temp_v0 = func_800E1900(&D_8014EB40, temp_a1, *(s32 *)((u8 *)temp_a1 + 0x8));
    if (temp_v0 == 0) {
        *(s32 *)((u8 *)temp_s0 + 0xC) = 1;
    }
    return temp_v0;
}

INCLUDE_ASM("asm/nonmatchings/unit_77020", func_800D64F0);

INCLUDE_ASM("asm/nonmatchings/unit_77020", func_800D6518);

INCLUDE_ASM("asm/nonmatchings/unit_77020", func_800D6558);

INCLUDE_ASM("asm/nonmatchings/unit_77020", func_800D6610);

void func_800D6650(void *arg0) {
    void *temp_v0;
    void *temp_v1;

    temp_v1 = *(s32 *)((u8 *)arg0 + 0xC);
    temp_v0 = *(s32 *)((u8 *)temp_v1 + 0x0);
    func_800E1390(*(s32 *)((u8 *)temp_v0 + 0x0), *(s32 *)((u8 *)temp_v0 + 0x4), *(s32 *)((u8 *)temp_v1 + 0x4));
}

INCLUDE_ASM("asm/nonmatchings/unit_77020", func_800D667C);

INCLUDE_ASM("asm/nonmatchings/unit_77020", func_800D66A8);
