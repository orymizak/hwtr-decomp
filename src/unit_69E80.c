#include "common.h"
#include "include_asm.h"
typedef struct { s32 *t; s16 a; s16 b; s32 c; s32 d; s32 e; s16 f; s16 g; } Seq9_;
typedef struct { s32 a; s32 b; s32 c; } V12_;
typedef struct { u16 flags; u16 a; u16 b; u16 c; u8 d8; u8 d9; u8 dA; u8 dB; } E0Cc_;
typedef struct { u8 p0[0xC]; s16 *ids; u8 p10[3]; s8 n; } Tb9F_;
typedef struct { u8 p0[8]; Tb9F_ *t; u8 pC[0xA]; s16 cur; } Ob9F_;
typedef struct { s32 a; u8 p4[0x10]; } SbA00_;
typedef struct { u8 p0[0x18]; u8 n; u8 p19[3]; SbA00_ *sub; u8 b20; u8 p21[7]; s32 w28; } TrA00_;
s32 func_800C9590();
extern char D_80101080[];
extern s32 (*D_80140FE4)(char *);
extern s32 (*D_80140FE0)(char *, void *);
extern u8 D_80138D38[];
s32 sprintf(char *, const char *, ...);
void func_80087F7C(u16, u16, s32);
void func_80087EA4(u16, V12_ *);
u8 func_8008E38C(V12_, u8, u8, s32);
s32 func_8008E510(u8);

s32 Memory_Alloc(s32);
s32 func_800C9280();
s32 func_800C96DC();
extern void *D_800F5F08;
s32 Memory_Free(s32);
extern s32 D_800F9F90;
extern s32 D_800F9F94;
extern s32 D_800F9F98;

INCLUDE_ASM("asm/nonmatchings/unit_69E80", func_800C9280);

INCLUDE_ASM("asm/nonmatchings/unit_69E80", func_800C9590);

INCLUDE_ASM("asm/nonmatchings/unit_69E80", func_800C96DC);

INCLUDE_ASM("asm/nonmatchings/unit_69E80", func_800C9848);

void func_800C9978(void) {
    s32 i;
    u8 *t;

    func_800C9280();
    for (i = 0; i < 33; i++) {
        t = (&D_800F5F08)[i];
        if (t[0x20] != 0 && *(s32 *)(t + 0x28) == 0) {
            *(s32 *)((u8 *)(&D_800F5F08)[i] + 0x28) = Memory_Alloc(t[0x20] * 0x34);
        }
    }
    func_800C96DC();
}

void func_800C9A00(void) {
    s32 i;
    s32 j;

    func_800C9590();
    for (i = 0; i < 33; i++) {
        if (((TrA00_ **)&D_800F5F08)[i]->b20) {
            Memory_Free(((TrA00_ **)&D_800F5F08)[i]->w28);
            ((TrA00_ **)&D_800F5F08)[i]->w28 = 0;
        }
    }
    for (i = 0; i < 33; i++) {
        for (j = 0; j < ((TrA00_ **)&D_800F5F08)[i]->n; j++) {
            if (((TrA00_ **)&D_800F5F08)[i]->sub[j].a != 0) {
                Memory_Free(((TrA00_ **)&D_800F5F08)[i]->sub[j].a);
                ((TrA00_ **)&D_800F5F08)[i]->sub[j].a = 0;
            }
        }
    }
}

void func_800C9AF0(s32 n, s32 b) {
    D_800F9F90 = n;
    if (n != 0) {
        D_800F9F94 = b;
        D_800F9F98 = Memory_Alloc(n * 2);
        memset((void *)D_800F9F98, 0, D_800F9F90 * 2);
    }
}


void func_800C9B40(void) {
    if (D_800F9F90 != 0) {
        Memory_Free(D_800F9F98);
        D_800F9F98 = 0;
        D_800F9F94 = 0;
        D_800F9F90 = 0;
    }
}

void func_800C9B84(s32 i) {
    E0Cc_ *e = &((E0Cc_ *)D_800F9F94)[i];
    u8 *s = (u8 *)D_800F9F98 + i * 2;
    V12_ v;
    u16 f = e->flags;

    if (f & 1) {
        return;
    }
    if (f & 2) {
        func_80087F7C(e->a, e->c, i);
        func_80087EA4(e->a, &v);
        if (e->flags & 0x28) {
            func_8008E38C(v, e->d8, e->dA, 0);
        } else if (e->flags & 0x10) {
            s[0] = func_8008E38C(v, e->d8, e->dA, 1);
        }
        if (e->flags & 4) {
            func_80087F7C(e->b, e->c, i);
            func_80087EA4(e->b, &v);
            if (e->flags & 0x28) {
                func_8008E38C(v, e->d9, e->dB, 0);
            } else if (e->flags & 0x10) {
                s[1] = func_8008E38C(v, e->d9, e->dB, 1);
            }
        }
    }
}

void func_800C9D00(s32 i) {
    E0Cc_ *e = &((E0Cc_ *)D_800F9F94)[i];
    u8 *s = (u8 *)D_800F9F98 + i * 2;

    if (e->flags & 2) {
        if (e->flags & 0x10) {
            if (s[0] != 0xFF) {
                func_8008E510(s[0]);
            }
            if (e->flags & 4) {
                if (s[1] != 0xFF) {
                    func_8008E510(s[1]);
                }
            }
        }
    }
}

INCLUDE_ASM("asm/nonmatchings/unit_69E80", func_800C9DA0);

s32 func_800C9E70(void) {
    char buf[80];

    sprintf(buf, D_80101080);
    if (D_80140FE4(buf) == 0x100) {
        return D_80140FE0(buf, D_80138D38) == 0x100;
    }
    return 0;
}

s32 func_800C9ED0(Seq9_ *s) {
    s->c = s->t[s->a];
    s->e = s->t[s->b];
    s->g = -1;
    s->f = 0;
    s->d = 0;
    return 0;
}


s32 func_800C9F18(Ob9F_ *o, s32 id) {
    s32 i;

    if (o->cur == -1) {
        for (i = 0; i < o->t->n; i++) {
            if (o->t->ids[i * 2] == id) {
                o->cur = id;
                break;
            }
        }
    }
    return 0;
}

INCLUDE_ASM("asm/nonmatchings/unit_69E80", func_800C9F78);
