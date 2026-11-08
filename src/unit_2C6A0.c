#include "common.h"
#include "include_asm.h"
u8 (*D_8014C120)(void);
void func_8008E700(void);
typedef struct { u8 id; u8 b; u8 pad[2]; s32 c; } Ent8_;
extern Ent8_ D_80118538[];
void (*D_8014C130)(s32);
extern s32 D_800F378C[];
extern s32 D_800F3770[];
extern s32 D_80141448;
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

void func_8008CDAC(void) {
    func_8008E700();
    D_80141780();
    D_8014C0A8(0xE, 0, 1.0f, 1.0f);
}


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

s32 func_8008D460(u32 i) {
    switch (i) {
        case 0:
        case 1:
        case 2:
        case 3:
        case 4:
        case 5:
        case 6:
            return D_800F3770[i];
    }
    return D_80141448;
}


s32 func_8008D48C(u32 i) {
    switch (i) {
        case 0:
        case 1:
        case 2:
        case 3:
        case 4:
        case 5:
        case 6:
            return D_800F378C[i];
    }
    return D_80141448;
}

INCLUDE_ASM("asm/nonmatchings/unit_2C6A0", func_8008D4B8);

INCLUDE_ASM("asm/nonmatchings/unit_2C6A0", func_8008D4E4);

INCLUDE_ASM("asm/nonmatchings/unit_2C6A0", func_8008D5EC);

INCLUDE_ASM("asm/nonmatchings/unit_2C6A0", func_8008D848);

INCLUDE_ASM("asm/nonmatchings/unit_2C6A0", func_8008D9A0);

INCLUDE_ASM("asm/nonmatchings/unit_2C6A0", func_8008DF18);

INCLUDE_ASM("asm/nonmatchings/unit_2C6A0", func_8008E0BC);

void func_8008E2B0(void) {
    u32 i;

    memset(D_80118538, 0, 16);
    for (i = 0; i < 2; i++) {
        D_80118538[i].id = D_8014C120();
    }
}

INCLUDE_ASM("asm/nonmatchings/unit_2C6A0", func_8008E310);

INCLUDE_ASM("asm/nonmatchings/unit_2C6A0", func_8008E38C);

void func_8008E510(u8 i) {
    Ent8_ *e = &D_80118538[i];

    D_8014C130(e->id);
    e->b = 0;
    e->c = 0;
}


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
