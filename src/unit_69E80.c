#include "common.h"
#include "include_asm.h"
typedef struct { s32 *t; s16 a; s16 b; s32 c; s32 d; s32 e; s16 f; s16 g; } Seq9_;
typedef struct { s32 a; s32 b; s32 c; } V12_;
typedef struct { u16 flags; u16 a; u16 b; u16 c; u8 d8; u8 d9; u8 dA; u8 dB; } E0Cc_;
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
    s32 var_s1;
    u8 temp_v1;
    void **var_s0;
    void *temp_v0;

    func_800C9280();
    var_s1 = 0;
    var_s0 = &D_800F5F08;
    do {
        temp_v0 = *var_s0;
        temp_v1 = (*(u8 *)((u8 *)(temp_v0) + 0x20));
        if ((temp_v1 != 0) && ((*(s32 *)((u8 *)(temp_v0) + 0x28)) == 0)) {
            (*(s32 *)((u8 *)((*var_s0)) + 0x28)) = Memory_Alloc(temp_v1 * 0x34);
        }
        var_s1 += 1;
        var_s0 += 1;
    } while (var_s1 < 0x21);
    func_800C96DC();
}

INCLUDE_ASM("asm/nonmatchings/unit_69E80", func_800C9A00);

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

void func_800C9D00(s32 arg0) {
    u16 *temp_s0;
    u16 temp_a0;
    u8 temp_a0_2;
    u8 temp_a0_3;
    void *temp_s1;

    temp_s0 = D_800F9F94 + (arg0 * 0xC);
    temp_a0 = *temp_s0;
    temp_s1 = D_800F9F98 + (arg0 * 2);
    if (temp_a0 & 2) {
        if (temp_a0 & 0x10) {
            temp_a0_2 = (*(u8 *)((u8 *)(temp_s1) + 0x0));
            if (temp_a0_2 != 0xFF) {
                func_8008E510(temp_a0_2);
            }
            if (*temp_s0 & 4) {
                temp_a0_3 = (*(u8 *)((u8 *)(temp_s1) + 0x1));
                if (temp_a0_3 != 0xFF) {
                    func_8008E510(temp_a0_3);
                }
            }
        }
    }
}

INCLUDE_ASM("asm/nonmatchings/unit_69E80", func_800C9DA0);

INCLUDE_ASM("asm/nonmatchings/unit_69E80", func_800C9E70);

s32 func_800C9ED0(Seq9_ *s) {
    s->c = s->t[s->a];
    s->e = s->t[s->b];
    s->g = -1;
    s->f = 0;
    s->d = 0;
    return 0;
}


INCLUDE_ASM("asm/nonmatchings/unit_69E80", func_800C9F18);

INCLUDE_ASM("asm/nonmatchings/unit_69E80", func_800C9F78);
