#include "common.h"
#include "include_asm.h"
s32 func_8008D460(s32);
s32 func_8008D48C(s32);
void func_800CB4D4(s32, s32, s32, s32, s32, s32);
void func_800CAE10(void);
void func_800CB034(s32);
void func_800CB268(void);
void func_800CB0DC(s32);
/* TODO (Clean version):
   Replace temporary decomp structures with documented layout:
   typedef struct {
       s16 id;
       s16 x;
   } IdEntry;

   typedef struct {
       u8 pad00[0xC];
       IdEntry *ids;
       u8 pad10[3];
       s8 count;
   } IdTable;
*/
typedef struct { s16 id; s16 x; } Id4_;
typedef struct { u8 pad[0xC]; Id4_ *ids; u8 pad2[3]; s8 n; } IdTab_;
extern s32 D_800FA264;
extern s32 D_800FA268;
extern void (*D_8014C0A8)(s32, s32, f32, f32);
u8 func_800CCD94(u8);
extern u8 (*D_801455A0)(s32, s32);
extern u32 D_800FA260;
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

/* TODO (Clean version):
   Indirect call via 'fp' is a matching artifact to pin D_801455A0 in a register.
   Logic performs edge detection for buttons 0x10 and 0x11 (bits 1 and 4 represent newly pressed state):
   u32 r = 0;
   s32 pad = ((u8)D_80125590 != 1);
   if (D_801455A0(pad, 0x10)) r |= 0x1;
   if (D_801455A0(pad, 0x11)) r |= 0x8;
   if ((r & 0x1) != (D_800FA260 & 0x1)) r |= 0x2;
   if ((r & 0x8) != (D_800FA260 & 0x8)) r |= 0x10;
   D_800FA260 = r;
   return r;
*/
u32 func_800CC1FC(void) {
    u8 (**fp)(s32, s32);
    s32 pad;
    u32 r;
    u32 old;

    pad = (u8)D_80125590 != 1;
    fp = &D_801455A0;
    r = (*fp)(pad, 0x10) != 0;
    if ((*fp)(pad, 0x11)) {
        r |= 8;
    }
    old = D_800FA260;
    r = ((r & 1) != (old & 1)) ? r | 2 : r;
    r = ((r & 8) != (old & 8)) ? r | 0x10 : r;
    D_800FA260 = r;
    return r;
}

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

/* TODO (Clean version):
   (&D_800FA510)[i] accesses contiguous global variables D_800FA510..D_800FA520.
   Declare as an array:
   extern s32 D_800FA510[5];
   func_800CB034(D_800FA510[i]);
*/
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

/* Same edge detection as func_800CC1FC; repeats buttons 0x10/0x11 as menu inputs 0xB/0xA. */
void func_800CC70C(void) {
    s32 pad;
    u32 r;
    u32 old;

    pad = (u8)D_80125590 != 1;
    r = D_801455A0(pad, 0x10) != 0;
    if (D_801455A0(pad, 0x11)) {
        r |= 8;
    }
    old = D_800FA260;
    r = ((r & 1) != (old & 1)) ? r | 2 : r;
    r = ((r & 8) != (old & 8)) ? r | 0x10 : r;
    D_800FA260 = r;
    if ((r & 9) == 1) {
        if (r & 2) {
            func_800C9F18(&D_800FAB88, 0xB);
            D_800FA264 = -8;
        } else if (D_800FA264++ >= 3) {
            func_800C9F18(&D_800FAB88, 0xB);
            D_800FA264 = 0;
        }
    } else if ((r & 9) == 8) {
        if (r & 0x10) {
            func_800C9F18(&D_800FAB88, 0xA);
            D_800FA268 = -8;
        } else if (D_800FA268++ >= 3) {
            func_800C9F18(&D_800FAB88, 0xA);
            D_800FA268 = 0;
        }
    }
}

INCLUDE_ASM("asm/nonmatchings/unit_6C240", func_800CC890);

INCLUDE_ASM("asm/nonmatchings/unit_6C240", func_800CCA3C);

INCLUDE_ASM("asm/nonmatchings/unit_6C240", func_800CCBE8);

/* Raises a 0-255 level to the next 10% step; plays sound 0x31 on change. */
u8 func_800CCD94(u8 arg0) {
    s32 v;
    s32 cur;
    s32 target;

    v = arg0;
    cur = v * 100 / 255;
    cur = cur / 10 * 10;
    if (cur >= 100) {
        return 255;
    }
    target = cur + 10;
    if (target >= 100) {
        D_8014C0A8(0x31, 1, 1.0f, 1.0f);
        return 255;
    }
    while (cur < target) {
        v++;
        cur = v * 100 / 255;
        cur = cur / 10 * 10;
    }
    /* TODO (Bug check): 'v < 0' is dead code; 'v' starts as u8 and is only incremented. */
    if (v < 0) {
        return 0;
    }
    if (v > 255) {
        return 255;
    }
    D_8014C0A8(0x31, 1, 1.0f, 1.0f);
    return v;
}

/* Lowers a 0-255 level to the previous 10% step; plays sound 0x31 on change. */
u8 func_800CCEFC(u8 arg0) {
    s32 v;
    s32 cur;
    s32 target;

    v = arg0;
    cur = v * 100 / 255;
    cur = cur / 10 * 10;
    if (cur <= 0) {
        return 0;
    }
    target = cur - 10;
    if (target <= 0) {
        D_8014C0A8(0x31, 1, 1.0f, 1.0f);
        return 0;
    }
    while (target < cur) {
        v--;
        cur = v * 100 / 255;
        cur = cur / 10 * 10;
    }
    if (v <= 0) {
        return 0;
    }
    /* TODO (Bug check): 'v >= 255' is unreachable; 'v' is only decremented from arg0 <= 255. */
    if (v >= 255) {
        return 255;
    }
    D_8014C0A8(0x31, 1, 1.0f, 1.0f);
    return v;
}

/* TODO (Clean version):
   Offsets 0x87C, 0x87D, and 0x880 into func_800B027C() represent audio settings.
   Define an options structure:
   typedef struct {
       u8 pad00[0x87C];
       u8 bgm_volume;    // 0x87C
       u8 sfx_volume;    // 0x87D
       u8 pad87E[2];
       u8 voice_volume;  // 0x880
   } GameSettings;
   GameSettings *opt = func_800B027C();
*/
void func_800CD054(void) {
    u8 *opt = func_800B027C();
    u8 cur = opt[0x87C];
    u8 next;

    if (cur != 0xFF) {
        next = func_800CCD94(cur);
        opt[0x87C] = next;
        D_8014105C(next);
    }
}

void func_800CD0A4(void) {
    u8 *opt = func_800B027C();
    u8 cur = opt[0x87C];
    u8 next;

    if (cur != 0) {
        next = func_800CCEFC(cur);
        opt[0x87C] = next;
        D_8014105C(next);
    }
}

void func_800CD0F0(void) {
    u8 *opt = func_800B027C();
    u8 cur = opt[0x880];
    u8 next;

    if (cur != 0xFF) {
        next = func_800CCD94(cur);
        opt[0x880] = next;
        D_80141064(next);
    }
}

void func_800CD140(void) {
    u8 *opt = func_800B027C();
    u8 cur = opt[0x880];
    u8 next;

    if (cur != 0) {
        next = func_800CCEFC(cur);
        opt[0x880] = next;
        D_80141064(next);
    }
}

void func_800CD18C(void) {
    u8 *opt = func_800B027C();
    u8 cur = opt[0x87D];
    u8 next;

    if (cur != 0xFF) {
        next = func_800CCD94(cur);
        opt[0x87D] = next;
        D_80141054(next);
    }
}

void func_800CD1DC(void) {
    u8 *opt = func_800B027C();
    u8 cur = opt[0x87D];
    u8 next;

    if (cur != 0) {
        next = func_800CCEFC(cur);
        opt[0x87D] = next;
        D_80141054(next);
    }
}

void func_800CD228(void) {
    func_800CA8F0(D_800FA51C);
    D_8014C0AC(D_8014C0B0());
}

void func_800CD26C(void) {
    func_800CACA8(D_800FA51C);
}

/* TODO (Clean version):
   'fp' is a matching workaround to cache D_8014C0B0 in a register across calls:
   s32 val = D_8014C0B0();
   func_800CB168();
   func_800CB2C0(D_800FA51C, 0);
   func_800CB4D4(func_8008D460(val), 160, 155, 255, 255, 255);
   func_800CB4D4(func_8008D48C(val), 160, 175, 255, 255, 255);
   func_800CB28C();
*/
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
    s32 next;

    next = D_8014C0B0() + 1;
    /* TODO (Clean version): 'next &= -((u8)next < 7)' is branchless wrap-around: next = (next >= 7) ? 0 : next; */
    next &= -((u8)next < 7);
    D_8014C0B4();
    D_8014C0AC(next & 0xFF);
}

void func_800CD45C(void) {
    s32 t;
    u8 r;

    t = D_8014C0B0();
    if ((t & 0xFF) == 0) {
        r = 6;
    } else {
        r = t - 1;
    }
    D_8014C0B4();
    D_8014C0AC(r & 0xFF);
}
