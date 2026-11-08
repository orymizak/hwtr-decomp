#include "common.h"
#include "include_asm.h"
s32 func_800C9F18(void *,s32);
extern u8 D_800F9F70;

extern s32 (*D_801455A0)(s32);
extern s32 D_80124D48;
extern s32 D_80124D51;
extern s32 D_80124D40;
extern s32 (*D_80141048)();
extern s32 D_80124D44;

void func_800C8110(void) {
    func_800C9F18(&D_800F9F70, 0x11);
}

s32 func_800C8134(s32 arg0) {
    return D_801455A0(arg0 & 0xFF) & 0xFF;
}

s32 *func_800C815C(void) {
    return &D_80124D48;
}

s32 *func_800C8168(void) {
    return &D_80124D51;
}

void func_800C8174(void) {
    u32 var_v1;

    var_v1 = 0;
    do {
        *(u8 *)(&(*(u8 *)&D_80124D48) + var_v1) = 0x20;
        var_v1 += 1;
    } while (var_v1 < 8U);
}

void func_800C81A0(void) {
    u32 var_v1;

    var_v1 = 0;
    do {
        *(u8 *)(&(*(u8 *)&D_80124D51) + var_v1) = 0x20;
        var_v1 += 1;
    } while (var_v1 < 8U);
}

void func_800C81CC(s32 arg0, s8 arg1) {
    void *var_a2;
    void *var_v1;

    var_a2 = &(*(u8 *)&D_80124D51);
    var_v1 = &(*(u8 *)&D_80124D51);
    if (!(arg0 & 0xFF)) {
        var_a2 = &(*(u8 *)&D_80124D48);
        var_v1 = &(*(u8 *)&D_80124D48);
    }
    do {
        (*(u8 *)((u8 *)(var_v1) + 0x0)) = (u8) (*(u8 *)((u8 *)(var_v1) + 0x1));
        var_v1 += 1;
    } while ((u32) var_v1 < (u32) (var_a2 + 7));
    (*(u8 *)((u8 *)(var_a2) + 0x7)) = arg1;
    (*(u8 *)((u8 *)(var_a2) + 0x8)) = 0;
}

INCLUDE_ASM("asm/nonmatchings/unit_68D10", func_800C8214);

void func_800C82B4(void) {
    s32 (**fp)() = &D_80141048;

    if ((u32)((*fp)() - D_80124D44) >= 1000) {
        D_80124D44 = (*fp)();
        ((s32 (*)(void *, s32))func_800C9F18)(&D_800F9F70, 0x7C);
    }
}

void func_800C8320(void) {
    s32 (**fp)() = &D_80141048;

    if ((u32)((*fp)() - D_80124D44) >= 3000) {
        D_80124D44 = (*fp)();
        ((s32 (*)(void *, s32))func_800C9F18)(&D_800F9F70, 0x2D);
    }
}


void func_800C838C(void) {
    s32 (**fp)() = &D_80141048;

    if ((u32)((*fp)() - D_80124D44) >= 5000) {
        D_80124D44 = (*fp)();
        ((s32 (*)(void *, s32))func_800C9F18)(&D_800F9F70, 0x18);
    }
}


void func_800C83F8(void) {
    s32 (**fp)() = &D_80141048;

    if ((u32)((*fp)() - D_80124D44) >= 30000) {
        D_80124D44 = (*fp)();
        ((s32 (*)(void *, s32))func_800C9F18)(&D_800F9F70, 0x2E);
    }
}


void func_800C8464(void) {
    if ((u32) (D_80141048() - D_80124D40) >= 0x1388U) {
        func_800C9F18(&D_800F9F70, 0x10);
    }
}

void func_800C84B0(void) {
    if ((u32) (D_80141048() - D_80124D40) >= 0x7530U) {
        func_800C9F18(&D_800F9F70, 0x38);
    }
}

void func_800C84FC(void) {
    D_80124D40 = D_80141048();
}

void func_800C8528(void) {
    D_80124D44 = D_80141048();
}

INCLUDE_ASM("asm/nonmatchings/unit_68D10", func_800C8554);

INCLUDE_ASM("asm/nonmatchings/unit_68D10", func_800C891C);

INCLUDE_ASM("asm/nonmatchings/unit_68D10", func_800C8CE4);

INCLUDE_ASM("asm/nonmatchings/unit_68D10", func_800C8D40);
