#include "common.h"
#include "include_asm.h"
typedef struct { u8 pad[4]; s32 a; s32 b; void *c; } Buf20_;
s32 func_8007FD40(s32);
s32 func_80080070(s32);
extern s32 D_800F2690;

typedef struct { u32 w0; s32 w4; s32 w8; void *wC; } Hp2C0_;
typedef struct { Hp2C0_ *a; Hp2C0_ *b; } Hp2_;
extern Hp2_ D_8010C410;
extern u8 D_8010C420[];

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

void func_800802C0(void) {
    D_8010C410.a = (Hp2C0_ *)D_8010C420;
    D_8010C410.b = (Hp2C0_ *)(D_8010C420 + 0x41F0);
    D_8010C410.a->w8 = 0;
    D_8010C410.b->w8 = 0;
    D_8010C410.a->w4 = 0;
    D_8010C410.b->w4 = 0;
    D_8010C410.a->wC = (u8 *)D_8010C410.a + 0x1C30;
    D_8010C410.b->wC = (u8 *)D_8010C410.b + 0x1C30;
    D_8010C410.a->w0 = 0xA87BC3E8;
    D_8010C410.b->w0 = 0xA87BC3E8;
    D_800F2690 = 0;
}

void func_80080360(void) {
    D_8010C410.a = 0;
    D_8010C410.b = 0;
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

void func_80080500(void) {
    s32 i = D_800F2690 ^ 1;

    ((Buf20_ * *)&D_8010C410)[i]->a = 0;
    ((Buf20_ * *)&D_8010C410)[i]->b = 0;
    D_800F2690 = i;
    ((Buf20_ * *)&D_8010C410)[i]->c = (u8 *)((Buf20_ * *)&D_8010C410)[i] + 0x1C30;
}

