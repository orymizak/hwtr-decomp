#include "common.h"
#include "include_asm.h"
s32 alHeapDBAlloc(s32,s32, void *,s32, s32);

extern s32 D_80127BA0;
extern s32 D_80127BA4;
extern s32 D_800FAEC0;
extern s32 D_80127BB0;
extern s32 D_80127BB4;
extern s32 D_80127BB8;
extern u32 D_80127BBC;

void func_800D23B0(s32 arg0, s32 arg1) {
    func_800D2450((void *) arg0, 0U, arg1);
    alHeapInit(&(*(u8 *)&D_80127BA0), (u32) arg0, (u32) arg1);
}

void func_800D23FC(s32 arg0) {
    alHeapDBAlloc(0, 0, &(*(u8 *)&D_80127BA0), 1, arg0);
}

s32 func_800D242C(void) {
    return D_80127BA4 - D_80127BA0;
}

s32 *func_800D2444(void) {
    return &D_80127BA0;
}

void func_800D2450(void *arg0, u8 arg1, s32 arg2) {
  volatile long long pad;
  s32 var_v0;
  void *var_a0;
  u8 new_var;
  var_a0 = arg0;
  var_v0 = arg2 - 1;
  if (arg2 != 0) {
    do
    {
      new_var = arg1;
      *((u8 *) var_a0) = new_var;
      var_v0 -= 1;
      var_a0 += 1;
    }
    while (var_v0 != (-1));
  }
}

INCLUDE_ASM("asm/nonmatchings/unit_72FB0", func_800D247C);

INCLUDE_ASM("asm/nonmatchings/unit_72FB0", func_800D24F0);

s32 func_800D2594(u32 arg0) {
    if ((u32) (D_80127BBC + 0xB8) < arg0) {
        if (D_800FAEC0 != 0) {
            D_800FAEC0 = 0;
            return D_80127BB4;
        }
        goto block_7;
    }
    if (arg0 < (u32) D_80127BBC) {
        if (D_800FAEC0 != 0) {
            D_800FAEC0 = 0;
            return D_80127BB8;
        }
        goto block_7;
    }
    D_800FAEC0 = 1;
block_7:
    return D_80127BB0;
}
