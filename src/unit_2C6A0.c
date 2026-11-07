#include "common.h"
#include "include_asm.h"
extern s32 D_800F36D0[];
extern s32 D_8014C1A8;

extern s32 D_800F36F4;
extern s32 D_801184B4;
extern s32 D_801184B8;
extern s32 (*D_801416A0)(s32);
extern s32 D_8011852C;
extern u8 D_80118530;
extern u32 D_80118534;

INCLUDE_ASM("asm/nonmatchings/unit_2C6A0", func_8008BAA0);

INCLUDE_ASM("asm/nonmatchings/unit_2C6A0", func_8008BE7C);

s32 func_8008C08C(void) {
  if (D_8014C1A8 != 2) {
    ((s32 (*)()) D_800F36D0[D_8014C1A8])();
    return 0;
  }
  if (D_8014C1A8) {
    return 1;
  }
  else
  {
    return 1;
  }
}

INCLUDE_ASM("asm/nonmatchings/unit_2C6A0", func_8008C0D4);

INCLUDE_ASM("asm/nonmatchings/unit_2C6A0", func_8008C548);

INCLUDE_ASM("asm/nonmatchings/unit_2C6A0", func_8008CDAC);

INCLUDE_ASM("asm/nonmatchings/unit_2C6A0", func_8008CDFC);

INCLUDE_ASM("asm/nonmatchings/unit_2C6A0", func_8008CF90);

INCLUDE_ASM("asm/nonmatchings/unit_2C6A0", func_8008D060);

s32 func_8008D3AC(void) {
    return D_800F36F4;
}

void func_8008D3B8(void *arg0) {
    D_801184B4 = (*(u8 *)((u8 *)(arg0) + 0x18)) + 6;
    D_801184B8 = 0;
}

void func_8008D3D4(void) {
    D_801416A0((s32) (D_801184B8 * 0x64) / (s32) (D_801184B4 - 1));
    D_801184B8 += 1;
}

INCLUDE_ASM("asm/nonmatchings/unit_2C6A0", func_8008D460);

INCLUDE_ASM("asm/nonmatchings/unit_2C6A0", func_8008D48C);

INCLUDE_ASM("asm/nonmatchings/unit_2C6A0", func_8008D4B8);

INCLUDE_ASM("asm/nonmatchings/unit_2C6A0", func_8008D4E4);

INCLUDE_ASM("asm/nonmatchings/unit_2C6A0", func_8008D5EC);

INCLUDE_ASM("asm/nonmatchings/unit_2C6A0", func_8008D848);

INCLUDE_ASM("asm/nonmatchings/unit_2C6A0", func_8008D9A0);

INCLUDE_ASM("asm/nonmatchings/unit_2C6A0", func_8008DF18);

INCLUDE_ASM("asm/nonmatchings/unit_2C6A0", func_8008E0BC);

INCLUDE_ASM("asm/nonmatchings/unit_2C6A0", func_8008E2B0);

INCLUDE_ASM("asm/nonmatchings/unit_2C6A0", func_8008E310);

INCLUDE_ASM("asm/nonmatchings/unit_2C6A0", func_8008E38C);

INCLUDE_ASM("asm/nonmatchings/unit_2C6A0", func_8008E510);

INCLUDE_ASM("asm/nonmatchings/unit_2C6A0", func_8008E558);

void func_8008E568(s32 arg0) {
    if ((arg0 == 3) || ((D_80118530 != 0) && ((u32) D_80118534 >= 0x3E9U))) {
        D_80118530 = 0;
        D_8011852C = arg0;
        D_80118534 = 0;
    }
}

INCLUDE_ASM("asm/nonmatchings/unit_2C6A0", func_8008E5B8);

INCLUDE_ASM("asm/nonmatchings/unit_2C6A0", func_8008E700);

INCLUDE_ASM("asm/nonmatchings/unit_2C6A0", func_8008E80C);
