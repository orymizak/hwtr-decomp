#include "common.h"
#include "ultra64.h"
#include "include_asm.h"
#include "car.h"
typedef struct { s32 w0; u8 pad[0x8C]; u8 x90[0x94]; u8 x124[0xA0]; } Ent1C4_;
typedef Vec3f V3a5_;
typedef Mtx3f M3a5_;
typedef struct { s32 active; u8 id; u8 idx; u8 p6[2]; u8 f8[0xC]; V3a5_ v14; f32 f20; f32 f24; f32 f28; M3a5_ m2C; u8 p50[0xC]; u16 *p5C; f32 f60; s32 w64; u8 p68[0x28]; M3a5_ m90; u8 pB4[0x24]; f32 fD8; u8 pDC[0x14]; V3a5_ vF0; V3a5_ vFC; V3a5_ v108; V3a5_ v114; u8 p120[4]; M3a5_ m124; V3a5_ v148; u8 p154[0x24]; V3a5_ v178; u8 p184[0x40]; } Prta5_;
typedef struct { u8 p0[0x54]; u16 *p54; } P758a5_;
extern V3a5_ D_80100A00;
extern u32 (*D_8014169C)(s32);
void func_800AC880(f32 *, f32, f32, f32, f32, V3a5_);
void func_8009A4E4(void *);
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

void func_800A5AB0(Car *c) {
    V3a5_ d;
    V3a5_ col;
    V3a5_ t;
    V3a5_ cr;
    M3a5_ tr;
    M3a5_ rm;
    M3a5_ r;
    Prta5_ *p;
    CarPoint *e;
    u16 *src;
    u16 *dst;
    u32 i;
    u32 j;
    f32 len;
    f32 h;

    col = D_80100A00;
    e = c->points;
    for (i = 0; i < c->nPoints; e++, i++) {
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
        p->id = c->id;
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
        d.x = e->pos.x - c->center.x;
        d.y = e->pos.y - c->center.y;
        d.z = e->pos.z - c->center.z;
        t.x = c->rot.m[0][0] * d.x + c->rot.m[0][1] * d.y + c->rot.m[0][2] * d.z;
        t.y = c->rot.m[1][0] * d.x + c->rot.m[1][1] * d.y + c->rot.m[1][2] * d.z;
        t.z = c->rot.m[2][0] * d.x + c->rot.m[2][1] * d.y + c->rot.m[2][2] * d.z;
        p->vFC = t;
        p->vFC.x += c->pos.x;
        p->vFC.y += c->pos.y;
        p->vFC.z += c->pos.z;
        d.x = e->world.x - c->pos.x;
        d.y = e->world.y - c->pos.y;
        d.z = e->world.z - c->pos.z;
        len = sqrtf(d.x * d.x + d.y * d.y + d.z * d.z);
        len = 1.0f / len;
        d.x *= len;
        d.y *= len;
        d.z *= len;
        d.x *= (f32)D_8014169C(20) * 17.6f;
        d.y *= (f32)D_8014169C(20) * 17.6f;
        d.z *= (f32)D_8014169C(20) * 17.6f;
        d.z = (f32)D_8014169C(30) * 17.6f;
        t.x = e->world.x - c->pos.x;
        t.y = e->world.y - c->pos.y;
        t.z = e->world.z - c->pos.z;
        cr.x = c->angVel.y * t.z - t.y * c->angVel.z;
        cr.y = t.x * c->angVel.z - c->angVel.x * t.z;
        cr.z = c->angVel.x * t.y - t.x * c->angVel.y;
        p->v114.x = c->vel.x + cr.x;
        p->v114.y = c->vel.y + cr.y;
        p->v114.z = c->vel.z + cr.z;
        p->v114.x += d.x;
        p->v114.y += d.y;
        p->v114.z += d.z;
        p->v108.x = p->v114.x * p->fD8;
        p->v108.y = p->v114.y * p->fD8;
        p->v108.z = p->v114.z * p->fD8;
        p->v178.x = c->rot.m[0][0];
        p->v178.y = c->rot.m[1][0];
        p->v178.z = c->rot.m[2][0];
        p->v178.x *= e->rate;
        p->v178.y *= e->rate;
        p->v178.z *= e->rate;
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
        src = c->roomObj->rooms;
        if (c->state != 1) {
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
    Ent1C4_ *e = (Ent1C4_ *)&D_8011E870;
    u32 i;

    for (i = 0; i < 8; e++, i++) {
        if (e->w0 == 1 && ((u8 *)e)[4] == *(u8 *)arg0) {
            func_8009A770((s32 *)((u8 *)e + 8));
            e->w0 = 0;
        }
    }
}

#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define MAX(a, b) ((a) > (b) ? (a) : (b))
typedef struct { u8 p0[0x64]; Car *car; } ContactObj_;
typedef struct { ContactObj_ *obj; u8 p4[0xC]; Vec3f n; } Contact_;
void func_800961B0(Car *p, u8 id, u8 flip);
/* Crashes the car when the impact speed along the contact normal exceeds a threshold that grows with
 * its k53C factor (clamped to [0.25, 4]) and the normal's alignment with the car's forward axis. */
void func_800A66C0(Contact_ *h) {
    Car *car;
    Vec3f fwd;
    f32 s;
    f32 d;
    f32 speed;
    f32 limit;
    f32 nx;
    f32 ny;
    f32 nz;

    car = h->obj->car;
    s = MIN(MAX(car->k53C, 0.25f), 4.0f);
    fwd.x = car->rot.m[0][2];
    fwd.y = car->rot.m[1][2];
    fwd.z = car->rot.m[2][2];
    nx = h->n.x;
    ny = h->n.y;
    nz = h->n.z;
    d = nx * fwd.x + ny * fwd.y + nz * fwd.z;
    d = MAX(d, 0.0f);
    s += d;
    speed = -(car->vel.x * nx + car->vel.y * ny + car->vel.z * nz);
    if (s < 1.0f) {
        limit = (s - 0.25f) * 938.6667f + -704.0f + 2112.0f;
    } else {
        limit = (s - 1.0f) * 821.3333f + 0.0f + 2112.0f;
    }
    if (limit < speed) {
        func_800961B0(car, 0, 0);
    }
}

INCLUDE_ASM("asm/nonmatchings/unit_464D0", func_800A6850);

typedef struct { u16 kind; u8 id; u8 idx; s8 rot[6]; s16 pos[3]; } PrtRec_;
/* Packs the active parts into 16-byte records (two rotation rows as s8, position as s16); returns the
 * bytes written, 0 if out (size bytes) is too small. */
u32 func_800A6BA4(PrtRec_ *out, u32 size) {
    u32 n;
    Prta5_ *e;
    u32 i;
    PrtRec_ *r;
    s32 v;

    n = 0;
    for (e = (Prta5_ *)&D_8011E870, i = 0; i < 8; e++, i++) {
        if (e->active == 0) {
            continue;
        }
        n += sizeof(PrtRec_);
        if (size < n) {
            return 0;
        }
        r = out++;
        r->kind = e->active;
        v = e->m124.m[0][0] * 127.0f;
        if (v < -127) {
            v = -127;
        }
        if (v > 127) {
            v = 127;
        }
        r->rot[0] = v;
        v = e->m124.m[0][1] * 127.0f;
        if (v < -127) {
            v = -127;
        }
        if (v > 127) {
            v = 127;
        }
        r->rot[1] = v;
        v = e->m124.m[0][2] * 127.0f;
        if (v < -127) {
            v = -127;
        }
        if (v > 127) {
            v = 127;
        }
        r->rot[2] = v;
        v = e->m124.m[1][0] * 127.0f;
        if (v < -127) {
            v = -127;
        }
        if (v > 127) {
            v = 127;
        }
        r->rot[3] = v;
        v = e->m124.m[1][1] * 127.0f;
        if (v < -127) {
            v = -127;
        }
        if (v > 127) {
            v = 127;
        }
        r->rot[4] = v;
        v = e->m124.m[1][2] * 127.0f;
        if (v < -127) {
            v = -127;
        }
        if (v > 127) {
            v = 127;
        }
        r->rot[5] = v;
        r->pos[0] = e->vFC.x;
        r->pos[1] = e->vFC.y;
        r->pos[2] = e->vFC.z;
        if (e->active == 1) {
            r->id = e->id;
            r->idx = e->idx;
        }
    }
    if (n == 0) {
        return (size >= 4) ? 4 : 0;
    }
    return n;
}

INCLUDE_ASM("asm/nonmatchings/unit_464D0", func_800A6D98);
