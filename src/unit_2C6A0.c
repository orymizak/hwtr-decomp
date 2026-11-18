#include "common.h"
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
extern void (*audCarSndInit)(u8, u8, u8 *);
typedef struct { s32 w; u8 p[16]; } E14t_;
typedef struct { u8 pad[0x30]; E14t_ e[1]; } T8D_;
typedef struct { u8 pad[0x12]; u8 b; u8 pad2[0x19]; } E2Cd_;
typedef struct { u8 pad[0x30]; E2Cd_ e[1]; } T8B_;
typedef struct { s32 kind; u8 p4; u8 slot; u8 p6[14]; } E20_;
typedef struct { u8 p0[0x30]; E20_ ent[6]; } Cfg8B_;
typedef struct { u32 time; u32 best; u8 id; } Res8C_;
typedef struct { u8 id; u8 b; u8 pad[2]; s32 c; } Ent8b_;
typedef struct { u8 id; u8 pad[0xF]; } E10g_;
extern void (*audCarSndStopEngine[])(u8);
extern void (*audMusicStop)(void);
void RaceAudio_Pause(void);
extern void (*svcHudCountdown)(s32);
extern void (*svcWorldUpdate)(u32);
extern u8 (*sysInputIsKeyHeldAny)(s32);
extern void (*sysInputRumbleStopAll)(void);
u8 func_80098BF4(void);
void Race_ReadPlayerControls(u8);
void func_80093998(s32);
void func_800AF67C(void);
void func_800A59B8(s32);
void func_8008EA6C(s32);
void func_8008AD04(void);
void RaceClock_Advance(s32);
void RaceAudio_Update(s32);
void func_80087B08(void);
void func_800ABC80(u8, s32, u8);
void func_800AEE20(u32);
void RaceClock_Start(void);
u32 func_800AF174(void);
void func_800AF180(u32);
void Pause_SetPlayer(u8);
extern s32 D_800F3740[];
extern s32 gRaceEndTime;
extern u8 gRaceSkipArmed;
void func_800AEFF8(void);
void func_800A7C8C(u8, u32 *, u32 *);
void func_800ABEA8(u8, u8);
void func_800ABEBC(u8);
void RaceAudio_Silence(void);
extern u8 gHumanRacer[];
extern u8 D_801184A8;
extern u8 D_801184A9;
extern u8 D_801184AA;
extern u8 gRaceIsDemo;
extern u8 gRacePausePlayer;
extern s32 gRaceStartDelay;
extern s32 gRaceAudioTick;
extern u8 (*svcRacePreLoad)(u8 *);
extern u8 (*svcTrackLoad)(u8 *);
extern u8 (*svcRacePostLoad)(u8 *);
extern void (*sysInputRumbleResume)(void);
u8 RaceAudio_Load(void);
void RaceClock_Reset(void);
void func_800908E8(void);
s32 func_800908A8(void);
void func_80090934(void);
void func_8009ABC8(void);
void func_80085780(void);
u8 func_800A90F0(u8 *);
u8 func_800AEA40(u8 *);
u8 func_80087990(u8 *);
u8 func_80080550(u8 *);
u8 func_80098BC0(u8 *);
u8 func_8009930C(u8 *);
u8 func_800853F4(u8 *);
u8 func_80091210(u8 *);
u8 func_8008E8F0(u8 *);
u8 func_800AF230(u8 *);
u8 func_8008955C(u8 *);
u8 func_800A7B88(u8 *);
u8 func_800A58D0(u8 *);
u8 func_800AEF80(u8 *);
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
extern u8 (*svcRacePreUnload)(u8 *);
extern u8 (*svcRacePostUnload)(u8 *);
extern u8 (*svcTrackUnload)();
extern u32 gRacerCount;
extern u8 gRacePoints[];
extern s32 gRaceOutcome;
u8 func_800AEFBC(u8 *);
u8 func_800A5950(u8 *);
u8 func_800A7C2C(u8 *);
u8 func_80089848(u8 *);
u8 func_800AF5B4(u8 *);
u8 func_8008EA64(u8 *);
u8 RaceAudio_Unload(u8 *);
u8 func_80091BA0(u8 *);
u8 func_800858E8(u8 *);
u8 func_800AEAFC(u8 *);
u8 func_800993B8(u8 *);
u8 func_80098BEC(u8 *);
u8 func_800806FC();
u8 func_80087AD4(u8 *);
u8 func_800A9370(u8 *);
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

void Race_LoadProgressInit(u8 *arg0);
void Race_LoadProgressStep(void);
s32 RaceAudio_Init(u8 *t);
    /* TODO (Clean version / Bug check): Define a unified GameConfig struct for 'cfg' instead of raw byte offsets:
    typedef struct {
        u32 flags;              // 0x00: bit 0: is_practice, bit 2: has_time_limit
        u8 pad04[0x14];
        u8 racer_count;         // 0x18: gRacerCount
        u8 pad19[7];
        u32 time_limit;         // 0x20: gRaceTimeLimit
        u8 pad24[12];
        struct {
            s32 kind;           // 0x00: 1 = player racer
            u8 pad04;
            u8 slot;            // 0x05
            u8 pad06[14];
        } racers[6];            // 0x30
    } GameConfig;
    Also remove the redundant function pointer cast on RaceAudio_Init(cfg).
    */
s32 Race_Init(u8 *cfg) {
    u8 i;

    gRaceOutcome = 0;
    gRacerCount = cfg[0x18];
    gHumanCount = 0;
    for (i = 0; i < gRacerCount; i++) {
        gRacePoints[i] = 0;
        if (((Cfg8B_ *)cfg)->ent[i].kind == 1) {
            gHumanRacer[((Cfg8B_ *)cfg)->ent[i].slot] = i;
            gHumanCount++;
        }
    }
    for (i = 0; i < 2; i++) {
        gHumanFinished[i] = 0;
    }
    if (*(s32 *)cfg & 4) {
        gRaceTimeLimit = *(s32 *)(cfg + 0x20);
        gRaceHasTimeLimit = 1;
    } else {
        gRaceHasTimeLimit = 0;
    }
    gRaceIsDemo = *(s32 *)cfg & 1;
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
    if (!func_800A90F0(cfg)) {
        return 0;
    }
    Race_LoadProgressStep();
    Race_LoadProgressStep();
    if (!svcTrackLoad(cfg)) {
        return 0;
    }
    Race_LoadProgressStep();
    if (!func_80087990(cfg)) {
        return 0;
    }
    if (!func_80080550(cfg)) {
        return 0;
    }
    if (!func_80098BC0(cfg)) {
        return 0;
    }
    if (!func_8009930C(cfg)) {
        return 0;
    }
    Race_LoadProgressStep();
    if (!func_800AEA40(cfg)) {
        return 0;
    }
    if (!func_800853F4(cfg)) {
        return 0;
    }
    if (!func_80091210(cfg)) {
        return 0;
    }
    if (!((u8 (*)(u8 *))RaceAudio_Init)(cfg)) {
        return 0;
    }
    if (!func_8008E8F0(cfg)) {
        return 0;
    }
    if (!func_800AF230(cfg)) {
        return 0;
    }
    if (!func_8008955C(cfg)) {
        return 0;
    }
    if (!func_800A7B88(cfg)) {
        return 0;
    }
    if (!func_800A58D0(cfg)) {
        return 0;
    }
    if (!func_800AEF80(cfg)) {
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
    func_800908E8();
    if (gHumanCount == 1 && gRacerCount == 6) {
        gRaceStartDelay = func_800908A8();
    } else {
        gRaceStartDelay = 0;
        func_80090934();
    }
    gRaceSimTime = 0;
    gRaceLastTime = 0;
    gRaceAudioTick = svcTimeGetMillis();
    func_8009ABC8();
    func_80085780();
    sysInputRumbleResume();
    return 1;
}

    /* TODO (Clean version / Bug check): Replace raw buffer 'o' with a structured RaceSummary struct:
    typedef struct {
        s32 race_result_code;   // 0x00: gRaceOutcome
        u8 pad04[0x2C];
        struct {
            u8 pad00[0x12];
            u8 points;          // 0x12: gRacePoints[i]
            u8 pad13[0x19];
        } racers[6];            // 0x30, stride 0x2C
    } RaceSummary;
    Also consider replacing cascading 'err += func() == 0' with a boolean failure accumulator:
    bool ok = svcRacePreUnload(o) && func_800AEFBC(o) && ... && svcRacePostUnload(o);
    */
u8 Race_Shutdown(u8 *o) {
    s32 err;
    u8 i;

    memset(o, 0, 0x124);
    if (svcRacePreUnload(o) == 0) {
        return 0;
    }
    err = func_800AEFBC(o) == 0;
    err += func_800A5950(o) == 0;
    err += func_800A7C2C(o) == 0;
    err += func_80089848(o) == 0;
    err += func_800AF5B4(o) == 0;
    err += func_8008EA64(o) == 0;
    err += RaceAudio_Unload(o) == 0;
    err += func_80091BA0(o) == 0;
    err += func_800858E8(o) == 0;
    err += func_800AEAFC(o) == 0;
    err += func_800993B8(o) == 0;
    err += func_80098BEC(o) == 0;
    err += func_800806FC() == 0;
    err += func_80087AD4(o) == 0;
    err += svcTrackUnload() == 0;
    err += func_800A9370(o) == 0;
    if (svcRacePostUnload(o) == 0) {
        return 0;
    }
    for (i = 0; i < gRacerCount; i++) {
        ((T8B_ *)o)->e[i].b = gRacePoints[i];
    }
    *(s32 *)o = gRaceOutcome;
    return err == 0;
}

    /* TODO (Clean version / Bug check): Eliminate dead branch and clean up state dispatch:
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

    /* TODO (Clean version / Bug check): Use struct assignment and static lookup table for points:
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

    func_800AEFF8();
    gRaceEndTime = gRaceLastTime;
    for (i = 0; i < (s8)gRacerCount; i++) {
        func_800A7C8C(i, &r[i].time, &r[i].best);
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
        func_800ABEA8(p->id, i + 1);
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
        func_800ABEBC(r[b].id);
    }
    RaceAudio_Silence();
    gRaceSkipArmed = 0;
}

void Race_PauseBegin(void);
u8 Race_IsFinished(void);
    /* TODO (Clean version / Bug check): Factor out duplicate car freeze/reset logic into an inline helper:
    static inline void freeze_car(Car *c) {
        if (c->flags & 3) {
            c->yaw = 1.0f;
            c->unk014 = 0.0f;
            c->unk018 = 0.0f;
            c->unk024 = 1;
            c->unk025 = 0;
            c->unk027 = 0;
        }
    }
    And define constants for countdown timer thresholds (1200ms, 2550ms, 3800ms, 5000ms).
    */
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
        gRacePausePlayer = func_80098BF4();
        if ((u32)gRacePhase < 2) {
            for (i = 0; i < gHumanCount; i++) {
                Race_ReadPlayerControls(i);
            }
        }
        if (gRaceShowResults == 0) {
            if (gRacePhase != 0) {
                func_80093998(10);
                func_8009ABC8();
                func_800AF67C();
                func_800A59B8(10);
            }
            func_8008EA6C(10);
            func_8008AD04();
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
        func_80087B08();
    }
    if (gRacePhase == 1) {
        for (i = 0; i < gHumanCount; i++) {
            if (gHumanFinished[i] == 0) {
                func_800ABC80(i, 1, gHumanRacer[i]);
            } else {
                func_800ABC80(i, 3, gHumanRacer[i]);
            }
        }
    } else if (gRacePhase == 2 || gRacePhase == 3) {
        for (i = 0; i < gHumanCount; i++) {
            func_800ABC80(i, 3, gHumanRacer[i]);
        }
    }
    svcWorldUpdate(gRaceShowResults == 0 ? delta : 0);
    func_800AEE20(delta);
    gRaceLastTime = now;
    switch ((u32)gRacePhase) {
    case 0:
        if (RaceClock_Get() >= gRaceStartDelay) {
            func_80090934();
        } else if (sysInputIsKeyHeldAny(0x12)) {
            gRaceStartDelay = RaceClock_Get();
        }
        if (RaceClock_Get() >= gRaceStartDelay + 1200 && D_801184A8 == 0) {
            audSfxPlay(6, 1, 1.0f, 1.0f);
            svcHudCountdown(3);
            D_801184A8 = 1;
        } else if (RaceClock_Get() >= gRaceStartDelay + 2550 && D_801184A9 == 0) {
            audSfxPlay(7, 1, 1.0f, 1.0f);
            svcHudCountdown(2);
            D_801184A9 = 1;
        } else if (RaceClock_Get() >= gRaceStartDelay + 3800 && D_801184AA == 0) {
            audSfxPlay(8, 1, 1.0f, 1.0f);
            svcHudCountdown(1);
            D_801184AA = 1;
        } else if (RaceClock_Get() >= gRaceStartDelay + 5000) {
            func_800AEFF8();
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
                    c = Car_Get(i);
                    if (c->flags & 3) {
                        c->yaw = 1.0f;
                        c->unk014 = 0.0f;
                        c->unk018 = 0.0f;
                        c->unk024 = 1;
                        c->unk025 = 0;
                        c->unk027 = 0;
                    }
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
            c = Car_Get(i);
            if (c->flags & 3) {
                c->yaw = 1.0f;
                c->unk014 = 0.0f;
                c->unk018 = 0.0f;
                c->unk024 = 1;
                c->unk025 = 0;
                c->unk027 = 0;
            }
        }
        gRaceEndTime = gRaceLastTime - 4000;
        break;
    case 3:
        if (gRaceSkipArmed == 0) {
            gRaceSkipArmed = sysInputIsKeyHeldAny(0x12) == 0;
        }
        if (now - gRaceEndTime > 4000) {
            if (gRaceShowResults == 0) {
                svcRacePhaseChanged(3);
            }
            gRaceShowResults = 1;
            n = func_800AF174();
            if (n != 0) {
                i = (now - gRaceEndTime - 4000) / 2000;
                func_800AF180(i % n);
            }
            if (now - gRaceEndTime > 30000 || (gRaceSkipArmed != 0 && sysInputIsKeyHeldAny(0x12)) || sysInputIsKeyHeldAny(0x1A)) {
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
    /* TODO (Clean version / Bug check): Hoist shared pause-exit state updates out of the switch cases:
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

    /* TODO (Clean version / Bug check): Define named timeout constant and simplify boolean return:
    #define RACE_TIMEOUT_MS 1799999 // 0x1B773F (~30 minutes)
    if (RaceClock_Get() > RACE_TIMEOUT_MS) {
        return 1;
    }
    if (gRaceHasTimeLimit != 0) {
        return RaceClock_Get() >= gRaceTimeLimit;
    }
    */
u8 Race_IsFinished(void) {
    u8 ok = 1;
    u8 i;

    if (RaceClock_Get() > 0x1B773F) {
        return 1;
    }
    if (gRaceHasTimeLimit != 0) {
        ok = !(RaceClock_Get() < gRaceTimeLimit);
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

void Race_LoadProgressInit(u8 *arg0) {
    gRaceLoadSteps = arg0[0x18] + 6;
    gRaceLoadStep = 0;
}

    /* TODO (Clean version / Bug check): Express percentage calculation with named macro:
    #define CALC_PROGRESS_PCT(curr, total) (((curr) * 100) / ((total) - 1))
    svcLoadingProgress(CALC_PROGRESS_PCT(gRaceLoadStep, gRaceLoadSteps));
    */
void Race_LoadProgressStep(void) {
    svcLoadingProgress((s32) (gRaceLoadStep * 0x64) / (s32) (gRaceLoadSteps - 1));
    gRaceLoadStep += 1;
}

    /* TODO (Clean version / Bug check): Simplify range switch cases 0..6 to a bounds check:
    return (i <= 6) ? D_800F3770[i] : D_80141448;
    */
s32 Jukebox_GetArtist(u32 i) {
    switch (i) {
        case 0:
        case 1:
        case 2:
        case 3:
        case 4:
        case 5:
        case 6:
            return D_800F3770[i];
    }
    return D_80141448;
}


s32 Jukebox_GetTitle(u32 i) {
    switch (i) {
        case 0:
        case 1:
        case 2:
        case 3:
        case 4:
        case 5:
        case 6:
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
s32 RaceAudio_Init(u8 *t) {
    u8 n;
    u8 i;

    memset(D_801184C8, 0, 0x60);
    memset(&D_8011852C, 0, 0xC);
    D_80118530 = 1;
    n = t[0x18];
    D_801184C2 = 0;
    D_801184C1 = n;
    for (i = 0; i < D_801184C1; i++) {
        if (((T8D_ *)t)->e[i].w == 1) {
            D_801184C8[D_801184C1].b0 = i;
            D_801184C2++;
        }
    }
    audCarSndInit(D_801184C2, D_801184C1, t);
    D_80118528 = 0;
    return 1;
}

INCLUDE_ASM("asm/nonmatchings/unit_2C6A0", RaceAudio_Load);

INCLUDE_ASM("asm/nonmatchings/unit_2C6A0", RaceAudio_Unload);

INCLUDE_ASM("asm/nonmatchings/unit_2C6A0", RaceAudio_Update);

INCLUDE_ASM("asm/nonmatchings/unit_2C6A0", RaceAudio_OnContact);

    /* TODO (Clean version / Bug check): Replace raw byte offset dereferencing with proper Vector3 and Entity structs:
    typedef struct {
        u8 pad00[0x60];
        struct { u8 pad00[0x84]; f32 x, y, z; } *transform; // 0x60
        u8 *audio_emitter;                                   // 0x64
    } SoundSource;
    Vec3f diff;
    // diff.x = s1->transform->x - s2->transform->x; ...
    f32 dist = sqrtf(diff.x * diff.x + diff.y * diff.y + diff.z * diff.z);
    audCarSndPlayOneShot(*s, (dist < 352.0f) ? 2 : (dist < 880.0f) ? 3 : 4, dist / (dist < 352.0f ? 352.0f : dist < 880.0f ? 880.0f : 2304.0f));
    */
void RaceAudio_OnCollision(u8 **pair) {
    u8 *s;
    u8 *t;
    f32 d;
    Vec3f v;

    if (D_80118528 != 1) {
        s = *(u8 **) (pair[0] + 0x64);
        t = *(u8 **) (pair[1] + 0x64);
        if ((u32) s < (u32) t) {
            s = t;
        }
        if (s != NULL) {
            v.x = 0.0f;
            v.y = 0.0f;
            v.z = 0.0f;
            if (*(u8 **) (pair[0] + 0x60) != NULL) {
                v.x += *(f32 *) (*(u8 **) (pair[0] + 0x60) + 0x84);
                v.y += *(f32 *) (*(u8 **) (pair[0] + 0x60) + 0x88);
                v.z += *(f32 *) (*(u8 **) (pair[0] + 0x60) + 0x8C);
            }
            if (*(u8 **) (pair[1] + 0x60) != NULL) {
                v.x -= *(f32 *) (*(u8 **) (pair[1] + 0x60) + 0x84);
                v.y -= *(f32 *) (*(u8 **) (pair[1] + 0x60) + 0x88);
                v.z -= *(f32 *) (*(u8 **) (pair[1] + 0x60) + 0x8C);
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

    /* TODO (Clean version / Bug check): Negative function pointer array indexing 'fp[-2]' should be refactored:
    audSndSrcStop(e->id);
    audSndSrcDestroy(e->id); // fp[-2] accesses the pointer at (audSndSrcStop - 2 * sizeof(void*)). Group these into an audio driver vtable.
    */
void RaceSfx_Shutdown(void) {
    u32 i;
    Ent8b_ *e;
    void (**fp)(s32);

    for (i = 0, fp = &audSndSrcStop, e = ((Ent8b_ *)&gRaceSfx); i < 2; i++, e++) {
        fp[0](e->id);
        fp[-2](e->id);
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

INCLUDE_ASM("asm/nonmatchings/unit_2C6A0", RaceAudio_Silence);

    /* TODO (Clean version / Bug check): Remove convoluted pointer cast '(*(E10g_ * *)&D_800F3704)[i].id':
    Both E10_ and E10g_ have the id byte at offset 0. Use direct member access:
    audSndSrcStop(D_800F3704[i].b0);
    */
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
        audSndSrcStop((*(E10g_ * *)&D_800F3704)[i].id);
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

