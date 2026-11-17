#include "common.h"
#include "include_asm.h"
typedef struct { s32 x; s32 y; s32 z; } V3i9_;
typedef struct { s32 d[0x22]; } B88_;
typedef struct { f32 x, y, z; } V3_D34;
typedef struct { f32 m[3][4]; } M_D34;
typedef struct { V3_D34 pos; u8 p0C[8]; u8 flags; u8 p15[3]; V3_D34 dir; V3_D34 world; u8 contact; u8 p31[3]; V3_D34 n; V3_D34 pt; V3_D34 vel; u8 p58[0x18]; } W_D34;
typedef struct { u8 p0[0x10]; f32 yaw; u8 p14[0x6C]; V3_D34 up; u8 p8C[0xC]; V3_D34 pos; u8 pA4[0xC]; V3_D34 vel; u8 pBC[4]; f32 rot[3][3]; u8 pE4[0x30]; V3_D34 av; u8 p120[0x24]; W_D34 w[(0x3E4 - 0x144) / 0x70]; u8 n; u8 p3E5[2]; u8 cnt; u8 cnt2; u8 p3E9[0x1F3]; V3_D34 c; } C_D34;
typedef struct { f32 x; f32 y; f32 z; } V3_164;
typedef struct { f32 x; f32 y; f32 z; } V3_1B0;
typedef struct { f32 m[3][3]; } M3_1B0;
extern void (*D_80141760)(u8, u8, V3_1B0);
extern void (*D_80145590)(u8, u8);
extern void (*D_8014C11C)(u8, f32);
extern void (*D_8014C114)(u8, s32, s32);
extern u8 D_80138D6A;
typedef struct { f32 x; f32 y; f32 z; } V3_954;
extern u8 D_80138D6D;
extern u8 D_80138D6E;
extern u8 D_80138D6F;
extern u8 D_80138D70;
extern u8 D_80138D71;
void func_800A5AB0(u8 *);
void func_800911E0(u8);
extern s32 D_8011B320;
extern s32 (*D_8014169C)(s32);
extern void (*D_8014C0A8)(s32, s32, f32, f32);
u8 func_800977AC(u8 *);
void func_800949E4(u8 *);
void func_80092534(u8 *);
u32 func_800A76D8(void);
void func_800AEFF8(void);
void func_80095804(u8 *);
void func_80091E30(u8 *, f32);
void func_8009887C(u8 *);
f32 sinf(f32);
f32 cosf(f32);
f32 sqrtf(f32);
void func_80094234(void);
void func_800863A4(u8 *, u8 *);
B88_ *func_8008637C(u8 *);
void func_800ACDC8(void *, f32);
void func_8007A9E0(void *);
extern void (*D_8014556C)(s32);
extern s32 (*D_80145570)(s32, s32);
extern void (*D_8014558C)(s32, s32, s32);
s32 func_8008D3AC(void);
void func_80095FE4(s32, u8 *, u8 *);
extern u8 D_800F38A0[];
void *memset(void *dst, s32 value, u32 size);
extern s32 (*D_80141698)();
s32 func_80099434(void *);
s32 func_8009951C(void *);
extern u8 D_80118634;
extern u8 D_80118630;

extern s32 D_8011B318;

INCLUDE_ASM("asm/nonmatchings/unit_31E10", func_80091210);

INCLUDE_ASM("asm/nonmatchings/unit_31E10", func_80091B20);

INCLUDE_ASM("asm/nonmatchings/unit_31E10", func_80091BA0);

void func_80091DC4(s32 arg0) {
  s32 temp_v0;
  u32 temp_v1;
  temp_v0 = D_80141698();
  temp_v1 = temp_v0 - (*((s32 *) (((u8 *) arg0) + 0x4A8)));
  if (!(temp_v1 >= 0x65U)) {
    *((s32 *) (((u8 *) arg0) + 0x4AC)) = (u32) ((*((s32 *) (((u8 *) arg0) + 0x4AC))) + temp_v1);
  }
  else
  {
    *((s32 *) (((u8 *) arg0) + 0x4AC)) = 0U;
  }
  *((s32 *) (((u8 *) arg0) + 0x4A8)) = temp_v0;
  if (((*((u8 *) (((u8 *) arg0) + 0x4B4))) != 0) && (((u32) (*((s32 *) (((u8 *) arg0) + 0x4AC)))) >= 0x1F5U)) {
    *((u8 *) (((u8 *) arg0) + 0x4B4)) = 0U;
  }
}

typedef struct { f32 x; f32 y; f32 z; } V3_E30;
typedef struct { f32 m[3][3]; } M3_E30;
extern u8 D_8011B31C;
void func_8008AEC0(s32, s32, s32, s32, s32, s32, s32, u8 *, u16 *, u32 *);
void func_8008E568(s32);
void func_800ABF2C(u8, u32, u16, u8);
static inline V3_E30 mul_E30(M3_E30 *m, V3_E30 *v) {
    V3_E30 r;

    r.x = m->m[0][0] * v->x + m->m[0][1] * v->y + m->m[0][2] * v->z;
    r.y = m->m[1][0] * v->x + m->m[1][1] * v->y + m->m[1][2] * v->z;
    r.z = m->m[2][0] * v->x + m->m[2][1] * v->y + m->m[2][2] * v->z;
    return r;
}
void func_80091E30(u8 *p, f32 dt) {
    s32 a[3];
    s32 b[3];
    V3_E30 v;
    V3_E30 w;
    M3_E30 t;
    s32 ms;
    u8 n;
    u16 x;
    u32 pts;
    u8 c;
    s32 z;
    s32 k;
    s32 q;
    s32 el;

    ms = dt * 1000.0f;
    pts = 0;
    x = 0;
    n = 0;
    if (p[0x4B4] == 0) {
        if (p[0x3E7] == 0) {
            p[0x4B4] = 1;
            *(s32 *)(p + 0x4A0) = 0;
            t.m[0][0] = *(f32 *)(p + 0xC0);
            t.m[0][1] = *(f32 *)(p + 0xCC);
            t.m[0][2] = *(f32 *)(p + 0xD8);
            t.m[1][0] = *(f32 *)(p + 0xC4);
            t.m[1][1] = *(f32 *)(p + 0xD0);
            t.m[1][2] = *(f32 *)(p + 0xDC);
            t.m[2][0] = *(f32 *)(p + 0xC8);
            t.m[2][1] = *(f32 *)(p + 0xD4);
            t.m[2][2] = *(f32 *)(p + 0xE0);
            v = *(V3_E30 *)(p + 0x114);
            w = mul_E30(&t, &v);
            *(V3_E30 *)(p + 0x47C) = w;
            *(f32 *)(p + 0x488) = 0.0f;
            *(f32 *)(p + 0x48C) = 0.0f;
            *(f32 *)(p + 0x490) = 0.0f;
            *(f32 *)(p + 0x494) = 0.0f;
            *(f32 *)(p + 0x498) = 0.0f;
            *(f32 *)(p + 0x49C) = 0.0f;
        }
    } else {
        if (p[0x3E8] == 0) {
            if (p[0x3E7] != 0) {
                p[0x4B4] = 0;
            }
            *(s32 *)(p + 0x4A0) += ms;
            t.m[0][0] = *(f32 *)(p + 0xC0);
            t.m[0][1] = *(f32 *)(p + 0xCC);
            t.m[0][2] = *(f32 *)(p + 0xD8);
            t.m[1][0] = *(f32 *)(p + 0xC4);
            t.m[1][1] = *(f32 *)(p + 0xD0);
            t.m[1][2] = *(f32 *)(p + 0xDC);
            t.m[2][0] = *(f32 *)(p + 0xC8);
            t.m[2][1] = *(f32 *)(p + 0xD4);
            t.m[2][2] = *(f32 *)(p + 0xE0);
            v = *(V3_E30 *)(p + 0x114);
            v = mul_E30(&t, &v);
            if (!(v.x * *(f32 *)(p + 0x47C) >= 0.0f)) {
                if (((*(f32 *)(p + 0x494)) < 0.0f ? -(*(f32 *)(p + 0x494)) : (*(f32 *)(p + 0x494))) < ((*(f32 *)(p + 0x488)) < 0.0f ? -(*(f32 *)(p + 0x488)) : (*(f32 *)(p + 0x488)))) {
                    *(f32 *)(p + 0x494) = *(f32 *)(p + 0x488);
                }
            }
            if (!(v.y * *(f32 *)(p + 0x480) >= 0.0f)) {
                if (((*(f32 *)(p + 0x498)) < 0.0f ? -(*(f32 *)(p + 0x498)) : (*(f32 *)(p + 0x498))) < ((*(f32 *)(p + 0x48C)) < 0.0f ? -(*(f32 *)(p + 0x48C)) : (*(f32 *)(p + 0x48C)))) {
                    *(f32 *)(p + 0x498) = *(f32 *)(p + 0x48C);
                }
            }
            if (!(v.z * *(f32 *)(p + 0x484) >= 0.0f)) {
                if (((*(f32 *)(p + 0x49C)) < 0.0f ? -(*(f32 *)(p + 0x49C)) : (*(f32 *)(p + 0x49C))) < ((*(f32 *)(p + 0x490)) < 0.0f ? -(*(f32 *)(p + 0x490)) : (*(f32 *)(p + 0x490)))) {
                    *(f32 *)(p + 0x49C) = *(f32 *)(p + 0x490);
                }
            }
            w.x = v.x * dt;
            w.y = v.y * dt;
            w.z = v.z * dt;
            *(f32 *)(p + 0x488) = w.x + *(f32 *)(p + 0x488);
            *(f32 *)(p + 0x48C) = w.y + *(f32 *)(p + 0x48C);
            *(f32 *)(p + 0x490) = w.z + *(f32 *)(p + 0x490);
            *(V3_E30 *)(p + 0x47C) = v;
        } else {
            *(s32 *)(p + 0x4A0) += ms;
            p[0x4B4] = 0;
            el = *(s32 *)(p + 0x4A0);
            a[0] = (s32)(*(f32 *)(p + 0x494) * 180.0f / 3.14159f);
            b[0] = (s32)(*(f32 *)(p + 0x488) * 180.0f / 3.14159f) - a[0];
            a[1] = (s32)(*(f32 *)(p + 0x498) * 180.0f / 3.14159f);
            b[1] = (s32)(*(f32 *)(p + 0x48C) * 180.0f / 3.14159f) - a[1];
            a[2] = (s32)(*(f32 *)(p + 0x49C) * 180.0f / 3.14159f);
            b[2] = (s32)(*(f32 *)(p + 0x490) * 180.0f / 3.14159f) - a[2];
            func_8008AEC0(a[0], b[0], a[1], b[1], a[2], b[2], el, &n, &x, &pts);
            if (pts != 0) {
                if ((*(u32 *)(p + 4) & 1) && (pts > 1000 || D_8014169C(8) == 0)) {
                    if (D_8014169C(2) != 0) {
                        func_8008E568(1);
                    } else {
                        func_8008E568(2);
                    }
                }
                if (D_8011B31C == 0) {
                    pts = 0;
                }
                func_800ABF2C(p[0xC], pts, x, n);
                q = *(s32 *)(p + 0x4B0);
                k = n;
                *(s32 *)(p + 0x4B0) = q + pts;
                c = k;
                p[0x6CC] += k;
                if (p[0x6CC] > 10) {
                    c = k + 10 - p[0x6CC];
                    p[0x6CC] = 10;
                }
                if ((*(u32 *)(p + 0x6CC) & 0xFFFF0000) == 0x0A000000) {
                    func_8008E568(3);
                    p[0x6CD] = 1;
                }
                z = c;
                n = z;
                if (n) {
                    D_8014C0A8(0x3A, 1, 1.0f, 1.0f);
                } else {
                    D_8014C0A8(0x3B, 1, 1.0f, 1.0f);
                }
            }
        }
        *(s32 *)(p + 0x4A4) += ms;
    }
}

INCLUDE_ASM("asm/nonmatchings/unit_31E10", func_80092534);

void func_80093164(u8 *p, f32 dt) {
    V3_164 col;
    u8 f;
    f32 v;

    if (p[0x6C1] != 0) {
        func_80094234();
        p[0x6C1] = 0;
    } else if (p[0x4B8] != 0) {
        if (p[0x25] != 0 && *(u32 *)(p + 0x4BC) >= 1000) {
            ((void (*)(u8 *))func_80094234)(p);
        }
    } else if (p[0x25] != 0 && *(s32 *)(p + 0x774) == 0 && (p[0x3E7] != p[0x3E4] || *(f32 *)(p + 0xBC) < 176.0f)) {
        ((void (*)(u8 *))func_80094234)(p);
    }
    if (p[0x4B8] == 0) {
        if (p[0x27] != 0 && p[0x29] == 0) {
            if (D_8011B320 & 0x40) {
                func_800977AC(p);
            } else if (p[0x6CC] != 0) {
                if (func_800977AC(p)) {
                    p[0x6CC]--;
                }
            } else {
                D_8014C0A8(0x2C, 0, 1.0f, 1.0f);
            }
            p[0x6CD] = 1;
        }
        p[0x28] = p[0x26];
        p[0x29] = p[0x27];
        f = 0;
        if (p[0x3E7] == p[0x3E4] && *(f32 *)(p + 0xE0) > 0.9848f && (*(s32 **)(*(u8 **)(p + 0x758) + 0x70))[2] == 0 && p[0x4B8] == 0 && (*(u32 *)(p + 4) & 0x20) && *(f32 *)(p + 0xBC) > 88.0f) {
            f = 1;
        }
        f = f || *(s32 *)(p + 0x630) == -1;
        if (f) {
            *(V3i9_ *)(p + 0x600) = *(V3i9_ *)(p + 0x98);
            *(f32 *)(p + 0x60C) = *(f32 *)(p + 0xC0);
            *(f32 *)(p + 0x610) = *(f32 *)(p + 0xC4);
            *(f32 *)(p + 0x614) = *(f32 *)(p + 0xC8);
            *(f32 *)(p + 0x618) = *(f32 *)(p + 0xCC);
            *(f32 *)(p + 0x61C) = *(f32 *)(p + 0xD0);
            *(f32 *)(p + 0x620) = *(f32 *)(p + 0xD4);
            *(f32 *)(p + 0x624) = *(f32 *)(p + 0xD8);
            *(f32 *)(p + 0x628) = *(f32 *)(p + 0xDC);
            *(f32 *)(p + 0x62C) = *(f32 *)(p + 0xE0);
            *(s32 *)(p + 0x630) = **(s32 **)(*(u8 **)(p + 0x758) + 0x70);
        }
        if (p[0x6BC] != 0 && (p[0x24] != 0 || *(f32 *)(p + 0x18) > 0.2f) && p[0x3E7] == p[0x3E4]) {
            col.x = *(f32 *)(p + 0xC4);
            col.y = *(f32 *)(p + 0xD0);
            col.z = *(f32 *)(p + 0xDC);
            if (*(f32 *)(p + 0xB0) * col.x + *(f32 *)(p + 0xB4) * col.y + *(f32 *)(p + 0xB8) * col.z > 0.0f) {
                *(f32 *)(p + 0xA4) *= 0.9f;
                *(f32 *)(p + 0xA8) *= 0.9f;
                *(f32 *)(p + 0xAC) *= 0.9f;
            }
        }
        if (p[0x13C] == 0) {
            func_800949E4(p);
        }
        if (p[0x3E7] == 0) {
            func_80092534(p);
        } else {
            p[0x6C3] = 0;
        }
    }
    f = 0;
    if ((*(u32 *)(p + 4) & 1) && p[0x4B4] != 0 && *(u32 *)(p + 0x4A0) > 500 && *(f32 *)(p + 0xB8) > 0.0f) {
        f = func_800A76D8() - *(s32 *)(p + 0x770) > 500;
    }
    func_800ACDC8(p + 0x2C, dt);
    if (f && *(f32 *)(p + 0xB8) < 0.0f && D_8014169C(2) == 0) {
        func_800AEFF8();
    }
    if ((*(u32 *)(p + 4) & 1) && p[0x4B8] != 0 && *(s32 *)(p + 0x4BC) == 500 && D_8014169C(2) == 0) {
        func_800AEFF8();
    }
    if (p[0x13C] == 0) {
        func_8007A9E0(p + 0xC0);
        func_80095804(p);
    }
    if (p[0x4B8] == 0) {
        if (*(u32 *)(p + 4) & 1) {
            func_80091E30(p, dt);
        }
        if (*(f32 *)(p + 0x14) < 0.0f || *(f32 *)(p + 0x14) > 0.0f || *(f32 *)(p + 0x18) < 0.0f || *(f32 *)(p + 0x18) > 0.0f) {
            p[0x13C] = 0;
        }
    }
    if (p[0x3F4] != 0) {
        v = *(f32 *)(p + 0x14);
    } else {
        v = *(f32 *)(p + 0x18);
    }
    if (v > 0.0f && p[0x4B8] == 0) {
        *(u32 *)(p + 8) |= 4;
    } else {
        *(u32 *)(p + 8) &= ~4;
    }
    if (*(f32 *)(p + 0x14) > 0.5f && (*(f32 *)(p + 0x10) < 0.0f ? -*(f32 *)(p + 0x10) : *(f32 *)(p + 0x10)) > 0.5f && p[0x3E7] == p[0x3E4] && *(f32 *)(p + 0xBC) < 88.0f && func_800A76D8() - *(s32 *)(p + 0x770) < 100) {
        *(s32 *)(p + 0x76C) += 10;
    } else {
        *(s32 *)(p + 0x76C) = 0;
    }
    if (*(u32 *)(p + 0x76C) > 500) {
        func_8009887C(p);
    }
}

void func_800937E8(u8 *p, f32 dt) {
    u8 f;

    if (p[0x6C1] != 0) {
        func_80094234();
        func_800863A4(p, p + 0x634);
        p[0x6C1] = 0;
    }
    f = 0;
    if (p[0x3E7] == p[0x3E4] && *(f32 *) (p + 0xE0) > 0.9848f && p[0x4B8] == 0) {
        f = (*(u32 *) (p + 4) >> 5) & 1;
    }
    f = f | (*(s32 *) (p + 0x630) == -1);
    if (f != 0) {
        *(V3i9_ *) (p + 0x600) = *(V3i9_ *) (p + 0x98);
        *(f32 *) (p + 0x60C) = *(f32 *) (p + 0xC0);
        *(f32 *) (p + 0x610) = *(f32 *) (p + 0xC4);
        *(f32 *) (p + 0x614) = *(f32 *) (p + 0xC8);
        *(f32 *) (p + 0x618) = *(f32 *) (p + 0xCC);
        *(f32 *) (p + 0x61C) = *(f32 *) (p + 0xD0);
        *(f32 *) (p + 0x620) = *(f32 *) (p + 0xD4);
        *(f32 *) (p + 0x624) = *(f32 *) (p + 0xD8);
        *(f32 *) (p + 0x628) = *(f32 *) (p + 0xDC);
        *(f32 *) (p + 0x62C) = *(f32 *) (p + 0xE0);
        *(s32 *) (p + 0x630) = **(s32 **) (*(u8 **) (p + 0x758) + 0x70);
        *(B88_ *) (p + 0x634) = *func_8008637C(p);
    }
    if (p[0x4B8] != 0 || p[0x47B] != 0) {
        func_800ACDC8(p + 0x2C, dt);
        func_8007A9E0(p + 0xC0);
    }
}

void func_80093990(void) {

}

INCLUDE_ASM("asm/nonmatchings/unit_31E10", func_80093998);

INCLUDE_ASM("asm/nonmatchings/unit_31E10", func_80093B74);

void func_80093D34(C_D34 *car) {
    V3_D34 col1;
    V3_D34 col0;
    V3_D34 r1;
    V3_D34 r2;
    M_D34 m;
    V3_D34 rel;
    V3_D34 t;
    V3_D34 a;
    V3_D34 b;
    V3_D34 d;
    V3_D34 cr;
    f32 c, s, inv;
    u32 i;
    f32 rx, ry, rz;

    m.m[0][0] = car->rot[0][0];
    m.m[0][1] = car->rot[0][1];
    m.m[0][2] = car->rot[0][2];
    m.m[0][3] = car->pos.x;
    m.m[1][0] = car->rot[1][0];
    m.m[1][1] = car->rot[1][1];
    m.m[1][2] = car->rot[1][2];
    m.m[1][3] = car->pos.y;
    m.m[2][0] = car->rot[2][0];
    m.m[2][1] = car->rot[2][1];
    m.m[2][2] = car->rot[2][2];
    m.m[2][3] = car->pos.z;
    col0.x = car->rot[0][0];
    col0.y = car->rot[1][0];
    col0.z = car->rot[2][0];
    col1.x = car->rot[0][1];
    col1.y = car->rot[1][1];
    col1.z = car->rot[2][1];
    c = cosf(-car->yaw);
    s = sinf(-car->yaw);
    a.x = col0.x * s;
    a.y = col0.y * s;
    a.z = col0.z * s;
    b.x = col1.x * c;
    b.y = col1.y * c;
    b.z = col1.z * c;
    r1.x = b.x - a.x;
    r1.y = b.y - a.y;
    r1.z = b.z - a.z;
    r2.x = b.x + a.x;
    r2.y = b.y + a.y;
    r2.z = b.z + a.z;
    car->cnt = 0;
    car->cnt2 = 0;
    for (i = 0; i < car->n; i++) {
        if (car->w[i].flags & 2) {
            if (i < 2) {
                car->w[i].dir = r1;
            } else {
                car->w[i].dir = r2;
            }
        } else {
            car->w[i].dir = col1;
        }
        if (car->w[i].contact) {
            t.x = car->w[i].n.x * (car->w[i].dir.x * car->w[i].n.x + car->w[i].dir.y * car->w[i].n.y + car->w[i].dir.z * car->w[i].n.z);
            t.y = car->w[i].n.y * (car->w[i].dir.x * car->w[i].n.x + car->w[i].dir.y * car->w[i].n.y + car->w[i].dir.z * car->w[i].n.z);
            t.z = car->w[i].n.z * (car->w[i].dir.x * car->w[i].n.x + car->w[i].dir.y * car->w[i].n.y + car->w[i].dir.z * car->w[i].n.z);
            t.x = car->w[i].dir.x - t.x;
            t.y = car->w[i].dir.y - t.y;
            t.z = car->w[i].dir.z - t.z;
            inv = 1.0f / sqrtf(t.x * t.x + t.y * t.y + t.z * t.z);
            t.x *= inv;
            t.y *= inv;
            t.z *= inv;
            car->w[i].dir = t;
        }
        rel.x = rx = car->w[i].pos.x - car->c.x;
        rel.y = ry = car->w[i].pos.y - car->c.y;
        rel.z = rz = car->w[i].pos.z - car->c.z;
        car->w[i].world.x = m.m[0][0] * rx + m.m[0][1] * ry + m.m[0][2] * rz + m.m[0][3];
        car->w[i].world.y = m.m[1][0] * rx + m.m[1][1] * ry + m.m[1][2] * rz + m.m[1][3];
        car->w[i].world.z = m.m[2][0] * rx + m.m[2][1] * ry + m.m[2][2] * rz + m.m[2][3];
        if (car->w[i].contact) {
            d.x = car->w[i].pt.x - car->pos.x;
            d.y = car->w[i].pt.y - car->pos.y;
            d.z = car->w[i].pt.z - car->pos.z;
            cr.x = car->av.y * d.z - d.y * car->av.z;
            cr.y = d.x * car->av.z - car->av.x * d.z;
            cr.z = car->av.x * d.y - d.x * car->av.y;
            car->w[i].vel.x = car->vel.x + cr.x;
            car->w[i].vel.y = car->vel.y + cr.y;
            car->w[i].vel.z = car->vel.z + cr.z;
            if (car->w[i].contact) {
                car->cnt++;
                if (-(car->w[i].n.x * car->up.x + car->w[i].n.y * car->up.y + car->w[i].n.z * car->up.z) > 0.5f) {
                    car->cnt2++;
                }
            }
        }
    }
}

INCLUDE_ASM("asm/nonmatchings/unit_31E10", func_80094234);

INCLUDE_ASM("asm/nonmatchings/unit_31E10", func_800946DC);

INCLUDE_ASM("asm/nonmatchings/unit_31E10", func_800949E4);

INCLUDE_ASM("asm/nonmatchings/unit_31E10", func_80095804);

INCLUDE_ASM("asm/nonmatchings/unit_31E10", func_80095970);

INCLUDE_ASM("asm/nonmatchings/unit_31E10", func_80095C60);

INCLUDE_ASM("asm/nonmatchings/unit_31E10", func_80095D5C);

INCLUDE_ASM("asm/nonmatchings/unit_31E10", func_80095FE4);

void *func_800960D8(s32 arg0) {
    return ((arg0 & 0xFF) * 0x77C) + &D_80118630;
}

s32 func_80096100(void) {
    return D_8011B318;
}

s32 func_8009610C(s32 arg0) {
    return *(s32 *)(&D_80118634 + ((arg0 & 0xFF) * 0x77C)) & 1;
}

void func_80096138(s32 arg0, s32 arg1) {
    void *temp_s0;

    temp_s0 = ((arg0 & 0xFF) * 0x77C) + &D_80118630;
    if ((*(s32 *)((u8 *)(temp_s0) + 0x6D8)) == 2) {
        func_8009951C(temp_s0);
    }
    (*(s32 *)((u8 *)(temp_s0) + 0x6D8)) = arg1;
    if (arg1 == 2) {
        func_80099434(temp_s0);
    }
}

void func_800961B0(u8 *p, u8 id, u8 flip) {
    V3_1B0 v;
    M3_1B0 t;
    M3_1B0 a;
    M3_1B0 b;
    V3_1B0 w;
    M3_1B0 *r;
    M3_1B0 *in;
    s32 dx;
    s32 dy;
    s32 dz;
    s32 one;
    s32 (**rnd)(s32);

    if (p[0x4B8] == 0) {
        one = 1;
        p[0x4B8] = one;
        if (flip) {
            *(f32 *)(p + 0xB0) = 0.0f;
            *(f32 *)(p + 0xB4) = 0.0f;
            *(f32 *)(p + 0xB8) = 0.0f;
            *(f32 *)(p + 0x114) = -*(f32 *)(p + 0x114);
            *(f32 *)(p + 0x118) = -*(f32 *)(p + 0x118);
            *(f32 *)(p + 0x11C) = -*(f32 *)(p + 0x11C);
        }
        *(f32 *)(p + 0xB0) *= 0.5f;
        *(f32 *)(p + 0xB4) *= 0.5f;
        *(f32 *)(p + 0xB8) *= 0.5f;
        v = *(V3_1B0 *)(p + 0xB0);
        if (*(u32 *)(p + 4) & 1) {
            func_800A5AB0(p);
        }
        rnd = &D_8014169C;
        *(f32 *)(p + 0xB8) += ((*rnd)(10) + 10) * 17.6f;
        *(f32 *)(p + 0xA4) = *(f32 *)(p + 0xB0) * *(f32 *)(p + 0x74);
        *(f32 *)(p + 0xA8) = *(f32 *)(p + 0xB4) * *(f32 *)(p + 0x74);
        *(f32 *)(p + 0xAC) = *(f32 *)(p + 0xB8) * *(f32 *)(p + 0x74);
        dx = (*rnd)(24) - 12;
        dy = (*rnd)(24) - 12;
        dz = (*rnd)(24) - 12;
        *(f32 *)(p + 0x114) += dx;
        *(f32 *)(p + 0x118) += dy;
        *(f32 *)(p + 0x11C) += dz;
        r = (M3_1B0 *)(p + 0xC0);
        in = (M3_1B0 *)(p + 0x2C);
        t.m[0][0] = r->m[0][0];
        t.m[0][1] = r->m[1][0];
        t.m[0][2] = r->m[2][0];
        t.m[1][0] = r->m[0][1];
        t.m[1][1] = r->m[1][1];
        t.m[1][2] = r->m[2][1];
        t.m[2][0] = r->m[0][2];
        t.m[2][1] = r->m[1][2];
        t.m[2][2] = r->m[2][2];
        a.m[0][0] = r->m[0][0] * in->m[0][0] + r->m[0][1] * in->m[1][0] + r->m[0][2] * in->m[2][0];
        a.m[0][1] = r->m[0][0] * in->m[0][1] + r->m[0][1] * in->m[1][1] + r->m[0][2] * in->m[2][1];
        a.m[0][2] = r->m[0][0] * in->m[0][2] + r->m[0][1] * in->m[1][2] + r->m[0][2] * in->m[2][2];
        a.m[1][0] = r->m[1][0] * in->m[0][0] + r->m[1][1] * in->m[1][0] + r->m[1][2] * in->m[2][0];
        a.m[1][1] = r->m[1][0] * in->m[0][1] + r->m[1][1] * in->m[1][1] + r->m[1][2] * in->m[2][1];
        a.m[1][2] = r->m[1][0] * in->m[0][2] + r->m[1][1] * in->m[1][2] + r->m[1][2] * in->m[2][2];
        a.m[2][0] = r->m[2][0] * in->m[0][0] + r->m[2][1] * in->m[1][0] + r->m[2][2] * in->m[2][0];
        a.m[2][1] = r->m[2][0] * in->m[0][1] + r->m[2][1] * in->m[1][1] + r->m[2][2] * in->m[2][1];
        a.m[2][2] = r->m[2][0] * in->m[0][2] + r->m[2][1] * in->m[1][2] + r->m[2][2] * in->m[2][2];
        b.m[0][0] = a.m[0][0] * t.m[0][0] + a.m[0][1] * t.m[1][0] + a.m[0][2] * t.m[2][0];
        b.m[0][1] = a.m[0][0] * t.m[0][1] + a.m[0][1] * t.m[1][1] + a.m[0][2] * t.m[2][1];
        b.m[0][2] = a.m[0][0] * t.m[0][2] + a.m[0][1] * t.m[1][2] + a.m[0][2] * t.m[2][2];
        b.m[1][0] = a.m[1][0] * t.m[0][0] + a.m[1][1] * t.m[1][0] + a.m[1][2] * t.m[2][0];
        b.m[1][1] = a.m[1][0] * t.m[0][1] + a.m[1][1] * t.m[1][1] + a.m[1][2] * t.m[2][1];
        b.m[1][2] = a.m[1][0] * t.m[0][2] + a.m[1][1] * t.m[1][2] + a.m[1][2] * t.m[2][2];
        b.m[2][0] = a.m[2][0] * t.m[0][0] + a.m[2][1] * t.m[1][0] + a.m[2][2] * t.m[2][0];
        b.m[2][1] = a.m[2][0] * t.m[0][1] + a.m[2][1] * t.m[1][1] + a.m[2][2] * t.m[2][1];
        b.m[2][2] = a.m[2][0] * t.m[0][2] + a.m[2][1] * t.m[1][2] + a.m[2][2] * t.m[2][2];
        w.x = b.m[0][0] * *(f32 *)(p + 0x114) + b.m[0][1] * *(f32 *)(p + 0x118) + b.m[0][2] * *(f32 *)(p + 0x11C);
        w.y = b.m[1][0] * *(f32 *)(p + 0x114) + b.m[1][1] * *(f32 *)(p + 0x118) + b.m[1][2] * *(f32 *)(p + 0x11C);
        w.z = b.m[2][0] * *(f32 *)(p + 0x114) + b.m[2][1] * *(f32 *)(p + 0x118) + b.m[2][2] * *(f32 *)(p + 0x11C);
        *(V3_1B0 *)(p + 0xE4) = w;
        *(u32 *)(p + 8) &= ~2;
        D_80141760(p[0], id, v);
        if (*(u32 *)(p + 4) & 1) {
            if ((*rnd)(2) != 0) {
                p[0x4B9] = one;
            }
            func_800911E0(p[0]);
            D_80145590(p[0], D_80138D6A);
            D_8014C0A8(0x1D, 1, 1.0f, 1.0f);
        }
        D_8014C11C(p[0], 1.0f);
        D_8014C114(p[0], 0x1D, 1);
    }
}

static inline void push_954(u8 *c, V3_954 *at, V3_954 *f, V3_954 *a) {
    V3_954 tq;

    a->x = at->x - *(f32 *)(c + 0x98);
    a->y = at->y - *(f32 *)(c + 0x9C);
    a->z = at->z - *(f32 *)(c + 0xA0);
    tq.x = a->y * f->z - f->y * a->z;
    tq.y = f->x * a->z - a->x * f->z;
    tq.z = a->x * f->y - f->x * a->y;
    *(f32 *)(c + 0x130) = tq.x + *(f32 *)(c + 0x130);
    *(f32 *)(c + 0x134) = tq.y + *(f32 *)(c + 0x134);
    *(f32 *)(c + 0x138) = tq.z + *(f32 *)(c + 0x138);
    *(f32 *)(c + 0x124) = *(f32 *)(c + 0x124) + f->x;
    *(f32 *)(c + 0x128) = *(f32 *)(c + 0x128) + f->y;
    *(f32 *)(c + 0x12C) = *(f32 *)(c + 0x12C) + f->z;
}

void func_80096954(u8 *c) {
    V3_954 axis;
    V3_954 pos;
    V3_954 pt;
    V3_954 fl;
    V3_954 F;
    V3_954 tmp;
    f32 d;
    f32 f;
    s32 (**rnd)(s32);

    c[0x6C3] = 0;
    pos.x = *(f32 *)(c + 0x98) + *(f32 *)(c + 0x8C);
    pos.y = *(f32 *)(c + 0x9C) + *(f32 *)(c + 0x90);
    pos.z = *(f32 *)(c + 0xA0) + *(f32 *)(c + 0x94);
    axis.x = *(f32 *)(c + 0xC0);
    axis.y = *(f32 *)(c + 0xCC);
    axis.z = *(f32 *)(c + 0xD8);
    d = axis.x * *(f32 *)(c + 0x720) + axis.y * *(f32 *)(c + 0x724) + axis.z * *(f32 *)(c + 0x728);
    if ((d < 0.0f ? -d : d) > 0.7071f) {
        *(u32 *)(c + 0x75C) += 10;
        if (*(u32 *)(c + 0x75C) > D_80138D6D) {
            V3_954 tq;
            f = *(f32 *)(c + 0x5E8) * *(f32 *)(c + 0x5E8) + *(f32 *)(c + 0x5F0) * *(f32 *)(c + 0x5F0);
            if (d > 0.0f) {
                f = -f;
            }
            fl.x = 0.0f;
            fl.y = 0.0f;
            fl.z = f;
            tmp.x = *(f32 *)(c + 0xC0) * fl.x + *(f32 *)(c + 0xC4) * fl.y + *(f32 *)(c + 0xC8) * fl.z; tmp.y = *(f32 *)(c + 0xCC) * fl.x + *(f32 *)(c + 0xD0) * fl.y + *(f32 *)(c + 0xD4) * fl.z; tmp.z = *(f32 *)(c + 0xD8) * fl.x + *(f32 *)(c + 0xDC) * fl.y + *(f32 *)(c + 0xE0) * fl.z; F = tmp; axis.x *= *(f32 *)(c + 0x74); axis.y *= *(f32 *)(c + 0x74); axis.z *= *(f32 *)(c + 0x74); pt.x = pos.x + axis.x; pt.y = pos.y + axis.y; pt.z = pos.z + axis.z; tmp.x = pt.x - *(f32 *)(c + 0x98); tmp.y = pt.y - *(f32 *)(c + 0x9C); tmp.z = pt.z - *(f32 *)(c + 0xA0); tq.x = tmp.y * F.z - F.y * tmp.z; tq.y = F.x * tmp.z - tmp.x * F.z; tq.z = tmp.x * F.y - F.x * tmp.y; *(f32 *)(c + 0x130) = tq.x + *(f32 *)(c + 0x130); *(f32 *)(c + 0x134) = tq.y + *(f32 *)(c + 0x134); *(f32 *)(c + 0x138) = tq.z + *(f32 *)(c + 0x138); *(f32 *)(c + 0x124) = *(f32 *)(c + 0x124) + F.x; *(f32 *)(c + 0x128) = *(f32 *)(c + 0x128) + F.y; *(f32 *)(c + 0x12C) = *(f32 *)(c + 0x12C) + F.z; axis.x = -axis.x; axis.y = -axis.y; axis.z = -axis.z; pt.x = pos.x + axis.x; pt.y = pos.y + axis.y; pt.z = pos.z + axis.z; F.x = -F.x; F.y = -F.y; F.z = -F.z; push_954(c, &pt, &F, &tmp);
        }
    } else {
        *(u32 *)(c + 0x75C) = 0;
    }
    axis.x = *(f32 *)(c + 0xC4);
    axis.y = *(f32 *)(c + 0xD0);
    axis.z = *(f32 *)(c + 0xDC);
    d = axis.x * *(f32 *)(c + 0x720) + axis.y * *(f32 *)(c + 0x724) + axis.z * *(f32 *)(c + 0x728);
    if ((d < 0.0f ? -d : d) > 0.7071f) {
        *(u32 *)(c + 0x760) += 10;
        if (*(u32 *)(c + 0x760) > D_80138D6E) {
            f = *(f32 *)(c + 0x5EC) * *(f32 *)(c + 0x5EC) + *(f32 *)(c + 0x5F0) * *(f32 *)(c + 0x5F0);
            if (d > 0.0f) {
                f = -f;
            }
            fl.x = 0.0f;
            fl.y = 0.0f;
            fl.z = f;
            tmp.x = *(f32 *)(c + 0xC0) * fl.x + *(f32 *)(c + 0xC4) * fl.y + *(f32 *)(c + 0xC8) * fl.z; tmp.y = *(f32 *)(c + 0xCC) * fl.x + *(f32 *)(c + 0xD0) * fl.y + *(f32 *)(c + 0xD4) * fl.z; tmp.z = *(f32 *)(c + 0xD8) * fl.x + *(f32 *)(c + 0xDC) * fl.y + *(f32 *)(c + 0xE0) * fl.z; F = tmp; axis.x *= *(f32 *)(c + 0x74); axis.y *= *(f32 *)(c + 0x74); axis.z *= *(f32 *)(c + 0x74); pt.x = pos.x + axis.x; pt.y = pos.y + axis.y; pt.z = pos.z + axis.z; push_954(c, &pt, &F, &tmp); axis.x = -axis.x; axis.y = -axis.y; axis.z = -axis.z; pt.x = pos.x + axis.x; pt.y = pos.y + axis.y; pt.z = pos.z + axis.z; F.x = -F.x; F.y = -F.y; F.z = -F.z; push_954(c, &pt, &F, &tmp);
        }
    } else {
        *(u32 *)(c + 0x760) = 0;
    }
    axis.x = *(f32 *)(c + 0xC8);
    axis.y = *(f32 *)(c + 0xD4);
    axis.z = *(f32 *)(c + 0xE0);
    d = axis.x * *(f32 *)(c + 0x720) + axis.y * *(f32 *)(c + 0x724) + axis.z * *(f32 *)(c + 0x728);
    if (-d > 0.7071f) {
        if (*(u32 *)(c + 0x764) == 0) {
            rnd = &D_8014169C;
            c[0x768] = (u32)(*rnd)(100) > D_80138D70;
            if (c[0x768]) {
                c[0x769] = (*rnd)(2);
            }
        }
        *(u32 *)(c + 0x764) += 10;
        if (c[0x768]) {
            if (*(u32 *)(c + 0x764) > D_80138D6F) {
                f = *(f32 *)(c + 0x5E8) * *(f32 *)(c + 0x5E8) + *(f32 *)(c + 0x5F0) * *(f32 *)(c + 0x5F0);
                if (c[0x769]) {
                    f = -f;
                }
                fl.x = f;
                fl.y = 0.0f;
                fl.z = 0.0f;
                tmp.x = *(f32 *)(c + 0xC0) * fl.x + *(f32 *)(c + 0xC4) * fl.y + *(f32 *)(c + 0xC8) * fl.z; tmp.y = *(f32 *)(c + 0xCC) * fl.x + *(f32 *)(c + 0xD0) * fl.y + *(f32 *)(c + 0xD4) * fl.z; tmp.z = *(f32 *)(c + 0xD8) * fl.x + *(f32 *)(c + 0xDC) * fl.y + *(f32 *)(c + 0xE0) * fl.z; F = tmp; axis.x *= *(f32 *)(c + 0x74); axis.y *= *(f32 *)(c + 0x74); axis.z *= *(f32 *)(c + 0x74); pt.x = pos.x + axis.x; pt.y = pos.y + axis.y; pt.z = pos.z + axis.z; push_954(c, &pt, &F, &tmp); axis.x = -axis.x; axis.y = -axis.y; axis.z = -axis.z; pt.x = pos.x + axis.x; pt.y = pos.y + axis.y; pt.z = pos.z + axis.z; F.x = -F.x; F.y = -F.y; F.z = -F.z; push_954(c, &pt, &F, &tmp);
            }
        } else if (*(u32 *)(c + 0x764) > D_80138D71 * 10) {
            func_800961B0(c, 0, 0);
        }
    } else {
        *(u32 *)(c + 0x764) = 0;
    }
}

INCLUDE_ASM("asm/nonmatchings/unit_31E10", func_8009751C);

INCLUDE_ASM("asm/nonmatchings/unit_31E10", func_800977AC);

INCLUDE_ASM("asm/nonmatchings/unit_31E10", func_800979F4);

typedef struct { f32 x; f32 y; f32 z; } V3_B00;
typedef struct { V3_B00 n; f32 d; } P4_B00;
typedef struct { f32 cur; u8 p4[0x10]; f32 hi; f32 lo; } R_B00;
typedef struct { V3_B00 pos; f32 fC; u8 p10[4]; u8 flags; u8 p15[3]; V3_B00 dir; V3_B00 world; u8 contact; u8 c31; u8 p32[2]; V3_B00 n; V3_B00 pt; V3_B00 vel; u8 p58[8]; u8 b60; u8 p61[3]; f32 h64; f32 ang; f32 rate; } W_B00;
typedef struct { void (*a)(u8, f32); void (*b)(u8, u8, P4_B00 *); void (*c)(u8, s32); } T_B00;
extern u8 D_8011B31B;
extern void (*D_80141748)(u8, V3_B00 *, void *);
extern void (*D_8014174C)(u8, u8, f32, f32, f32, s32, s32, s32, V3_B00 *, V3_B00 *);
extern T_B00 D_80141754;
u32 func_800AEF70(void);
void func_80090410(u8, V3_B00 *, V3_B00 *, V3_B00 *, V3_B00 *, s32 *, u8 *, u8 *);
static inline f32 maxf_B00(f32 a, f32 b) { return a > b ? a : b; }
void func_80097B00(u32 dt) {
    V3_B00 d;
    V3_B00 o;
    V3_B00 v1;
    V3_B00 v2;
    V3_B00 v3;
    u8 near[8];
    P4_B00 pl;
    s32 s90;
    u8 s94;
    u8 j;
    u8 cnt;
    u8 i;
    u8 *c;
    W_B00 *wh;
    s32 hit;
    f32 h;
    f32 r;
    f32 f;
    R_B00 *rr;

    cnt = D_8011B31B;
    for (i = 0; i < cnt; i++) {
        near[i] = 0;
    }
    for (i = 0; i < func_800AEF70(); i++) {
        func_80090410(i, &o, &v1, &v2, &v3, &s90, &s94, &j);
        for (j = 0; j < cnt; j++) {
            if (near[j] == 0) {
                c = &D_80118630 + j * 0x77C;
                d.x = *(f32 *)(c + 0x98) + *(f32 *)(c + 0x8C);
                d.y = *(f32 *)(c + 0x9C) + *(f32 *)(c + 0x90);
                d.z = *(f32 *)(c + 0xA0) + *(f32 *)(c + 0x94);
                d.x -= o.x;
                d.y -= o.y;
                d.z -= o.z;
                f = ((((((d.x) < 0.0f ? -(d.x) : (d.x))) > ( ((d.y) < 0.0f ? -(d.y) : (d.y))) ? (((d.x) < 0.0f ? -(d.x) : (d.x))) : ( ((d.y) < 0.0f ? -(d.y) : (d.y))))) > ( ((d.z) < 0.0f ? -(d.z) : (d.z))) ? (((((d.x) < 0.0f ? -(d.x) : (d.x))) > ( ((d.y) < 0.0f ? -(d.y) : (d.y))) ? (((d.x) < 0.0f ? -(d.x) : (d.x))) : ( ((d.y) < 0.0f ? -(d.y) : (d.y))))) : ( ((d.z) < 0.0f ? -(d.z) : (d.z))));
                near[j] = f < 2400.0f;
            }
        }
    }
    for (i = 0; i < cnt; i++) {
        V3_B00 w;

        c = &D_80118630 + i * 0x77C;
        o.x = *(f32 *)(c + 0x98) + *(f32 *)(c + 0x8C);
        o.y = *(f32 *)(c + 0x9C) + *(f32 *)(c + 0x90);
        o.z = *(f32 *)(c + 0xA0) + *(f32 *)(c + 0x94);
        w.x = *(f32 *)(c + 0xC0) * *(f32 *)(c + 0x5DC) + *(f32 *)(c + 0xC4) * *(f32 *)(c + 0x5E0) + *(f32 *)(c + 0xC8) * *(f32 *)(c + 0x5E4);
        w.y = *(f32 *)(c + 0xCC) * *(f32 *)(c + 0x5DC) + *(f32 *)(c + 0xD0) * *(f32 *)(c + 0x5E0) + *(f32 *)(c + 0xD4) * *(f32 *)(c + 0x5E4);
        w.z = *(f32 *)(c + 0xD8) * *(f32 *)(c + 0x5DC) + *(f32 *)(c + 0xDC) * *(f32 *)(c + 0x5E0) + *(f32 *)(c + 0xE0) * *(f32 *)(c + 0x5E4);
        d = w;
        d.x = o.x - d.x;
        d.y = o.y - d.y;
        d.z = o.z - d.z;
        D_80141748(i, &d, c + 0xC0);
        rr = (R_B00 *)(c + 0x3EC);
        if (near[i]) {
            for (j = 0, wh = (W_B00 *)(c + 0x144); j < c[0x3E4]; j++, wh++) {
                d.x = wh->dir.y * wh->n.z - wh->n.y * wh->dir.z;
                d.y = wh->n.x * wh->dir.z - wh->dir.x * wh->n.z;
                d.z = wh->dir.x * wh->n.y - wh->n.x * wh->dir.y;
                hit = 0;
                if (wh->b60 && wh->contact && c[0x778] == 0) {
                    d.x *= wh->fC;
                    d.y *= wh->fC;
                    d.z *= wh->fC;
                    if (j == 0 || j == 2 || j == 4) {
                        d.x = wh->pt.x + d.x;
                        d.y = wh->pt.y + d.y;
                        d.z = wh->pt.z + d.z;
                    } else {
                        d.x = wh->pt.x - d.x;
                        d.y = wh->pt.y - d.y;
                        d.z = wh->pt.z - d.z;
                    }
                    hit = 1;
                }
                h = 0.0f;
                if (wh->flags & 2) {
                    h = *(f32 *)(c + 0x10);
                }
                if (j >= 2) {
                    h = -h;
                }
                r = wh->h64 - ((wh->flags & 1) ? *(f32 *)(c + 0x524) : *(f32 *)(c + 0x50C));
                if (c[0x13C] == 0) {
                    wh->ang += wh->rate * ((f32) dt / 1000.0f);
                }
                while (wh->ang < -314.159f) {
                    wh->ang += 314.159f;
                }
                while (wh->ang > 314.159f) {
                    wh->ang -= 314.159f;
                }
                D_8014174C(i, j, h, r, wh->ang, wh->contact, wh->c31, hit, &wh->pt, &d);
            }
        }
        f = 2.0f * ((rr->cur - rr->lo) / (rr->hi - rr->lo)) - 1.0f;
        f = maxf_B00(f, 0.0f);
        if (c[0x4B8] != 0) {
            f = 0.0f;
        }
        D_80141754.a(i, f);
        if (c[0x710] != 0) {
            pl.n = *(V3_B00 *)(c + 0x720);
            pl.d = *(f32 *)(c + 0x72C) - (pl.n.x * *(f32 *)(c + 0x714) + pl.n.y * *(f32 *)(c + 0x718) + pl.n.z * *(f32 *)(c + 0x71C));
        }
        D_80141754.b(i, c[0x710], &pl);
        D_80141754.c(i, *(s32 *)(c + 8));
        c[0x778] = 0;
    }
}

INCLUDE_ASM("asm/nonmatchings/unit_31E10", func_80098300);

INCLUDE_ASM("asm/nonmatchings/unit_31E10", func_80098584);

INCLUDE_ASM("asm/nonmatchings/unit_31E10", func_8009887C);

s32 func_80098BC0(void) {
    memset(D_800F38A0, 0, 8);
    return 1;
}


s32 func_80098BEC(void) {
    return 1;
}

s32 func_80098BF4(void) {
    u8 a;
    u8 b;
    u8 i;
    s32 n;
    void *e;
    void (**p)(s32);

    n = 0;
    for (i = 0; i < 2; i++) {
        if (((s32 *) D_800F38A0)[i] != 0) {
            ((s32 *) D_800F38A0)[i]--;
        }
    }
    for (i = 0; i < 2; i++) {
        e = func_800960D8(i);
        if (e != NULL && (((s32 *) e)[1] & 1)) {
            if (func_8008D3AC() == 1) {
                func_80095FE4(i, &a, &b);
                D_8014558C(i, a, b);
            }
            n++;
        }
    }
    p = &D_8014556C;
    (*p)(0);
    (*p)(1);
    if (n == 2) {
        if ((u8) D_80145570(0, 0xD)) {
            return 1;
        }
        if ((u8) D_80145570(1, 0xD)) {
            return 2;
        }
    } else if (n == 1) {
        if ((u8) D_80145570(0, 0xD)) {
            return 1;
        }
    }
    return 0;
}

INCLUDE_ASM("asm/nonmatchings/unit_31E10", func_80098D84);

INCLUDE_ASM("asm/nonmatchings/unit_31E10", func_80098E84);
