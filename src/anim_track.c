#include "common.h"
#include "include_asm.h"

typedef struct {
    s32 x;
    s32 y;
    s32 z;
} Vec3;

typedef struct {
    /* 0x00 */ u32 duration;
    /* 0x04 */ u32 keyCount;
    /* 0x08 */ u8 unk08[4];
    /* 0x0C */ s32 id;
    /* 0x10 */ u32 time;
    /* 0x14 */ u8 active;
    /* 0x18 */ u32 seekTime;
    /* 0x1C */ s32 seekMode;
    /* 0x20 */ u8 unk20[0x3C];
    /* 0x5C */ Vec3 result;
} AnimTrack; /* size = 0x68 */

extern u32 D_800F3010;
extern AnimTrack *D_800F3014;

void Memory_Free(void *ptr);

typedef s32 (*AnimFn_)();
typedef struct { AnimFn_ count; AnimFn_ info; AnimFn_ keys; } AnimIf_;
extern AnimIf_ D_80141720;
void *Memory_Alloc(s32 size);
void *memset(void *dst, s32 value, u32 size);
s32 func_80087990(void) {
    u32 i;

    if ((D_800F3010 = D_80141720.count()) != 0) {
        if ((D_800F3014 = Memory_Alloc(D_800F3010 * sizeof(AnimTrack))) == NULL) {
            return 0;
        }
        memset(D_800F3014, 0, D_800F3010 * sizeof(AnimTrack));
        for (i = 0; i < D_800F3010; i++) {
            D_80141720.info(i, &D_800F3014[i].duration, &D_800F3014[i].keyCount, D_800F3014[i].unk08, &D_800F3014[i].id);
            D_80141720.keys(i, 0, &D_800F3014[i].unk20[4], &D_800F3014[i].unk20[0x10]);
            D_80141720.keys(i, 1, &D_800F3014[i].unk20[0x20], &D_800F3014[i].unk20[0x2C]);
        }
    }
    return 1;
}

s32 func_80087AD4(void) {
    if (D_800F3014 != NULL) {
        Memory_Free(D_800F3014);
        D_800F3014 = NULL;
    }
    return 1;
}

extern u32 D_800F3018;
u32 func_800A76D8(void);
u32 func_800A76E4(void);
void func_80087C28(s32 index);
void func_800C9D00(s32);
void func_800C9DA0(s32);
void func_80087B08(void) {
    u32 now;
    u32 prev;
    u32 dt;
    u32 step;
    u32 i;
    AnimTrack *t;

    if (D_800F3014 != NULL) {
        now = func_800A76D8();
        now += func_800A76E4();
        prev = D_800F3018;
        D_800F3018 = now;
        dt = now - prev;
        if (dt != 0) {
            for (i = 0, t = D_800F3014; i < D_800F3010; i++, t++) {
                if (t->active) {
                    if (t->seekTime != 0) {
                        step = dt;
                        if (t->seekTime < dt) {
                            step = t->seekTime;
                        }
                        if (step != 0) {
                            t->time += step;
                            t->seekTime -= step;
                            func_80087C28(i);
                        }
                        if (t->seekTime == 0) {
                            func_800C9D00(t->seekMode);
                        } else {
                            func_800C9DA0(t->seekMode);
                        }
                    }
                } else {
                    t->time += dt;
                    func_80087C28(i);
                }
            }
        }
    }
}

typedef struct { f32 x; f32 y; f32 z; f32 w; } Quat_;
typedef struct { f32 x; f32 y; f32 z; } Vec3_;
typedef struct { f32 m[3][3]; } Mat3_;
typedef struct {
    /* 0x00 */ u32 duration;
    /* 0x04 */ u32 keyCount;
    /* 0x08 */ u8 unk08[4];
    /* 0x0C */ s32 id;
    /* 0x10 */ u32 time;
    /* 0x14 */ u8 active;
    /* 0x18 */ u32 seekTime;
    /* 0x1C */ s32 seekMode;
    /* 0x20 */ s32 key;
    /* 0x24 */ Vec3_ pos0;
    /* 0x30 */ Quat_ rot0;
    /* 0x40 */ Vec3_ pos1;
    /* 0x4C */ Quat_ rot1;
    /* 0x5C */ Vec3_ result;
} AnimTrk_;
extern void (*D_80141728)(s32, s32, Vec3_ *, Quat_ *);
extern void (*D_8014172C)(s32, Vec3_ *, Mat3_ *);
Vec3_ func_800A9068(f32 t, Vec3_ a, Vec3_ b);
Quat_ func_800A8EBC(f32 t, Quat_ a, Quat_ b);
void func_800A8BA4(Quat_ q, Mat3_ *out);

void func_80087C28(s32 index) {
    AnimTrk_ *t = (AnimTrk_ *)&D_800F3014[index];
    Mat3_ m;
    Mat3_ m2;
    Quat_ q;
    Vec3_ v;
    u32 kc1;
    s32 key;
    s32 key1;
    f32 f;
    void (**fp)(s32, s32, Vec3_ *, Quat_ *);

    t->time = t->time % t->duration;
    kc1 = t->keyCount - 1;
    key = t->time * kc1 / t->duration;
    key1 = key + 1;
    if (key != t->key) {
        fp = &D_80141728;
        (*fp)(index, key, &t->pos0, &t->rot0);
        (*fp)(index, key1, &t->pos1, &t->rot1);
        t->key = key;
    }
    f = (f32)(u32)(kc1 * t->time - key * t->duration) / (f32)t->duration;
    v = func_800A9068(f, t->pos0, t->pos1);
    t->result = v;
    q = func_800A8EBC(f, t->rot0, t->rot1);
    func_800A8BA4(q, &m);
    m2.m[0][0] = m.m[0][0];
    m2.m[0][1] = m.m[1][0];
    m2.m[0][2] = m.m[2][0];
    m2.m[1][0] = m.m[0][1];
    m2.m[1][1] = m.m[1][1];
    m2.m[1][2] = m.m[2][1];
    m2.m[2][0] = m.m[0][2];
    m2.m[2][1] = m.m[1][2];
    m2.m[2][2] = m.m[2][2];
    D_8014172C(index, &t->result, &m2);
}

void func_80087EA4(s32 index, Vec3 *out) {
    AnimTrack *track = &D_800F3014[index];

    *out = track->result;
}

s32 func_80087EE0(s32 id) {
    u32 i;

    for (i = 0; i < D_800F3010; i++) {
        if (D_800F3014[i].id == id) {
            return i;
        }
    }
    return 0;
}

s32 func_80087F2C(u32 index) {
    if (index >= D_800F3010) {
        return 0;
    }
    D_800F3014[index].active = 1;
    return 1;
}

void func_80087F7C(s32 index, u32 t, s32 mode) {
    u32 base = (u32)D_800F3014;
    AnimTrack *track = (AnimTrack *)(index * sizeof(AnimTrack) + base);
    u32 last;

    if (track->seekTime == 0) {
        last = track->keyCount - 1;
        if (last < t) {
            t = last;
        }
        track->seekMode = mode;
        track->seekTime = t * track->duration / last;
    }
}

s32 func_80087FF4(s32 arg0, u32 arg1) {
    return (arg1 >= 4) * 4;
}

void func_80088004(void) {
}
