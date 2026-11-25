#include "common.h"
#include "math.h"
#include "race.h"
#include "include_asm.h"
typedef struct { s32 a; s32 b; s32 c; } V12_;
typedef struct { u8 busy; u8 pad[3]; V12_ v; u8 pad2[0x28]; } Q817_;
typedef struct { u8 busy; u8 pad[3]; V12_ v; f32 c; f32 d; u16 h18; u16 h1A; u8 pad2[0x18]; } QAD_;
typedef struct { V12_ pos; f32 c; f32 d; u8 p14[4]; s32 x18; f32 x1C; u8 p20[0x30]; s32 x50; V12_ p54; V12_ p60; f32 f6C; f32 f70; f32 f74; f32 f78; u16 id; u8 p7E[0xA]; } TrAD_;
extern u16 D_80114814[];
u8 Bld_ReadNode(Q817_ *, u16);

s32 Memory_Free(s32);
extern s32 D_80114800;
extern s32 gBldLapLength;
typedef struct { u16 n0; u16 n; u16 ids[6]; u8 *p20; u8 *recs; } WpTbl81_;
s32 Bld_Free(void);

/* .BLD header: magic 0xDF00, lap length, then the waypoint table (D_80114810), all little-endian. */
typedef struct { u16 magic; u8 pad2[2]; s32 lapLength; WpTbl81_ t; } BldHdr_;
extern u16 D_80114808;
#define gBldHdr (*(BldHdr_ *)&D_80114808)
extern char D_800FFA20[];
extern char D_800FFA28[];
extern s32 (*svcFileGetSize)(char *);
extern s32 (*svcFileLoad)(char *, void *);
void *Memory_Alloc(s32);
s32 sprintf(char *, const char *, ...);

/* Loads the track's .BLD file (named after the subtitle, or the track name and number); returns 0 if
 * the file is short. */
s32 Bld_Load(RaceSetup *setup) {
    char name[0x100];
    u8 *d;
    s32 size;
    s32 i;
    u16 *ids;

    D_80114800 = 0;
    if (setup->flags & 0x100) {
        sprintf(name, D_800FFA20, setup->sub);
    } else {
        sprintf(name, D_800FFA28, setup->name, setup->track);
    }
    size = svcFileGetSize(name);
    if (size == 0) {
        return 1;
    }
    D_80114800 = (s32)Memory_Alloc(size);
    if (svcFileLoad(name, (void *)D_80114800) != size) {
        return 0;
    }
    d = (u8 *)D_80114800;
    gBldHdr.magic = d[0] | (d[1] << 8);
    gBldHdr.lapLength = d[2] | (d[3] << 8) | (d[4] << 16) | (d[5] << 24);
    gBldHdr.t.n0 = d[6] | (d[7] << 8);
    gBldHdr.t.n = d[8] | (d[9] << 8);
    for (i = 0, ids = gBldHdr.t.ids; i < 6; i++) {
        ids[i] = d[0xA + i * 2] | (d[0xB + i * 2] << 8);
    }
    if (gBldHdr.magic != 0xDF00) {
        Bld_Free();
        return 1;
    }
    gBldHdr.t.p20 = (u8 *)D_80114800 + 0x16;
    gBldHdr.t.recs = gBldHdr.t.p20 + gBldHdr.t.n0;
    return 1;
}

s32 Bld_Free(void) {
    if (D_80114800 != 0) {
        Memory_Free(D_80114800);
    }
    D_80114800 = 0;
    return 1;
}

extern u8 *D_80114820;
u8 Bld_ReadNode(Q817_ *q, u16 off) {
    u8 *p;
    u8 *o;
    u8 i;
    u8 j;
    u8 *d;
    s32 v;

    o = (u8 *)q;
    p = D_80114820 + off;
    o[0] = *p++;
    switch (o[0]) {
    case 0:
        v = p[0] | (p[1] << 8) | (p[2] << 16) | (p[3] << 24);
        *(f32 *)(o + 4) = v / 4096.0f;
        p += 4;
        v = p[0] | (p[1] << 8) | (p[2] << 16) | (p[3] << 24);
        *(f32 *)(o + 8) = v / 4096.0f;
        p += 4;
        v = p[0] | (p[1] << 8) | (p[2] << 16) | (p[3] << 24);
        *(f32 *)(o + 0xC) = v / 4096.0f;
        p += 4;
        *(f32 *)(o + 0x10) = *p++;
        *(f32 *)(o + 0x14) = *p++;
        *(u16 *)(o + 0x18) = p[0] | (p[1] << 8);
        p += 2;
        *(u16 *)(o + 0x1A) = p[0] | (p[1] << 8);
        return 19;
    case 1:
        *(f32 *)(o + 4) = (s16)(p[0] | (p[1] << 8)) / 32.0f;
        p += 2;
        *(f32 *)(o + 8) = (s16)(p[0] | (p[1] << 8)) / 32.0f;
        p += 2;
        *(f32 *)(o + 0xC) = (s16)(p[0] | (p[1] << 8)) / 32.0f;
        p += 2;
        *(f32 *)(o + 0x10) = p[0];
        *(f32 *)(o + 0x14) = p[1];
        return 9;
    case 2:
        *(u16 *)(o + 4) = p[0] | (p[1] << 8);
        return 3;
    case 3:
        o[4] = *p++;
        for (i = 0; i < o[4]; i++) {
            ((s32 *)(o + 8))[i] = p[0] | (p[1] << 8) | (p[2] << 16) | (p[3] << 24);
            p += 4;
        }
        for (i = 0; i < o[4]; i++) {
            ((u16 *)(o + 0x10))[i] = p[0] | (p[1] << 8);
            p += 2;
        }
        for (i = 0; i < o[4]; i++) {
            for (j = 0; j < 16; j++) {
                /* TODO(match-debt): integer address math keeps GCC's operand order (i * 16 + o). */
                d = (u8 *)(i * 16 + (s32)o) + 0x14;
                d[j] = *p++;
            }
        }
        return o[4] * 4 + 2 + o[4] * 2 + o[4] * 16;
    case 4:
        o[4] = *p;
        return 2;
    }
    return 0;
}

s32 Bld_InitRacer(TrAD_ *t, u8 i) {
    QAD_ q;
    u8 n;

    if (D_80114800 == 0) {
        return 0;
    }
    memset(t, 0, sizeof(TrAD_));
    t->id = D_80114814[i];
    t->x50 = -1;
    n = Bld_ReadNode((Q817_ *)&q, t->id);
    if (n == 0) {
        return 0;
    }
    if (q.busy != 0) {
        return 0;
    }
    t->pos = q.v;
    t->c = q.c;
    t->d = q.d;
    t->x18 = q.h18;
    t->x1C = q.h1A * 0.1f;
    t->id += n;
    t->p54 = t->pos;
    t->p60 = t->pos;
    t->f6C = t->c;
    t->f70 = t->d;
    t->f74 = t->c;
    t->f78 = t->d;
    return 1;
}

typedef struct { u8 kind; u8 p1[3]; Vec3f v; f32 f30; f32 f34; u16 h38; u16 h3A; } RecA_;
typedef struct { u8 kind; u8 p1[3]; u16 h4; } RecB_;
typedef struct { u8 kind; u8 p1[3]; u8 b4; u8 p5[3]; s32 w[2]; u16 h[2]; u8 nm[2][16]; } RecD_;
typedef union { RecA_ a; RecB_ b; RecD_ d; } RecC_;
typedef struct { Vec3f pos; f32 c; f32 d10; f32 ang; s32 flags; f32 f1C; f32 f20; s32 n; s32 w[2]; u8 nm[2][16]; s32 sel; Vec3f p0; Vec3f p1; f32 a0; f32 b0; f32 a1; f32 b1; u16 idx; u8 p7E[2]; f32 dur; f32 time; } TrkC_;
extern u8 D_800F26A0;
f32 Math_Acos(f32);
Vec3f Vec3_Lerp(f32 t, Vec3f a, Vec3f b);

s32 Bld_StepRacer(TrkC_ *a, f32 dt, u8 flag, Vec3f n) {
    RecC_ rec;
    Vec3f old;
    Vec3f d;
    Vec3f e;
    Vec3f t;
    Vec3f v;
    u8 k;
    u8 i;
    u8 j;
    f32 l1;
    f32 l2;
    f32 c;
    f32 f;
    f32 s;
    f32 s2;
    f32 dts;
    f32 i1;
    f32 i2;

    if (D_80114800 == 0) {
        return 0;
    }
    if (a->dur < a->time + dt) {
        do {
            k = Bld_ReadNode((Q817_ *)&rec, a->idx);
            switch (rec.a.kind) {
            case 2:
                a->idx = rec.b.h4;
                break;
            case 3:
                if (a->sel == -1) {
                    a->n = rec.d.b4;
                    for (i = 0; i < rec.d.b4; i++) {
                        a->w[i] = rec.d.w[i];
                        for (j = 0; j < 16; j++) {
                            a->nm[i][j] = rec.d.nm[i][j];
                        }
                    }
                    return 0;
                }
                a->idx = rec.d.h[a->sel];
                a->sel = -1;
                break;
            case 0:
                a->idx += k;
                break;
            case 1:
                a->idx += k;
                break;
            case 4:
                a->idx += k;
                a->f20 = rec.d.b4 * (a->dur / 1000.0f);
                a->f20 = a->f20 - (a->time - a->dur) / 1000.0f;
                break;
            }
        } while (rec.a.kind >= 2);
        a->time -= a->dur;
        a->dur = 100.0f;
        old = a->p0;
        a->p0 = a->p1;
        if (rec.a.kind == 0) {
            a->p1 = rec.a.v;
            a->flags = rec.a.h38;
            a->f1C = rec.a.h3A * (a->dur / 1000.0f);
            a->f1C = a->f1C - a->time / 1000.0f;
            a->a0 = a->a1;
            a->b0 = a->b1;
        } else {
            a->p1.x = a->p0.x + rec.a.v.x;
            a->p1.y = a->p0.y + rec.a.v.y;
            a->p1.z = a->p0.z + rec.a.v.z;
            a->a0 = a->a1;
            a->b0 = a->b1;
        }
        if (rec.a.kind == 0) {
            a->a1 = rec.a.f30;
            a->b1 = rec.a.f34;
        } else {
            a->a1 = rec.a.f30;
            a->b1 = rec.a.f34;
        }
        a->ang = 0.0f;
        if (flag) {
            d.x = a->p0.x - old.x;
            d.y = a->p0.y - old.y;
            d.z = a->p0.z - old.z;
            e.x = a->p1.x - a->p0.x;
            e.y = a->p1.y - a->p0.y;
            e.z = a->p1.z - a->p0.z;
            s = d.x * n.x + d.y * n.y + d.z * n.z;
            t.x = n.x * s;
            t.y = n.y * s;
            t.z = n.z * s;
            d.x = d.x - t.x;
            d.y = d.y - t.y;
            d.z = d.z - t.z;
            s2 = e.x * n.x + e.y * n.y + e.z * n.z;
            t.x = n.x * s2;
            t.y = n.y * s2;
            t.z = n.z * s2;
            e.x = e.x - t.x;
            e.y = e.y - t.y;
            e.z = e.z - t.z;
            l1 = sqrtf(d.x * d.x + d.y * d.y + d.z * d.z);
            l2 = sqrtf(e.x * e.x + e.y * e.y + e.z * e.z);
            if ((l1 > 1.0f) & (l2 > 1.0f)) {
                i1 = 1.0f / l1;
                i2 = 1.0f / l2;
                d.x *= i1;
                d.y *= i1;
                d.z *= i1;
                e.x *= i2;
                e.y *= i2;
                e.z *= i2;
                c = d.x * e.x + d.y * e.y + d.z * e.z;
                a->ang = Math_Acos((((c > -1.0f) ? c : -1.0f) < 1.0f) ? ((c > -1.0f) ? c : -1.0f) : 1.0f);
                t.x = d.y * e.z - e.y * d.z;
                t.y = e.x * d.z - d.x * e.z;
                t.z = d.x * e.y - e.x * d.y;
                if (t.x * n.x + t.y * n.y + t.z * n.z < 0.0f) {
                    a->ang = -a->ang;
                }
                a->ang = a->ang / 0.1f;
            }
        }
    }
    a->time += dt;
    f = a->time / a->dur;
    v = Vec3_Lerp(f, a->p0, a->p1);
    a->pos = v;
    a->c = a->a0 * (1.0f - f) + a->a1 * f;
    a->d10 = a->b0 * (1.0f - f) + a->b1 * f;
    dts = dt / 1000.0f;
    a->f1C -= dts;
    if (a->flags & 2) {
        a->f20 -= dts;
    }
    if (D_800F26A0 != 0) {
        a->pos.x = -a->pos.x;
    }
    return 1;
}

typedef struct { u16 a; u16 key; f32 x; f32 y; f32 z; f32 rx; f32 ry; f32 rz; } Wp81_;
extern WpTbl81_ D_80114810;
#define ABS(x) ((x) < 0.0f ? -(x) : (x))
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define RD16(p) ((p)[0] | ((p)[1] << 8))
/* Picks the record for the given time: binary-searches the table by key (time / 10), then steps
 * forward (wrapping) past every record lying within 120 units of one of the n positions. */
u8 Bld_FindRecord(u32 time, u32 n, Vec3f *pts, Wp81_ *out) {
    u32 i = 0;
    u32 lo;
    u32 hi;
    u32 j;
    u8 *r;
    u8 found;
    u32 key;
    f32 d[3];
    f32 m;

    if (D_80114800 == 0 || D_80114810.n == 0) {
        return 0;
    }
    lo = 0;
    time /= 10;
    hi = (D_80114810.n - 1) / 16;
    while (lo < hi) {
        i = (lo + hi) >> 1;
        r = D_80114810.recs + i * 16;
        key = RD16(r + 2);
        if (key < time) {
            lo = i + 1;
        } else if (time < key) {
            hi = i;
        } else {
            lo = hi = i;
        }
    }
    i = lo;
    r = D_80114810.recs + i * 16;
    key = RD16(r + 2);
    if (key < time) {
        i = 0;
    }
    do {
        r = D_80114810.recs + i * 16;
        out->a = RD16(r);
        r += 2;
        out->key = RD16(r);
        r += 2;
        out->x = (s16)RD16(r);
        r += 2;
        out->y = (s16)RD16(r);
        r += 2;
        out->z = (s16)RD16(r);
        r += 2;
        out->rx = (s16)RD16(r) / 4096.0f;
        r += 2;
        out->ry = (s16)RD16(r) / 4096.0f;
        r += 2;
        out->rz = (s16)RD16(r) / 4096.0f;
        found = 0;
        for (j = 0; j < n; j++) {
            d[0] = out->x - pts[j].x;
            d[1] = out->y - pts[j].y;
            d[2] = out->z - pts[j].z;
            m = MAX(MAX(ABS(d[0]), ABS(d[1])), ABS(d[2]));
            if (m < 120.0f) {
                i++;
                found = 1;
                if (i == D_80114810.n >> 4) {
                    i = 0;
                }
                break;
            }
        }
    } while (found);
    return 1;
}

s32 Bld_GetStartPos(u8 i, V12_ *out) {
    Q817_ q;

    if (D_80114800 == 0) {
        return 0;
    }
    if (Bld_ReadNode(&q, D_80114814[i]) == 0) {
        return 0;
    }
    if (q.busy != 0) {
        return 0;
    }
    *out = q.v;
    return 1;
}


s32 Bld_GetLapLength(void) {
    return gBldLapLength;
}
