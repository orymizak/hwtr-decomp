#include "common.h"
#include "include_asm.h"
extern s32 gReplayLastTime;
s32 RaceClock_Get(void);
s32 Cars_SaveReplay(u8 *, s32);
s32 TrackObj_SaveReplay(u8 *, s32);
s32 AnimTrack_SaveReplay(u8 *, s32);
s32 Debris_SaveReplay(u8 *, s32);
s32 RaceCam_SaveReplay(u8 *, s32);
void Cars_LoadReplay(u8 *, s32);
void TrackObj_LoadReplay(u8 *, s32);
void AnimTrack_LoadReplay(u8 *, s32);
void Debris_LoadReplay(u8 *, s32);
void RaceCam_LoadReplay(u8 *, s32);
extern s32 gReplayUsed;
extern u8 gReplayBuf[];
#define REPLAY_BUF_SIZE 0x1000

extern s32 gReplayCount;

/* Replay snapshot in gReplayBuf: the sizes of the five module blocks, their total, then the
   blocks (cars, track objects, animated paths, debris, cameras). */
typedef struct {
    /* 0x00 */ s32 sizes[5];
    /* 0x14 */ s32 total;
    /* 0x18 */ u8 data[1];
} ReplaySnap;
#define REPLAY_SNAP(p) ((ReplaySnap *)(p))
#define REPLAY_HDR_SIZE 0x18 /* offset of ReplaySnap.data */

inline s32 Replay_Init(void) {
    gReplayCount = 0;
    gReplayUsed = 0;
    memset(gReplayBuf, 0, REPLAY_BUF_SIZE);
    return 1;
}


s32 Replay_Shutdown(void) {
    return Replay_Init();
}


/* Appends a snapshot (header of five block sizes + total, then the blocks) at most once per second. */
void Replay_Record(void) {
    u8 *h;
    u8 *q;
    u32 room;
    u32 i;
    /* TODO(match-debt): the header size only goes through a variable here; a literal folds into q - (h + 0x18). */
    s32 hdr = REPLAY_HDR_SIZE;

    if (gReplayCount != 0 && (u32)(RaceClock_Get() - gReplayLastTime) < 1000) {
        return;
    }
    room = REPLAY_BUF_SIZE - gReplayUsed;
    if (room < REPLAY_HDR_SIZE) {
        return;
    }
    h = gReplayBuf;
    for (i = 0; i < gReplayCount; i++) {
        h += REPLAY_SNAP(h)->total + REPLAY_HDR_SIZE;
    }
    q = h + REPLAY_HDR_SIZE;
    room -= REPLAY_HDR_SIZE;
    if ((REPLAY_SNAP(h)->sizes[0] = Cars_SaveReplay(q, room)) == 0) {
        return;
    }
    room -= REPLAY_SNAP(h)->sizes[0];
    q += REPLAY_SNAP(h)->sizes[0];
    if ((REPLAY_SNAP(h)->sizes[1] = TrackObj_SaveReplay(q, room)) == 0) {
        return;
    }
    room -= REPLAY_SNAP(h)->sizes[1];
    q += REPLAY_SNAP(h)->sizes[1];
    if ((REPLAY_SNAP(h)->sizes[2] = AnimTrack_SaveReplay(q, room)) == 0) {
        return;
    }
    room -= REPLAY_SNAP(h)->sizes[2];
    q += REPLAY_SNAP(h)->sizes[2];
    if ((REPLAY_SNAP(h)->sizes[3] = Debris_SaveReplay(q, room)) == 0) {
        return;
    }
    room -= REPLAY_SNAP(h)->sizes[3];
    q += REPLAY_SNAP(h)->sizes[3];
    if ((REPLAY_SNAP(h)->sizes[4] = RaceCam_SaveReplay(q, room)) == 0) {
        return;
    }
    q += REPLAY_SNAP(h)->sizes[4];
    REPLAY_SNAP(h)->total = (q - hdr) - h;
    gReplayCount++;
    gReplayUsed += REPLAY_HDR_SIZE + REPLAY_SNAP(h)->total;
    gReplayLastTime = RaceClock_Get();
}

s32 Replay_GetCount(void) {
    return gReplayCount;
}

void Replay_Show(u32 n) {
    u8 *h = gReplayBuf;
    u8 *q;
    u32 i;

    /* TODO (Bug check): Missing bounds check on `n`. If `n >= gReplayCount`, pointer traversal iterates past valid recorded snapshots:
       if (n >= gReplayCount) {
           return;
       }
    */
    for (i = 0; i < n; i++) {
        h += REPLAY_SNAP(h)->total + REPLAY_HDR_SIZE;
    }
    q = h + REPLAY_HDR_SIZE;
    Cars_LoadReplay(q, REPLAY_SNAP(h)->sizes[0]);
    q += REPLAY_SNAP(h)->sizes[0];
    TrackObj_LoadReplay(q, REPLAY_SNAP(h)->sizes[1]);
    q += REPLAY_SNAP(h)->sizes[1];
    AnimTrack_LoadReplay(q, REPLAY_SNAP(h)->sizes[2]);
    q += REPLAY_SNAP(h)->sizes[2];
    Debris_LoadReplay(q, REPLAY_SNAP(h)->sizes[3]);
    RaceCam_LoadReplay(q + REPLAY_SNAP(h)->sizes[3], REPLAY_SNAP(h)->sizes[4]);
}

