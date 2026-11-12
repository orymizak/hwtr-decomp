#include "common.h"
#include "include_asm.h"
typedef struct { u8 p0[4]; u16 *unk4; } BufD_;
typedef struct { u32 w0; u32 w1; } GfxA_;
typedef struct { u8 p0[0x34]; GfxA_ *unk34; } ObjA2_;
extern s32 D_80138AA8;
extern s32 D_8014C1A8;
typedef struct { s32 f0; s32 f4; u8 p8[0x548]; } S_D_801455C0;
void func_80075600(s32 arg0, s8 arg1, s8 arg2, s8 arg3);
void func_80075640(s32 arg0, f32 arg1, f32 arg2, f32 arg3, f32 arg4);
f32 sinf(f32);
typedef struct { s16 x; s16 y; s16 z; u8 pad[10]; } Vtx6D_;
typedef struct { Vtx6D_ v[32]; u8 p200[0x30]; f32 r; f32 a; u8 p238; u8 mode; } Ring6D_;
f64 func_800E6E80(f64);
f64 func_800E6F60(f64);
void func_8006D6EC(Ring6D_ *s, u32 t);
void func_8006D608(s32 *, s32);
void func_8006CF10(s32 *p, s32 dt);
extern S_D_801455C0 D_801455C0[];
void func_8006CA88(S_D_801455C0 *, s32, f32, u8);
s32 func_8007F5C4(s32,s32);
s32 func_8006DD60(s32 *,s32,s32);
extern s32 D_8014D980;

s32 func_800757E8(s32);
s32 func_8006D15C(s32 *, s32, s32);
s32 Memory_Free(s32);
extern s32 D_800F05E0;
s32 func_8007F57C(s32);
extern s32 D_800F05E4;

INCLUDE_ASM("asm/nonmatchings/unit_0D1F0", func_8006C5F0);

INCLUDE_ASM("asm/nonmatchings/unit_0D1F0", func_8006CA88);

void func_8006CF10(s32 *p, s32 dt) {
    u8 v;
    f32 t;
    u32 i;

    if (dt >= p[0]) {
        p[0] = 0;
        if (p[1] != -1) {
            func_800757E8(p[1]);
            p[1] = -1;
        }
        return;
    }
    if (p[1] != -1) {
        v = p[0] * 255.0f / 1000.0f;
        func_80075600(p[1], v, v, v);
        func_80075640(p[1], sinf(p[0] / 1000.0f * 3.141592f) * 1800.0f, *(f32 *)&p[135], *(f32 *)&p[139], *(f32 *)&p[143]);
    }
    p[0] -= dt;
    if (p[0] < 900) {
        t = (u32)dt / ((930 - p[0]) / 30);
    } else {
        t = (u32)dt;
    }
    func_8006D6EC((Ring6D_ *)(p + 4), t);
    for (i = 0; i < 12; i++) {
        func_8006D608(p + 148 + i * 16, dt);
    }
}

INCLUDE_ASM("asm/nonmatchings/unit_0D1F0", func_8006D15C);

INCLUDE_ASM("asm/nonmatchings/unit_0D1F0", func_8006D608);

void func_8006D6EC(Ring6D_ *s, u32 t) {
    u32 i;
    f32 ang;

    if (s->mode == 0) {
        for (i = 0; i < 32; i++) {
            ang = (f32) i * 3.14159265 / 16.0 + s->a;
            s->v[i].x = func_800E6E80(ang) * (s->r + (i & 1) * 65.0);
            s->v[i].y = func_800E6F60(ang) * (s->r + (i & 1) * 65.0);
            s->v[i].z = -((i & 1) * 65.0 / 3.0);
        }
    } else if (s->mode != 2) {
        for (i = 0; i < 32; i++) {
            ang = (f32) i * 3.14159265 / 16.0 + s->a;
            s->v[i].x = func_800E6E80(ang) * (s->r + (i & 1) * 65.0);
            s->v[i].y = func_800E6F60(ang) * (s->r + (i & 1) * 65.0);
            s->v[i].z = 0;
        }
    } else {
        return;
    }
    s->r = s->r + (f32) t * 3.5;
    s->a = s->a + (f32) t * 0.001;
}

void func_8006DA40(void *arg0) {
    s32 temp_a0;

    temp_a0 = *(s32 *)((u8 *)arg0 + 0x4);
    if (temp_a0 != -1) {
        func_800757E8(temp_a0);
    }
    *(s32 *)((u8 *)arg0 + 0x0) = 0;
    *(s32 *)((u8 *)arg0 + 0x4) = -1;
}

void func_8006DA88(void) {
    u32 var_a0;

    var_a0 = 0;
    do {
        D_801455C0[var_a0].f0 = 0;
        D_801455C0[var_a0].f4 = -1;
        var_a0 += 1;
    } while (var_a0 < 8U);
}

void func_8006DAC4(s32 arg0) {
    void *var_s0;
    u32 var_s1;

    var_s1 = 0;
    var_s0 = &(*(u8 *)&D_801455C0);
    do {
        ((s32 (*)(void *, s32))func_8006CF10)(var_s0, arg0);
        var_s1 += 1;
        var_s0 += 0x550;
    } while (var_s1 < 8U);
}

void func_8006DB1C(s32 arg0, s32 arg1) {
    void *var_s0;
    u32 var_s1;

    var_s1 = 0;
    var_s0 = &(*(u8 *)&D_801455C0);
    do {
        ((s32 (*)(void *, s32, s32))func_8006D15C)(var_s0, arg0, arg1 & 0xFF);
        var_s1 += 1;
        var_s0 += 0x550;
    } while (var_s1 < 8U);
}

void func_8006DB84(s32 a, f32 b, u8 c) {
    s32 min = D_801455C0[0].f0;
    u32 best = 0;
    u32 i;

    for (i = 1; i < 8; i++) {
        s32 v = D_801455C0[i].f0;

        if (v == 0) {
            best = i;
            break;
        }
        if (v < min) {
            best = i;
            min = v;
        }
    }
    func_8006CA88(&D_801455C0[best], a, b, c);
}


INCLUDE_ASM("asm/nonmatchings/unit_0D1F0", func_8006DC28);

void func_8006DC90(void) {
    func_8006DD60(&D_8014D980, 0x140, 0xF0);
}

void func_8006DCB8(void) {
    if (D_800F05E0 != 0) {
        Memory_Free(D_800F05E0);
        D_800F05E0 = 0;
    }
}

void func_8006DCE8(s32 arg0) {
    if (D_800F05E0 != 0) {
        Memory_Free(D_800F05E0);
        D_800F05E0 = 0;
    }
    D_800F05E0 = func_8007F57C(arg0);
}

void func_8006DD34(ObjA2_ *o) {
    if ((*(GfxA_ * *)&D_800F05E0) != 0) {
        *o->unk34 = *(*(GfxA_ * *)&D_800F05E0);
    }
}


INCLUDE_ASM("asm/nonmatchings/unit_0D1F0", func_8006DD60);

void func_8006DEF4(u16 *pal, s32 *idx) {
    s32 i;
    u16 *dst = (*(BufD_ * *)&D_800F05E0)->unk4;

    for (i = 0; i < 0x400; i++) {
        *dst++ = pal[*idx++];
    }
}

void func_8006DF34(s32 arg0) {
    register s32 var_a1;
    register s32 *var_v1;
    register u16 *var_a2;
    register s16 temp_v0;

    if ((NULL != (*(void * *)&D_800F05E0)) && (D_800F05E4 != 0) && ((1 != D_8014C1A8) & (D_80138AA8 != 0))) {
        var_a2 = (*(u32 *)((u8 *)((*(void * *)&D_800F05E0)) + 0x4));
        var_a1 = 0;
        var_v1 = &D_8014D980;
        do {
            temp_v0 = *(s16 *)((2 * *var_v1) + arg0);
            var_v1 += 1;
            *var_a2 = temp_v0;
            var_a2 += 1;
            var_a1 += 1;
        } while (var_a1 < 0x400);
    }
}

void func_8006DFBC(void) {
    if (D_800F05E0 != 0) {
        Memory_Free(D_800F05E0);
        D_800F05E0 = 0;
    }
    D_800F05E0 = func_8007F5C4(0x20, 0x20);
    D_800F05E4 = 1;
}

void func_8006E00C(s32 arg0) {
    if (D_800F05E0 != 0) {
        Memory_Free(D_800F05E0);
        D_800F05E0 = 0;
    }
    D_800F05E0 = func_8007F57C(arg0);
    D_800F05E4 = 0;
}
