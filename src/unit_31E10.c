#include "common.h"
#include "include_asm.h"
typedef struct { s32 x; s32 y; s32 z; } V3i9_;
typedef struct { s32 d[0x22]; } B88_;
typedef struct { f32 x, y, z; } V3_D34;
typedef struct { f32 m[3][4]; } M_D34;
typedef struct { V3_D34 pos; u8 p0C[8]; u8 flags; u8 p15[3]; V3_D34 dir; V3_D34 world; u8 contact; u8 p31[3]; V3_D34 n; V3_D34 pt; V3_D34 vel; u8 p58[0x18]; } W_D34;
typedef struct { u8 p0[0x10]; f32 yaw; u8 p14[0x6C]; V3_D34 up; u8 p8C[0xC]; V3_D34 pos; u8 pA4[0xC]; V3_D34 vel; u8 pBC[4]; f32 rot[3][3]; u8 pE4[0x30]; V3_D34 av; u8 p120[0x24]; W_D34 w[(0x3E4 - 0x144) / 0x70]; u8 n; u8 p3E5[2]; u8 cnt; u8 cnt2; u8 p3E9[0x1F3]; V3_D34 c; } C_D34;
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

INCLUDE_ASM("asm/nonmatchings/unit_31E10", func_80091E30);

INCLUDE_ASM("asm/nonmatchings/unit_31E10", func_80092534);

INCLUDE_ASM("asm/nonmatchings/unit_31E10", func_80093164);

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

INCLUDE_ASM("asm/nonmatchings/unit_31E10", func_800961B0);

INCLUDE_ASM("asm/nonmatchings/unit_31E10", func_80096954);

INCLUDE_ASM("asm/nonmatchings/unit_31E10", func_8009751C);

INCLUDE_ASM("asm/nonmatchings/unit_31E10", func_800977AC);

INCLUDE_ASM("asm/nonmatchings/unit_31E10", func_800979F4);

INCLUDE_ASM("asm/nonmatchings/unit_31E10", func_80097B00);

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
        e = ((void * (*)(s32 arg0))func_800960D8)(i);
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
