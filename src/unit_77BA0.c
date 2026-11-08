#include "common.h"
#include "include_asm.h"
typedef struct { u16 a; u16 b; u16 id; u8 state; u8 flags; u8 x; u8 y; } PakSlotQ_;
typedef struct { u8 ch; u8 pad[0xF]; } MsgR_;



extern PakSlotQ_ D_80140F68[4];
s32 func_800D7208(u16 cmd, void *args);
s32 osSetIntMask(s32);
s32 func_800D6FA0(s32 ch) {
    MsgR_ m;
    s32 r;
    s32 mask;

    m.ch = ch;
    r = func_800D7208(0x301, &m);
    mask = osSetIntMask(1);
    if (r == 0) {
        D_80140F68[ch].y = 2;
    } else {
        D_80140F68[ch].y = 0;
    }
    osSetIntMask(mask);
    return r;
}

INCLUDE_ASM("asm/nonmatchings/unit_77BA0", func_800D7030);
