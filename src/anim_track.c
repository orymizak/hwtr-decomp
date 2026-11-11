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

INCLUDE_ASM("asm/nonmatchings/anim_track", func_80087990);

s32 func_80087AD4(void) {
    if (D_800F3014 != NULL) {
        Memory_Free(D_800F3014);
        D_800F3014 = NULL;
    }
    return 1;
}

INCLUDE_ASM("asm/nonmatchings/anim_track", func_80087B08);

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

void func_80087F7C(s32 arg0, u32 arg1, s32 arg2) {
  u32 temp_v1;
  u8 *new_var;
  s32 new_var2;
  u32 var_a1;
  void *temp_a0;
  new_var2 = (*(s32 *)&D_800F3014);
  var_a1 = arg1;
  temp_a0 = (arg0 * 0x68) + new_var2;
  if ((*((s32 *) (((u8 *) temp_a0) + 0x18))) == 0) {
    temp_v1 = (*((s32 *) (((u8 *) temp_a0) + 0x4))) - 1;
    if (temp_v1 < var_a1) {
      var_a1 = temp_v1;
    }
    *((s32 *) (((u8 *) temp_a0) + 0x1C)) = arg2;
    new_var = (u8 *) temp_a0;
    *((s32 *) (new_var + 0x18)) = (u32) (((u32) (var_a1 * (*((s32 *) (new_var + 0x0))))) / temp_v1);
  }
}

s32 func_80087FF4(s32 arg0, u32 arg1) {
    return (arg1 >= 4) * 4;
}

void func_80088004(void) {
}
