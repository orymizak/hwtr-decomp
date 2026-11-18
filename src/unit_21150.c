#include "common.h"
#include "math.h"
#include "include_asm.h"
typedef struct { s32 a; s32 b; s32 c; } V12_;
typedef struct { u8 busy; u8 pad[3]; V12_ v; u8 pad2[0x28]; } Q817_;
typedef struct { u8 busy; u8 pad[3]; V12_ v; f32 c; f32 d; u16 h18; u16 h1A; u8 pad2[0x18]; } QAD_;
typedef struct { V12_ pos; f32 c; f32 d; u8 p14[4]; s32 x18; f32 x1C; u8 p20[0x30]; s32 x50; V12_ p54; V12_ p60; f32 f6C; f32 f70; f32 f74; f32 f78; u16 id; u8 p7E[0xA]; } TrAD_;
extern u16 D_80114814[];
u8 func_80080730(Q817_ *, u16);

s32 Memory_Free(s32);
extern s32 D_80114800;
extern s32 D_8011480C;

INCLUDE_ASM("asm/nonmatchings/unit_21150", func_80080550);

s32 func_800806FC(void) {
    if (D_80114800 != 0) {
        Memory_Free(D_80114800);
    }
    D_80114800 = 0;
    return 1;
}

INCLUDE_ASM("asm/nonmatchings/unit_21150", func_80080730);

s32 func_80080AD0(TrAD_ *t, u8 i) {
    QAD_ q;
    u8 n;

    if (D_80114800 == 0) {
        return 0;
    }
    memset(t, 0, sizeof(TrAD_));
    t->id = D_80114814[i];
    t->x50 = -1;
    n = func_80080730((Q817_ *)&q, t->id);
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

INCLUDE_ASM("asm/nonmatchings/unit_21150", func_80080C20);

typedef struct { u16 a; u16 key; f32 x; f32 y; f32 z; f32 rx; f32 ry; f32 rz; } Wp81_;
typedef struct { u16 n0; u16 n; u16 ids[6]; u8 *p20; u8 *recs; } WpTbl81_;
extern WpTbl81_ D_80114810;
#define ABS(x) ((x) < 0.0f ? -(x) : (x))
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define RD16(p) ((p)[0] | ((p)[1] << 8))
/* Picks the record for the given time: binary-searches the table by key (time / 10), then steps
 * forward (wrapping) past every record lying within 120 units of one of the n positions. */
u8 func_80081384(u32 time, u32 n, Vec3f *pts, Wp81_ *out) {
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

s32 func_800817A8(u8 i, V12_ *out) {
    Q817_ q;

    if (D_80114800 == 0) {
        return 0;
    }
    if (func_80080730(&q, D_80114814[i]) == 0) {
        return 0;
    }
    if (q.busy != 0) {
        return 0;
    }
    *out = q.v;
    return 1;
}


s32 func_8008182C(void) {
    return D_8011480C;
}
