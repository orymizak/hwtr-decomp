#include "common.h"
#include "ultra64.h"
#include "include_asm.h"
    /* TODO (Clean version): Replace temporary types TAE_ and E14t_ with unified types once layout is verified:
    typedef struct {
        s32 active;
        u8 pad[16];
    } TextAnimEntry;

    typedef struct {
        u8 pad0[0x18];
        u8 count;
        u8 pad1[3];
        s32 flags;
        u8 pad2[0x10];
        TextAnimEntry entries[];
    } TextAnimGroup;
    */
typedef struct { s32 w; u8 p[16]; } E14t_;
typedef struct { u8 pad[0x18]; u8 n; u8 p2[3]; s32 w1C; u8 p3[0x10]; E14t_ e[1]; } TAE_;
extern s32 (*D_801416AC)();
extern s32 (*D_801416B0)();
extern void (*D_801416DC)(s32, u8, u16, s32, s32);
extern char D_80100AF0[];
void func_80097B00(void);
void func_800A5A1C(u32);
void func_800AEB28(u32);
void func_800ABCA4(void);
u32 strlen(const char *);
extern s32 D_801415B8;
extern u8 D_800F3B80;
extern u8 D_800F3B81;

extern s32 (*D_801416A4)();
extern s32 (*D_801416A8)();
extern s32 D_801345C8;

s32 func_800AEA40(TAE_ *t) {
    u8 i;

    D_801345C8 = 0;
    for (i = 0; i < t->n; i++) {
        if (t->e[i].w == 1) {
            D_801345C8++;
        }
    }
    if (D_801345C8 == 0) {
        D_801345C8 = 1;
    }
    D_801415B8 = t->n;
    /* TODO (Clean version): Simplify flag assignment:
       D_800F3B81 = (t->w1C & 0x200) != 0;
    */
    if (t->w1C & 0x200) {
        D_800F3B81 = 1;
    } else {
        D_800F3B81 = 0;
    }
    D_800F3B80 = 0;
    return 1;
}

s32 func_800AEAFC(void) {
    return 1;
}

void func_800AEB04(void) {
    D_801416A4();
}

INCLUDE_ASM("asm/nonmatchings/unit_4F640", func_800AEB28);

void func_800AEDFC(void) {
    D_801416A8();
}

void func_800AEE20(u32 dt) {
    char buf[0x100];
    f32 fps;
    u16 i;

    func_80097B00();
    func_800A5A1C(dt);
    D_801416A4();
    D_801416AC();
    func_800AEB28(dt);
    func_800ABCA4();
    if (D_800F3B80) {
        fps = 0.0f;
        if (dt != 0) {
            fps = 1000.0f / (f32)dt;
        }
        sprintf(buf, D_80100AF0, (s32)(fps + 0.5f));
        /* TODO (Clean version / Optimization): Avoid calling strlen(buf) on each iteration; cache length in a local variable:
           u32 len = strlen(buf);
           for (i = 0; i < len; i++) {
               D_801416DC(0, buf[i], i * 16 + 16, 16, 0);
           }
        */
        for (i = 0; i < strlen(buf); i++) {
            D_801416DC(0, buf[i], i * 16 + 16, 16, 0);
        }
    }
    D_801416B0();
    D_801416A8();
}

s32 func_800AEF70(void) {
    return D_801345C8;
}
