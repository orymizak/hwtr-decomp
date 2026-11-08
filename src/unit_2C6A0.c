#include "common.h"
#include "include_asm.h"
extern void (*D_80141780)(void);
extern void (*D_8014C0A8)(s32, s32, f32, f32);
extern u8 (*D_8014C120)(void);
void func_8008E700(void);
typedef struct { u8 id; u8 b; u8 pad[2]; s32 c; } Ent8_;
typedef struct { u8 b0; u8 pad[0xB]; s32 wC; } E10_;
extern u8 D_801184C1;
extern u8 D_800F3700;
extern E10_ *D_800F3704;
extern void (*D_8014C0DC)(u8, s32);
extern void (*D_8014C12C)(u8, s32, s32, s32);
extern s32 (*D_8014C0B0)();
extern void (*D_8014C0AC)(s32);
extern u8 D_800F36E8;
extern u32 D_800F36EC;
extern u32 D_800F36E4;
extern u8 D_80118498[];
u32 func_800A76D8(void);
u8 func_800A7C34(u8);
extern Ent8_ D_80118538[];
extern void (*D_8014C130)(s32);
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

u8 func_8008CF90(void) {
    u8 ok = 1;
    u8 i;

    if (func_800A76D8() > 0x1B773F) {
        return 1;
    }
    if (D_800F36E8 != 0) {
        ok = !(func_800A76D8() < D_800F36EC);
    } else {
        for (i = 0; i < D_800F36E4; i++) {
            u8 v = func_800A7C34(i);

            if (v == 0) {
                ok = 0;
            }
            D_80118498[i] = v;
        }
    }
    return ok;
}

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

void func_8008E80C(void) {
    u8 i;

    for (i = 0; i < D_801184C1; i++) {
        D_8014C0DC(i, 0);
    }
    for (i = 0; i < D_800F3700; i++) {
        D_8014C12C(D_800F3704[i].b0, D_800F3704[i].wC, 0, 1);
    }
    D_8014C0AC(D_8014C0B0());
}

