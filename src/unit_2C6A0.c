#include "common.h"
#include "race.h"
#include "input.h"
#include "physics.h"
#include "include_asm.h"
#include "car.h"
extern void (*svcGamePauseBegin)(void);
extern void (*audSfxPlay)(s32, s32, f32, f32);
extern u8 (*audSndSrcCreate)(void);
typedef struct { u8 id; u8 b; u8 pad[2]; s32 c; } Ent8_;
typedef struct { u8 b0; u8 pad[0xB]; s32 wC; } E10_;
typedef struct { u8 b0; u8 pad[0xF]; } E10b_;
extern E10b_ D_801184C8[];
extern u8 D_801184C2;
extern s32 D_80118528;
extern void (*audCarSndInit)(u8, u8, RaceSetup *);
typedef struct { u32 time; u32 best; u8 id; } Res8C_;
typedef struct { u8 id; u8 b; u8 pad[2]; s32 c; } Ent8b_;
extern void (*audCarSndStopEngine[])(u8);
extern void (*audMusicStop)(void);
void RaceAudio_Pause(void);
extern void (*svcHudCountdown)(s32);
extern void (*svcWorldUpdate)(u32);
extern u8 (*sysInputIsKeyHeldAny)(s32);
extern void (*sysInputRumbleStopAll)(void);
u8 Race_PollPauseButton(void);
void Race_ReadPlayerControls(u8);
void Cars_Update(s32);
void TrackObj_Update(void);
void Debris_Update(s32);
void RaceCam_Update(s32);
void Pickup_Update(void);
void RaceClock_Advance(s32);
void RaceAudio_Update(s32);
void AnimTrack_Update(void);
void Hud_SetPlayer(u8, s32, u8);
void RaceView_Draw(u32);
void RaceClock_Start(void);
u32 Replay_GetCount(void);
void Replay_Show(u32);
void Pause_SetPlayer(u8);
extern s32 D_800F3740[];
extern s32 gRaceEndTime;
extern u8 gRaceSkipArmed;
void Replay_Record(void);
void Lap_GetTimes(u8, u32 *, u32 *);
void Hud_SetResultRacer(u8, u8);
void Hud_SetBestLapRacer(u8);
void RaceAudio_Silence(void);
extern u8 gHumanRacer[];
extern u8 D_801184A8;
extern u8 D_801184A9;
extern u8 D_801184AA;
extern u8 gRaceIsDemo;
extern u8 gRacePausePlayer;
extern s32 gRaceStartDelay;
extern s32 gRaceAudioTick;
extern u8 (*svcRacePreLoad)(RaceSetup *);
extern u8 (*svcTrackLoad)(RaceSetup *);
extern u8 (*svcRacePostLoad)(RaceSetup *);
extern void (*sysInputRumbleResume)(void);
u8 RaceAudio_Load(void);
void RaceClock_Reset(void);
void RaceCam_StartIntro(void);
s32 RaceCam_GetIntroDuration(void);
void RaceCam_EndIntro(void);
void Coll_Update(void);
void Ai_StartRace(void);
u8 Hud_Init(RaceSetup *);
u8 RaceView_Init(RaceSetup *);
u8 AnimTrack_Init(RaceSetup *);
u8 Bld_Load(RaceSetup *);
u8 Rumble_Init(RaceSetup *);
u8 Coll_Init(RaceSetup *);
u8 Ai_Init(RaceSetup *);
u8 Cars_Init(RaceSetup *);
u8 RaceCam_Init(RaceSetup *);
u8 TrackObj_Init(RaceSetup *);
u8 Pickup_Init(RaceSetup *);
u8 Lap_Init(RaceSetup *);
u8 Debris_Init(RaceSetup *);
u8 Replay_Init(RaceSetup *);
extern void (*audCarSndPlayOneShot)(u8, s32, f32);
extern u32 (*svcGamePauseUpdate)(void);
extern void (*svcGamePauseEnd)(s32);
extern s32 (*svcTimeGetMillis)(void);
extern void (*svcRacePhaseChanged)(s32);
extern u8 gRacePauseRequested;
extern u8 gRacePauseReleased;
extern u8 gRaceShowResults;
extern s32 gRaceSimTime;
extern s32 gRaceLastTime;
extern u8 (*svcRacePreUnload)(RaceResult *);
extern u8 (*svcRacePostUnload)(RaceResult *);
extern u8 (*svcTrackUnload)();
extern u32 gRacerCount;
extern u8 gRacePoints[];
extern s32 gRaceOutcome;
u8 Replay_Shutdown(RaceResult *);
u8 Debris_Shutdown(RaceResult *);
u8 Lap_Shutdown(RaceResult *);
u8 Pickup_Shutdown(RaceResult *);
u8 TrackObj_Shutdown(RaceResult *);
u8 RaceCam_Shutdown(RaceResult *);
u8 RaceAudio_Unload(RaceResult *);
u8 Cars_Shutdown(RaceResult *);
u8 Ai_Shutdown(RaceResult *);
u8 RaceView_Shutdown(RaceResult *);
u8 Coll_Shutdown(RaceResult *);
u8 Rumble_Shutdown(RaceResult *);
u8 Bld_Free();
u8 AnimTrack_Shutdown(RaceResult *);
u8 Hud_Shutdown(RaceResult *);
extern u8 D_801184C1;
extern u8 D_800F3700;
extern E10_ *D_800F3704;
extern void (*audCarSndStartEngine)(u8, s32);
extern void (*audSndSrcPlay)(u8, s32, s32, s32);
extern s32 (*audMusicGetTrack)();
extern void (*audMusicPlayTrack)(s32);
extern u8 gRaceHasTimeLimit;
extern u32 gRaceTimeLimit;
extern u32 gHumanCount;
extern u8 gHumanFinished[];
u32 RaceClock_Get(void);
u8 Car_CheckFinished(u8);
extern Ent8_ gRaceSfx[];
extern void (*audSndSrcStop)(s32);
extern s32 D_800F378C[];
extern s32 D_800F3770[];
extern s32 D_80141448;
extern s32 gGameStateFuncs[];
extern s32 gGameState;

extern s32 gRacePhase;
extern s32 gRaceLoadSteps;
extern s32 gRaceLoadStep;
extern s32 (*svcLoadingProgress)(s32);
extern s32 D_8011852C;
extern u8 D_80118530;
extern u32 D_80118534;

void Race_LoadProgressInit(RaceSetup *cfg);
void Race_LoadProgressStep(void);
u8 RaceAudio_Init(RaceSetup *t);
s32 Race_Init(RaceSetup *cfg) {
    u8 i;

    gRaceOutcome = 0;
    gRacerCount = cfg->count;
    gHumanCount = 0;
    for (i = 0; i < gRacerCount; i++) {
        gRacePoints[i] = 0;
        if (cfg->p[i].kind == 1) {
            gHumanRacer[cfg->p[i].player] = i;
            gHumanCount++;
        }
    }
    for (i = 0; i < 2; i++) {
        gHumanFinished[i] = 0;
    }
    if (cfg->flags & 4) {
        gRaceTimeLimit = cfg->timeLimit;
        gRaceHasTimeLimit = 1;
    } else {
        gRaceHasTimeLimit = 0;
    }
    gRaceIsDemo = cfg->flags & 1;
    D_801184AA = 0;
    D_801184A9 = 0;
    D_801184A8 = 0;
    gRaceShowResults = 0;
    gRacePauseReleased = 1;
    gRacePausePlayer = 0;
    gRacePauseRequested = 0;
    if (!svcRacePreLoad(cfg)) {
        return 0;
    }
    Race_LoadProgressInit(cfg);
    Race_LoadProgressStep();
    if (!Hud_Init(cfg)) {
        return 0;
    }
    Race_LoadProgressStep();
    Race_LoadProgressStep();
    if (!svcTrackLoad(cfg)) {
        return 0;
    }
    Race_LoadProgressStep();
    if (!AnimTrack_Init(cfg)) {
        return 0;
    }
    if (!Bld_Load(cfg)) {
        return 0;
    }
    if (!Rumble_Init(cfg)) {
        return 0;
    }
    if (!Coll_Init(cfg)) {
        return 0;
    }
    Race_LoadProgressStep();
    if (!RaceView_Init(cfg)) {
        return 0;
    }
    if (!Ai_Init(cfg)) {
        return 0;
    }
    if (!Cars_Init(cfg)) {
        return 0;
    }
    if (!RaceAudio_Init(cfg)) {
        return 0;
    }
    if (!RaceCam_Init(cfg)) {
        return 0;
    }
    if (!TrackObj_Init(cfg)) {
        return 0;
    }
    if (!Pickup_Init(cfg)) {
        return 0;
    }
    if (!Lap_Init(cfg)) {
        return 0;
    }
    if (!Debris_Init(cfg)) {
        return 0;
    }
    if (!Replay_Init(cfg)) {
        return 0;
    }
    if (!RaceAudio_Load()) {
        return 0;
    }
    Race_LoadProgressStep();
    if (!svcRacePostLoad(cfg)) {
        return 0;
    }
    gGameState = 0;
    gRacePhase = 0;
    svcRacePhaseChanged(0);
    RaceClock_Reset();
    RaceCam_StartIntro();
    if (gHumanCount == 1 && gRacerCount == 6) {
        gRaceStartDelay = RaceCam_GetIntroDuration();
    } else {
        gRaceStartDelay = 0;
        RaceCam_EndIntro();
    }
    gRaceSimTime = 0;
    gRaceLastTime = 0;
    gRaceAudioTick = svcTimeGetMillis();
    Coll_Update();
    Ai_StartRace();
    sysInputRumbleResume();
    return 1;
}

    /* TODO (Clean version / Bug check): [evaluated 2026-11-18: done with RaceResult (race.h); the
    `err += f() == 0` chain stays: && short-circuits and changes behaviour and the ROM] */
u8 Race_Shutdown(RaceResult *o) {
    s32 err;
    u8 i;

    memset(o, 0, RACE_RESULT_SIZE);
    if (svcRacePreUnload(o) == 0) {
        return 0;
    }
    err = Replay_Shutdown(o) == 0;
    err += Debris_Shutdown(o) == 0;
    err += Lap_Shutdown(o) == 0;
    err += Pickup_Shutdown(o) == 0;
    err += TrackObj_Shutdown(o) == 0;
    err += RaceCam_Shutdown(o) == 0;
    err += RaceAudio_Unload(o) == 0;
    err += Cars_Shutdown(o) == 0;
    err += Ai_Shutdown(o) == 0;
    err += RaceView_Shutdown(o) == 0;
    err += Coll_Shutdown(o) == 0;
    err += Rumble_Shutdown(o) == 0;
    err += Bld_Free() == 0;
    err += AnimTrack_Shutdown(o) == 0;
    err += svcTrackUnload() == 0;
    err += Hud_Shutdown(o) == 0;
    if (svcRacePostUnload(o) == 0) {
        return 0;
    }
    for (i = 0; i < gRacerCount; i++) {
        o->r[i].points = gRacePoints[i];
    }
    o->outcome = gRaceOutcome;
    return err == 0;
}

    /* TODO (Clean version / Bug check): [evaluated 2026-11-18: the single `return 1` changes the
    ROM; the dead if/else is in the original] Eliminate dead branch and clean up state dispatch:
    if (gGameState != 2) {
        gGameStateFuncs[gGameState]();
        return 0;
    }
    return 1; // Both if (gGameState) and else branches redundantly return 1
    */
s32 Game_RunState(void) {
  if (gGameState != 2) {
    ((s32 (*)()) gGameStateFuncs[gGameState])();
    return 0;
  }
  if (gGameState) {
    return 1;
  }
  else
  {
    return 1;
  }
}

    /* TODO (Clean version / Bug check): [evaluated 2026-11-18: the struct assignment and a points
    table both change the ROM] Use struct assignment and static lookup table for points:
    static const u8 sPointsTable[] = { 10, 8, 7, 6, 5, 4 };
    ...
    if (b != i) {
        tmp = r[i];
        r[i] = r[b];
        r[b] = tmp;
    }
    gRacePoints[p->id] = (p->time != 0 && (u32)i < 6) ? sPointsTable[i] : 0;
    */
void Race_ComputeResults(void) {
    Res8C_ r[6];
    Res8C_ tmp;
    s8 i;
    s8 j;
    s8 b;
    Res8C_ *p;

    Replay_Record();
    gRaceEndTime = gRaceLastTime;
    for (i = 0; i < (s8)gRacerCount; i++) {
        Lap_GetTimes(i, &r[i].time, &r[i].best);
        r[i].id = i;
    }
    for (i = 0; i < (s8)gRacerCount; i++) {
        b = i;
        for (j = i + 1; j < (s8)gRacerCount; j++) {
            if (r[j].time != 0 && (r[j].time < r[b].time || r[b].time == 0)) {
                b = j;
            }
        }
        if (b != i) {
            tmp.time = r[i].time;
            tmp.id = r[i].id;
            tmp.best = r[i].best;
            r[i].time = r[b].time;
            r[i].id = r[b].id;
            r[i].best = r[b].best;
            r[b].time = tmp.time;
            r[b].id = tmp.id;
            r[b].best = tmp.best;
        }
        p = &r[i];
        Hud_SetResultRacer(p->id, i + 1);
        if (p->time != 0) {
            switch (i) {
            case 0:
                gRacePoints[r[i].id] = 10;
                break;
            case 1:
                gRacePoints[r[i].id] = 8;
                break;
            case 2:
                gRacePoints[r[i].id] = 7;
                break;
            case 3:
                gRacePoints[r[i].id] = 6;
                break;
            case 4:
                gRacePoints[r[i].id] = 5;
                break;
            case 5:
                gRacePoints[r[i].id] = 4;
                break;
            default:
                gRacePoints[p->id] = 0;
                break;
            }
        } else {
            gRacePoints[p->id] = 0;
        }
    }
    b = -1;
    for (j = 0; j < (s8)gRacerCount; j++) {
        if (r[j].best != 0 && (b == -1 || r[j].best < r[b].best || r[b].best == 0)) {
            b = j;
        }
    }
    if (b != -1) {
        Hud_SetBestLapRacer(r[b].id);
    }
    RaceAudio_Silence();
    gRaceSkipArmed = 0;
}

void Race_PauseBegin(void);
u8 Race_IsFinished(void);
/* Stops a car that is still driving (flags & 3) when the race ends. */
static inline void Car_StopAtFinish(Car *c) {
    if (c->flags & 3) {
        c->yaw = 1.0f;
        c->unk014 = 0.0f;
        c->unk018 = 0.0f;
        c->unk024 = 1;
        c->unk025 = 0;
        c->unk027 = 0;
    }
}

/* Countdown after the intro (gRaceStartDelay): "3", "2", "1", then the race starts. */
#define COUNTDOWN_3_MS 1200
#define COUNTDOWN_2_MS 2550
#define COUNTDOWN_1_MS 3800
#define COUNTDOWN_GO_MS 5000

void Race_Update(void) {
    u32 now;
    u32 delta;
    s32 prev;
    u32 i;
    u32 n;
    Car *c;
    s32 unused[2];

    now = svcTimeGetMillis();
    prev = gRaceLastTime;
    delta = now - prev;
    if (prev + 100 < now) {
        gRaceLastTime = now - 100;
        gRaceSimTime += gRaceLastTime - prev;
    }
    while (gRaceSimTime < now) {
        gRacePausePlayer = Race_PollPauseButton();
        if ((u32)gRacePhase < 2) {
            for (i = 0; i < gHumanCount; i++) {
                Race_ReadPlayerControls(i);
            }
        }
        if (gRaceShowResults == 0) {
            if (gRacePhase != 0) {
                Cars_Update(10);
                Coll_Update();
                TrackObj_Update();
                Debris_Update(10);
            }
            RaceCam_Update(10);
            Pickup_Update();
        }
        RaceClock_Advance(10);
        gRaceSimTime += 10;
    }
    if (gRaceAudioTick < now) {
        if (gRacePhase != 3) {
            RaceAudio_Update(100);
        }
        while (gRaceAudioTick < now) {
            gRaceAudioTick += 100;
        }
    }
    if (gRaceShowResults == 0) {
        AnimTrack_Update();
    }
    if (gRacePhase == 1) {
        for (i = 0; i < gHumanCount; i++) {
            if (gHumanFinished[i] == 0) {
                Hud_SetPlayer(i, 1, gHumanRacer[i]);
            } else {
                Hud_SetPlayer(i, 3, gHumanRacer[i]);
            }
        }
    } else if (gRacePhase == 2 || gRacePhase == 3) {
        for (i = 0; i < gHumanCount; i++) {
            Hud_SetPlayer(i, 3, gHumanRacer[i]);
        }
    }
    svcWorldUpdate(gRaceShowResults == 0 ? delta : 0);
    RaceView_Draw(delta);
    gRaceLastTime = now;
    switch ((u32)gRacePhase) {
    case 0:
        if (RaceClock_Get() >= gRaceStartDelay) {
            RaceCam_EndIntro();
        } else if (sysInputIsKeyHeldAny(KEY_A)) {
            gRaceStartDelay = RaceClock_Get();
        }
        if (RaceClock_Get() >= gRaceStartDelay + COUNTDOWN_3_MS && D_801184A8 == 0) {
            audSfxPlay(6, 1, 1.0f, 1.0f);
            svcHudCountdown(3);
            D_801184A8 = 1;
        } else if (RaceClock_Get() >= gRaceStartDelay + COUNTDOWN_2_MS && D_801184A9 == 0) {
            audSfxPlay(7, 1, 1.0f, 1.0f);
            svcHudCountdown(2);
            D_801184A9 = 1;
        } else if (RaceClock_Get() >= gRaceStartDelay + COUNTDOWN_1_MS && D_801184AA == 0) {
            audSfxPlay(8, 1, 1.0f, 1.0f);
            svcHudCountdown(1);
            D_801184AA = 1;
        } else if (RaceClock_Get() >= gRaceStartDelay + COUNTDOWN_GO_MS) {
            Replay_Record();
            RaceClock_Start();
            gRacePhase = 1;
            svcRacePhaseChanged(1);
            D_801184A8 = 0;
            D_801184A9 = 0;
            D_801184AA = 0;
        }
        break;
    case 1:
        if (Race_IsFinished()) {
            if (gRaceIsDemo == 0) {
                sysInputRumbleStopAll();
                Race_ComputeResults();
                gRacePhase = 3;
                svcRacePhaseChanged(2);
                gRaceOutcome = 1;
                for (i = 0; i < gRacerCount; i++) {
                    Car_StopAtFinish(Car_Get(i));
                }
            } else {
                gGameState = 2;
            }
        }
        break;
    case 2:
        sysInputRumbleStopAll();
        Race_ComputeResults();
        gRacePhase = 3;
        for (i = 0; i < gRacerCount; i++) {
            Car_StopAtFinish(Car_Get(i));
        }
        gRaceEndTime = gRaceLastTime - 4000;
        break;
    case 3:
        if (gRaceSkipArmed == 0) {
            gRaceSkipArmed = sysInputIsKeyHeldAny(KEY_A) == 0;
        }
        if (now - gRaceEndTime > 4000) {
            if (gRaceShowResults == 0) {
                svcRacePhaseChanged(3);
            }
            gRaceShowResults = 1;
            n = Replay_GetCount();
            if (n != 0) {
                i = (now - gRaceEndTime - 4000) / 2000;
                Replay_Show(i % n);
            }
            if (now - gRaceEndTime > 30000 || (gRaceSkipArmed != 0 && sysInputIsKeyHeldAny(KEY_A)) || sysInputIsKeyHeldAny(0x1A)) {
                gGameState = 2;
            }
        }
        break;
    }
    if (gRacePauseReleased != 0 && gRacePausePlayer != 0) {
        gRacePauseRequested = 1;
    }
    if (gRacePausePlayer == 0 && gRacePauseReleased == 0) {
        gRacePauseReleased = 1;
    }
    if (gGameState == 0 && gRaceIsDemo == 0 && gRacePauseRequested != 0 && (u32)gRacePhase < 2) {
        Pause_SetPlayer(gRacePausePlayer);
        Race_PauseBegin();
        gGameState = 1;
    }
}

void Race_PauseBegin(void) {
    RaceAudio_Pause();
    svcGamePauseBegin();
    audSfxPlay(0xE, 0, 1.0f, 1.0f);
}


void RaceAudio_Resume(void);
    /* TODO (Clean version / Bug check): [evaluated 2026-11-18: not equivalent: case 1 calls
    RaceAudio_Resume between the steps and other results must do nothing; hoisting also changes the
    ROM] Hoist shared pause-exit state updates out of the switch cases:
    svcGamePauseEnd(choice);
    gRacePauseRequested = 0;
    gRacePauseReleased = 0;
    v = svcTimeGetMillis();
    gRaceSimTime = v;
    gRaceLastTime = v;
    switch (choice) { ... }
    */
void Race_PauseUpdate(void) {
    s32 v;

    switch (svcGamePauseUpdate()) {
        case 1:
            svcGamePauseEnd(1);
            RaceAudio_Resume();
            gRacePauseRequested = 0;
            gRacePauseReleased = 0;
            v = svcTimeGetMillis();
            gRaceSimTime = v;
            gRaceLastTime = v;
            gGameState = 0;
            break;
        case 2:
            svcGamePauseEnd(2);
            gRacePauseRequested = 0;
            gRacePauseReleased = 0;
            v = svcTimeGetMillis();
            gRaceSimTime = v;
            gRaceLastTime = v;
            gGameState = 2;
            gRaceOutcome = 2;
            break;
        case 3:
            svcGamePauseEnd(3);
            gRacePauseRequested = 0;
            gRacePauseReleased = 0;
            v = svcTimeGetMillis();
            gRaceSimTime = v;
            gRaceLastTime = v;
            if (gRacePhase == 0) {
                gGameState = 2;
            } else {
                gGameState = 0;
                gRacePhase = 2;
                svcRacePhaseChanged(3);
                gRaceShowResults = 1;
            }
            gRaceOutcome = 3;
            break;
    }
}

#define RACE_MAX_TIME_MS 1799999 /* a race ends after 30 minutes */

u8 Race_IsFinished(void) {
    u8 ok = 1;
    u8 i;

    if (RaceClock_Get() > RACE_MAX_TIME_MS) {
        return 1;
    }
    if (gRaceHasTimeLimit != 0) {
        ok = RaceClock_Get() >= gRaceTimeLimit;
    } else {
        for (i = 0; i < gHumanCount; i++) {
            u8 v = Car_CheckFinished(i);

            if (v == 0) {
                ok = 0;
            }
            gHumanFinished[i] = v;
        }
    }
    return ok;
}

INCLUDE_ASM("asm/nonmatchings/unit_2C6A0", Race_ReadPlayerControls);

s32 Race_GetPhase(void) {
    return gRacePhase;
}

void Race_LoadProgressInit(RaceSetup *cfg) {
    gRaceLoadSteps = cfg->count + 6; /* one step per racer plus the fixed loads */
    gRaceLoadStep = 0;
}

void Race_LoadProgressStep(void) {
    svcLoadingProgress((s32) (gRaceLoadStep * 100) / (s32) (gRaceLoadSteps - 1)); /* percent */
    gRaceLoadStep += 1;
}

s32 Jukebox_GetArtist(u32 i) {
    if (i <= 6) {
        return D_800F3770[i];
    }
    return D_80141448;
}


s32 Jukebox_GetTitle(u32 i) {
    if (i <= 6) {
        return D_800F378C[i];
    }
    return D_80141448;
}

s32 Music_GetSongTrack(u32 i) {
    i &= 0xFF;
    if (i < 12) {
        return D_800F3740[i];
    }
    return 0;
}


    /* TODO (Bug potential): In the loop populating D_801184C8, index 'D_801184C1' (total racer count) is used instead of 'D_801184C2' (player index):
    D_801184C8[D_801184C1].b0 = i; // Overwrites the same slot at index 'n', potentially out of bounds when n == 6!
    Likely intended:
    D_801184C8[D_801184C2].b0 = i;
    D_801184C2++;
    */
u8 RaceAudio_Init(RaceSetup *t) {
    u8 n;
    u8 i;

    memset(D_801184C8, 0, 0x60);
    memset(&D_8011852C, 0, 0xC);
    D_80118530 = 1;
    n = t->count;
    D_801184C2 = 0;
    D_801184C1 = n;
    for (i = 0; i < D_801184C1; i++) {
        if (t->p[i].kind == 1) {
            D_801184C8[D_801184C1].b0 = i;
            D_801184C2++;
        }
    }
    audCarSndInit(D_801184C2, D_801184C1, t);
    D_80118528 = 0;
    return 1;
}

INCLUDE_ASM("asm/nonmatchings/unit_2C6A0", RaceAudio_Load);

extern u8 (*audCarSndShutdown)(void);
void Memory_Free(void *);

u8 RaceAudio_Unload(RaceResult *unused) {
    u8 i;
    void (**fp)(s32);
    void (**cfp)(u8);

    RaceSfx_Shutdown();
    if (D_800F3700 != 0) {
        i = 0;
        if (i < D_800F3700) {
            fp = &audSndSrcStop;
            do {
                fp[0](D_800F3704[i].b0);
                fp[-2](D_800F3704[i].b0); /* audSndSrcDestroy */
                i++;
            } while (i < D_800F3700);
        }
        Memory_Free(D_800F3704);
        D_800F3704 = NULL;
        D_800F3700 = 0;
    }
    i = 0;
    if (D_801184C1 != 0) {
        cfp = audCarSndStopEngine;
        do {
            cfp[0](i);
            cfp[14](i);
            cfp[4](i);
            cfp[7](i);
            cfp[12](i);
            cfp[10](i);
            i++;
        } while (i < D_801184C1);
    }
    return audCarSndShutdown();
}

INCLUDE_ASM("asm/nonmatchings/unit_2C6A0", RaceAudio_Update);

INCLUDE_ASM("asm/nonmatchings/unit_2C6A0", RaceAudio_OnContact);

void RaceAudio_OnCollision(Collider **pair) {
    u8 *s;
    u8 *t;
    f32 d;
    Vec3f v;

    if (D_80118528 != 1) {
        s = pair[0]->owner;
        t = pair[1]->owner;
        if ((u32) s < (u32) t) {
            s = t;
        }
        if (s != NULL) {
            v.x = 0.0f;
            v.y = 0.0f;
            v.z = 0.0f;
            if (pair[0]->body != NULL) {
                v.x += pair[0]->body->vel.x;
                v.y += pair[0]->body->vel.y;
                v.z += pair[0]->body->vel.z;
            }
            if (pair[1]->body != NULL) {
                v.x -= pair[1]->body->vel.x;
                v.y -= pair[1]->body->vel.y;
                v.z -= pair[1]->body->vel.z;
            }
            d = sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);
            if (d > 176.0f) {
                if (d < 352.0f) {
                    audCarSndPlayOneShot(*s, 2, d / 352.0f);
                } else if (d < 880.0f) {
                    audCarSndPlayOneShot(*s, 3, d / 880.0f);
                } else {
                    audCarSndPlayOneShot(*s, 4, d / 2304.0f);
                }
            }
        }
    }
}

void RaceSfx_Init(void) {
    u32 i;

    memset(gRaceSfx, 0, 16);
    for (i = 0; i < 2; i++) {
        gRaceSfx[i].id = audSndSrcCreate();
    }
}

    /* TODO (Clean version / Bug check): [evaluated 2026-11-18: fp[-2] is audSndSrcDestroy (the
    pointer two slots before audSndSrcStop). Calling both by name, or through a struct based
    at audSndSrcCreate, changes the base register and the offsets, so the ROM changes.] */
void RaceSfx_Shutdown(void) {
    u32 i;
    Ent8b_ *e;
    void (**fp)(s32);

    for (i = 0, fp = &audSndSrcStop, e = ((Ent8b_ *)&gRaceSfx); i < 2; i++, e++) {
        fp[0](e->id);
        fp[-2](e->id); /* audSndSrcDestroy */
    }
    memset(((Ent8b_ *)&gRaceSfx), 0, 16);
}

extern void (*audSndSrcSetPosition)(u8, Vec3f);
extern void (*audSndSrcSetVolume)(u8, f32);
/* Plays a sound on the free channel used least recently; returns the channel or 0xFF. */
u8 RaceSfx_Play(Vec3f pos, u8 bank, u8 vol, u8 loop) {
    u32 now;
    u32 best;
    u32 i;
    u32 slot;
    Ent8_ *e;

    if (D_80118528 == 1) {
        return 0xFF;
    }
    slot = 10000;
    now = RaceClock_Get();
    best = now;
    for (i = 0, e = gRaceSfx; i < 2; i++, e++) {
        if (e->b == 0 && (u32)e->c < best) {
            best = e->c;
            slot = i;
        }
    }
    if (best == now || slot == 1000) {
        return 0xFF;
    }
    e = &gRaceSfx[slot];
    audSndSrcStop(e->id);
    audSndSrcSetPosition(e->id, pos);
    audSndSrcSetVolume(e->id, vol / 255.0f);
    audSndSrcPlay(e->id, bank, 1, loop);
    e->c = now;
    if (loop) {
        e->b = 1;
    }
    return slot;
}

void RaceSfx_Stop(u8 i) {
    Ent8_ *e = &gRaceSfx[i];

    audSndSrcStop(e->id);
    e->b = 0;
    e->c = 0;
}


void RaceSfx_SetPosition(s32 a, Vec3f b) {
}

void RaceAudio_QueueAnnouncer(s32 arg0) {
    if ((arg0 == 3) || ((D_80118530 != 0) && ((u32) D_80118534 >= 0x3E9U))) {
        D_80118530 = 0;
        D_8011852C = arg0;
        D_80118534 = 0;
    }
}

void RaceAudio_Silence(void) {
    u8 i;
    void (**fp)(u8);

    for (i = 0; i < 2; i++) {
        audSndSrcStop(gRaceSfx[i].id);
    }
    if (D_800F3700 != 0) {
        for (i = 0; i < D_800F3700; i++) {
            audSndSrcStop(D_800F3704[i].b0);
        }
    }
    i = 0;
    if (D_801184C1 != 0) {
        fp = audCarSndStopEngine;
        do {
            fp[0](i);
            fp[14](i);
            fp[4](i);
            fp[7](i);
            fp[12](i);
            fp[10](i);
            i++;
        } while (i < D_801184C1);
    }
    D_80118528 = 1;
}

void RaceAudio_Pause(void) {
    u8 i;
    void (**fp)(u8);

    i = 0;
    if (D_801184C1 != 0) {
        fp = audCarSndStopEngine;
        do {
            fp[0](i);
            fp[14](i);
            fp[4](i);
            fp[7](i);
            fp[12](i);
            fp[10](i);
            i++;
        } while (i < D_801184C1);
    }
    for (i = 0; i < D_800F3700; i++) {
        audSndSrcStop(D_800F3704[i].b0);
    }
    audMusicStop();
}

void RaceAudio_Resume(void) {
    u8 i;

    for (i = 0; i < D_801184C1; i++) {
        audCarSndStartEngine(i, 0);
    }
    for (i = 0; i < D_800F3700; i++) {
        audSndSrcPlay(D_800F3704[i].b0, D_800F3704[i].wC, 0, 1);
    }
    audMusicPlayTrack(audMusicGetTrack());
}

