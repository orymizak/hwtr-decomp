#include "common.h"
#include "include_asm.h"
extern s32 D_80138E38;
void Debug_FormatNumber(u16, char *);
void Debug_DrawString(char *, s32, s32, s32);
typedef struct { u8 pad[0xC]; u8 r; u8 g; u8 b; s8 on; } Col16_;
f32 sqrtf(f32);
extern f64 D_800FF8E0;

s32 Memory_Alloc(s32);
f32 func_8007ADF0(f32, f32);
extern s32 D_8014551C;

INCLUDE_ASM("asm/nonmatchings/unit_19D40", func_80079140);

INCLUDE_ASM("asm/nonmatchings/unit_19D40", func_80079BD4);

INCLUDE_ASM("asm/nonmatchings/unit_19D40", func_80079DBC);

void func_80079E48(void *arg0) {
    Memory_Alloc(*(u16 *)((u8 *)arg0 + 0x4) * 8);
}

s32 func_80079E68(void *arg0) {
    s32 var_v0;
    u16 temp_a0;

    temp_a0 = *(u16 *)((u8 *)arg0 + 0x6);
    var_v0 = 0;
    if (temp_a0 != 0) {
        var_v0 = Memory_Alloc(temp_a0 * 0x10);
    }
    return var_v0;
}

void func_80079E90(Col16_ *src, Col16_ *dst, u32 n) {
    u32 i;

    for (i = 0; i < n; i++, src++) {
        if (src->on != 0) {
            dst->r = src->r << 3;
            dst->g = src->g << 3;
            dst->b = src->b << 3;
        }
        dst++;
    }
}


INCLUDE_ASM("asm/nonmatchings/unit_19D40", func_80079EF4);

void *func_80079FC0(u16 *n) {
    return ((void * (*)(s32 size))Memory_Alloc)(*n * 12 + *n * 160 + *n * 48 + 16);
}


INCLUDE_ASM("asm/nonmatchings/unit_19D40", func_8007A004);

INCLUDE_ASM("asm/nonmatchings/unit_19D40", func_8007A858);

INCLUDE_ASM("asm/nonmatchings/unit_19D40", func_8007A9E0);

f32 func_8007AC64(f32 arg0) {
    f32 temp_fv0;
    f32 var_fv0;

    if (fabsf(arg0) > 1.0f) {
        return 0.0f;
    }
    temp_fv0 = sqrtf(1.0f - (arg0 * arg0));
    if (temp_fv0 != 0.0f) {
        return func_8007ADF0(arg0, temp_fv0);
    }
    var_fv0 = 1.5707964f;
    if (!(arg0 > 0.0f)) {
        var_fv0 = -1.5707964f;
    }
    return var_fv0;
}

f32 func_8007AD24(f32 arg0) {
  f32 temp_fv0;
  f32 var_fv0;
  if (fabsf(arg0) > 1.0f) {
    return 0.0f;
  }
  temp_fv0 = sqrtf(1.0f - (arg0 * arg0));
  if (temp_fv0 != 0.0f) {
    return (f32) (D_800FF8E0 - ((f64) func_8007ADF0(arg0, temp_fv0)));
  }
  if (arg0) {
 do { } while (0);
    var_fv0 = 0.0f;
  }
  else
  {
    var_fv0 = 0.0f;
  }
  if (!(arg0 > 0.0f)) {
    var_fv0 = 3.1415927f;
  }
  return var_fv0;
}

INCLUDE_ASM("asm/nonmatchings/unit_19D40", func_8007ADF0);

INCLUDE_ASM("asm/nonmatchings/unit_19D40", func_8007AFAC);

void func_8007AFD4(void *arg0, void *arg1, void *arg2) {
    *(f32 *)((u8 *)arg2 + 0x0) = (f32) ((*(f32 *)((u8 *)arg0 + 0x0) * *(f32 *)((u8 *)arg1 + 0x0)) + (*(f32 *)((u8 *)arg0 + 0x4) * *(f32 *)((u8 *)arg1 + 0x10)) + (*(f32 *)((u8 *)arg0 + 0x8) * *(f32 *)((u8 *)arg1 + 0x20)));
    *(f32 *)((u8 *)arg2 + 0x4) = (f32) ((*(f32 *)((u8 *)arg0 + 0x0) * *(f32 *)((u8 *)arg1 + 0x4)) + (*(f32 *)((u8 *)arg0 + 0x4) * *(f32 *)((u8 *)arg1 + 0x14)) + (*(f32 *)((u8 *)arg0 + 0x8) * *(f32 *)((u8 *)arg1 + 0x24)));
    *(f32 *)((u8 *)arg2 + 0x8) = (f32) ((*(f32 *)((u8 *)arg0 + 0x0) * *(f32 *)((u8 *)arg1 + 0x8)) + (*(f32 *)((u8 *)arg0 + 0x4) * *(f32 *)((u8 *)arg1 + 0x18)) + (*(f32 *)((u8 *)arg0 + 0x8) * *(f32 *)((u8 *)arg1 + 0x28)));
    *(f32 *)((u8 *)arg2 + 0xC) = (f32) ((*(f32 *)((u8 *)arg0 + 0x0) * *(f32 *)((u8 *)arg1 + 0xC)) + (*(f32 *)((u8 *)arg0 + 0x4) * *(f32 *)((u8 *)arg1 + 0x1C)) + (*(f32 *)((u8 *)arg0 + 0x8) * *(f32 *)((u8 *)arg1 + 0x2C)) + *(f32 *)((u8 *)arg0 + 0xC));
    *(f32 *)((u8 *)arg2 + 0x10) = (f32) ((*(f32 *)((u8 *)arg0 + 0x10) * *(f32 *)((u8 *)arg1 + 0x0)) + (*(f32 *)((u8 *)arg0 + 0x14) * *(f32 *)((u8 *)arg1 + 0x10)) + (*(f32 *)((u8 *)arg0 + 0x18) * *(f32 *)((u8 *)arg1 + 0x20)));
    *(f32 *)((u8 *)arg2 + 0x14) = (f32) ((*(f32 *)((u8 *)arg0 + 0x10) * *(f32 *)((u8 *)arg1 + 0x4)) + (*(f32 *)((u8 *)arg0 + 0x14) * *(f32 *)((u8 *)arg1 + 0x14)) + (*(f32 *)((u8 *)arg0 + 0x18) * *(f32 *)((u8 *)arg1 + 0x24)));
    *(f32 *)((u8 *)arg2 + 0x18) = (f32) ((*(f32 *)((u8 *)arg0 + 0x10) * *(f32 *)((u8 *)arg1 + 0x8)) + (*(f32 *)((u8 *)arg0 + 0x14) * *(f32 *)((u8 *)arg1 + 0x18)) + (*(f32 *)((u8 *)arg0 + 0x18) * *(f32 *)((u8 *)arg1 + 0x28)));
    *(f32 *)((u8 *)arg2 + 0x1C) = (f32) ((*(f32 *)((u8 *)arg0 + 0x10) * *(f32 *)((u8 *)arg1 + 0xC)) + (*(f32 *)((u8 *)arg0 + 0x14) * *(f32 *)((u8 *)arg1 + 0x1C)) + (*(f32 *)((u8 *)arg0 + 0x18) * *(f32 *)((u8 *)arg1 + 0x2C)) + *(f32 *)((u8 *)arg0 + 0x1C));
    *(f32 *)((u8 *)arg2 + 0x20) = (f32) ((*(f32 *)((u8 *)arg0 + 0x20) * *(f32 *)((u8 *)arg1 + 0x0)) + (*(f32 *)((u8 *)arg0 + 0x24) * *(f32 *)((u8 *)arg1 + 0x10)) + (*(f32 *)((u8 *)arg0 + 0x28) * *(f32 *)((u8 *)arg1 + 0x20)));
    *(f32 *)((u8 *)arg2 + 0x24) = (f32) ((*(f32 *)((u8 *)arg0 + 0x20) * *(f32 *)((u8 *)arg1 + 0x4)) + (*(f32 *)((u8 *)arg0 + 0x24) * *(f32 *)((u8 *)arg1 + 0x14)) + (*(f32 *)((u8 *)arg0 + 0x28) * *(f32 *)((u8 *)arg1 + 0x24)));
    *(f32 *)((u8 *)arg2 + 0x28) = (f32) ((*(f32 *)((u8 *)arg0 + 0x20) * *(f32 *)((u8 *)arg1 + 0x8)) + (*(f32 *)((u8 *)arg0 + 0x24) * *(f32 *)((u8 *)arg1 + 0x18)) + (*(f32 *)((u8 *)arg0 + 0x28) * *(f32 *)((u8 *)arg1 + 0x28)));
    *(f32 *)((u8 *)arg2 + 0x2C) = (f32) ((*(f32 *)((u8 *)arg0 + 0x20) * *(f32 *)((u8 *)arg1 + 0xC)) + (*(f32 *)((u8 *)arg0 + 0x24) * *(f32 *)((u8 *)arg1 + 0x1C)) + (*(f32 *)((u8 *)arg0 + 0x28) * *(f32 *)((u8 *)arg1 + 0x2C)) + *(f32 *)((u8 *)arg0 + 0x2C));
}

INCLUDE_ASM("asm/nonmatchings/unit_19D40", func_8007B230);

void func_8007B510(u16 n) {
    char buf[8];
    s32 y;
    s32 x;

    Debug_FormatNumber(n, buf);
    D_80138E38 = 0x8037FB2C;
    for (y = 0; y < 16; y++) {
        for (x = 0; x < 48; x++) {
            ((u16 *)0x8037FB2C)[y * 640 + x] = 0;
        }
    }
    Debug_DrawString(buf, 3, 0xFFFF, 1);
}


INCLUDE_ASM("asm/nonmatchings/unit_19D40", func_8007B598);

s32 func_8007BE30(void) {
    s32 *var_v0;
    s32 var_v1;

    var_v1 = 0x1F;
    var_v0 = &D_8014551C;
    do {
        *var_v0 = 0;
        var_v1 -= 1;
        var_v0 -= 1;
    } while (var_v1 >= 0);
    return 1;
}

INCLUDE_ASM("asm/nonmatchings/unit_19D40", func_8007BE54);

INCLUDE_ASM("asm/nonmatchings/unit_19D40", func_8007BEBC);

INCLUDE_ASM("asm/nonmatchings/unit_19D40", func_8007BF20);
