#include "common.h"
#include "include_asm.h"
typedef struct { s32 w0; u8 pad[0x8C]; u8 x90[0x94]; u8 x124[0xA0]; } Ent1C4_;
void func_800ACDC8(void *, f32);
void func_8007A9E0(void *);

s32 func_8009A770(s32 *);
extern s32 D_8011E870;

s32 func_800A58D0(void) {
    u8 *e = (u8 *)&D_8011E870;
    u32 i;

    memset(e, 0, 0xE20);
    for (i = 0; i < 8; i++) {
        *(s32 *)e = 0;
        *(u8 **)(e + 0x68) = e + 0x90;
        *(u8 **)(e + 0x54) = e + 0x1A8;
        *(u8 **)(e + 0x58) = e + 0x1B4;
        *(u8 *)(e + 0x50) = 1;
        *(u8 **)(e + 0x5C) = e + 0x1C0;
        e += 0x1C4;
    }
    return 1;
}


s32 func_800A5950(void) {
    Ent1C4_ *e = (Ent1C4_ *)&D_8011E870;
    u32 i;

    for (i = 0; i < 8; i++, e++) {
        if (e->w0 == 1) {
            func_8009A770((s32 *)((u8 *)e + 8));
            e->w0 = 0;
        }
    }
    return 1;
}


void func_800A59B8(void) {
    Ent1C4_ *e = (Ent1C4_ *)&D_8011E870;
    u32 i;

    for (i = 0; i < 8; i++, e++) {
        if (e->w0 != 0) {
            func_800ACDC8(e->x90, 0.01f);
            func_8007A9E0(e->x124);
        }
    }
}


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
