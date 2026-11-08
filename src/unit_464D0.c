#include "common.h"
#include "include_asm.h"

s32 func_8009A770(s32 *);
extern s32 D_8011E870;

INCLUDE_ASM("asm/nonmatchings/unit_464D0", func_800A58D0);

INCLUDE_ASM("asm/nonmatchings/unit_464D0", func_800A5950);

INCLUDE_ASM("asm/nonmatchings/unit_464D0", func_800A59B8);

INCLUDE_ASM("asm/nonmatchings/unit_464D0", func_800A5A1C);

INCLUDE_ASM("asm/nonmatchings/unit_464D0", func_800A5AB0);

void func_800A6630(void *arg0) {
  s32 *var_s0;
  unsigned int new_var;
  s32 *var_s1;
  u32 var_s2;
  var_s1 = &D_8011E870;
  var_s2 = 0;
  new_var = 1;
  var_s0 = ((u8 *) (&D_8011E870)) + 8;
  do
  {
    if (((*var_s1) == new_var) && ((*((u8 *) (((u8 *) var_s0) - 0x4))) == (*((u8 *) arg0)))) {
      func_8009A770(var_s0);
      *var_s1 = 0;
    }
    var_s0 += 113;
    var_s2 += 1;
    var_s1 += 113;
  }
  while (var_s2 < 8U);
}

INCLUDE_ASM("asm/nonmatchings/unit_464D0", func_800A66C0);

INCLUDE_ASM("asm/nonmatchings/unit_464D0", func_800A6850);

INCLUDE_ASM("asm/nonmatchings/unit_464D0", func_800A6BA4);

INCLUDE_ASM("asm/nonmatchings/unit_464D0", func_800A6D98);
