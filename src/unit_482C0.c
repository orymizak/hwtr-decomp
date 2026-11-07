#include "common.h"
#include "include_asm.h"
void *func_800960D8(u32);
extern s32 D_800F3908;
extern s32 D_800F390C;
extern s32 D_800F3910;
extern s8 D_800F3918;
extern s8 D_800F3919;
extern s8 D_800F391A;

extern s32 D_800F3900;
extern s32 D_800F3904;

void func_800A76C0(s32 arg0) {
    D_800F3900 += arg0;
}

s32 func_800A76D8(void) {
    return D_800F3900;
}

s32 func_800A76E4(void) {
    return D_800F3904;
}

void func_800A76F0(void) {
    D_800F3900 = 0;
}

INCLUDE_ASM("asm/nonmatchings/unit_482C0", func_800A76FC);

INCLUDE_ASM("asm/nonmatchings/unit_482C0", func_800A780C);

s32 func_800A7B88(void *arg0) {
  s32 new_var;
  D_800F3904 = 0;
  D_800F3908 = (s32) (*((u8 *) (((u8 *) arg0) + 0x17)));
  D_800F390C = (s32) (*((u8 *) (((u8 *) arg0) + 0x19)));
  new_var = *((s32 *) (((u8 *) arg0) + 0x0));
  D_800F3910 = (s32) (*((u8 *) (((u8 *) arg0) + 0x1A)));
  if (new_var & 2) {
    D_800F391A = 1;
  }
  else
  {
    D_800F391A = 0;
  }
  if ((*((s32 *) (((u8 *) arg0) + 0x0))) & 4) {
    D_800F3919 = 1;
  }
  else
  {
    D_800F3919 = 0;
  }
  if ((*((s32 *) (((u8 *) arg0) + 0x0))) & 8) {
    D_800F3918 = 1;
  }
  else
  {
    D_800F3918 = 0;
  }
  return 1;
}

s32 func_800A7C2C(void) {
    return 1;
}

s32 func_800A7C34(u8 arg0) {
  void *temp_v0;
  temp_v0 = func_800960D8(arg0 & 0xFF);
  if ((*((u8 *) (((u8 *) temp_v0) + 0x47B))) == 0) {
    if (temp_v0 || arg0) {
      return 0;
    }
    else
    {
      return 0;
    }
  }
  *((s32 *) (((u8 *) temp_v0) + 0x14)) = 0;
  *((s32 *) (((u8 *) temp_v0) + 0x18)) = 0;
  *((u8 *) (((u8 *) temp_v0) + 0x24)) = 1;
  *((u8 *) (((u8 *) temp_v0) + 0x25)) = 0;
  *((u8 *) (((u8 *) temp_v0) + 0x27)) = 0;
  *((f32 *) (((u8 *) temp_v0) + 0x10)) = 1.0f;
  return 1;
}

INCLUDE_ASM("asm/nonmatchings/unit_482C0", func_800A7C8C);
