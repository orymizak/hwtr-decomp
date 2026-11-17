#include "common.h"
#include "include_asm.h"
typedef struct { s32 w0; u8 pad[0x8C]; u8 x90[0x94]; u8 x124[0xA0]; } Ent1C4_;
typedef struct { f32 x; f32 y; f32 z; } V3a5_;
typedef struct { f32 m[3][3]; } M3a5_;
typedef struct { s32 active; u8 id; u8 idx; u8 p6[2]; u8 f8[0xC]; V3a5_ v14; f32 f20; f32 f24; f32 f28; M3a5_ m2C; u8 p50[0xC]; u16 *p5C; f32 f60; s32 w64; u8 p68[0x28]; M3a5_ m90; u8 pB4[0x24]; f32 fD8; u8 pDC[0x14]; V3a5_ vF0; V3a5_ vFC; V3a5_ v108; V3a5_ v114; u8 p120[4]; M3a5_ m124; V3a5_ v148; u8 p154[0x24]; V3a5_ v178; u8 p184[0x40]; } Prta5_;
typedef struct { f32 x; f32 y; f32 z; f32 w; f32 h; u8 p14[0x10]; f32 x2; f32 y2; f32 z2; u8 p30[0x3C]; f32 f6C; } Emta5_;
typedef struct { u8 p0[0x54]; u16 *p54; } P758a5_;
typedef struct { u8 b0; u8 p1[0x97]; V3a5_ pos98; u8 pA4[0xC]; V3a5_ vB0; u8 pBC[4]; M3a5_ rot; u8 pE4[0x30]; V3a5_ v114; u8 p120[0x24]; Emta5_ emit[1]; } Cara5_;
extern V3a5_ D_80100A00;
extern u32 (*D_8014169C)(s32);
void func_800AC880(f32 *, f32, f32, f32, f32, V3a5_);
void func_8009A4E4(void *);
f32 sqrtf(f32);
void func_800ACDC8(void *, f32);
void func_8007A9E0(void *);

s32 func_8009A770(s32 *);
extern s32 D_8011E870;

s32 func_800A58D0(void) {
    u8 *e = (u8 *)&D_8011E870;
    u32 i;

    memset(e, 0, 0xE20);
    for (i = 0; i < 8; i++) {
        *(s32 *)e = 0;
        *(u8 **)(e + 0x68) = e + 0x90;
        *(u8 **)(e + 0x54) = e + 0x1A8;
        *(u8 **)(e + 0x58) = e + 0x1B4;
        *(u8 *)(e + 0x50) = 1;
        *(u8 **)(e + 0x5C) = e + 0x1C0;
        e += 0x1C4;
    }
    return 1;
}


s32 func_800A5950(void) {
    Ent1C4_ *e = (Ent1C4_ *)&D_8011E870;
    u32 i;

    for (i = 0; i < 8; i++, e++) {
        if (e->w0 == 1) {
            func_8009A770((s32 *)((u8 *)e + 8));
            e->w0 = 0;
        }
    }
    return 1;
}


void func_800A59B8(void) {
    Ent1C4_ *e = (Ent1C4_ *)&D_8011E870;
    u32 i;

    for (i = 0; i < 8; i++, e++) {
        if (e->w0 != 0) {
            func_800ACDC8(e->x90, 0.01f);
            func_8007A9E0(e->x124);
        }
    }
}


INCLUDE_ASM("asm/nonmatchings/unit_464D0", func_800A5A1C);

void func_800A5AB0(Cara5_ *c) {
    V3a5_ d;
    V3a5_ col;
    V3a5_ t;
    V3a5_ cr;
    M3a5_ tr;
    M3a5_ rm;
    M3a5_ r;
    Prta5_ *p;
    Emta5_ *e;
    u16 *src;
    u16 *dst;
    u32 i;
    u32 j;
    f32 len;
    f32 h;

    col = D_80100A00;
    e = c->emit;
    for (i = 0; i < *((u8 *)c + 0x3E4); e++, i++) {
        p = ((Prta5_ *)&D_8011E870);
        for (j = 0; j < 8; j++, p++) {
            if (p->active == 0) {
                break;
            }
        }
        if (j == 8) {
            return;
        }
        p->active = 1;
        p->id = c->b0;
        p->idx = i;
        h = 2.0f * e->h;
        func_800AC880(p->m90.m[0], 0.51813471f, e->w / 2.0f, h, h, col);
        p->m124.m[0][0] = c->rot.m[0][0];
        p->m124.m[0][1] = c->rot.m[0][1];
        p->m124.m[0][2] = c->rot.m[0][2];
        p->m124.m[1][0] = c->rot.m[1][0];
        p->m124.m[1][1] = c->rot.m[1][1];
        p->m124.m[1][2] = c->rot.m[1][2];
        p->m124.m[2][0] = c->rot.m[2][0];
        p->m124.m[2][1] = c->rot.m[2][1];
        p->m124.m[2][2] = c->rot.m[2][2];
        d.x = e->x - *(f32 *)((u8 *)c + 0x5DC);
        d.y = e->y - *(f32 *)((u8 *)c + 0x5E0);
        d.z = e->z - *(f32 *)((u8 *)c + 0x5E4);
        t.x = c->rot.m[0][0] * d.x + c->rot.m[0][1] * d.y + c->rot.m[0][2] * d.z;
        t.y = c->rot.m[1][0] * d.x + c->rot.m[1][1] * d.y + c->rot.m[1][2] * d.z;
        t.z = c->rot.m[2][0] * d.x + c->rot.m[2][1] * d.y + c->rot.m[2][2] * d.z;
        p->vFC = t;
        p->vFC.x += c->pos98.x;
        p->vFC.y += c->pos98.y;
        p->vFC.z += c->pos98.z;
        d.x = e->x2 - c->pos98.x;
        d.y = e->y2 - c->pos98.y;
        d.z = e->z2 - c->pos98.z;
        len = sqrtf(d.x * d.x + d.y * d.y + d.z * d.z);
        len = 1.0f / len;
        d.x *= len;
        d.y *= len;
        d.z *= len;
        d.x *= (f32)D_8014169C(20) * 17.6f;
        d.y *= (f32)D_8014169C(20) * 17.6f;
        d.z *= (f32)D_8014169C(20) * 17.6f;
        d.z = (f32)D_8014169C(30) * 17.6f;
        t.x = e->x2 - c->pos98.x;
        t.y = e->y2 - c->pos98.y;
        t.z = e->z2 - c->pos98.z;
        cr.x = c->v114.y * t.z - t.y * c->v114.z;
        cr.y = t.x * c->v114.z - c->v114.x * t.z;
        cr.z = c->v114.x * t.y - t.x * c->v114.y;
        p->v114.x = c->vB0.x + cr.x;
        p->v114.y = c->vB0.y + cr.y;
        p->v114.z = c->vB0.z + cr.z;
        p->v114.x += d.x;
        p->v114.y += d.y;
        p->v114.z += d.z;
        p->v108.x = p->v114.x * p->fD8;
        p->v108.y = p->v114.y * p->fD8;
        p->v108.z = p->v114.z * p->fD8;
        p->v178.x = c->rot.m[0][0];
        p->v178.y = c->rot.m[1][0];
        p->v178.z = c->rot.m[2][0];
        p->v178.x *= e->f6C;
        p->v178.y *= e->f6C;
        p->v178.z *= e->f6C;
        tr.m[0][0] = p->m124.m[0][0];
        tr.m[0][1] = p->m124.m[1][0];
        tr.m[0][2] = p->m124.m[2][0];
        tr.m[1][0] = p->m124.m[0][1];
        tr.m[1][1] = p->m124.m[1][1];
        tr.m[1][2] = p->m124.m[2][1];
        tr.m[2][0] = p->m124.m[0][2];
        tr.m[2][1] = p->m124.m[1][2];
        tr.m[2][2] = p->m124.m[2][2];
        rm.m[0][0] = p->m124.m[0][0] * p->m90.m[0][0] + p->m124.m[0][1] * p->m90.m[1][0] + p->m124.m[0][2] * p->m90.m[2][0];
        rm.m[0][1] = p->m124.m[0][0] * p->m90.m[0][1] + p->m124.m[0][1] * p->m90.m[1][1] + p->m124.m[0][2] * p->m90.m[2][1];
        rm.m[0][2] = p->m124.m[0][0] * p->m90.m[0][2] + p->m124.m[0][1] * p->m90.m[1][2] + p->m124.m[0][2] * p->m90.m[2][2];
        rm.m[1][0] = p->m124.m[1][0] * p->m90.m[0][0] + p->m124.m[1][1] * p->m90.m[1][0] + p->m124.m[1][2] * p->m90.m[2][0];
        rm.m[1][1] = p->m124.m[1][0] * p->m90.m[0][1] + p->m124.m[1][1] * p->m90.m[1][1] + p->m124.m[1][2] * p->m90.m[2][1];
        rm.m[1][2] = p->m124.m[1][0] * p->m90.m[0][2] + p->m124.m[1][1] * p->m90.m[1][2] + p->m124.m[1][2] * p->m90.m[2][2];
        rm.m[2][0] = p->m124.m[2][0] * p->m90.m[0][0] + p->m124.m[2][1] * p->m90.m[1][0] + p->m124.m[2][2] * p->m90.m[2][0];
        rm.m[2][1] = p->m124.m[2][0] * p->m90.m[0][1] + p->m124.m[2][1] * p->m90.m[1][1] + p->m124.m[2][2] * p->m90.m[2][1];
        rm.m[2][2] = p->m124.m[2][0] * p->m90.m[0][2] + p->m124.m[2][1] * p->m90.m[1][2] + p->m124.m[2][2] * p->m90.m[2][2];
        r.m[0][0] = rm.m[0][0] * tr.m[0][0] + rm.m[0][1] * tr.m[1][0] + rm.m[0][2] * tr.m[2][0];
        r.m[0][1] = rm.m[0][0] * tr.m[0][1] + rm.m[0][1] * tr.m[1][1] + rm.m[0][2] * tr.m[2][1];
        r.m[0][2] = rm.m[0][0] * tr.m[0][2] + rm.m[0][1] * tr.m[1][2] + rm.m[0][2] * tr.m[2][2];
        r.m[1][0] = rm.m[1][0] * tr.m[0][0] + rm.m[1][1] * tr.m[1][0] + rm.m[1][2] * tr.m[2][0];
        r.m[1][1] = rm.m[1][0] * tr.m[0][1] + rm.m[1][1] * tr.m[1][1] + rm.m[1][2] * tr.m[2][1];
        r.m[1][2] = rm.m[1][0] * tr.m[0][2] + rm.m[1][1] * tr.m[1][2] + rm.m[1][2] * tr.m[2][2];
        r.m[2][0] = rm.m[2][0] * tr.m[0][0] + rm.m[2][1] * tr.m[1][0] + rm.m[2][2] * tr.m[2][0];
        r.m[2][1] = rm.m[2][0] * tr.m[0][1] + rm.m[2][1] * tr.m[1][1] + rm.m[2][2] * tr.m[2][1];
        r.m[2][2] = rm.m[2][0] * tr.m[0][2] + rm.m[2][1] * tr.m[1][2] + rm.m[2][2] * tr.m[2][2];
        t.x = r.m[0][0] * p->v178.x + r.m[0][1] * p->v178.y + r.m[0][2] * p->v178.z;
        t.y = r.m[1][0] * p->v178.x + r.m[1][1] * p->v178.y + r.m[1][2] * p->v178.z;
        t.z = r.m[2][0] * p->v178.x + r.m[2][1] * p->v178.y + r.m[2][2] * p->v178.z;
        p->v148 = t;
        p->w64 = 6;
        dst = p->p5C;
        src = (*(P758a5_ **)((u8 *)c + 0x758))->p54;
        if (*(s32 *)((u8 *)c + 0x6D8) != 1) {
            src += i;
        }
        *dst = *src;
        p->m2C.m[0][0] = p->m124.m[0][0];
        p->m2C.m[0][1] = p->m124.m[0][1];
        p->m2C.m[0][2] = p->m124.m[0][2];
        p->m2C.m[1][0] = p->m124.m[1][0];
        p->m2C.m[1][1] = p->m124.m[1][1];
        p->m2C.m[1][2] = p->m124.m[1][2];
        p->m2C.m[2][0] = p->m124.m[2][0];
        p->m2C.m[2][1] = p->m124.m[2][1];
        p->m2C.m[2][2] = p->m124.m[2][2];
        p->v14.x = p->vFC.x + p->vF0.x;
        p->v14.y = p->vFC.y + p->vF0.y;
        p->v14.z = p->vFC.z + p->vF0.z;
        p->f20 = e->w * 0.5f;
        p->f24 = e->h * 0.5f;
        p->f28 = e->h * 0.5f;
        p->f60 = e->h * 0.5f;
        func_8009A4E4(p->f8);
    }
}

void func_800A6630(void *arg0) {
  s32 *var_s0;
  unsigned int new_var;
  s32 *var_s1;
  u32 var_s2;
  var_s1 = &D_8011E870;
  var_s2 = 0;
  new_var = 1;
  var_s0 = ((u8 *) (&D_8011E870)) + 8;
  do
  {
    if (((*var_s1) == new_var) && ((*((u8 *) (((u8 *) var_s0) - 0x4))) == (*((u8 *) arg0)))) {
      func_8009A770(var_s0);
      *var_s1 = 0;
    }
    var_s0 += 113;
    var_s2 += 1;
    var_s1 += 113;
  }
  while (var_s2 < 8U);
}

INCLUDE_ASM("asm/nonmatchings/unit_464D0", func_800A66C0);

INCLUDE_ASM("asm/nonmatchings/unit_464D0", func_800A6850);

INCLUDE_ASM("asm/nonmatchings/unit_464D0", func_800A6BA4);

INCLUDE_ASM("asm/nonmatchings/unit_464D0", func_800A6D98);
