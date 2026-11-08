#include "common.h"
#include "include_asm.h"
s32 func_8008D460(s32);
s32 func_8008D48C(s32);
void func_800CB4D4(s32, s32, s32, s32, s32, s32);
void func_800CAE10(void);
void func_800CB034(s32);
void func_800CB268(void);
void func_800CB0DC(s32);
typedef struct { s16 id; s16 x; } Id4_;
typedef struct { u8 pad[0xC]; Id4_ *ids; u8 pad2[3]; s8 n; } IdTab_;
extern IdTab_ *D_800FAB90;
extern u8 (*D_801455A4)(void);
s32 func_800CB168();
s32 func_800CB28C();
s32 func_800CB2C0(s32,s32);
s32 func_800C9F18(void *,s32);
extern u8 D_800FAB88;

extern s8 D_80125590;
s32 func_800CA200();
s32 func_800CA210();
s32 func_800CACA8(s32);
extern s32 D_800FA520;
s32 func_800CA8F0(s32);
extern s8 D_800FA240;
extern s8 D_800FA241;
extern s32 D_800FA510;
extern s32 D_800FA514;
extern s32 D_800FA518;
void *func_800B027C();
u8 func_800CCD94(u8);
extern s32 (*D_8014105C)(s32);
u8 func_800CCEFC(u8);
extern s32 (*D_80141064)(s32);
extern s32 (*D_80141054)(s32);
extern s32 D_800FA51C;
extern s32 (*D_8014C0AC)(s32);
extern s32 (*D_8014C0B0)();
extern s32 (*D_8014C0B4)();

s32 func_800CB640(s16 id) {
    Id4_ *p = D_800FAB90->ids;
    u8 i;

    for (i = 0; i < D_800FAB90->n; i++, p++) {
        if (p->id == id) {
            return 1;
        }
    }
    return 0;
}


INCLUDE_ASM("asm/nonmatchings/unit_6C240", func_800CB694);

INCLUDE_ASM("asm/nonmatchings/unit_6C240", func_800CC1FC);

void func_800CC2B4(s8 arg0) {
    D_80125590 = arg0;
}

void func_800CC2C0(void) {
    func_800CA200();
}

void func_800CC2DC(void) {
    func_800CA210();
}

void func_800CC2F8(void) {
    func_800C9F18(&D_800FAB88, 0);
}

void func_800CC31C(void) {
    func_800CB168();
    func_800CB2C0(D_800FA520, 1);
    func_800CB28C();
}

void func_800CC350(void) {
    func_800CB168();
    func_800CB2C0(D_800FA520, 0);
    func_800CB28C();
}

void func_800CC384(void) {
    func_800CACA8(D_800FA520);
}

void func_800CC3A8(void) {
    func_800CA8F0(D_800FA520);
}

void func_800CC3CC(void) {
    u8 i;

    func_800CAE10();
    for (i = 0; i < 5; i++) {
        func_800CB034((&D_800FA510)[i]);
    }
    func_800CB268();
}

void func_800CC430(void) {

}

void func_800CC438(void) {
    u8 i;

    for (i = 0; i < 5; i++) {
        func_800CB0DC((&D_800FA510)[i]);
    }
}


void func_800CC490(void) {

}

void func_800CC498(void) {
    D_800FA241 = 0;
    D_800FA240 = 0;
}

void func_800CC4AC(void) {
    func_800CA8F0(D_800FA510);
}

void func_800CC4D0(void) {
    func_800CACA8(D_800FA510);
}

void func_800CC4F4(void) {
    if (D_801455A4() != 0) {
        func_800C9F18(&D_800FAB88, 7);
    } else {
        func_800C9F18(&D_800FAB88, 8);
    }
}


void func_800CC544(void) {
    func_800CB168();
    func_800CB2C0(D_800FA510, 0);
    func_800CB28C();
}

void func_800CC578(void) {
    func_800CB168();
    func_800CB2C0(D_800FA510, 1);
    func_800CB28C();
}

void func_800CC5AC(void) {
    func_800CB168();
    func_800CB2C0(D_800FA510, 2);
    func_800CB28C();
}

void func_800CC5E0(void) {
    func_800CB168();
    func_800CB2C0(D_800FA510, 3);
    func_800CB28C();
}

void func_800CC614(void) {
    func_800CA8F0(D_800FA514);
}

void func_800CC638(void) {
    func_800CACA8(D_800FA514);
}

void func_800CC65C(void) {
    func_800CB168();
    func_800CB2C0(D_800FA514, 0);
    func_800CB28C();
}

void func_800CC690(void) {
    func_800CB168();
    func_800CB2C0(D_800FA514, 1);
    func_800CB28C();
}

void func_800CC6C4(void) {
    func_800CA8F0(D_800FA518);
}

void func_800CC6E8(void) {
    func_800CACA8(D_800FA518);
}

INCLUDE_ASM("asm/nonmatchings/unit_6C240", func_800CC70C);

INCLUDE_ASM("asm/nonmatchings/unit_6C240", func_800CC890);

INCLUDE_ASM("asm/nonmatchings/unit_6C240", func_800CCA3C);

INCLUDE_ASM("asm/nonmatchings/unit_6C240", func_800CCBE8);

INCLUDE_ASM("asm/nonmatchings/unit_6C240", func_800CCD94);

INCLUDE_ASM("asm/nonmatchings/unit_6C240", func_800CCEFC);

void func_800CD054(void) {
    u8 temp_a0;
    u8 temp_v0_2;
    void *temp_v0;

    temp_v0 = func_800B027C();
    temp_a0 = *(u8 *)((u8 *)temp_v0 + 0x87C);
    if (temp_a0 != 0xFF) {
        temp_v0_2 = func_800CCD94(temp_a0);
        *(u8 *)((u8 *)temp_v0 + 0x87C) = temp_v0_2;
        D_8014105C(temp_v0_2 & 0xFF);
    }
}

void func_800CD0A4(void) {
    u8 temp_a0;
    u8 temp_v0_2;
    void *temp_v0;

    temp_v0 = func_800B027C();
    temp_a0 = *(u8 *)((u8 *)temp_v0 + 0x87C);
    if (temp_a0 != 0) {
        temp_v0_2 = func_800CCEFC(temp_a0);
        *(u8 *)((u8 *)temp_v0 + 0x87C) = temp_v0_2;
        D_8014105C(temp_v0_2 & 0xFF);
    }
}

void func_800CD0F0(void) {
    u8 temp_a0;
    u8 temp_v0_2;
    void *temp_v0;

    temp_v0 = func_800B027C();
    temp_a0 = *(u8 *)((u8 *)temp_v0 + 0x880);
    if (temp_a0 != 0xFF) {
        temp_v0_2 = func_800CCD94(temp_a0);
        *(u8 *)((u8 *)temp_v0 + 0x880) = temp_v0_2;
        D_80141064(temp_v0_2 & 0xFF);
    }
}

void func_800CD140(void) {
    u8 temp_a0;
    u8 temp_v0_2;
    void *temp_v0;

    temp_v0 = func_800B027C();
    temp_a0 = *(u8 *)((u8 *)temp_v0 + 0x880);
    if (temp_a0 != 0) {
        temp_v0_2 = func_800CCEFC(temp_a0);
        *(u8 *)((u8 *)temp_v0 + 0x880) = temp_v0_2;
        D_80141064(temp_v0_2 & 0xFF);
    }
}

void func_800CD18C(void) {
    u8 temp_a0;
    u8 temp_v0_2;
    void *temp_v0;

    temp_v0 = func_800B027C();
    temp_a0 = *(u8 *)((u8 *)temp_v0 + 0x87D);
    if (temp_a0 != 0xFF) {
        temp_v0_2 = func_800CCD94(temp_a0);
        *(u8 *)((u8 *)temp_v0 + 0x87D) = temp_v0_2;
        D_80141054(temp_v0_2 & 0xFF);
    }
}

void func_800CD1DC(void) {
    u8 temp_a0;
    u8 temp_v0_2;
    void *temp_v0;

    temp_v0 = func_800B027C();
    temp_a0 = *(u8 *)((u8 *)temp_v0 + 0x87D);
    if (temp_a0 != 0) {
        temp_v0_2 = func_800CCEFC(temp_a0);
        *(u8 *)((u8 *)temp_v0 + 0x87D) = temp_v0_2;
        D_80141054(temp_v0_2 & 0xFF);
    }
}

void func_800CD228(void) {
    func_800CA8F0(D_800FA51C);
    D_8014C0AC(D_8014C0B0());
}

void func_800CD26C(void) {
    func_800CACA8(D_800FA51C);
}

void func_800CD290(void) {
    s32 (**fp)() = &D_8014C0B0;

    func_800CB168();
    func_800CB2C0(D_800FA51C, 0);
    func_800CB4D4(func_8008D460((*fp)()), 160, 155, 255, 255, 255);
    func_800CB4D4(func_8008D48C((*fp)()), 160, 175, 255, 255, 255);
    func_800CB28C();
}


void func_800CD338(void) {
    s32 (**fp)() = &D_8014C0B0;

    func_800CB168();
    func_800CB2C0(D_800FA51C, 1);
    func_800CB4D4(func_8008D460((*fp)()), 160, 155, 255, 255, 255);
    func_800CB4D4(func_8008D48C((*fp)()), 160, 175, 255, 255, 255);
    func_800CB28C();
}


void func_800CD3E0(void) {
    D_8014C0B4();
}

void func_800CD404(void) {
    s32 temp_s0;
    s32 temp_s0_2;

    temp_s0 = D_8014C0B0() + 1;
    temp_s0_2 = temp_s0 & -((u32) (temp_s0 & 0xFF) < 7U);
    D_8014C0B4();
    D_8014C0AC(temp_s0_2 & 0xFF);
}

INCLUDE_ASM("asm/nonmatchings/unit_6C240", func_800CD45C);
