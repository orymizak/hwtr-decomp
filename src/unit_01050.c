#include "common.h"
#include "ultra64.h"
#include "include_asm.h"
#include "car.h"
void Snd_SetVolume(s32, s32);
void *Memory_Alloc(s32);
extern void *gWaveTblBuffer;
extern u8 D_80138AB0;
void Input_MirrorSteering(void);
u8 Race_Shutdown(s32);
void Particles_FreeAll(void);
void Screen_SetColorShift(s32);
typedef struct Node64_ { u8 pad[0xC]; f32 v; u8 pad2[4]; struct Node64_ *next; u8 pad3[2]; u8 id; } Node64_;
extern Node64_ *gSndSrcList;
void nuPiReadRom(u32 devAddr, void *dram, u32 size);
void Snd_LoadWaveBank(void *data, s32 a);
typedef struct { s32 a; s32 b; s32 c; } Vec3J_;
extern Vec3J_ gListenerVel;
typedef struct { s32 a; s32 b; s32 c; } Vec3I_;
extern Vec3I_ gListenerPos;
extern Vec3f gListenerOriA;
extern Vec3f gListenerOriB;
typedef struct { f32 f0; } S_D_800F01B4;
extern S_D_800F01B4 gCarSndPrevThrottle[];
typedef struct { f32 f0; u8 p4[0x48]; } S_D_800EFF54;
extern S_D_800EFF54 D_800EFF54[];
extern f64 D_800FE868;
typedef struct { s32 f0; } S_D_800EFC78;
typedef struct { u8 kind; u8 mode; u8 pad[2]; u8 flag; u8 pad2[0x47]; } Slot_;
typedef struct { s32 h[6]; f32 rpm; f32 f1C; f32 f20; f32 f24; f32 f28; u8 p2C[0x18]; u8 b44; u8 b45; u8 p46[6]; } Snd29_;
typedef struct Nd29_ { u8 p0[0x10]; s32 h; struct Nd29_ *next; } Nd29_;
typedef struct N64c_ { f32 x; f32 y; f32 z; f32 v; s32 f10; struct N64c_ *next; u8 b18; u8 b19; u8 id; u8 b1B; } N64c_;
typedef struct { s32 w; u8 p4[0x10]; } E14s_;
typedef struct { u32 f0; u8 p4[0x14]; u8 n18; u8 p19[3]; u32 f1C; u8 p20[0x10]; E14s_ e[1]; } Cfg9_;
f32 Snd3D_GetDoppler(void *pos, void *vel);
s32 Snd3D_GetPan(void *);
extern u8 D_80138AAE;
extern u8 D_80138AAF;
extern u8 D_80138AB1;
void Video_EnterRaceMode(void);
void Particles_Init(void);
void World_SetupLod(s32, u8);
u8 Race_Init(Cfg9_ *);
extern s32 D_800F0108[];
extern s32 D_800F0120;
extern s32 D_800F0124[];
extern s32 D_800F013C;
extern s32 D_800F0140[];
extern s32 D_800F0158;
s32 Rand_Range(s32);
extern s32 gSndSrcCount;
extern Vec3f D_800FE7B8;
extern u8 gReverbOn;
extern f64 D_800FE848;
void Snd_SelectFx(s32);
extern Snd29_ gCarSnd[];
void Snd_SetPan(s32, s32);
extern Slot_ D_800EFF7C[];
extern u8 gTrackSndIndex;
s32 Snd_SetPitch(s32 id, f32 val);
extern s32 gSfxHandle01;
extern s32 gSfxHandle0D;
extern s32 gSfxTable[];
extern s32 audCarSndUpdate;
extern s32 audSfxPlay;
extern s32 audMusicPlayTrack;
extern s32 audMusicGetTrack;
extern s32 audListenerSetPosition;
extern s32 audListenerSetOrientation;
extern s32 audListenerSetVelocity;
extern s32 audCarSndSetPosition;
extern s32 audCarSndSetVelocity;
extern s32 audSetReverb;
extern s32 audCarSndStartEngine;
extern s32 audCarSndStopEngine;
extern s32 audCarSndSetRpm;
extern s32 audCarSndSetThrottle;
extern s32 audCarSndStartLoopA;
extern s32 audCarSndStopLoopA;
extern s32 audCarSndSetLoopAVolume;
extern s32 audCarSndStartLoopB;
extern s32 audCarSndStopLoopB;
extern s32 audCarSndSetLoopBVolume;
extern s32 audCarSndPlayOneShot2;
extern s32 audCarSndStopOneShot2;
extern s32 audCarSndPlayOneShot;
extern s32 audCarSndStopOneShot;
extern s32 audCarSndPlayLoopC;
extern s32 audCarSndStopLoopC;
extern s32 audCarSndSetLoopCVolume;
extern s32 audSndSrcCreate;
extern s32 audSndSrcSetPosition;
extern s32 audSndSrcPlay;
extern s32 audSndSrcStop;
extern s32 audCarSndUpdateSplit;
extern s32 audVoicePlayRandom;
extern S_D_800EFC78 gSongTable[];
s32 Snd_Play(s32, s32,s32,s32, s32);
extern s32 gMusicHandle;
extern f32 gMusicVolume;
extern u8 D_800EFF60;
extern u8 D_800EFF4C;
extern u8 D_800EFF48;
extern u8 D_800EFF5C;
extern u8 D_800EFF44;
extern u8 D_800EFF58;
extern u8 D_800EFF40;
extern u8 D_800EFF50;
s32 Snd_FadeOutAll(s32,s32);
s32 Audio_FadeOutAll(void);
s32 Snd_SetMasterVolume(s32, s32);
s32 Snd_Stop(s32,s32);
s32 Snd_IsPlaying(s32);
extern s32 gSfxHandle37;
extern s32 gSfxHandle36;
extern f64 D_800FE8A8;
extern s32 sysScreenBeginFrame;
extern void *svcTimeGetMillis;
extern void *svcRandRange;
extern void *svcScreenEndFrame;
extern void *svcScreenGetWidth;
extern void *svcScreenGetHeight;
extern void *svcScreenSetViewport;
extern void *svcCameraApply;
extern void *svcCameraSetPerspective;
extern s32 D_801416CC;
extern void *svcTrackLoad;
extern void *svcTrackUnload;
extern void *svcWorldDraw;
extern void *svcTrackGetLinkCount;
extern void *svcTrackGetLink;
extern void *svcTrackSetLinkTransform;
extern void *svcTrackGetCameraCount;
extern void *svcTrackGetCamera;
extern void *svcTrackGetPathCount;
extern void *svcTrackGetPath;
extern void *svcTrackGetPathKey;
extern void *svcTrackSetPathTransform;
extern void *svcTrackGetMarkerCount;
extern void *svcTrackGetMarker;
extern void *svcModelLoadDefault;
extern void *svcModelUnload;
extern void *svcCarInfoLoadEx;
extern void *svcModelSetTransform;
extern void *svcModelSetWheel;
extern void *svcModelSetEngineShake;
extern void *svcModelSetShadowPlane;
extern s32 D_80141768;
extern void *svcFxCarScrape;
extern void *svcGamePauseBegin;
extern void *svcGamePauseEnd;
extern void *svcGamePauseUpdate;
extern void *svcDraw2DBeginPrim;
extern void *svcRacePreLoad;
extern void *svcRacePostLoad;
extern void *svcRacePreUnload;
extern void *svcRacePostUnload;

s32 Video_LeaveRaceMode();
s32 Game_RunState();
s32 Input_RumbleStopAll();
s32 Pause_Init();
s32 Input_RumbleResume();
s32 Pause_End(s32);
s32 Pause_Update();
s32 Font_DrawChar(s32, s32, s32, s32, s32);
s32 AudioTask_Init(s32 *);
extern s32 gSndConfig;
extern u8 gMusicTrack;
extern f32 gSfxVolume;
extern f64 D_800FE898;
extern f32 gVoiceVolume;
extern f64 D_800FE8A0;

s32 func_80060450(void) {
    return 1;
}

void Track_Load();
void Track_Unload();
extern void *svcWorldUpdate;
void World_Update();
void World_Draw();
extern void *svcWorldSetCollLevel;
void World_SetCollLevel();
void Track_GetLinkCount();
void Track_GetLink();
void Track_SetLinkTransform();
extern void *svcTrackSmashObject;
void Track_SmashObject();
void Track_GetCameraCount();
void Track_GetCamera();
void Track_GetMarkerCount();
void Track_GetMarker();
void Model_LoadDefault();
void Model_Unload();
extern void *svcModelGetColor;
void Model_GetColor();
void CarInfo_LoadEx();
extern void *svcModelWreck;
void Model_Wreck();
extern void *svcModelRepair;
void Model_Repair();
extern void *svcModelSetPartTransform;
void Model_SetPartTransform();
void Model_SetTransform();
void Model_SetWheel();
void Model_SetShadowPlane();
void Model_SetEngineShake();
extern void *svcModelApplyFlags;
void Model_ApplyFlags();
extern void *svcModelFlashOn;
void Model_FlashOn();
extern void *svcModelFlashOff;
void Model_FlashOff();
extern void *svcModelSetVisible;
void Model_SetVisible();
extern void *svcModelSetAlpha;
void Model_SetAlpha();
extern void *svcFontLoad;
void Font_Load();
extern void *svcFontFree;
void Font_Free();
extern void *svcFontGetCharWidth;
void Font_GetCharWidth();
extern void *svcFontDrawChar;
s32 Font_DrawCharSvc(s32, s32, s32, s32);
extern void *svcDraw2DSetColor;
void Draw2D_SetColor();
extern void *svcScreenBeginFrame;
void Screen_BeginFrame();
void Screen_EndFrame();
void Screen_GetWidth();
void Screen_GetHeight();
void Screen_SetViewport();
void Camera_Apply();
void Camera_SetPerspectiveRace();
void Time_GetMillis();
void Game_PauseBegin();
void Game_PauseEnd();
void Game_PauseUpdate();
void Fx_CarScrape();
void Track_GetPathCount();
void Track_GetPath();
void Track_GetPathKey();
void Track_SetPathTransform();
extern void *svcTrackLoadDup;
void Track_LoadDup();
extern void *svcTrackGetEntity;
void Track_GetEntity();
extern void *svcTrackGetEntityCount;
void Track_GetEntityCount();
extern void *svcTrackSetEntityVisible;
void Track_SetEntityVisible();
s32 Svc_RacePreLoadNop(void);
s32 Svc_RacePostLoadNop(void);
s32 Svc_RacePreUnloadNop(void);
s32 Race_PostUnload(void);
extern void *svcFileGetSize;
void File_GetSize();
extern void *svcFileLoad;
void File_Load();
void Draw2D_BeginPrim();

/* Fills the service table at svcTimeGetMillis..svcRacePostUnload (font, file, timer and game callbacks);
 * Svc_Uninstall clears it again. */
s32 Svc_Install(void) {
    svcTrackLoad = &Track_Load;
    svcTrackUnload = &Track_Unload;
    svcWorldUpdate = &World_Update;
    svcWorldDraw = &World_Draw;
    svcWorldSetCollLevel = &World_SetCollLevel;
    svcTrackGetLinkCount = &Track_GetLinkCount;
    svcTrackGetLink = &Track_GetLink;
    svcTrackSetLinkTransform = &Track_SetLinkTransform;
    svcTrackSmashObject = &Track_SmashObject;
    svcTrackGetCameraCount = &Track_GetCameraCount;
    svcTrackGetCamera = &Track_GetCamera;
    svcTrackGetMarkerCount = &Track_GetMarkerCount;
    svcTrackGetMarker = &Track_GetMarker;
    svcModelLoadDefault = &Model_LoadDefault;
    svcModelUnload = &Model_Unload;
    svcModelGetColor = &Model_GetColor;
    svcCarInfoLoadEx = &CarInfo_LoadEx;
    svcModelWreck = &Model_Wreck;
    svcModelRepair = &Model_Repair;
    svcModelSetPartTransform = &Model_SetPartTransform;
    svcModelSetTransform = &Model_SetTransform;
    svcModelSetWheel = &Model_SetWheel;
    svcModelSetShadowPlane = &Model_SetShadowPlane;
    svcModelSetEngineShake = &Model_SetEngineShake;
    svcModelApplyFlags = &Model_ApplyFlags;
    svcModelFlashOn = &Model_FlashOn;
    svcModelFlashOff = &Model_FlashOff;
    svcModelSetVisible = &Model_SetVisible;
    svcModelSetAlpha = &Model_SetAlpha;
    svcFontLoad = &Font_Load;
    svcFontFree = &Font_Free;
    svcFontGetCharWidth = &Font_GetCharWidth;
    svcFontDrawChar = &Font_DrawCharSvc;
    svcDraw2DSetColor = &Draw2D_SetColor;
    svcScreenBeginFrame = &Screen_BeginFrame;
    svcScreenEndFrame = &Screen_EndFrame;
    svcScreenGetWidth = &Screen_GetWidth;
    svcScreenGetHeight = &Screen_GetHeight;
    svcScreenSetViewport = &Screen_SetViewport;
    svcCameraApply = &Camera_Apply;
    svcCameraSetPerspective = &Camera_SetPerspectiveRace;
    svcTimeGetMillis = &Time_GetMillis;
    svcRandRange = &Rand_Range;
    svcGamePauseBegin = &Game_PauseBegin;
    svcGamePauseEnd = &Game_PauseEnd;
    svcGamePauseUpdate = &Game_PauseUpdate;
    svcFxCarScrape = &Fx_CarScrape;
    svcTrackGetPathCount = &Track_GetPathCount;
    svcTrackGetPath = &Track_GetPath;
    svcTrackGetPathKey = &Track_GetPathKey;
    svcTrackSetPathTransform = &Track_SetPathTransform;
    svcTrackLoadDup = &Track_LoadDup;
    svcTrackGetEntity = &Track_GetEntity;
    svcTrackGetEntityCount = &Track_GetEntityCount;
    svcTrackSetEntityVisible = &Track_SetEntityVisible;
    svcRacePreLoad = &Svc_RacePreLoadNop;
    svcRacePostLoad = &Svc_RacePostLoadNop;
    svcRacePreUnload = &Svc_RacePreUnloadNop;
    svcRacePostUnload = &Race_PostUnload;
    svcFileGetSize = &File_GetSize;
    svcFileLoad = &File_Load;
    svcDraw2DBeginPrim = &Draw2D_BeginPrim;
    return 1;
}

s32 Svc_Uninstall(void) {
    svcTrackLoad = 0;
    svcTrackUnload = 0;
    svcWorldDraw = 0;
    svcTrackGetLinkCount = 0;
    svcTrackGetLink = 0;
    svcTrackSetLinkTransform = 0;
    svcTrackGetCameraCount = 0;
    svcTrackGetCamera = 0;
    svcTrackGetMarkerCount = 0;
    svcTrackGetMarker = 0;
    svcModelLoadDefault = 0;
    svcModelUnload = 0;
    svcCarInfoLoadEx = 0;
    svcModelSetTransform = 0;
    svcModelSetWheel = 0;
    svcModelSetShadowPlane = 0;
    svcModelSetEngineShake = 0;
    D_80141768 = 0;
    sysScreenBeginFrame = 0;
    svcScreenEndFrame = 0;
    svcScreenGetWidth = 0;
    svcScreenGetHeight = 0;
    svcScreenSetViewport = 0;
    svcCameraApply = 0;
    svcCameraSetPerspective = 0;
    D_801416CC = 0;
    svcTimeGetMillis = 0;
    svcRandRange = 0;
    svcRacePreLoad = 0;
    svcRacePostLoad = 0;
    svcRacePreUnload = 0;
    svcRacePostUnload = 0;
    svcFxCarScrape = 0;
    svcGamePauseBegin = 0;
    svcGamePauseEnd = 0;
    svcGamePauseUpdate = 0;
    svcTrackGetPathCount = 0;
    svcTrackGetPath = 0;
    svcTrackGetPathKey = 0;
    svcTrackSetPathTransform = 0;
    svcDraw2DBeginPrim = 0;
    return 1;
}

s32 Svc_RacePreLoadNop(void) {
    return 1;
}

s32 Svc_RacePostLoadNop(void) {
    return 1;
}

s32 Svc_RacePreUnloadNop(void) {
    return 1;
}

s32 Race_PostUnload(void) {
    Video_LeaveRaceMode();
    return 1;
}

u8 Game_StartRace(Cfg9_ *c) {
    s32 n = 0;
    s32 i;

    Video_EnterRaceMode();
    Particles_Init();
    for (i = 0; i < c->n18; i++) {
        if (c->e[i].w == 1) {
            n++;
        }
    }
    if (c->f1C & 8) {
        D_80138AAE = 1;
    } else {
        D_80138AAE = 0;
    }
    if (c->f1C & 1) {
        D_80138AAF = 1;
    } else {
        D_80138AAF = 0;
    }
    if (c->f1C & 0x20) {
        D_80138AB1 = 1;
    } else {
        D_80138AB1 = 0;
    }
    if (c->f0 & 0x40) {
        D_80138AB0 = 1;
        Input_MirrorSteering();
    } else {
        D_80138AB0 = 0;
    }
    World_SetupLod(n, c->n18);
    if (!(c->f0 & 0x10)) {
        Screen_SetColorShift(0);
    } else {
        Screen_SetColorShift(1);
    }
    return Race_Init(c);
}

s32 Music_Stop(void);
u8 Game_EndRace(s32 a) {
    u8 *p = &D_80138AB0;
    u8 r;

    Input_RumbleStopAll();
    Music_Stop();
    if (*p != 0) {
        Input_MirrorSteering();
    }
    *p = 0;
    r = Race_Shutdown(a);
    Particles_FreeAll();
    Screen_SetColorShift(0);
    return r;
}


s32 Game_Update(void) {
    return Game_RunState() & 0xFF;
}

void Game_PauseBegin(void) {
    Input_RumbleStopAll();
    Audio_FadeOutAll();
    Pause_Init();
}

void Game_PauseEnd(s32 arg0) {
    Input_RumbleResume();
    Pause_End(arg0);
}

void Game_PauseUpdate(void) {
    Pause_Update();
}

s32 Font_DrawCharSvc(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    return Font_DrawChar(arg0 & 0xFF, arg1 & 0xFF, arg2 & 0xFFFF, arg3 & 0xFFFF, 0) & 0xFFFF;
}

void Audio_InitDriver(void) {
    AudioTask_Init(&gSndConfig);
}

INCLUDE_ASM("asm/nonmatchings/unit_01050", Audio_Init);

void CarSnd_Init(s32 unused, u8 count, u8 *info);
s32 CarSnd_Shutdown();
s32 Sfx_Play(u32 id, s32 mode, f32 vol, f32 pitch);
s32 Music_PlayTrack(u32 track);
u8 Music_GetTrack(void);
s32 CarSnd_Update();
s32 CarSnd_UpdateSplit(void);
s32 Listener_SetPosition(Vec3I_ v);
s32 Listener_SetOrientation(Vec3f a, Vec3f b);
s32 Listener_SetVelocity(Vec3J_ v);
s32 CarSnd_SetPosition(u8 i, Vec3f v);
s32 CarSnd_SetVelocity(u8 i, Vec3f v);
s32 Audio_SetReverb(s32 a, f32 v);
s32 CarSnd_StartEngine();
s32 CarSnd_StopEngine(u8 i);
s32 CarSnd_SetRpm(s32 arg0, s32 arg1);
s32 CarSnd_SetThrottle(u8 car, f32 v);
s32 CarSnd_StartLoopA();
s32 CarSnd_StopLoopA(u8 car);
s32 CarSnd_SetLoopAVolume(s32 arg0, s32 arg1);
s32 CarSnd_StartLoopB();
s32 CarSnd_StopLoopB(u8 car);
s32 CarSnd_SetLoopBVolume(s32 arg0, s32 arg1);
s32 CarSnd_PlayOneShot(u8 i, s32 k, f32 lvl);
s32 CarSnd_StopOneShot(u8 car);
s32 CarSnd_PlayLoopC(u8 i, s32 k);
s32 CarSnd_StopLoopC(u8 car);
s32 CarSnd_SetLoopCVolume(s32 arg0, s32 arg1);
u8 SndSrc_Create(void);
s32 SndSrc_Destroy();
s32 SndSrc_Play();
s32 SndSrc_Stop(u8 id);
s32 SndSrc_SetVolume(u8 id, f32 v);
s32 SndSrc_SetPosition(u8 id, Vec3f v);
s32 Voice_PlayRandom(s32 kind);
extern s32 audCarSndInit;
extern s32 audCarSndShutdown;
extern s32 audMusicStop;
extern s32 audSndSrcDestroy;
extern s32 audSndSrcSetVolume;
/* Installs the sound driver entry points used by the rest of the game. */
s32 Audio_InstallApi(void) {
    audCarSndInit = (s32)CarSnd_Init;
    audCarSndShutdown = (s32)CarSnd_Shutdown;
    audSfxPlay = (s32)Sfx_Play;
    audMusicPlayTrack = (s32)Music_PlayTrack;
    audMusicStop = (s32)Music_Stop;
    audMusicGetTrack = (s32)Music_GetTrack;
    audCarSndUpdate = (s32)CarSnd_Update;
    audCarSndUpdateSplit = (s32)CarSnd_UpdateSplit;
    audListenerSetPosition = (s32)Listener_SetPosition;
    audListenerSetOrientation = (s32)Listener_SetOrientation;
    audListenerSetVelocity = (s32)Listener_SetVelocity;
    audCarSndSetPosition = (s32)CarSnd_SetPosition;
    audCarSndSetVelocity = (s32)CarSnd_SetVelocity;
    audSetReverb = (s32)Audio_SetReverb;
    audCarSndStartEngine = (s32)CarSnd_StartEngine;
    audCarSndStopEngine = (s32)CarSnd_StopEngine;
    audCarSndSetRpm = (s32)CarSnd_SetRpm;
    audCarSndSetThrottle = (s32)CarSnd_SetThrottle;
    audCarSndStartLoopA = (s32)CarSnd_StartLoopA;
    audCarSndStopLoopA = (s32)CarSnd_StopLoopA;
    audCarSndSetLoopAVolume = (s32)CarSnd_SetLoopAVolume;
    audCarSndStartLoopB = (s32)CarSnd_StartLoopB;
    audCarSndStopLoopB = (s32)CarSnd_StopLoopB;
    audCarSndSetLoopBVolume = (s32)CarSnd_SetLoopBVolume;
    audCarSndPlayOneShot = (s32)CarSnd_PlayOneShot;
    audCarSndStopOneShot = (s32)CarSnd_StopOneShot;
    audCarSndPlayOneShot2 = (s32)CarSnd_PlayOneShot;
    audCarSndStopOneShot2 = (s32)CarSnd_StopOneShot;
    audCarSndPlayLoopC = (s32)CarSnd_PlayLoopC;
    audCarSndStopLoopC = (s32)CarSnd_StopLoopC;
    audCarSndSetLoopCVolume = (s32)CarSnd_SetLoopCVolume;
    audSndSrcCreate = (s32)SndSrc_Create;
    audSndSrcDestroy = (s32)SndSrc_Destroy;
    audSndSrcPlay = (s32)SndSrc_Play;
    audSndSrcStop = (s32)SndSrc_Stop;
    audSndSrcSetVolume = (s32)SndSrc_SetVolume;
    audSndSrcSetPosition = (s32)SndSrc_SetPosition;
    audVoicePlayRandom = (s32)Voice_PlayRandom;
    return 1;
}

s32 Audio_UninstallApi(void) {
    s32 *p = &audCarSndUpdate;

    *p = 0;
    audCarSndUpdateSplit = 0;
    audSfxPlay = 0;
    audMusicPlayTrack = 0;
    audMusicGetTrack = 0;
    *p = 0;
    audListenerSetPosition = 0;
    audListenerSetOrientation = 0;
    audListenerSetVelocity = 0;
    audCarSndSetPosition = 0;
    audCarSndSetVelocity = 0;
    audSetReverb = 0;
    audCarSndStartEngine = 0;
    audCarSndStopEngine = 0;
    audCarSndSetRpm = 0;
    audCarSndSetThrottle = 0;
    audCarSndStartLoopA = 0;
    audCarSndStopLoopA = 0;
    audCarSndSetLoopAVolume = 0;
    audCarSndStartLoopB = 0;
    audCarSndStopLoopB = 0;
    audCarSndSetLoopBVolume = 0;
    audCarSndPlayOneShot = 0;
    audCarSndStopOneShot = 0;
    audCarSndPlayOneShot2 = 0;
    audCarSndStopOneShot2 = 0;
    audCarSndPlayLoopC = 0;
    audCarSndStopLoopC = 0;
    audCarSndSetLoopCVolume = 0;
    audSndSrcCreate = 0;
    audSndSrcPlay = 0;
    audSndSrcStop = 0;
    audCarSndSetLoopCVolume = 0;
    audSndSrcSetPosition = 0;
    audVoicePlayRandom = 0;
    return 1;
}


void CarSnd_Init(s32 unused, u8 count, u8 *info) {
    s32 i;
    u32 type;

    for (i = 0; i < count; i++) {
        D_800EFF7C[i].flag = 0;
        type = Car_Get(i)->w540 & 0xFF000000;
        switch (type) {
        case 0x00000000:
            D_800EFF7C[i].kind = 4;
            D_800EFF7C[i].mode = 3;
            break;
        case 0x01000000:
            D_800EFF7C[i].kind = 6;
            D_800EFF7C[i].mode = 5;
            break;
        case 0x02000000:
            D_800EFF7C[i].kind = 8;
            D_800EFF7C[i].mode = 7;
            break;
        case 0x03000000:
            D_800EFF7C[i].kind = 10;
            D_800EFF7C[i].mode = 9;
            break;
        case 0x04000000:
            D_800EFF7C[i].kind = 12;
            D_800EFF7C[i].mode = 11;
            break;
        case 0x05000000:
            D_800EFF7C[i].kind = 14;
            D_800EFF7C[i].mode = 13;
            break;
        case 0x06000000:
            D_800EFF7C[i].kind = 16;
            D_800EFF7C[i].mode = 15;
            break;
        case 0x07000000:
            D_800EFF7C[i].kind = 18;
            D_800EFF7C[i].mode = 17;
            break;
        case 0x08000000:
            D_800EFF7C[i].kind = 20;
            D_800EFF7C[i].mode = 19;
            break;
        case 0x09000000:
            D_800EFF7C[i].kind = 2;
            D_800EFF7C[i].mode = 1;
            break;
        case 0x0A000000:
            D_800EFF7C[i].kind = 21;
            D_800EFF7C[i].mode = 22;
            break;
        default:
            D_800EFF7C[i].kind = 10;
            D_800EFF7C[i].mode = 9;
            break;
        }
    }
    gTrackSndIndex = info[0x16] * 3 + (info[0x17] - 1);
    gMusicTrack = 0;
}

INCLUDE_ASM("asm/nonmatchings/unit_01050", CarSnd_Shutdown);

s32 Sfx_Play(u32 id, s32 mode, f32 vol, f32 pitch) {
    s32 snd;
    s32 v;
    s32 pri;
    s32 h;

    snd = gSfxTable[id];
    v = gSfxVolume * 128.0f * vol;
    if (snd == 0) {
        return 0;
    }
    if (mode == 0) {
        pri = 0x30;
    } else if (mode == 1) {
        pri = 0x60;
    } else {
        return 0;
    }
    if (pitch > 12.0) {
        pitch = 12.0f;
    }
    if (v == 0) {
        v = 1;
    }
    switch (id) {
        case 0x1:
            if (Snd_IsPlaying(gSfxHandle01) == 0) {
                h = Snd_Play(snd, v, 0x80, 0, pri);
                Snd_SetPitch(h, pitch);
                gSfxHandle01 = h;
            }
            return 1;
        case 0xD:
            if (Snd_IsPlaying(gSfxHandle0D) == 0) {
                h = Snd_Play(snd, v, 0x80, 0, pri);
                Snd_SetPitch(h, pitch);
                gSfxHandle0D = h;
            }
            return 1;
        case 0x36:
        case 0x38:
            gSfxHandle36 = Snd_Play(snd, v, 0x80, 1, pri);
            return 1;
        case 0x37:
            gSfxHandle37 = Snd_Play(snd, v, 0x80, 1, pri);
            return 1;
        default:
            Snd_SetPitch(Snd_Play(snd, v, 0x80, 0, pri), pitch);
            return 1;
    }
}

s32 Music_PlayTrack(u32 track) {
    register s32 song;
    register s32 vol;

    song = gSongTable[track].f0;
    vol = 128.0f * gMusicVolume;
    if (Snd_IsPlaying(gMusicHandle) != 0) {
        Snd_Stop(gMusicHandle, 0);
        gMusicHandle = 0;
    }
    gMusicHandle = Snd_Play(song, vol, 0x80, 0, 0xFF);
    *(s8 *)&gMusicTrack = track;
    return 1;
}

s32 Music_Stop(void) {
    s32 *p = &gMusicHandle;

    if (Snd_IsPlaying(*p) != 0) {
        Snd_Stop(*p, 0);
        *p = 0;
    }
    return 1;
}


u8 Music_GetTrack(void) {
    return gMusicTrack;
}

s32 Voice_PlayRandom(s32 kind) {
    s32 vol = gVoiceVolume * 255.0f;
    s32 snd = 0;

    switch ((u32)kind) {
    case 0:
        snd = D_800F0108[Rand_Range(D_800F0120)];
        break;
    case 1:
        snd = D_800F0140[Rand_Range(D_800F0158)];
        break;
    case 2:
        snd = D_800F0124[Rand_Range(D_800F013C)];
        break;
    }
    if (snd != 0) {
        return Snd_Play(snd, vol, 0x80, 0, 0x70);
    }
}

INCLUDE_ASM("asm/nonmatchings/unit_01050", CarSnd_Update);

s32 CarSnd_UpdateSplit(void) {
    s32 p0;
    s32 p1;
    s32 p2;
    s32 p3;
    s32 p4;
    s32 p5;
    s32 i;
    s32 v;
    s32 pan;
    f32 vol;
    f32 x;
    f32 zero;
    f32 pitch;
    Nd29_ *n;

    n = (*(Nd29_ * *)&gSndSrcList);
    i = 0;
    p1 = Snd_IsPlaying(gCarSnd[i].h[1]);
    p0 = Snd_IsPlaying(gCarSnd[i].h[0]);
    p2 = Snd_IsPlaying(gCarSnd[i].h[2]);
    p3 = Snd_IsPlaying(gCarSnd[i].h[3]);
    p4 = Snd_IsPlaying(gCarSnd[i].h[4]);
    p5 = Snd_IsPlaying(gCarSnd[i].h[5]);
    if (p0 || p2 || p3 || p4 || p5 || p1) {
        vol = gSfxVolume;
        x = gCarSnd[i].f1C;
        zero = 0.0f;
        pan = 0x80;
        if (p1) {
            pitch = (gCarSnd[i].b45 != 13) ? (gCarSnd[i].rpm - 1000.0f) * 0.0021818182f + -17.0f : 0.0f;
            pitch = pitch + zero;
            if (pitch > 12.0) {
                pitch = 12.0f;
            }
            v = vol * 128.0f * (1.0 - x);
            if (v == 0) {
                v = 1;
            }
            Snd_SetVolume(gCarSnd[i].h[1], v);
            Snd_SetPan(gCarSnd[i].h[1], pan);
            Snd_SetPitch(gCarSnd[i].h[1], pitch);
        }
        if (p0) {
            pitch = (gCarSnd[i].rpm - 1000.0f) * 0.0021818182f + -17.0f;
            pitch = pitch + zero;
            if (pitch > 12.0) {
                pitch = 12.0f;
            }
            v = vol * 128.0f * x;
            if (v == 0) {
                v = 1;
            }
            Snd_SetVolume(gCarSnd[i].h[0], v);
            Snd_SetPan(gCarSnd[i].h[0], pan);
            Snd_SetPitch(gCarSnd[i].h[0], pitch);
        }
        if (p2) {
            v = gCarSnd[i].f20 * 128.0f * vol;
            if (v == 0) {
                v = 1;
            }
            Snd_SetVolume(gCarSnd[i].h[2], v);
            Snd_SetPan(gCarSnd[i].h[2], pan);
            Snd_SetPitch(gCarSnd[i].h[2], zero);
        }
        if (p3) {
            v = gCarSnd[i].f24 * 128.0f * vol;
            if (v == 0) {
                v = 1;
            }
            Snd_SetVolume(gCarSnd[i].h[3], v);
            Snd_SetPan(gCarSnd[i].h[3], pan);
            Snd_SetPitch(gCarSnd[i].h[3], zero);
        }
        if (p4) {
            v = vol * 128.0f;
            if (v == 0) {
                v = 1;
            }
            Snd_SetVolume(gCarSnd[i].h[4], v);
            Snd_SetPan(gCarSnd[i].h[4], pan);
            Snd_SetPitch(gCarSnd[i].h[4], zero);
        }
        if (p5) {
            v = gCarSnd[i].f28 * 128.0f * vol;
            if (v == 0) {
                v = 1;
            }
            Snd_SetVolume(gCarSnd[i].h[5], v);
            Snd_SetPan(gCarSnd[i].h[5], pan);
            Snd_SetPitch(gCarSnd[i].h[5], zero);
        }
    }
    i++;
    p1 = Snd_IsPlaying(gCarSnd[i].h[1]);
    p0 = Snd_IsPlaying(gCarSnd[i].h[0]);
    p2 = Snd_IsPlaying(gCarSnd[i].h[2]);
    p3 = Snd_IsPlaying(gCarSnd[i].h[3]);
    p4 = Snd_IsPlaying(gCarSnd[i].h[4]);
    p5 = Snd_IsPlaying(gCarSnd[i].h[5]);
    if (p0 || p2 || p3 || p4 || p5 || p1) {
        vol = gSfxVolume;
        x = gCarSnd[i].f1C;
        zero = 0.0f;
        pan = 0x80;
        if (p1) {
            pitch = (gCarSnd[i].b45 != 13) ? (gCarSnd[i].rpm - 1000.0f) * 0.0021818182f + -17.0f : 0.0f;
            pitch = pitch + zero;
            if (pitch > 12.0) {
                pitch = 12.0f;
            }
            v = vol * 128.0f * (1.0 - x);
            if (v == 0) {
                v = 1;
            }
            Snd_SetVolume(gCarSnd[i].h[1], v);
            Snd_SetPan(gCarSnd[i].h[1], pan);
            Snd_SetPitch(gCarSnd[i].h[1], pitch);
        }
        if (p0) {
            if (gCarSnd[i].b44 != 14) {
                pitch = (gCarSnd[i].rpm - 1000.0f) * 0.0021818182f + -17.0f;
            } else {
                pitch = 0.0f;
            }
            pitch = pitch + zero;
            if (pitch > 12.0) {
                pitch = 12.0f;
            }
            v = vol * 128.0f;
            if (v == 0) {
                v = 1;
            }
            Snd_SetVolume(gCarSnd[i].h[0], v);
            Snd_SetPan(gCarSnd[i].h[0], pan);
            Snd_SetPitch(gCarSnd[i].h[0], pitch);
        }
        if (p2) {
            v = gCarSnd[i].f20 * 128.0f * vol;
            if (v == 0) {
                v = 1;
            }
            Snd_SetVolume(gCarSnd[i].h[2], v);
            Snd_SetPan(gCarSnd[i].h[2], pan);
            Snd_SetPitch(gCarSnd[i].h[2], zero);
        }
        if (p3) {
            v = gCarSnd[i].f24 * 128.0f * vol;
            if (v == 0) {
                v = 1;
            }
            Snd_SetVolume(gCarSnd[i].h[3], v);
            Snd_SetPan(gCarSnd[i].h[3], pan);
            Snd_SetPitch(gCarSnd[i].h[3], zero);
        }
        if (p4) {
            v = vol * 128.0f;
            if (v == 0) {
                v = 1;
            }
            Snd_SetVolume(gCarSnd[i].h[4], v);
            Snd_SetPan(gCarSnd[i].h[4], pan);
            Snd_SetPitch(gCarSnd[i].h[4], zero);
        }
        if (p5) {
            v = gCarSnd[i].f28 * 128.0f * vol;
            if (v == 0) {
                v = 1;
            }
            Snd_SetVolume(gCarSnd[i].h[5], v);
            Snd_SetPan(gCarSnd[i].h[5], pan);
            Snd_SetPitch(gCarSnd[i].h[5], zero);
        }
    }
    while (n != NULL) {
        if (n->h != 0) {
            Snd_Stop(n->h, 1);
            n->h = 0;
        }
        n = n->next;
    }
    return 1;
}

s32 Listener_SetPosition(Vec3I_ v) {
    gListenerPos = v;
    return 1;
}

s32 Listener_SetOrientation(Vec3f a, Vec3f b) {
    gListenerOriA = a;
    gListenerOriB = b;
    return 1;
}

s32 Listener_SetVelocity(Vec3J_ v) {
    gListenerVel = v;
    return 1;
}


s32 CarSnd_SetPosition(u8 i, Vec3f v) {
    *(Vec3f *)((u8 *)&D_800EFF54[i] + 0x10) = v;
    return 1;
}


s32 CarSnd_SetVelocity(u8 i, Vec3f v) {
    *(Vec3f *)((u8 *)&D_800EFF54[i] + 0x1C) = v;
    return 1;
}


s32 Audio_SetReverb(s32 a, f32 v) {
    u8 f = gReverbOn;
    s16 ok = 1;

    if ((v == D_800FE848) != f) {
        Snd_SelectFx(v == 1.0f ? 5 : 2);
        if ((u8)ok) {
            gReverbOn = gReverbOn == 0;
        }
    }
    return ok;
}

INCLUDE_ASM("asm/nonmatchings/unit_01050", CarSnd_StartEngine);

s32 CarSnd_StopEngine(u8 i) {
    gCarSnd[i].p46[2] = 0;
    if (Snd_IsPlaying(gCarSnd[i].h[0]) != 0) {
        Snd_Stop(gCarSnd[i].h[0], 0);
        gCarSnd[i].h[0] = 0;
    }
    if (Snd_IsPlaying(gCarSnd[i].h[1]) != 0) {
        Snd_Stop(gCarSnd[i].h[1], 0);
        gCarSnd[i].h[1] = 0;
    }
    return 1;
}

s32 CarSnd_SetRpm(s32 arg0, s32 arg1) {
    *(s32 *)(&D_800EFF50 + ((arg0 & 0xFF) * 0x4C)) = arg1;
    return 1;
}

s32 CarSnd_SetThrottle(u8 car, f32 v) {
    if (gCarSndPrevThrottle[car].f0 < v || v == D_800FE868) {
        D_800EFF54[car].f0 = 1.0f;
    } else {
        D_800EFF54[car].f0 = 0;
    }
    gCarSndPrevThrottle[car].f0 = v;
    return 1;
}

INCLUDE_ASM("asm/nonmatchings/unit_01050", CarSnd_StartLoopA);

s32 CarSnd_StopLoopA(u8 car) {
    if (Snd_IsPlaying(gCarSnd[car].h[2]) != 0) {
        Snd_Stop(gCarSnd[car].h[2], 0);
        gCarSnd[car].h[2] = 0;
    }
    return 1;
}

s32 CarSnd_SetLoopAVolume(s32 arg0, s32 arg1) {
    *(s32 *)(&D_800EFF58 + ((arg0 & 0xFF) * 0x4C)) = arg1;
    return 1;
}

INCLUDE_ASM("asm/nonmatchings/unit_01050", CarSnd_StartLoopB);

s32 CarSnd_StopLoopB(u8 car) {
    if (Snd_IsPlaying(gCarSnd[car].h[3]) != 0) {
        Snd_Stop(gCarSnd[car].h[3], 0);
        gCarSnd[car].h[3] = 0;
    }
    return 1;
}

s32 CarSnd_SetLoopBVolume(s32 arg0, s32 arg1) {
    *(s32 *)(&D_800EFF5C + ((arg0 & 0xFF) * 0x4C)) = arg1;
    return 1;
}

f32 Snd3D_GetVolume(void *a);
s32 CarSnd_PlayOneShot(u8 i, s32 k, f32 lvl) {
    s32 snd = gSfxTable[k];
    s32 off;
    u8 *pos;
    f32 vol;
    s32 pan;
    f32 pitch;
    s32 busy;
    s32 v;

    if (snd == 0) {
        return 0;
    }
    off = i * 0x4C;
    pos = (u8 *)gSfxTable + 0x2D0 + off;
    vol = Snd3D_GetVolume(pos) * gSfxVolume;
    pan = Snd3D_GetPan(pos);
    pitch = Snd3D_GetDoppler(pos, (u8 *)gSfxTable + 0x2DC + off);
    busy = Snd_IsPlaying(*(s32 *)(&D_800EFF48 + off));
    v = lvl * 128.0f * vol;
    if (v == 0) {
        v = 1;
    }
    if (busy == 0) {
        *(s32 *)(&D_800EFF48 + off) = Snd_Play(snd, v, pan, 0, 0x40);
        Snd_SetPitch(*(s32 *)(&D_800EFF48 + off), pitch);
        return 1;
    }
    return 0;
}

s32 CarSnd_StopOneShot(u8 car) {
    if (Snd_IsPlaying(gCarSnd[car].h[4]) != 0) {
        Snd_Stop(gCarSnd[car].h[4], 0);
        gCarSnd[car].h[4] = 0;
    }
    return 1;
}

f32 Snd3D_GetVolume(void *a);
s32 CarSnd_PlayLoopC(u8 i, s32 k) {
    s32 snd = gSfxTable[k];
    s32 off;
    u8 *pos;
    f32 vol;
    s32 pan;
    f32 pitch;
    s32 busy;
    s32 v;

    if (snd == 0) {
        return 0;
    }
    off = i * 0x4C;
    pos = (u8 *)gSfxTable + 0x2D0 + off;
    vol = Snd3D_GetVolume(pos) * gSfxVolume;
    pan = Snd3D_GetPan(pos);
    pitch = Snd3D_GetDoppler(pos, (u8 *)gSfxTable + 0x2DC + off);
    busy = Snd_IsPlaying(*(s32 *)(&D_800EFF4C + off));
    v = *(f32 *)(&D_800EFF60 + off) * 128.0f * vol;
    if (v == 0) {
        v = 1;
    }
    if (busy == 0) {
        *(s32 *)(&D_800EFF4C + off) = Snd_Play(snd, v, pan, 0, 0x40);
        Snd_SetPitch(*(s32 *)(&D_800EFF4C + off), pitch);
        return 1;
    }
    return 0;
}

s32 CarSnd_StopLoopC(u8 car) {
    if (Snd_IsPlaying(gCarSnd[car].h[5]) != 0) {
        Snd_Stop(gCarSnd[car].h[5], 0);
        gCarSnd[car].h[5] = 0;
    }
    return 1;
}

s32 CarSnd_SetLoopCVolume(s32 arg0, s32 arg1) {
    *(s32 *)(&D_800EFF60 + ((arg0 & 0xFF) * 0x4C)) = arg1;
    return 1;
}

u8 SndSrc_Create(void) {
    Vec3f v = D_800FE7B8;
    N64c_ *n = Memory_Alloc(0x1C);
    N64c_ *p;
    N64c_ **head;
    s32 *cnt;

    n->x = v.x;
    n->y = v.y;
    n->z = v.z;
    n->v = 1.0f;
    n->f10 = 0;
    n->next = NULL;
    n->b19 = 0;
    n->b1B = 0;
    head = &(*(N64c_ * *)&gSndSrcList);
    if (*head == NULL) {
        *head = n;
        n->id = 1;
    } else {
        p = *head;
        while (p->next != NULL) {
            p = p->next;
        }
        p->next = n;
        n->id = p->id + 1;
    }
    cnt = &gSndSrcCount;
    (*cnt)++;
    return n->id;
}

s32 SndSrc_SetPosition(u8 id, Vec3f v) {
    Node64_ *p = gSndSrcList;

    while (p != NULL) {
        if (p->id == id) {
            ((Vec3f *)p)->x = v.x;
            ((Vec3f *)p)->y = v.y;
            ((Vec3f *)p)->z = v.z;
            return 1;
        }
        p = p->next;
    }
    return 0;
}


INCLUDE_ASM("asm/nonmatchings/unit_01050", SndSrc_Destroy);

INCLUDE_ASM("asm/nonmatchings/unit_01050", SndSrc_Play);

s32 SndSrc_Stop(u8 id) {
    Node64_ *p = gSndSrcList;

    while (p != NULL) {
        if (p->id == id) {
            break;
        }
        p = p->next;
    }
    if (p == NULL) {
        return 0;
    }
    p->pad3[1] = 0;
    if (Snd_IsPlaying(*(s32 *)p->pad2) != 0) {
        Snd_Stop(*(s32 *)p->pad2, 0);
        *(s32 *)p->pad2 = 0;
    }
    return 1;
}


s32 SndSrc_SetVolume(u8 id, f32 v) {
    Node64_ *p = gSndSrcList;

    while (p != NULL) {
        if (p->id == id) {
            break;
        }
        p = p->next;
    }
    if (p == NULL) {
        return 0;
    }
    p->v = v;
    return 1;
}


void Audio_SetMusicVolume(u8 v) {
    f32 *p = &gMusicVolume;

    *p = v / 255.0;
    if (Snd_IsPlaying(gMusicHandle) != 0) {
        Snd_SetVolume(gMusicHandle, *p * 128.0f);
    }
}


u8 Audio_GetMusicVolume(void) {
    return gMusicVolume * 255.0f;
}


void Audio_SetSfxVolume(s32 arg0) {
    gSfxVolume = (f32) ((f64) (arg0 & 0xFF) / D_800FE898);
}

u8 Audio_GetSfxVolume(void) {
    return gSfxVolume * 255.0f;
}


void Audio_SetVoiceVolume(s32 arg0) {
    gVoiceVolume = (f32) ((f64) (arg0 & 0xFF) / D_800FE8A0);
}

u8 Audio_GetVoiceVolume(void) {
    return gVoiceVolume * 255.0f;
}


void Audio_SetAmbientVolume(u8 vol) {
    Snd_SetMasterVolume(1, (f32)(vol / D_800FE8A8) * 32767.0f);
    if (vol == 0) {
        if (gSfxHandle37 != 0 && Snd_IsPlaying(gSfxHandle37) != 0) {
            Snd_Stop(gSfxHandle37, 0);
            gSfxHandle37 = 0;
        }
        if (gSfxHandle36 != 0 && Snd_IsPlaying(gSfxHandle36) != 0) {
            Snd_Stop(gSfxHandle36, 0);
            gSfxHandle36 = 0;
        }
    }
}

s32 Audio_FadeOutAll(void) {
    Snd_FadeOutAll(1, 1);
    return 1;
}

s32 func_80064980(void) {
    return 1;
}

f32 Vec3_DistSq(Vec3f *a, Vec3f *b);
f32 Snd3D_GetVolume(void *a) {
    f32 d = Vec3_DistSq(a, &gListenerPos);

    d = sqrtf(d);

    if (d >= 2400.0) {
        return 0.0f;
    }
    return (2400.0 - d) / 2400.0;
}


f32 Snd3D_ApproachDistance(Vec3f *a, Vec3f *off, Vec3f *b);
/* Doppler pitch offset (clamped to +-7) for a source at pos moving at vel. */
f32 Snd3D_GetDoppler(void *pos, void *vel) {
    f32 a;
    f32 b;
    f32 ratio;

    /* The listener velocity (gListenerVel) directly follows its position (gListenerPos). */
    a = Snd3D_ApproachDistance(pos, &((Vec3f *)&gListenerPos)[1], (Vec3f *)&gListenerPos);
    b = Snd3D_ApproachDistance((Vec3f *)&gListenerPos, vel, pos);
    if (b == 1.0f) {
        return 0.0f;
    }
    ratio = (a / 13500.0 + 1.0) / (1.0 - b / 13500.0);
    if (ratio < 1.0f) {
        ratio = 1.0f / ratio;
        if (ratio <= 2.0f) {
            return -(ratio - 1.0f) * 7.0f;
        }
        return -7.0f;
    }
    if (ratio > 1.0f) {
        if (ratio <= 2.0f) {
            return (ratio - 1.0f) * 7.0f;
        }
        return 7.0f;
    }
    return 0.0f;
}

INCLUDE_ASM("asm/nonmatchings/unit_01050", Snd3D_GetPan);

f32 Snd3D_ApproachDistance(Vec3f *a, Vec3f *off, Vec3f *b) {
    f32 d[3];
    f32 len;
    f32 nx;
    f32 ny;
    f32 nz;
    f32 da;

    d[0] = a->x - b->x;
    d[1] = a->y - b->y;
    d[2] = a->z - b->z;
    len = sqrtf(d[0] * d[0] + d[1] * d[1] + d[2] * d[2]);
    if (len == 0.0f) {
        return 0.0f;
    }
    nx = d[0] / len;
    ny = d[1] / len;
    nz = d[2] / len;
    da = -(nx * a->x + ny * a->y + nz * a->z);
    return len + (nx * (b->x + off->x) + ny * (b->y + off->y) + nz * (b->z + off->z) + da);
}

f32 Vec3_DistSq(Vec3f *a, Vec3f *b) {
    f32 dx = a->x - b->x;
    f32 dy = a->y - b->y;
    f32 dz = a->z - b->z;

    return dx * dx + dy * dy + dz * dz;
}

void Audio_LoadWaveBank(u32 devAddr, u32 size, s32 a) {
    nuPiReadRom(devAddr, (void *)0x80010400, size);
    Snd_LoadWaveBank((void *)0x80010400, a);
}


void Audio_LoadWaveBankEx(u32 a, u32 sizeA, u32 b, u32 sizeB) {
    nuPiReadRom(a, (void *)0x80010400, sizeA);
    gWaveTblBuffer = Memory_Alloc(sizeB);
    nuPiReadRom(b, gWaveTblBuffer, sizeB);
    Snd_LoadWaveBank((void *)0x80010400, (s32)gWaveTblBuffer);
}


void Audio_LoadSongData(s32 arg0, u32 arg1) {
    nuPiReadRom(arg0, 0x80020400, (arg1 & 1) + arg1);
}

void Audio_LoadSfxBank(s32 arg0, u32 arg1) {
    nuPiReadRom(arg0, 0x80000400, (arg1 & 1) + arg1);
    Snd_LoadSfxBank((void *)0x80000400);
}
