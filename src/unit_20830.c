#include "common.h"
#include "include_asm.h"
s32 func_8007FD40(s32);
s32 func_80080070(s32);
extern s32 D_800F2690;

extern s32 D_8010C410;
extern s32 D_8010C414;

INCLUDE_ASM("asm/nonmatchings/unit_20830", func_8007FC30);

void func_8007FD04(void *arg0, s32 arg1, s32 arg2, f32 arg3) {
  s32 new_var;
  s32 temp_v1;
  void *temp_v0;
  temp_v1 = *((s32 *) (((u8 *) arg0) + 0x8));
  if (((u32) (temp_v1 + 1)) < 0x257U) {
    ;
    new_var = arg1;
    *((s32 *) (((u8 *) ((temp_v1 * 4) + arg0)) + 0x10)) = new_var;
    *((s32 *) (((u8 *) ((temp_v1 * 4) + arg0)) + 0x970)) = arg2;
    *((f32 *) (((u8 *) ((temp_v1 * 4) + arg0)) + 0x12D0)) = arg3;
    *((s32 *) (((u8 *) arg0) + 0x8)) = (s32) ((*((s32 *) (((u8 *) arg0) + 0x8))) + 1);
  }
}

INCLUDE_ASM("asm/nonmatchings/unit_20830", func_8007FD40);

INCLUDE_ASM("asm/nonmatchings/unit_20830", func_80080070);

INCLUDE_ASM("asm/nonmatchings/unit_20830", func_800802C0);

void func_80080360(void) {
    D_8010C410 = 0;
    D_8010C414 = 0;
}

INCLUDE_ASM("asm/nonmatchings/unit_20830", func_80080374);

void func_80080460(s32 arg0, s32 arg1, f32 arg2) {
  s32 temp_a2;
  void *temp_v0;
  void *temp_v1;
  temp_v1 = *((s32 *) (((u8 *) (&(*(u8 *)&D_8010C410))) + (D_800F2690 * 4)));
  temp_a2 = *((s32 *) (((u8 *) temp_v1) + 0x8));
  if (((u32) (temp_a2 + 1)) < 0x257U) {
    ;
    *((s32 *) (((u8 *) ((temp_a2 * 4) + temp_v1)) + 0x10)) = arg0;
    *((s32 *) (((u8 *) ((temp_a2 * 4) + temp_v1)) + 0x970)) = arg1;
    *((f32 *) (((u8 *) ((temp_a2 * 4) + temp_v1)) + 0x12D0)) = arg2;
    *((s32 *) (((u8 *) temp_v1) + 0x8)) = (s32) ((*((s32 *) (((u8 *) temp_v1) + 0x8))) + 1);
  }
}

void func_800804B4(void) {
    func_8007FD40(*(s32 *)(&(*(u8 *)&D_8010C410) + (D_800F2690 * 4)));
    func_80080070(*(s32 *)(&(*(u8 *)&D_8010C410) + (D_800F2690 * 4)));
}

INCLUDE_ASM("asm/nonmatchings/unit_20830", func_80080500);
