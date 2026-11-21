#include "common.h"
#include "ultra64.h"
#include "include_asm.h"
typedef struct { u16 a; u16 b; u16 id; u8 state; u8 flags; u8 x; u8 y; } PakSlotQ_;
typedef struct { u8 ch; u8 pad[0xF]; } MsgR_;



extern PakSlotQ_ D_80140F68[4];
s32 nuSiSendMesg(u16 cmd, void *args);
s32 nuContRmbCheck(s32 ch) {
    MsgR_ m;
    s32 r;
    s32 mask;

    m.ch = ch;
    r = nuSiSendMesg(0x301, &m);
    mask = osSetIntMask(1);
    if (r == 0) {
        D_80140F68[ch].y = 2;
    } else {
        D_80140F68[ch].y = 0;
    }
    osSetIntMask(mask);
    return r;
}

void nuContRmbModeSet(u32 contNo, u8 mode) {
    s32 mask = osSetIntMask(1);
    u8 flags = D_80140F68[contNo].flags & 0x80;

    switch (mode) {
    case 0:
        D_80140F68[contNo].flags = mode | flags;
        break;
    case 1:
        if (D_80140F68[contNo].y == 2) {
            D_80140F68[contNo].flags = mode | flags;
        }
        break;
    case 2:
        D_80140F68[contNo].flags = mode | flags;
        if (D_80140F68[contNo].y == mode) {
            D_80140F68[contNo].x = 1;
        } else {
            D_80140F68[contNo].x = 0;
        }
        break;
    }
    osSetIntMask(mask);
}
