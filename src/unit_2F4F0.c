#include "common.h"
#include "include_asm.h"
typedef struct { s32 a; s32 b; s32 c; } V12_;
typedef struct { f32 x; f32 y; f32 z; } V3f_;
extern V12_ D_8011855C;
extern V12_ D_80118568;
extern f32 D_80118578[3][3];
typedef struct { s32 w; u8 pad[0x60]; } Ent64w_;
extern Ent64w_ D_801185B8[];
typedef struct { u8 on; u8 pad[0x63]; } Ent64_;
extern Ent64_ D_801185B4[];
extern u8 D_80118620;
typedef struct { s32 x; s32 y; s32 z; } Pos_;
typedef struct { f32 x; f32 y; f32 z; } Vec3f_;
typedef struct { u8 p0[0x44]; s32 mode; u8 pt; u8 car; u8 p4A[2]; u8 b4C; u8 p4D[3]; s32 timer; } Cam90_;
extern u8 (*D_80141718)(void);
extern void (*D_8014171C)(u8, Vec3f_ *, void *, s32 *, s32 *);
extern u8 (*D_8014169C)(s32);
extern u8 D_80118621;
extern s32 *D_80118624;
s32 func_80096100(void);
u8 *func_800960D8(u8);
f32 sqrtf(f32);
extern u8 D_80118558[];
s32 func_800A5894(void);
extern u8 D_801185AC;
extern u8 D_801185A4;
extern u8 D_801185A2;



INCLUDE_ASM("asm/nonmatchings/unit_2F4F0", func_8008E8F0);

s32 func_8008EA64(void) {
    return 1;
}

INCLUDE_ASM("asm/nonmatchings/unit_2F4F0", func_8008EA6C);

INCLUDE_ASM("asm/nonmatchings/unit_2F4F0", func_8009013C);

void func_80090410(u8 i, Pos_ *a1, f32 *a2, f32 *a3, f32 *a4, f32 *a5, s8 *a6, u8 *a7) {
    u8 *p = D_80118558 + i * 0x64;

    *a1 = *(Pos_ *)(p + 4);
    a2[0] = *(f32 *)(p + 0x1C);
    a2[1] = *(f32 *)(p + 0x28);
    a2[2] = *(f32 *)(p + 0x34);
    a3[0] = *(f32 *)(p + 0x20);
    a3[1] = *(f32 *)(p + 0x2C);
    a3[2] = *(f32 *)(p + 0x38);
    a4[0] = *(f32 *)(p + 0x24);
    a4[1] = *(f32 *)(p + 0x30);
    a4[2] = *(f32 *)(p + 0x3C);
    *a5 = *(f32 *)(p + 0x40);
    *a6 = *(s32 *)(p + 0x44) == 0;
    *a7 = p[0x49];
}

void func_800904C4(V12_ *a, V12_ *b, V3f_ *c, V3f_ *d) {
    *a = D_8011855C;
    *b = D_80118568;
    c->x = D_80118578[0][0];
    c->y = D_80118578[1][0];
    c->z = D_80118578[2][0];
    d->x = D_80118578[0][1];
    d->y = D_80118578[1][1];
    d->z = D_80118578[2][1];
}


void func_80090550(s32 arg0, s8 arg1) {
    *(u8 *)(&D_801185A2 + ((arg0 & 0xFF) * 0x64)) = arg1;
}

void func_80090578(Cam90_ *cam, u32 dt) {
    Vec3f_ pos;
    Vec3f_ p;
    Vec3f_ d;
    u8 b[0x28];
    s32 flags;
    s32 x;
    f32 radius;
    f32 mind;
    f32 dist;
    u32 i;
    u32 j;
    u32 count;
    u32 best;
    u32 bestCount;
    u32 bestPt;
    u32 bestCar;
    u8 *car;
    s32 v;
    s32 w;

    best = 0;
    radius = 0.0f;
    bestCar = 0;
    if (cam->timer < dt) {
        bestCount = 0;
        bestPt = 0;
        for (i = 0; i < D_80141718(); i++) {
            D_8014171C(i, &pos, b, &flags, &x);
            flags |= 1;
            if (flags & 2) {
                continue;
            }
            count = 0;
            if (flags & 1) {
                p = pos;
                radius = 5400.0f;
            } else {
                radius = 0.0f;
            }
            mind = 0.0f;
            for (j = 0; j < func_80096100(); j++) {
                car = func_800960D8(j);
                d.x = p.x - *(f32 *)(car + 0x98);
                d.y = p.y - *(f32 *)(car + 0x9C);
                d.z = p.z - *(f32 *)(car + 0xA0);
                dist = sqrtf(d.x * d.x + d.y * d.y + d.z * d.z);
                if (dist < radius && 0.0f < d.x * *(f32 *)(car + 0xB0) + d.y * *(f32 *)(car + 0xB4) + d.z * *(f32 *)(car + 0xB8)) {
                    count++;
                    if (mind < dist) {
                        mind = dist;
                        best = j;
                    }
                }
            }
            if (bestCount < count) {
                bestCount = count;
                bestPt = i;
                bestCar = best;
            }
        }
        if (bestCount != 0) {
            v = flags;
            cam->car = bestCar;
            cam->pt = bestPt;
            cam->mode = (-(v & 1) & 3) | 2;
        } else {
            do {
                cam->pt = D_8014169C(D_80118621);
            } while ((w = D_80118624[cam->pt * 5]) == 0);
            cam->mode = w;
            for (j = 0; j < 10; j++) {
                cam->car = D_8014169C(func_80096100());
                if (func_800960D8(cam->car)[0x4B8] == 0) {
                    break;
                }
            }
        }
        cam->b4C = 1;
        cam->timer = 4000;
    }
    cam->timer -= dt;
}

void func_800908A0(void) {

}

s32 func_800908A8(void) {
  s32 temp_v0;
  temp_v0 = func_800A5894();
  if (temp_v0 != 0) {
    if (temp_v0) {
      return (temp_v0 - 1) * 0xC8;
    }
    else
    {
      return (temp_v0 - 1) * 0xC8;
    }
  }
  return 0;
}

void func_800908E8(void) {
    u32 i;

    for (i = 0; i < D_80118620; i++) {
        D_801185B4[i].on = 1;
    }
}


void func_80090934(void) {
    u32 i;

    for (i = 0; i < D_80118620; i++) {
        if (D_801185B4[i].on != 0) {
            D_801185B8[i].w = 5000;
            D_801185B4[i].on = 0;
        }
    }
}


INCLUDE_ASM("asm/nonmatchings/unit_2F4F0", func_8009099C);

INCLUDE_ASM("asm/nonmatchings/unit_2F4F0", func_80090D80);

INCLUDE_ASM("asm/nonmatchings/unit_2F4F0", func_80090FA8);

void func_800911B4(s32 arg0) {
    *(u8 *)(&D_801185A4 + ((arg0 & 0xFF) * 0x64)) = 1;
}

void func_800911E0(s32 arg0) {
    *(s32 *)(&D_801185AC + ((arg0 & 0xFF) * 0x64)) = 0xFA;
}
