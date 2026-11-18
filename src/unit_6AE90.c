#include "common.h"
#include "include_asm.h"
/* TODO (Clean version): Unify duplicate structs (EA6_/EA7_, TA6_/TA7_) and define proper text layout structures:
   typedef struct {
       u8 glyph;
       u8 pad;
       s16 x;
       s16 y;
       s16 x2;
       s16 y2;
   } TextGlyphPos; // E0A5_

   typedef struct {
       u16 x;
       u16 y;
   } Point2u16;

   typedef struct {
       u8 glyph;
       u8 pad;
       Point2u16 cur_pos;
       Point2u16 orig_pos;
   } TextAnimItem;

   typedef struct {
       u8 pad0[2];
       u8 num_items;
       u8 pad3;
       TextAnimItem *items;
       u8 pad8[11];
       u8 anim_mode;
   } TextAnimController;
*/
typedef struct { s32 a; s32 b; u8 pad[0x10]; } E18c_;
typedef struct { u8 pad[8]; u8 n1; u8 p1[3]; E18c_ *a1; u8 n2; u8 p2[3]; E18c_ *a2; } TCB_;
typedef struct { u8 a; u8 pad; u16 x; u16 y; u16 x2; u16 y2; } E0A5_;
typedef struct { u16 x; u16 y; } P2A6_;
typedef struct { u8 a; u8 pad; P2A6_ p; P2A6_ p2; } EA6_;
typedef struct { u8 p0[2]; u8 n; u8 p3; EA6_ *items; u8 p8[0xB]; u8 mode; } TA6_;
typedef struct { u16 x; u16 y; } P2A7_;
typedef struct { u8 a; u8 pad; P2A7_ p; P2A7_ p2; } EA7_;
typedef struct { u8 p0[2]; u8 n; u8 p3; EA7_ *items; u8 p8[0xB]; u8 mode; } TA7_;
void func_800CA7A8(TA7_ *t);
extern s32 (*D_8014169C)(s32);
void func_800CA660(TA6_ *t);
extern void (*D_801416F0)(s32);
extern void (*D_801416A4)(void);
extern void (*D_801416AC)(void);
extern void (*D_801416C8)(f32, f32, f32, f32);
extern void (*D_8014178C)(s32, s32, s32, s32);
void func_800AEB28(s32);
void func_800CA478(u8 *t);
extern s32 (*D_801416D8)(s32, u8);
s8 func_80086F80(u8 font, u8 a, u8 b);
void func_800CA290(u8 *t);
void func_800CAE1C(E18c_ *);
void func_800CAF28(E18c_ *);

extern s32 D_80125580;
extern s8 D_80124D70;
extern s8 D_80124D71;
extern s32 (*D_801416A8)();
extern s32 (*D_801416B0)();

/* TODO (Bug check / Clean version):
   1. If character count t[2] is 0, loop conditions and indexing into items[0] cause out-of-bounds access. Add guard:
      if (t[2] == 0) return;
   2. Horizontal text centering: centers text string between min_x (*(s16*)(t+8)) and max_x (*(s16*)(t+0xA)).
   3. func_800CA478 below is an identical duplicate of this function.
*/
void func_800CA290(u8 *t) {
    u8 i;
    s32 w;
    s32 x;
    s32 y;

    if (*(s16 *)(t + 0xA) > 0x140) {
        *(s16 *)(t + 0xA) = 0x140;
    }
    i = 0;
    w = 0;
    x = (*(s16 *)(t + 8) + *(s16 *)(t + 0xA)) / 2;
    y = *(s16 *)(t + 0xC);
    for (; i < t[2] - 1; i++) {
        w += D_801416D8(0, (*(E0A5_ **)(t + 4))[i].a) + func_80086F80(0, (*(E0A5_ **)(t + 4))[i].a, (*(E0A5_ **)(t + 4))[i + 1].a);
    }
    w += D_801416D8(0, (*(E0A5_ **)(t + 4))[i].a);
    w /= 2;
    x -= w;
    (*(E0A5_ **)(t + 4))[0].x2 = x;
    (*(E0A5_ **)(t + 4))[0].y2 = y;
    for (i = 1; i < t[2]; i++) {
        x += D_801416D8(0, (*(E0A5_ **)(t + 4))[i - 1].a) + func_80086F80(0, (*(E0A5_ **)(t + 4))[i - 1].a, (*(E0A5_ **)(t + 4))[i].a);
        (*(E0A5_ **)(t + 4))[i].x2 = x;
        (*(E0A5_ **)(t + 4))[i].y2 = y;
    }
}

void func_800CA478(u8 *t) {
    u8 i;
    s32 w;
    s32 x;
    s32 y;

    if (*(s16 *)(t + 0xA) > 0x140) {
        *(s16 *)(t + 0xA) = 0x140;
    }
    i = 0;
    w = 0;
    x = (*(s16 *)(t + 8) + *(s16 *)(t + 0xA)) / 2;
    y = *(s16 *)(t + 0xC);
    for (; i < t[2] - 1; i++) {
        w += D_801416D8(0, (*(E0A5_ **)(t + 4))[i].a) + func_80086F80(0, (*(E0A5_ **)(t + 4))[i].a, (*(E0A5_ **)(t + 4))[i + 1].a);
    }
    w += D_801416D8(0, (*(E0A5_ **)(t + 4))[i].a);
    w /= 2;
    x -= w;
    (*(E0A5_ **)(t + 4))[0].x2 = x;
    (*(E0A5_ **)(t + 4))[0].y2 = y;
    for (i = 1; i < t[2]; i++) {
        x += D_801416D8(0, (*(E0A5_ **)(t + 4))[i - 1].a) + func_80086F80(0, (*(E0A5_ **)(t + 4))[i - 1].a, (*(E0A5_ **)(t + 4))[i].a);
        (*(E0A5_ **)(t + 4))[i].x2 = x;
        (*(E0A5_ **)(t + 4))[i].y2 = y;
    }
}

void func_800CA660(TA6_ *t) {
    u8 i;

    for (i = 0; i < t->n; i++) {
        switch (t->mode) {
        case 0:
            t->items[i].p = t->items[i].p2;
            break;
        case 1:
            t->items[i].p.x = t->items[i].p2.x + 320;
            t->items[i].p.y = t->items[i].p2.y;
            break;
        case 2:
            ((u16 *)&t->items[i].p)[0] = D_8014169C(320);
            ((u16 *)&t->items[i].p)[1] = D_8014169C(240);
            break;
        }
    }
}

/* TODO (Clean version): func_800CA7A8 is an exact duplicate of func_800CA660. Both animate item positions according to mode:
   mode 0: reset to p2
   mode 1: slide in from right (p2.x + 320)
   mode 2: scatter randomly across screen (rand(320), rand(240))
*/
void func_800CA7A8(TA7_ *t) {
    u8 i;

    for (i = 0; i < t->n; i++) {
        switch (t->mode) {
        case 0:
            t->items[i].p = t->items[i].p2;
            break;
        case 1:
            t->items[i].p.x = t->items[i].p2.x + 320;
            t->items[i].p.y = t->items[i].p2.y;
            break;
        case 2:
            ((u16 *)&t->items[i].p)[0] = D_8014169C(320);
            ((u16 *)&t->items[i].p)[1] = D_8014169C(240);
            break;
        }
    }
}

void func_800CA8F0(TCB_ *t) {
    u8 i;

    for (i = 0; i < t->n1; i++) {
        func_800CA290(&t->a1[i]);
        func_800CA660(&t->a1[i]);
    }
    for (i = 0; i < t->n2; i++) {
        func_800CA478(&t->a2[i]);
        func_800CA7A8(&t->a2[i]);
    }
}


INCLUDE_ASM("asm/nonmatchings/unit_6AE90", func_800CA9B8);

INCLUDE_ASM("asm/nonmatchings/unit_6AE90", func_800CAB30);

extern u32 D_800FA230;
extern u32 (*D_80141698)(void);
void func_800CA9B8(E18c_ *, f32);
void func_800CAB30(E18c_ *, f32);

/* Steps both element lists by elapsed ticks / 100 (0.15 on the first call, 0.1 after a gap of more than 100 ticks). */
void func_800CACA8(TCB_ *t) {
    f32 dt;
    u32 now;
    u32 d;
    u8 i;

    if (D_800FA230 == 0) {
        dt = 0.15f;
        D_800FA230 = D_80141698();
    } else {
        now = D_80141698();
        d = now - D_800FA230;
        if (d > 100) {
            dt = 0.1f;
        } else {
            dt = d / 100.0f;
        }
        D_800FA230 = now;
    }
    for (i = 0; i < t->n1; i++) {
        func_800CA9B8(&t->a1[i], dt);
    }
    for (i = 0; i < t->n2; i++) {
        func_800CAB30(&t->a2[i], dt);
    }
}

void func_800CAE10(void) {
    D_80125580 = 0;
}

INCLUDE_ASM("asm/nonmatchings/unit_6AE90", func_800CAE1C);

INCLUDE_ASM("asm/nonmatchings/unit_6AE90", func_800CAF28);

void func_800CB034(TCB_ *t) {
    u8 i;

    for (i = 0; i < t->n1; i++) {
        func_800CAE1C(&t->a1[i]);
    }
    for (i = 0; i < t->n2; i++) {
        func_800CAF28(&t->a2[i]);
    }
}


void func_800CB0DC(TCB_ *t) {
    u8 i;

    for (i = 0; i < t->n1; i++) {
        t->a1[i].b = 0;
    }
    for (i = 0; i < t->n2; i++) {
        t->a2[i].b = 0;
    }
}


/* TODO (Clean version): Projection matrix and screen fade step:
   - D_801416C8(48.0f, 5000.0f, 1.0471967f, 1.3333334f) sets camera perspective:
     near=48.0f, far=5000.0f, fov=60 deg (pi/3), aspect=4:3.
   - D_80124D70 is target alpha, D_80124D71 is current fade alpha (fade in by +15/frame, fade out by -1/frame).
   - D_8014178C draws a full-screen black overlay with current alpha.
*/
void func_800CB168(void) {
    u8 a;
    u8 b;

    D_801416F0(0);
    D_801416A4();
    D_801416AC();
    func_800AEB28(0);
    D_801416C8(48.0f, 5000.0f, 1.0471967f, 1.3333334f);
    a = D_80124D71;
    b = D_80124D70;
    if (a > b) {
        D_80124D71 = a - 1;
    } else if (a < b) {
        D_80124D71 = a + 15;
        if (b < (u8)D_80124D71) {
            D_80124D71 = b;
        }
    }
    D_8014178C(0, 0, 0, (u8)D_80124D71);
}

/* TODO (Clean version): Declare D_80124D70 as u8 instead of s8 to avoid ugly byte cast `*(u8 *)&D_80124D70 = 0x80`. */
void func_800CB268(void) {
    *(u8 *)&D_80124D70 = 0x80;
    D_80124D71 = 0;
}

void func_800CB280(void) {
    D_80124D70 = 0;
}

void func_800CB28C(void) {
    D_801416B0();
    D_801416A8();
}

INCLUDE_ASM("asm/nonmatchings/unit_6AE90", func_800CB2C0);

INCLUDE_ASM("asm/nonmatchings/unit_6AE90", func_800CB4D4);
