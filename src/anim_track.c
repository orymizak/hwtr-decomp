#include "common.h"
#include "math.h"
#include "include_asm.h"


/* Animated track path (moving scenery): keyframes are fetched in pairs and interpolated. */
typedef struct {
    /* 0x00 */ u32 duration;
    /* 0x04 */ u32 keyCount;
    /* 0x08 */ u8 unk08[4];
    /* 0x0C */ s32 id;
    /* 0x10 */ u32 time;
    /* 0x14 */ u8 active;
    /* 0x18 */ u32 seekTime;
    /* 0x1C */ s32 seekMode;
    /* 0x20 */ s32 key;      /* key currently loaded in pos0/rot0 */
    /* 0x24 */ Vec3f pos0;
    /* 0x30 */ Quat rot0;
    /* 0x40 */ Vec3f pos1;
    /* 0x4C */ Quat rot1;
    /* 0x5C */ Vec3f result;
} AnimTrack; /* size = 0x68 */

extern u32 gAnimTrackCount;
extern AnimTrack *gAnimTracks;

void Memory_Free(void *ptr);

/* Track path services: three consecutive pointers starting at svcTrackGetPathCount. */
typedef struct {
    s32 (*count)(void);
    void (*info)(s32, u32 *, u32 *, u8 *, s32 *);
    void (*keys)(s32, s32, Vec3f *, Quat *);
} AnimIf_;
extern AnimIf_ svcTrackGetPathCount;
void *Memory_Alloc(s32 size);
s32 AnimTrack_Init(void) {
    u32 i;

    if ((gAnimTrackCount = svcTrackGetPathCount.count()) != 0) {
        if ((gAnimTracks = Memory_Alloc(gAnimTrackCount * sizeof(AnimTrack))) == NULL) {
            return 0;
        }
        memset(gAnimTracks, 0, gAnimTrackCount * sizeof(AnimTrack));
        for (i = 0; i < gAnimTrackCount; i++) {
            svcTrackGetPathCount.info(i, &gAnimTracks[i].duration, &gAnimTracks[i].keyCount, gAnimTracks[i].unk08, &gAnimTracks[i].id);
            svcTrackGetPathCount.keys(i, 0, &gAnimTracks[i].pos0, &gAnimTracks[i].rot0);
            svcTrackGetPathCount.keys(i, 1, &gAnimTracks[i].pos1, &gAnimTracks[i].rot1);
        }
    }
    return 1;
}

s32 AnimTrack_Shutdown(void) {
    if (gAnimTracks != NULL) {
        Memory_Free(gAnimTracks);
        gAnimTracks = NULL;
    }
    return 1;
}

extern u32 gAnimTrackClock;
u32 RaceClock_Get(void);
u32 RaceClock_GetPreStart(void);
void AnimTrack_Evaluate(s32 index);
void TrackSfx_Stop(s32);
void TrackSfx_UpdatePos(s32);
void AnimTrack_Update(void) {
    u32 now;
    u32 prev;
    u32 dt;
    u32 step;
    u32 i;
    AnimTrack *t;

    if (gAnimTracks != NULL) {
        now = RaceClock_Get();
        now += RaceClock_GetPreStart();
        prev = gAnimTrackClock;
        gAnimTrackClock = now;
        dt = now - prev;
        if (dt != 0) {
            for (i = 0, t = gAnimTracks; i < gAnimTrackCount; i++, t++) {
                if (t->active) {
                    if (t->seekTime != 0) {
                        step = dt;
                        if (t->seekTime < dt) {
                            step = t->seekTime;
                        }
                        if (step != 0) {
                            t->time += step;
                            t->seekTime -= step;
                            AnimTrack_Evaluate(i);
                        }
                        if (t->seekTime == 0) {
                            TrackSfx_Stop(t->seekMode);
                        } else {
                            TrackSfx_UpdatePos(t->seekMode);
                        }
                    }
                } else {
                    t->time += dt;
                    AnimTrack_Evaluate(i);
                }
            }
        }
    }
}

extern void (*svcTrackGetPathKey)(s32, s32, Vec3f *, Quat *);
extern void (*svcTrackSetPathTransform)(s32, Vec3f *, Mtx3f *);
Vec3f Vec3_Lerp(f32 t, Vec3f a, Vec3f b);
Quat Quat_Slerp(f32 t, Quat a, Quat b);
void Quat_ToMtx3(Quat q, Mtx3f *out);

void AnimTrack_Evaluate(s32 index) {
    AnimTrack *t = &gAnimTracks[index];
    Mtx3f m;
    Mtx3f m2;
    Quat q;
    Vec3f v;
    u32 kc1;
    s32 key;
    s32 key1;
    f32 f;
    void (**fp)(s32, s32, Vec3f *, Quat *);

    t->time = t->time % t->duration;
    kc1 = t->keyCount - 1;
    key = t->time * kc1 / t->duration;
    key1 = key + 1;
    if (key != t->key) {
        fp = &svcTrackGetPathKey;
        (*fp)(index, key, &t->pos0, &t->rot0);
        (*fp)(index, key1, &t->pos1, &t->rot1);
        t->key = key;
    }
    f = (f32)(u32)(kc1 * t->time - key * t->duration) / (f32)t->duration;
    v = Vec3_Lerp(f, t->pos0, t->pos1);
    t->result = v;
    q = Quat_Slerp(f, t->rot0, t->rot1);
    Quat_ToMtx3(q, &m);

    MTX3_TRANSPOSE(m2.m, m.m);
    svcTrackSetPathTransform(index, &t->result, &m2);
}

void AnimTrack_GetPos(s32 index, Vec3i *out) {
    AnimTrack *track = &gAnimTracks[index];

    *out = *(Vec3i *)&track->result;
}

s32 AnimTrack_FindById(s32 id) {
    u32 i;

    for (i = 0; i < gAnimTrackCount; i++) {
        if (gAnimTracks[i].id == id) {
            return i;
        }
    }
    /* TODO (Bug potential): 
    Devolver 0 si no se encuentra es ambiguo si el índice 0 es un elemento válido.
    Idealmente debería retornar -1 (s32) o un código de error, pero se mantiene 
    para respetar el contrato original de la función.
    */
    return 0;
}

s32 AnimTrack_Start(u32 index) {
    if (index >= gAnimTrackCount) {
        return 0;
    }
    gAnimTracks[index].active = 1;
    return 1;
}

void AnimTrack_SeekKey(s32 index, u32 t, s32 mode) {
    u32 base = (u32)gAnimTracks;
    AnimTrack *track = (AnimTrack *)(index * sizeof(AnimTrack) + base);
    /* TODO(match-debt): &gAnimTracks[index] loads the base after the index product and adds the
       operands the other way round; only the byte offset added to a base read first matches. */
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

s32 AnimTrack_SaveReplay(s32 arg0, u32 arg1) {
    return (arg1 >= 4) ? 4 : 0;
}

void AnimTrack_LoadReplay(void) {
}
