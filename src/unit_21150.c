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

INCLUDE_ASM("asm/nonmatchings/unit_21150", Bld_ReadNode);

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

INCLUDE_ASM("asm/nonmatchings/unit_21150", Bld_StepRacer);

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
