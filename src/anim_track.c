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

INCLUDE_ASM("asm/nonmatchings/anim_track", func_80087C28);

void func_80087EA4(s32 index, Vec3 *out) {
    AnimTrack *track = &D_800F3014[index];

    *out = track->result;
}

INCLUDE_ASM("asm/nonmatchings/anim_track", func_80087EE0);

s32 func_80087F2C(u32 index) {
    if (index >= D_800F3010) {
        return 0;
    }
    D_800F3014[index].active = 1;
    return 1;
}

INCLUDE_ASM("asm/nonmatchings/anim_track", func_80087F7C);

s32 func_80087FF4(s32 arg0, u32 arg1) {
    return (arg1 >= 4) * 4;
}

void func_80088004(void) {
}
