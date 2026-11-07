#include "common.h"
#include "include_asm.h"
s32 func_800D26C0(s32);
extern void *D_800FB174;



s32 func_800D5490(s32 arg0) {
  s32 temp_s0;
  int new_var;
  temp_s0 = ((s32) (arg0 << 0x10)) >> 0xE;
  func_800D26C0((*((s32 *) (((u8 *) D_800FB174) + 0x34))) + (temp_s0 + (new_var = 0x24)));
  return *((s32 *) ((temp_s0 + (*((s32 *) (((u8 *) D_800FB174) + 0x34)))) + 0x24));
}

INCLUDE_ASM("asm/nonmatchings/unit_76090", func_800D54E0);
