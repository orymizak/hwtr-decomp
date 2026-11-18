#include "common.h"
#include "ultra64.h"
#include "math.h"
#include "include_asm.h"
typedef struct { u16 magic; u8 pad[0xA]; s32 c; s32 w10; u32 p14; u32 p18; u32 p1C; u32 p20; u32 p24; u32 p28; u32 p2C; s32 w30; } Hdr79_;
extern s32 D_80138E38;
void Debug_FormatNumber(u16, char *);
void Debug_DrawString(char *, s32, s32, s32);
typedef struct { u8 pad[0xC]; u8 r; u8 g; u8 b; s8 on; } Col16_;
typedef struct { u8 pad[0xC]; u8 r; u8 g; u8 b; u8 flags; } Col16b_;
typedef struct { u16 n; u8 pad[2]; Col16b_ *c; u32 p8; u32 pC; s32 w10; } Hdr7E_;
extern s32 D_801454A0;
void GfxPools_Shutdown(void);
void WorldData_Free(s32);
extern u8 D_800F20F6;
void func_80079EF4(Hdr7E_ *h);
extern f64 D_800FF8E0;

s32 Memory_Alloc(s32);
f32 func_8007ADF0(f32, f32);
extern s32 D_8014551C;

INCLUDE_ASM("asm/nonmatchings/unit_19D40", func_80079140);

INCLUDE_ASM("asm/nonmatchings/unit_19D40", func_80079BD4);

void func_80079DBC(Hdr79_ *h) {
    if (h->p14 != 0) {
        h->p14 += (u32)h;
        func_80079EF4(h->p14);
    }
    h->c = 0;
    h->magic = 0xF6B1;
    h->w30 = 0;
    h->p18 += (u32)h;
    h->p1C += (u32)h;
    h->p20 += (u32)h;
    h->p24 += (u32)h;
    h->p28 += (u32)h;
    h->p2C += (u32)h;
}


s32 func_80079E48(u8 *m) {
    return Memory_Alloc(*(u16 *)(m + 4) * 8);
}

s32 func_80079E68(u8 *m) {
    s32 buf;
    u16 n = *(u16 *)(m + 6);

    buf = 0;
    if (n != 0) {
        buf = Memory_Alloc(n * 0x10);
    }
    return buf;
}

void func_80079E90(Col16_ *src, Col16_ *dst, u32 n) {
    u32 i;

    for (i = 0; i < n; i++, src++) {
        if (src->on != 0) {
            dst->r = src->r << 3;
            dst->g = src->g << 3;
            dst->b = src->b << 3;
        }
        dst++;
    }
}


void func_80079EF4(Hdr7E_ *h) {
    u8 *sh;
    s32 i;

    h->w10 = 0;
    h->c = (Col16b_ *)((u32)h->c + (u32)h);
    h->p8 += (u32)h;
    h->pC += (u32)h;
    for (i = 0; i < h->n * 3; i++) {
        sh = &D_800F20F6;
        if (h->c[i].flags & 1) {
            h->c[i].r >>= *sh;
            h->c[i].g >>= *sh;
            h->c[i].b >>= *sh;
        }
    }
}


void *func_80079FC0(u16 *n) {
    return ((void * (*)(s32 size))Memory_Alloc)(*n * 12 + *n * 160 + *n * 48 + 16);
}


INCLUDE_ASM("asm/nonmatchings/unit_19D40", func_8007A004);

INCLUDE_ASM("asm/nonmatchings/unit_19D40", func_8007A858);

static inline void V3o_Normalize(Vec3f *v) {
    f32 inv = 1.0f / sqrtf(v->x * v->x + v->y * v->y + v->z * v->z);

    v->x *= inv;
    v->y *= inv;
    v->z *= inv;
}
static inline void V3o_Cross(Vec3f *r, Vec3f *a, Vec3f *b) {
    r->x = a->y * b->z - b->y * a->z;
    r->y = b->x * a->z - a->x * b->z;
    r->z = a->x * b->y - b->x * a->y;
}
/* Re-orthonormalises the rotation part of a 3x3 matrix, keeping the direction of its first column. */
void func_8007A9E0(f32 *m) {
    Vec3f a;
    Vec3f b;
    Vec3f c;
    Vec3f t;
    Vec3f u;

    a.x = m[0];
    a.y = m[3];
    a.z = m[6];
    b.x = m[1];
    b.y = m[4];
    b.z = m[7];
    c.x = 0.0f;
    c.y = 0.0f;
    c.z = 0.0f;
    V3o_Normalize(&a);
    V3o_Cross(&t, &a, &b);
    V3o_Normalize(&t);
    c = t;
    V3o_Cross(&u, &c, &a);
    V3o_Normalize(&u);
    b = u;
    m[0] = a.x;
    m[1] = b.x;
    m[2] = c.x;
    m[3] = a.y;
    m[4] = b.y;
    m[5] = c.y;
    m[6] = a.z;
    m[7] = b.z;
    m[8] = c.z;
}

f32 func_8007AC64(f32 x) {
    f32 c;
    f32 r;

    if (fabsf(x) > 1.0f) {
        return 0.0f;
    }
    c = sqrtf(1.0f - x * x);
    if (c != 0.0f) {
        return func_8007ADF0(x, c);
    }
    r = 1.5707964f;
    if (!(x > 0.0f)) {
        r = -1.5707964f;
    }
    return r;
}

f32 func_8007AD24(f32 x) {
    f32 s;

    if (fabsf(x) > 1.0f) {
        return 0.0f;
    }
    s = sqrtf(1.0f - x * x);
    if (s != 0.0f) {
        return D_800FF8E0 - func_8007ADF0(x, s);
    }
    if (x > 0.0f) {
        return 0.0f;
    }
    return 3.1415927f;
}

INCLUDE_ASM("asm/nonmatchings/unit_19D40", func_8007ADF0);

f32 func_8007AFAC(f32 x) {
    if (0.0f <= x) {
        return x;
    }
    return -x;
}


void func_8007AFD4(f32 a[3][4], f32 b[3][4], f32 out[3][4]) {
    out[0][0] = a[0][0] * b[0][0] + a[0][1] * b[1][0] + a[0][2] * b[2][0];
    out[0][1] = a[0][0] * b[0][1] + a[0][1] * b[1][1] + a[0][2] * b[2][1];
    out[0][2] = a[0][0] * b[0][2] + a[0][1] * b[1][2] + a[0][2] * b[2][2];
    out[0][3] = a[0][0] * b[0][3] + a[0][1] * b[1][3] + a[0][2] * b[2][3] + a[0][3];
    out[1][0] = a[1][0] * b[0][0] + a[1][1] * b[1][0] + a[1][2] * b[2][0];
    out[1][1] = a[1][0] * b[0][1] + a[1][1] * b[1][1] + a[1][2] * b[2][1];
    out[1][2] = a[1][0] * b[0][2] + a[1][1] * b[1][2] + a[1][2] * b[2][2];
    out[1][3] = a[1][0] * b[0][3] + a[1][1] * b[1][3] + a[1][2] * b[2][3] + a[1][3];
    out[2][0] = a[2][0] * b[0][0] + a[2][1] * b[1][0] + a[2][2] * b[2][0];
    out[2][1] = a[2][0] * b[0][1] + a[2][1] * b[1][1] + a[2][2] * b[2][1];
    out[2][2] = a[2][0] * b[0][2] + a[2][1] * b[1][2] + a[2][2] * b[2][2];
    out[2][3] = a[2][0] * b[0][3] + a[2][1] * b[1][3] + a[2][2] * b[2][3] + a[2][3];
}

INCLUDE_ASM("asm/nonmatchings/unit_19D40", func_8007B230);

void func_8007B510(u16 n) {
    char buf[8];
    s32 y;
    s32 x;

    Debug_FormatNumber(n, buf);
    D_80138E38 = 0x8037FB2C;
    for (y = 0; y < 16; y++) {
        for (x = 0; x < 48; x++) {
            ((u16 *)0x8037FB2C)[y * 640 + x] = 0;
        }
    }
    Debug_DrawString(buf, 3, 0xFFFF, 1);
}


INCLUDE_ASM("asm/nonmatchings/unit_19D40", func_8007B598);

s32 func_8007BE30(void) {
    s32 i;

    for (i = 0; i < 32; i++) {
        (&D_801454A0)[i] = 0;
    }
    return 1;
}

INCLUDE_ASM("asm/nonmatchings/unit_19D40", World_Load);

s32 World_Free(u8 i) {
    s32 *p;

    if (i < 32) {
        if (((s32 *)&D_801454A0)[i] == 0) {
            return 1;
        }
        GfxPools_Shutdown();
        WorldData_Free(((s32 *)&D_801454A0)[i]);
        ((s32 *)&D_801454A0)[i] = 0;
        return 1;
    }
    return 1;
}

INCLUDE_ASM("asm/nonmatchings/unit_19D40", func_8007BF20);
