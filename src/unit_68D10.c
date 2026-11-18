#include "common.h"
#include "include_asm.h"
u8 func_800C8554(u16 mask);
extern void (*D_80140FAC)(s32, s32, f32, f32);
u8 func_800C891C(u16 mask);
extern void (*D_8014556C)(s32);
extern u8 D_800F64B0[][14];
s32 Fsm_Trigger(void *,s32);
extern u8 gMenuFsm;

extern s32 (*D_801455A0)(s32);
extern s32 D_80124D48;
extern s32 D_80124D51;
extern s32 D_80124D40;
extern s32 (*sysTimeGetMillis)();
extern s32 D_80124D44;

void Menu_Advance(void) {
    Fsm_Trigger(&gMenuFsm, 0x11);
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
    u32 i;

    for (i = 0; i < 8; i++) {
        ((u8 *)&D_80124D48)[i] = ' ';
    }
}

void func_800C81A0(void) {
    u32 i;

    for (i = 0; i < 8; i++) {
        ((u8 *)&D_80124D51)[i] = ' ';
    }
}

void func_800C81CC(u8 player, s8 c) {
    u8 *buf;
    u8 *p;

    buf = (u8 *)&D_80124D51;
    p = (u8 *)&D_80124D51;
    if (!player) {
        buf = (u8 *)&D_80124D48;
        p = (u8 *)&D_80124D48;
    }
    do {
        p[0] = p[1];
        p++;
    } while (p < buf + 7);
    buf[7] = c;
    buf[8] = 0;
}

u8 Input_PressedOnce(u8 i, s32 k) {
    s32 j = k - 14;

    if ((u8)D_801455A0(i) != 0) {
        if (D_800F64B0[i][j] == 0) {
            D_800F64B0[i][j] = 1;
            return 1;
        }
        return 0;
    }
    D_800F64B0[i][j] = 0;
    return 0;
}


void func_800C82B4(void) {
    s32 (**fp)() = &sysTimeGetMillis;

    if ((u32)((*fp)() - D_80124D44) >= 1000) {
        D_80124D44 = (*fp)();
        Fsm_Trigger(&gMenuFsm, 0x7C);
    }
}

void func_800C8320(void) {
    s32 (**fp)() = &sysTimeGetMillis;

    if ((u32)((*fp)() - D_80124D44) >= 3000) {
        D_80124D44 = (*fp)();
        Fsm_Trigger(&gMenuFsm, 0x2D);
    }
}


void func_800C838C(void) {
    s32 (**fp)() = &sysTimeGetMillis;

    if ((u32)((*fp)() - D_80124D44) >= 5000) {
        D_80124D44 = (*fp)();
        Fsm_Trigger(&gMenuFsm, 0x18);
    }
}


void func_800C83F8(void) {
    s32 (**fp)() = &sysTimeGetMillis;

    if ((u32)((*fp)() - D_80124D44) >= 30000) {
        D_80124D44 = (*fp)();
        Fsm_Trigger(&gMenuFsm, 0x2E);
    }
}


void func_800C8464(void) {
    if ((u32) (sysTimeGetMillis() - D_80124D40) >= 0x1388U) {
        Fsm_Trigger(&gMenuFsm, 0x10);
    }
}

void func_800C84B0(void) {
    if ((u32) (sysTimeGetMillis() - D_80124D40) >= 0x7530U) {
        Fsm_Trigger(&gMenuFsm, 0x38);
    }
}

void func_800C84FC(void) {
    D_80124D40 = sysTimeGetMillis();
}

void func_800C8528(void) {
    D_80124D44 = sysTimeGetMillis();
}

/* Menu input for controller 1: newly pressed buttons allowed by mask play the menu sound and a
 * voice line; the other buttons feed the cheat-code buffer. Returns 1 if anything was pressed. */
u8 func_800C8554(u16 mask) {
    u8 hit = 0;

    D_8014556C(0);
    if (Input_PressedOnce(0, 0xE) && (mask & 0x1)) {
        func_800C84FC();
        hit = 1;
        D_80140FAC(0x2E, 1, 1.0f, 1.0f);
        Fsm_Trigger(&gMenuFsm, 6);
    }
    if (Input_PressedOnce(0, 0xF) && (mask & 0x2)) {
        func_800C84FC();
        hit = 1;
        D_80140FAC(0x2F, 1, 1.0f, 1.0f);
        Fsm_Trigger(&gMenuFsm, 8);
    }
    if (Input_PressedOnce(0, 0x12) && (mask & 0x10)) {
        func_800C84FC();
        hit = 1;
        D_80140FAC(0x32, 1, 1.0f, 1.0f);
        Fsm_Trigger(&gMenuFsm, 2);
    }
    if (Input_PressedOnce(0, 0x13) && (mask & 0x20)) {
        func_800C84FC();
        hit = 1;
        D_80140FAC(0x33, 1, 1.0f, 1.0f);
        Fsm_Trigger(&gMenuFsm, 0);
    }
    if (Input_PressedOnce(0, 0x11) && (mask & 0x8)) {
        func_800C84FC();
        hit = 1;
        D_80140FAC(0x31, 1, 1.0f, 1.0f);
        Fsm_Trigger(&gMenuFsm, 12);
    }
    if (Input_PressedOnce(0, 0x10) && (mask & 0x4)) {
        func_800C84FC();
        hit = 1;
        D_80140FAC(0x30, 1, 1.0f, 1.0f);
        Fsm_Trigger(&gMenuFsm, 10);
    }
    if (Input_PressedOnce(0, 0x1A) && (mask & 0x40)) {
        func_800C84FC();
        hit = 1;
        D_80140FAC(0x32, 1, 1.0f, 1.0f);
        Fsm_Trigger(&gMenuFsm, 4);
    }
    if (Input_PressedOnce(0, 0x14)) {
        hit = 1;
        func_800C81CC(0, 0x31);
    }
    if (Input_PressedOnce(0, 0x15)) {
        hit = 1;
        func_800C81CC(0, 0x32);
    }
    if (Input_PressedOnce(0, 0x16)) {
        hit = 1;
        func_800C81CC(0, 0x33);
        Fsm_Trigger(&gMenuFsm, 0xE);
    }
    if (Input_PressedOnce(0, 0x17)) {
        hit = 1;
        func_800C81CC(0, 0x34);
        Fsm_Trigger(&gMenuFsm, 0x62);
    }
    if (Input_PressedOnce(0, 0x18)) {
        hit = 1;
        func_800C81CC(0, 0x35);
    }
    if (Input_PressedOnce(0, 0x19)) {
        hit = 1;
        func_800C81CC(0, 0x36);
    }
    return hit;
}

/* Menu input for controller 2: newly pressed buttons allowed by mask play the menu sound and a
 * voice line; the other buttons feed the cheat-code buffer. Returns 1 if anything was pressed. */
u8 func_800C891C(u16 mask) {
    u8 hit = 0;

    D_8014556C(1);
    if (Input_PressedOnce(1, 0xE) && (mask & 0x1)) {
        func_800C84FC();
        hit = 1;
        D_80140FAC(0x2E, 1, 1.0f, 1.0f);
        Fsm_Trigger(&gMenuFsm, 7);
    }
    if (Input_PressedOnce(1, 0xF) && (mask & 0x2)) {
        func_800C84FC();
        hit = 1;
        D_80140FAC(0x2F, 1, 1.0f, 1.0f);
        Fsm_Trigger(&gMenuFsm, 9);
    }
    if (Input_PressedOnce(1, 0x12) && (mask & 0x10)) {
        func_800C84FC();
        hit = 1;
        D_80140FAC(0x32, 1, 1.0f, 1.0f);
        Fsm_Trigger(&gMenuFsm, 3);
    }
    if (Input_PressedOnce(1, 0x13) && (mask & 0x20)) {
        func_800C84FC();
        hit = 1;
        D_80140FAC(0x33, 1, 1.0f, 1.0f);
        Fsm_Trigger(&gMenuFsm, 1);
    }
    if (Input_PressedOnce(1, 0x11) && (mask & 0x8)) {
        func_800C84FC();
        hit = 1;
        D_80140FAC(0x31, 1, 1.0f, 1.0f);
        Fsm_Trigger(&gMenuFsm, 13);
    }
    if (Input_PressedOnce(1, 0x10) && (mask & 0x4)) {
        func_800C84FC();
        hit = 1;
        D_80140FAC(0x30, 1, 1.0f, 1.0f);
        Fsm_Trigger(&gMenuFsm, 11);
    }
    if (Input_PressedOnce(1, 0x1A) && (mask & 0x40)) {
        func_800C84FC();
        hit = 1;
        D_80140FAC(0x32, 1, 1.0f, 1.0f);
        Fsm_Trigger(&gMenuFsm, 5);
    }
    if (Input_PressedOnce(1, 0x14)) {
        hit = 1;
        func_800C81CC(1, 0x31);
    }
    if (Input_PressedOnce(1, 0x15)) {
        hit = 1;
        func_800C81CC(1, 0x32);
    }
    if (Input_PressedOnce(1, 0x16)) {
        hit = 1;
        func_800C81CC(1, 0x33);
        Fsm_Trigger(&gMenuFsm, 0xF);
    }
    if (Input_PressedOnce(1, 0x17)) {
        hit = 1;
        func_800C81CC(1, 0x34);
        Fsm_Trigger(&gMenuFsm, 0x63);
    }
    if (Input_PressedOnce(1, 0x18)) {
        hit = 1;
        func_800C81CC(1, 0x35);
    }
    if (Input_PressedOnce(1, 0x19)) {
        hit = 1;
        func_800C81CC(1, 0x36);
    }
    return hit;
}

s32 func_800C8CE4(u16 a, u16 b) {
    s32 r;

    r = 0;
    if (func_800C8554(a) || func_800C891C(b)) {
        r = 1;
    }
    return r;
}

void func_800C8D40(void) {
    void (**fp)(s32) = &D_8014556C;
    u8 i;
    u32 k;

    (*fp)(0);
    (*fp)(1);
    for (i = 0; i < 2; i++) {
        for (k = 14; k < 27; k++) {
            if ((u8)((s32 (*)(s32, s32))D_801455A0)(i, k) != 0) {
                return;
            }
        }
    }
    Fsm_Trigger(&gMenuFsm, 0x11);
}
