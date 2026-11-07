#include "common.h"
#include "include_asm.h"
s32 func_800A5894(void);
extern u8 D_801185AC;
extern u8 D_801185A4;
extern u8 D_801185A2;



INCLUDE_ASM("asm/nonmatchings/unit_2F4F0", func_8008E8F0);

s32 func_8008EA64(void) {
    return 1;
}

INCLUDE_ASM("asm/nonmatchings/unit_2F4F0", func_8008EA6C);

INCLUDE_ASM("asm/nonmatchings/unit_2F4F0", func_8009013C);

INCLUDE_ASM("asm/nonmatchings/unit_2F4F0", func_80090410);

INCLUDE_ASM("asm/nonmatchings/unit_2F4F0", func_800904C4);

void func_80090550(s32 arg0, s8 arg1) {
    *(u8 *)(&D_801185A2 + ((arg0 & 0xFF) * 0x64)) = arg1;
}

INCLUDE_ASM("asm/nonmatchings/unit_2F4F0", func_80090578);

void func_800908A0(void) {

}

s32 func_800908A8(void) {
  s32 temp_v0;
  temp_v0 = func_800A5894();
  if (temp_v0 != 0) {
    if (temp_v0) {
      return (temp_v0 - 1) * 0xC8;
    }
    else
    {
      return (temp_v0 - 1) * 0xC8;
    }
  }
  return 0;
}

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
