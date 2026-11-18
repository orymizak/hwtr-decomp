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

extern s32 gReplayCount;

s32 Replay_Init(void) {
    gReplayCount = 0;
    gReplayUsed = 0;
    memset(gReplayBuf, 0, 0x1000);
    return 1;
}


    /* TODO (Clean version): Replay_Shutdown is identical to Replay_Init. Both clear gReplayCount, gReplayUsed and zero 0x1000 bytes in gReplayBuf. */
s32 Replay_Shutdown(void) {
    gReplayCount = 0;
    gReplayUsed = 0;
    memset(gReplayBuf, 0, 0x1000);
    return 1;
}


/* Appends a snapshot (header of five block sizes + total, then the blocks) at most once per second. */
void Replay_Record(void) {
    u8 *h;
    u8 *q;
    u32 room;
    u32 i;
    /* TODO(match-debt): the header size only goes through a variable here; a literal folds into q - (h + 0x18). */
    /* TODO (Clean version): Replace raw s32 array indexing with a structured snapshot type:
       typedef struct {
           s32 module_sizes[5];
           s32 payload_size;
           u8 payload[];
       } SnapshotEntry;

       SnapshotEntry *entry = (SnapshotEntry *)h;
       entry->module_sizes[0] = Cars_SaveReplay(q, room);
       ...
       entry->payload_size = (q - entry->payload);
    */
    s32 hdr = 0x18;

    if (gReplayCount != 0 && (u32)(RaceClock_Get() - gReplayLastTime) < 1000) {
        return;
    }
    room = 0x1000 - gReplayUsed;
    if (room < 0x18) {
        return;
    }
    h = gReplayBuf;
    for (i = 0; i < gReplayCount; i++) {
        h += ((s32 *)h)[5] + 0x18;
    }
    q = h + 0x18;
    room -= 0x18;
    if ((((s32 *)h)[0] = Cars_SaveReplay(q, room)) == 0) {
        return;
    }
    room -= ((s32 *)h)[0];
    q += ((s32 *)h)[0];
    if ((((s32 *)h)[1] = TrackObj_SaveReplay(q, room)) == 0) {
        return;
    }
    room -= ((s32 *)h)[1];
    q += ((s32 *)h)[1];
    if ((((s32 *)h)[2] = AnimTrack_SaveReplay(q, room)) == 0) {
        return;
    }
    room -= ((s32 *)h)[2];
    q += ((s32 *)h)[2];
    if ((((s32 *)h)[3] = Debris_SaveReplay(q, room)) == 0) {
        return;
    }
    room -= ((s32 *)h)[3];
    q += ((s32 *)h)[3];
    if ((((s32 *)h)[4] = RaceCam_SaveReplay(q, room)) == 0) {
        return;
    }
    q += ((s32 *)h)[4];
    ((s32 *)h)[5] = (q - hdr) - h;
    gReplayCount++;
    gReplayUsed += 0x18 + ((s32 *)h)[5];
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
        h += ((s32 *)h)[5] + 0x18;
    }
    q = h + 0x18;
    Cars_LoadReplay(q, ((s32 *)h)[0]);
    q += ((s32 *)h)[0];
    TrackObj_LoadReplay(q, ((s32 *)h)[1]);
    q += ((s32 *)h)[1];
    AnimTrack_LoadReplay(q, ((s32 *)h)[2]);
    q += ((s32 *)h)[2];
    Debris_LoadReplay(q, ((s32 *)h)[3]);
    RaceCam_LoadReplay(q + ((s32 *)h)[3], ((s32 *)h)[4]);
}

