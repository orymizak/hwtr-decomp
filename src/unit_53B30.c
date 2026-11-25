#include "common.h"
#include "input.h"
#include "pak.h"
#include "save.h"
#include "race.h"
#include "profile.h"

/* Key newly pressed on any of the four pads. */
#define INPUT_PRESSED_ANY_PAD(key) \
    (Input_PressedOnce(0, key) || Input_PressedOnce(1, key) || Input_PressedOnce(2, key) || \
     Input_PressedOnce(3, key))
#include "ultra64.h"
#include "math.h"
#include "include_asm.h"
/* TODO(match-debt): empty loop needed to match (scheduling barrier). Assumed to be a debug hook
 * compiled out in release builds; verify against a real cause when one turns up. */
#define DEBUG_NOP() do { } while (0)

/* Per-player race result block (0x2C) inside the race state. */
typedef struct { s32 lapTime; u8 lap; u8 p5[3]; s32 raceTime; u8 pC[0x20]; } RaceRes_;
typedef struct {
    /* 0x00 */ s32 outcome;
    /* 0x04 */ u32 carUnlocksP1[2]; /* cars unlocked by this race, per player */
    /* 0x0C */ u32 carUnlocksP2[2];
    /* 0x14 */ u32 trackUnlocksP1;  /* tracks unlocked by this race, per player */
    /* 0x18 */ u32 trackUnlocksP2;
    /* 0x1C */ u8 p1C[0x20];
    /* 0x3C */ RaceRes_ res[2];
} RaceSt_;
typedef struct { s32 a; s32 b; s32 c; } V12_;
extern void (*sysModelLoadAsync)(s32, char *, s32);
extern void (*sysCarInfoLoad)(char *, void *, void *);
extern void (*D_80140FD8)(void);
void HighScores_UpdateArrows(void);
extern void (*sysScreenSetBgAlpha)(s32);
extern u8 (*sysPakRepair)(u8);
extern s32 D_80124CE4;
extern s32 D_80124CE8;
extern s32 D_80124CEC;
extern s32 D_80124CF0;
extern s32 D_80124CF4;
extern s32 D_80124CF8;
extern u8 D_80124CFC;
extern s32 gTrackNames[][3];
extern void (*sysWorldLoad)(u8, s32);
s8 NameList_Add(void *);
extern s16 D_800F616C;
extern s16 D_800F61D0;
extern u8 gProfileChoice;
extern void (*sysInputSaveBindings)(s32, s32);
u8 Cup_IsUnlocked(u8 i);
typedef struct { u8 a; u8 pad; s16 b; s16 c; u8 pad2[4]; } E0A_;
typedef struct { u8 pad[2]; u8 n; u8 pad2; E0A_ *arr; u8 pad3[8]; u8 r; u8 g; u8 bl; } T39_;
extern void (*sysScreenSetClearColor)(s32, s32, s32);
extern void (*sysScreenBeginFrame)(void);
extern void (*sysCameraSetPerspective)(f32, f32, f32, f32);
extern void (*sysAudioSetSfxVolume)(s32);
extern u8 (*sysIsPAL)(void);
extern u8 (*sysInputIsPad1Connected)(void);
extern void (*sysInputUpdate)(s32);
extern u8 (*sysInputBindKey)(s32, s32);
extern u8 (*sysGameEndRace)(void *);
extern u8 (*sysGameUpdate)(void);
extern void (*sysSfxPlay)(s32, s32, f32, f32);
extern Vec3f gMenuCarPos[];
s32 strcmp(const char *, const char *);
u32 SaveSlot_GetCount(u8);
extern u8 gChampProfileChoice;
void Controls_UpdateArrowsP1(void);
u8 Controls_HasConflict(void);
extern u8 D_80124C7E;
extern u8 D_80124C7F;
extern u8 D_80124160;
extern u8 D_801249F0;
extern u8 D_80124B30;
void Save_InitDefault(void *);
void Menu_SetupCamera(void);
extern u8 gNameChangedP1;
extern u8 gNameChangedP2;
extern s32 D_80124C78;
void Menu_LoadModels(s32);
s32 KeyRepeat_IsDue(u8 i);
s32 Pak_AnyChanged(void);
extern u8 gRaceResult;
extern s32 D_800F60E8[];
u8 Save_Probe(void);
void KeyRepeat_Accelerate(u8 i);
extern u8 gSaveLoaded;
extern u8 D_80100F88[];
typedef struct { u8 p0[0x10]; s32 unk10; f32 unk14; } ObjG1_;
typedef struct { s32 d[0x88C / 4]; } Big88C_;
typedef struct { u8 a; u8 pad; s16 x; s16 y; u8 pad2[4]; } E0A4_;
typedef struct { char *name; u8 pad[0xC]; } N10_;
typedef struct { f32 x; f32 y; f32 z; f32 x2; f32 y2; u8 pad[0x1C]; u16 h30; u16 h32; } E34b_;
typedef struct { u8 pad[0x20]; u8 n; u8 p[3]; N10_ *names; E34b_ *objs; } TBL_;
typedef struct { u16 idx; u8 pad[2]; s32 lo; s32 hi; s32 y; s32 reset; f32 x; } Scroll4E_;
typedef struct { u8 a; u8 b; u8 c; s8 lv; } Bars4_;
typedef struct {
    char text[0x18];
    u8 b18;
    u8 p19[7];
    s16 x;
    s16 y;
    s16 z;
    u8 align;
    u8 style;
    u8 sheet;
    u8 r;
    u8 g;
    u8 b;
} It2C_;
#define b0 text[0]
typedef struct { u8 n; u8 p1[3]; It2C_ *it; } Ls2C_;
typedef struct { s32 a; s32 b; s32 c; } V12c_;
typedef struct { u8 *s; s16 n; s16 x6; s16 x8; s16 y; u8 pC[2]; u8 e; u8 f; u8 g; u8 p11[3]; } ItAA4_;
typedef struct { u8 p0[0x1C]; ItAA4_ *it; } MnAA4_;
typedef struct { f32 a; f32 b; f32 c; f32 d; } V4B37_;
typedef struct { Vec3f p; u8 pC[0xC]; f32 f18; f32 f1C; f32 f20; u8 p24[0xE]; s16 s32; } EnB37_;
typedef struct { s32 *a; u8 p4[0xC]; } RfB37_;
typedef struct { u8 p0[0x20]; u8 n; u8 p21[3]; RfB37_ *r; EnB37_ *e; } ObB37_;
typedef struct { u8 p0[0xC]; f32 fC; f32 f10; u8 p14[0x20]; } ElC05_;
typedef struct { char *name; u8 n; u8 p5[3]; s32 v[3]; } Nm5010_;
typedef struct { char *s; u8 p4[0xC]; } NmC0_;
typedef struct { f32 x; f32 y; u8 p8[4]; f32 xC; f32 x10; u8 p14[0x1C]; s16 h30; s16 h32; } ElC0_;
typedef struct { u8 p0[0x20]; u8 n; u8 p21[3]; NmC0_ *names; ElC0_ *elems; } MnC0_;
typedef struct { char *s; u8 p4[0xC]; } NmC3_;
typedef struct { f32 x; f32 y; u8 p8[4]; f32 xC; f32 x10; u8 p14[0x20]; } ElC3_;
typedef struct { u8 p0[0x20]; u8 n; u8 p21[3]; NmC3_ *names; ElC3_ *elems; } MnC3_;
extern u8 D_80100CB4;
extern u8 D_80100CCC;
extern u8 D_80100CDC;
extern u8 D_800F6474;
s32 *Cheat_GetBufferP1(void);
void Cheat_ClearP1(void);
void MainMenu_InputP1(s32 id);
extern u8 D_800F6475;
s32 *Cheat_GetBufferP2(void);
void Cheat_ClearP2(void);
s32 Config_CheckCheatCode(char *code, void *cfg);
void MainMenu_InputP2(s32);
extern char D_80100FEC[];
void Register_PlaceCursor(s32 x, s32 y);
extern s32 D_800F5F70;
extern s32 D_800F5F74;
extern u8 D_800F63D0;
s32 Save_CreateOnPak(u32 i);
void Menu_Advance(void);
extern u8 D_801410A8[];
extern u8 D_800F6400;
extern char *D_800F649C[];
void Controls_PlaceArrow(u8 which, u8 name, s32 x, s32 y, u8 val);
extern u8 D_80100DC0;
extern u8 D_80100DC8;
u8 Lang_Load(void *);
u8 CarNames_Load(void *);
extern s32 D_801411D0;
extern s32 D_801411D4;
extern s32 D_801411D8;
extern s32 D_801411DC;
extern s32 D_801411E0;
extern s32 D_801411E4;
extern s32 D_8012406C;
extern s32 D_80124070;
extern s32 D_80124074;
extern s32 gGameModeNames;
extern s32 D_8012407C;
extern s32 D_80124080;
extern s32 D_80124084;
extern s32 D_80124088;
extern s32 D_80124110[];
extern Nm5010_ D_800F61F0[18];
void Menu_DrawModel(u8, f32, f32, f32, f32, f32, f32, f32);
void Menu_InitViewMtx(f32 (*m)[4]);
extern void (*sysCameraApply)(f32 (*)[4]);
extern u8 D_800F648F;
extern u8 D_800F648E;
void HighScores_PickRandom(void);
extern u8 D_80100BD8;
void MenuList_Reset(s32 arg0);
s32 Car_IsUnlocked(u8 i, Profile *p);
extern void (*sysWorldDraw)(u8);
extern void (*sysFontLoad)(s32, void *);
extern u8 D_80100F94;
void Music_PlaySong(void);
extern s32 (*sysScreenEndFrame)();
s32 Menu_EndFrame();
s32 Menu_LoadBg(s32 *a);
extern RaceSetup gRaceSetup;
extern u8 D_80124D10[];
extern s32 D_800F9FD0[];
extern u8 D_800FA010[][3];
extern s32 D_80101044[];
extern s32 D_80101050[];
extern u8 D_8010105C;
extern u8 D_80101064;
extern s32 (*sysGameStartRace)(void *);
void *memcpy(void *, const void *, size_t);
extern u8 *gLangText[];
extern s32 (*sysFontGetCharWidth)(s32, u8);
s8 Font_GetKerning(s32, u8, u8);
s32 SaveSlot_Add(u32 i, Profile *src);
extern char D_80100F70[];
extern s32 *D_80141268;
s32 *Jukebox_GetArtist(s32);
s32 *Jukebox_GetTitle(s32);
void Options_UpdateSongTexts(void);
extern u8 D_80100DF4;
extern s32 *D_80141480;
extern u8 D_80100E08;
extern ObjG1_ D_800F6358;
u8 Save_ReadAll(void);
void *Save_GetActiveProfile(void);
s32 SaveSlot_FindPlayer(s32);
extern u8 D_80100DAC;
extern u8 D_80100D94;
extern s32 *D_8014158C;
extern u8 D_80100D48;
extern u8 D_80100D58;
extern u8 D_80100D68;
extern u8 D_80100D7C;
extern s32 *D_80141464;
extern s32 *D_80141468;
extern s32 *D_8014146C;
extern u8 D_80100CC8;
extern u8 D_80100CF0;
extern u8 D_80100D08;
extern u8 D_80100D20;
extern u8 D_80100CAC;
extern s32 *D_8014124C;
extern s32 *D_80141250;
extern u8 D_80100D88;
extern s32 *D_80141470;
extern s32 *D_80141474;
extern s32 *D_80141478;
extern s32 *D_80141258;
extern s32 *D_8014125C;
extern u8 D_80100D38;
extern s32 *D_80141454;
extern s32 *D_80141458;
extern s32 *D_80141234;
extern s32 *D_80141238;
extern s32 *D_8014123C;
extern s32 *D_80141240;
extern s32 *D_80141244;
extern u8 D_80100FAC;
extern u8 D_80100FB4;
extern u8 D_80100FBC;
extern u8 D_80100FC4;
extern u8 D_80124CA0;
extern u8 D_80124CC0;
extern char *D_800F64A8[];
void HighScores_PlaceArrow(s32 i, s32 x, s32 y, s32 c);
extern char *D_800F6478[];
void Championship_PlaceArrow(s32 i, s32 x, s32 y, s32 c);
extern char *D_800F646C[];
void MainMenu_PlaceArrow(s32 i, s32 x, s32 y, s32 c);
extern s32 D_80141504;
extern s32 D_80141508;
extern s32 D_8014150C;
extern u8 D_80100CA0;
extern u8 D_80100EB4;
extern u8 D_80100F34;
extern u8 D_80100F48;
extern s32 *D_80141248;
extern s32 *D_80141254;
extern s32 D_80100DBC;
extern u8 D_80100E74;
extern ObjG1_ D_800F63A0;
extern Bars4_ D_801240E4;
extern u8 D_80100F0C;
extern u8 D_80100F14;
extern u8 D_80100F1C;
void Menu_SetText(Ls2C_ *m, u8 idx, void *str, s32 x, s32 y, s32 z, s32 align, s32 style, u8 sheet, u8 r, u8 g, u8 b);
extern Bars4_ D_801240E0;
extern u8 D_80100FA4;
extern ObjG1_ D_800F63B8;
extern void (*sysInputLoadBindings)(s32, s32);
extern void (*sysAudioSetMusicVolume)(s32);
extern void (*sysAudioSetVoiceVolume)(s32);
extern void (*sysMenuExitHook)(void);
extern void (*sysScreenFreeBg)(void);
void MenuFont_FreeAll(void);
void Menu_FreeAll(void);
extern u32 D_800F6078[];
u8 Input_PressedOnce(u8 i, s32 k);
extern void (*sysAudioSetAmbientVolume)(s32);
void MenuFont_LoadAll(void);
void MenuObjs_Alloc(void);
Big88C_ *Save_GetProfile(s32);
void Records_SaveRaceResult(void *);
void Track_SelectPrev(void);
extern u8 gPasswordP1;
extern u8 gPasswordP2;
s32 Password_Apply(void *, Profile *);
void Password_Get(s32, s32 *);
void Track_SelectNext(void);
extern s32 D_8014122C;
extern u8 D_80100CA4;
void Profile_InitDefault(void *arg0, s32 arg1);
void Controls_UpdateArrowsP2(void);
extern u8 D_800F648C;
extern u8 D_800F648D;
extern u8 gPakErrorPort;
extern u8 D_801240D5;
extern u8 gCupWon;
extern s8 D_800F60D4[][6];
void Garage_SetCarTextsP1(void);
void Garage_SetCarTextsP2(void);
extern s32 D_801240F4;
extern u8 D_80124C95;
void Menu_ResetTicker(ObjG1_ *o);
extern u8 D_800F6476;
s32 Menu_IsKeyHeld(s32, s32);
extern u8 D_800F6480;
extern u8 D_80124C94;
extern void (*sysFontDrawChar)(s32, s32, s32, s32, s32);
extern void (*sysDraw2DSetColor)(s32, s32, s32);
extern void Fsm_Trigger(void *, s32);
extern GameSave *gActiveSave;
extern u8 D_80123EBE;
extern u8 D_80123EBF;
extern u8 D_80123EC1;
s32 MenuCar_Draw(s32, s32, s32, s32);
void MenuGlobe_Draw(s8);
/* A menu text: a run of positioned glyphs drawn from one font sheet. */
typedef struct {
    /* 0x00 */ char str[0x18];
    /* 0x18 */ u8 nChars;
    /* 0x19 */ u8 p19[3];
    /* 0x1C */ E0A4_ *chars;
    /* 0x20 */ s16 x;
    /* 0x22 */ s16 y;
    /* 0x24 */ s16 z;
    /* 0x26 */ u8 align;
    /* 0x27 */ u8 style;
    /* 0x28 */ u8 sheet;
    /* 0x29 */ u8 r, g, b;
} MnText_; /* size = 0x2C */
typedef struct { u8 p0[0x10]; u8 r; u8 g; u8 b; u8 p13[5]; } MnSprite_;
typedef struct {
    u8 nText; u8 p1[3]; MnText_ *text;
    u8 nSprites; u8 p9[3]; MnSprite_ *sprites;
    u8 nButtons; u8 p11[3]; MnSprite_ *buttons;
    u8 nItems; u8 p19[0x13];
    u8 hiR; u8 hiG; u8 hiB;
} Menu_;
void Menu_Draw(Menu_ *m, s8 selText, s8 selButton, s32 arg3, s8 selItem, s32 arg5, s32 arg6, u8 noText);
extern s32 D_801240E8;
extern s32 D_801240EC;
extern s32 D_801240F0;
extern u8 (*sysModelPollLoad)();
extern u8 D_801240DF;
extern u8 D_80100FE4;
extern void *D_800F5F10;
extern void *D_800F5F0C;
extern s16 D_800F610C[];
extern s16 D_800F6170[];
extern u8 gRegisterKey;
void MenuCar_Show(u8 i);
u8 Keyboard_LoadCharset(s32);
extern u8 D_80100FDC;
extern u8 D_801240D6;
extern s8 D_801240D7;
extern s32 D_801240D8;
extern u8 D_801240DC;
extern s8 D_801240DD;
extern s8 D_801240DE;
extern s8 gRenamePending;
extern void *D_80124CE0;
void * SaveSlot_Get(u32, u32);
extern s32 D_80124150;
extern s32 D_80124154;
extern void *D_800F5F6C;
extern u8 gKeyboardText;
extern u8 D_800F64A4;
extern s8 gControlsRemapP2;
extern s8 gControlsRemapP1;
s32 Scroller_Reset(void *);
extern u8 D_800F2FA0;
extern u8 D_800F2FC0;
extern s8 D_800F61D6;
extern u8 D_800F63E8;
extern u8 D_80100F80;
extern u8 gMusicMode;
extern u8 gMenuMusicTrack;
extern u8 D_800F6490;
extern u8 D_80100F60;
extern s8 gKeyRepeatCountP2;
extern s8 gGarageDirtyP2;
void MenuCar_MoveTo(s32 i, s32 x, s32 y, s32 z);
extern u8 D_80100F58;
extern u8 gKeyRepeatCountP1;
extern s8 gGarageDirtyP1;
extern u8 gGarageReadyP1;
extern u8 gGarageReadyP2;
extern u8 gMainMenuSel;
extern u8 gKeyRepeatTime;
void Menu_StartDim(void);
void Menu_StartFadeOut(void);
void Menu_StartFadeIn(void);
s32 Tuning_Load(void);
extern s8 gMenuMusicStarted;
s32 Music_GetSongTrack(u32);
extern s8 gMenuSong;
extern u8 gMenuFsm;
extern char *gCarModelNames[];
extern f32 D_80123E88;
extern f32 D_80123E8C;
extern f32 D_80123E90;
f32 MenuPos_ToWorldX(s32);
f32 MenuPos_ToWorldY(s32);
f32 MenuPos_ToWorldZ(s32);
extern f32 D_80123E70;
extern Vec3f D_80123E74;
void MenuGlobe_Hide(void);
extern f32 D_80123E80;
extern f32 D_80123E84;
extern f32 D_80123E94;
extern u8 D_80123E98;
void MenuCar_Hide(u8 i);
/* Per-player camera zoom and field of view (current/target, radians). */
extern f32 D_800F60A0[2];
extern f32 D_800F60A8[2];
extern f32 D_800F60B0[2];
extern f32 D_800F60B8[2];
extern u8 D_800F60C0[2];

extern s32 (*sysScreenLoadBg)(s32);
extern s8 gPakDialogSel;
s32 Menu_LayoutTexts(s32);
extern s32 D_800F5F18;
extern s32 D_800F5F1C;
extern s32 (*audMusicStop)();
extern s8 gMusicFadeMode;
extern s32 D_800F6468;
extern s32 (*sysTimeGetMillis)();
s32 Menu_FreeModels();
extern s32 (*sysScreenAllocBg)();
s32 Lang_Free();
extern s32 (*sysMenuExit)();
s32 Menu_StartEnter(s32);
extern s32 gMenuObjs[];
s32 Menu_HandlePad1(s32);
extern s32 D_80100DD0;
extern u8 gMenuFadeMode;
extern u8 gMenuFadeLevel;
extern u8 gMenuHideText;
extern s32 D_800F5F20;
extern s32 D_80100DDC;
extern s32 D_80100DE4;
extern s32 D_800F5F24;
extern s32 D_80100DEC;
extern s8 gSplashSeen;
extern s8 D_80124C75;
s32 Menu_ResetTexts(s32);
extern s32 D_800F5F14;
extern s32 gKeyRepeatDelay;
extern s32 D_80124C88;
extern s32 (*sysModelAllocBuffers)();
extern s32 (*sysModelFreeBuffers)();
extern s32 (*sysModelFree)(s32);
extern s32 (*D_80140FDC)();
s32 NameList_Find(s32 *);
s32 NameList_Remove(s32 *);
extern s32 D_80100E10;
extern s32 (*sysWorldFree)(s32);
extern s32 D_800F5F28;
s32 Menu_Nop(s32);
extern s32 D_800F5F2C;
extern s8 gInChampionship;
extern s8 gChampionshipSel;
s32 Menu_StartLeave(s32);
extern s32 D_800F5F34;
extern s32 D_80100ED4;
extern s32 D_800F5F38;
extern s32 D_80100EFC;
extern s32 D_80124C98;
extern s32 D_800F5F3C;
extern s32 D_80100F04;
extern s32 D_800F5F30;
extern s8 gCupQuitChoice;
extern s32 D_800F6484;
extern s32 D_800F6488;
extern s32 D_800F5F40;
extern s32 D_800F5F44;
extern u8 gGarageCarP1;
extern Profile *gProfileP1;
extern u8 gNumPlayers;
extern u8 gGarageCarP2;
extern Profile *gProfileP2;
extern s32 D_800F5F48;
extern s32 D_800F6370;
extern s32 D_80100F78;
extern u8 gOptionsSel;
extern s32 D_800F5F60;
extern s32 (*sysFontFree)(s32);
extern u8 D_80124107;
extern u8 D_80124109;
extern u8 D_8012410B;
extern u8 D_80124108;
extern u8 D_8012410A;
extern u8 D_8012410C;
extern s32 D_800F5F4C;
extern s32 D_800F5F50;
extern s32 D_800F5F54;
extern s32 D_800F5F58;
extern s32 (*sysInputResetLayout)(s32);
extern s32 D_800F5F5C;
s32 Menu_HandlePad2(s32);
s32 Pak_GetStatus(u8);
extern u8 gLoadPakP1;
extern u8 gLoadSlotP1;
extern u8 gLoadPakP2;
extern u8 gLoadSlotP2;
extern s32 D_800F5F64;
extern s32 D_800F5F68;
extern s8 D_801240B3;
extern s8 D_801240B4;
extern s8 D_801240B6;
extern s8 D_801240B2;
extern u8 D_801240B5;
extern u8 gLoadPlayer;
extern u8 gDeleteConfirmSel;
s32 Save_Commit();
s32 SaveSlot_Delete(u8, s32);
extern s32 D_800F5F7C;
extern s32 D_800F5F78;
extern s32 D_800F5F80;
extern u8 gHighScoresAutoCycle;
extern u8 gHighScoresCycleTicks;
extern u8 gHighScoresPage;
extern s32 D_800F5F88;
extern s32 D_80101014;
extern s8 D_80124CFD;
extern s8 D_80124CFE;
extern s8 D_80124CFF;
extern s8 D_80124D00;
extern s8 D_80124D01;
extern s32 D_80101020;
extern s32 D_80124D04;
extern s8 D_80124D08;
extern s8 D_80124D09;
extern s32 (*sysRandRange)(s32);
s32 strcpy(s32 *, s32);
extern s32 D_801345B8;
extern s32 D_80138D20;

extern u32 D_800F6050[];
extern s32 (*sysTimeGetMillis)();

/* TODO (Bug potential): Discontinuity in delta time calculation. In Menu_DeltaSeconds, delta > 500 clamps to 0.5f (d / 1000.0f).
 * Here, if delta > 100, ret is set to 0.1f instead of 1.0f (delta / 100.0f), causing an abrupt 10x drop during lag spikes.
 * Clean version:
 *   ret = (delta > 100) ? 1.0f : (f32)delta / 100.0f;
 */
f32 Menu_DeltaTenths(u8 arg0) {
    u32 now;
    u32 delta;
    f32 ret;

    if (D_800F6050[arg0] == 0) {
        ret = 0.15f;
        D_800F6050[arg0] = sysTimeGetMillis();
    } else {
        now = sysTimeGetMillis();
        delta = now - D_800F6050[arg0];
        if (delta > 100) {
            ret = 0.1f;
        } else {
            ret = (f32)delta / 100.0f;
        }
        D_800F6050[arg0] = now;
    }
    return ret;
}


f32 Menu_DeltaSeconds(u8 i) {
    f32 r;
    u32 now;
    u32 d;

    if (D_800F6078[i] == 0) {
        r = 0.15f;
        D_800F6078[i] = sysTimeGetMillis();
    } else {
        now = sysTimeGetMillis();
        d = now - D_800F6078[i];
        if (d > 500) {
            r = 0.5f;
        } else {
            r = d / 1000.0f;
        }
        D_800F6078[i] = now;
    }
    return r;
}


void Menu_SubmitModel(s8 a) {
    if (a >= 0) {
        sysWorldDraw(a);
    }
}


s32 Menu_LoadBg(s32 *a) {
    return sysScreenLoadBg((s32)a);
}


void Menu_FreeBg(void) {
    sysScreenLoadBg(0);
}

/* Loads the 3x4 identity matrix and moves it back by the camera distance (866 units). */
void Menu_InitViewMtx(f32 (*m)[4]) {
    m[0][0] = m[0][1] = m[0][2] = m[0][3] = m[1][0] = m[1][1] = m[1][2] = m[1][3] = m[2][0] = m[2][1] = m[2][2] = m[2][3] = 0.0f;
    m[0][0] = m[1][1] = m[2][2] = 1.0f;
    m[0][3] += m[0][0] * 0.0f + m[0][1] * 0.0f + m[0][2] * -866.0f;
    m[1][3] += m[1][0] * 0.0f + m[1][1] * 0.0f + m[1][2] * -866.0f;
    m[2][3] += m[2][0] * 0.0f + m[2][1] * 0.0f + m[2][2] * -866.0f;
}

void Menu_SetupCamera(void) {
    sysScreenSetClearColor(0, 0, 0);
    sysScreenBeginFrame();
    sysCameraSetPerspective(CAM_NEAR, CAM_FAR, CAM_FOV_60DEG, CAM_ASPECT_4_3);
}


s32 Menu_EndFrame() {
    return sysScreenEndFrame();
}


void Menu_DrawChar(u8 arg0, u8 arg1, s32 arg2, s32 arg3, u8 arg4, u8 arg5, u8 arg6, u8 arg7) {
    s32 n = arg3 + 1;

    sysDraw2DSetColor(arg5, arg6, arg7);
    sysFontDrawChar(arg1, arg0, arg2, n, arg4);
}

/* Places an object: applies the pre-scale translation (shrunk with depth z), uniform scale and X/Y/Z
 * rotations to the current matrix, then submits it. */
void Menu_DrawModel(u8 id, f32 x, f32 y, f32 z, f32 rx, f32 ry, f32 rz, f32 scale) {
    f32 m[3][4];
    f32 k;

    Menu_InitViewMtx(m);
    k = (866.0f - z) / 866.0f;
    scale *= k;
    x *= k;
    y *= k;
    m[0][3] += m[0][0] * x + m[0][1] * y + m[0][2] * z;
    m[1][3] += m[1][0] * x + m[1][1] * y + m[1][2] * z;
    m[2][3] += m[2][0] * x + m[2][1] * y + m[2][2] * z;
    m[0][0] *= scale;
    m[0][1] *= scale;
    m[0][2] *= scale;
    m[1][0] *= scale;
    m[1][1] *= scale;
    m[1][2] *= scale;
    m[2][0] *= scale;
    m[2][1] *= scale;
    m[2][2] *= scale;

    {
        f32 c = cosf(rx);
        f32 s = sinf(rx);
        f32 a, b;

        a = m[0][1] * c + m[0][2] * s;
        b = m[0][2] * c - m[0][1] * s;
        m[0][1] = a;
        m[0][2] = b;
        a = m[1][1] * c + m[1][2] * s;
        b = m[1][2] * c - m[1][1] * s;
        m[1][1] = a;
        m[1][2] = b;
        a = m[2][1] * c + m[2][2] * s;
        b = m[2][2] * c - m[2][1] * s;
        m[2][1] = a;
        m[2][2] = b;
    }
    {
        f32 c = cosf(ry);
        f32 s = sinf(ry);
        f32 a, b;

        a = m[0][0] * c - m[0][2] * s;
        b = m[0][2] * c + m[0][0] * s;
        m[0][0] = a;
        m[0][2] = b;
        a = m[1][0] * c - m[1][2] * s;
        b = m[1][2] * c + m[1][0] * s;
        m[1][0] = a;
        m[1][2] = b;
        a = m[2][0] * c - m[2][2] * s;
        b = m[2][2] * c + m[2][0] * s;
        m[2][0] = a;
        m[2][2] = b;
    }
    {
        f32 c = cosf(rz);
        f32 s = sinf(rz);
        f32 a, b;

        a = m[0][0] * c + m[0][1] * s;
        b = m[0][1] * c - m[0][0] * s;
        m[0][0] = a;
        m[0][1] = b;
        a = m[1][0] * c + m[1][1] * s;
        b = m[1][1] * c - m[1][0] * s;
        m[1][0] = a;
        m[1][1] = b;
        a = m[2][0] * c + m[2][1] * s;
        b = m[2][1] * c - m[2][0] * s;
        m[2][0] = a;
        m[2][1] = b;
    }
    sysCameraApply(m);
    Menu_SubmitModel(id);
}

void Menu_DrawModels(ObB37_ *o) {
    u8 i;
    s32 r;

    for (i = 0; i < o->n; i++) {
        r = NameList_Find(o->r[i].a);
        Menu_DrawModel(r, o->e[i].p.x, o->e[i].p.y, o->e[i].p.z, o->e[i].f18, o->e[i].f1C, o->e[i].f20, o->e[i].s32 / 100.0f);
    }
}

void Menu_DrawText(MnText_ *t, u8 mode, u8 c) {
    u8 r = 0;
    u8 g = 0;
    u8 b = 0;
    s32 i;

    if (mode == 0) {
        r = t->r;
        g = t->g;
        b = t->b;
    }
    for (i = 0; i < t->nChars; i++) {
        E0A4_ *e = &t->chars[i];

        if (mode != 0) {
            Menu_DrawChar(e->a, t->sheet, e->x, e->y, mode, c, c, c);
        } else {
            Menu_DrawChar(e->a, t->sheet, e->x, e->y, 0, r, g, b);
        }
    }
}

void Menu_DrawSprite(T39_ *t, u8 r, u8 g, u8 b) {
    s32 i;

    for (i = 0; i < t->n; i++) {
        E0A_ *e = (E0A_ *)((u8 *)t->arr + i * 10);

        Menu_DrawChar(e->a, 1, e->b, e->c, 0, t->r, t->g, t->bl);
    }
}


void Menu_DrawButton(u8 *t, u8 r, s32 g, s32 b, u8 mode, s32 unused, u8 c) {
    s32 one = 1;
    s32 i;

    for (i = 0; i < t[2]; i++) {
        E0A4_ *e = &(*(E0A4_ **)(t + 4))[i];

        if (mode != 0) {
            Menu_DrawChar(e->a, one, e->x, e->y, mode, c, c, c);
        } else {
            Menu_DrawChar(e->a, one, e->x, e->y, 0, r, g, b);
        }
    }
}


void Menu_DrawItem(MnAA4_ *m, u8 idx, u8 col) {
    ItAA4_ *it;
    s32 i;
    s32 x;
    s32 y;

    it = &m->it[idx];
    i = 0;
    y = it->y;
    x = (it->x6 + it->x8) / 2 - (it->n * 15) / 2;
    for (; i < it->n; i++, x += 15) {
        Menu_DrawChar(it->s[i], 0, x, y, col, it->e, it->f, it->g);
    }
}

/* TODO (Clean version): [evaluated 2026-11-18: the angle is now CAM_FOV_130DEG. A PlayerCam
 * struct array cannot match: the data is laid out as five separate [2] arrays (struct of
 * arrays), so an array of structs would change every address.]
 * Unify per-player camera arrays into a struct and define angle constant:
 *   #define CAM_DEFAULT_FOV_RAD 2.2689264f // 130.0 degrees in radians (130 * M_PI / 180)
 *   typedef struct {
 *       f32 zoomCur; // D_800F60A0
 *       f32 zoomTgt; // D_800F60A8
 *       f32 fovCur;  // D_800F60B0
 *       f32 fovTgt;  // D_800F60B8
 *       u8  active;  // D_800F60C0
 *   } PlayerCam;
 */
void MenuCar_Init(u8 i) {
    D_800F60C0[i] = 0;
    D_800F60B0[i] = CAM_FOV_130DEG;
    D_800F60B8[i] = CAM_FOV_130DEG;
    D_800F60A0[i] = 1.0f;
    D_800F60A8[i] = 1.0f;
}

s32 MenuCar_ResetAngle(u8 i) {
    D_800F60B8[i] = CAM_FOV_130DEG;
    D_800F60B0[i] = CAM_FOV_130DEG;
    return 1;
}

void MenuCar_Show(u8 i) {
    if (i < 2) {
        D_800F60A0[i] = 0.0f;
        D_800F60C0[i] = 0;
        D_800F60A8[i] = 1.0f;
        D_800F60B8[i] = CAM_FOV_130DEG;
    }
}

void MenuCar_Hide(u8 i) {
    if (i < 2) {
        D_800F60A8[i] = 0.0f;
        D_800F60C0[i] = 0;
    }
}

void MenuCar_ScaleTo150(u8 i) {
    if (i < 2) {
        D_800F60C0[i] = 0;
        D_800F60A8[i] = 1.5f;
    }
}

void MenuCar_ScaleTo100(u8 i) {
    if (i < 2) {
        D_800F60C0[i] = 0;
        D_800F60A8[i] = 1.0f;
        D_800F60B8[i] = CAM_FOV_130DEG;
    }
}

void MenuCar_ScaleTo175(u8 i) {
    if (i < 2) {
        D_800F60C0[i] = 0;
        D_800F60A8[i] = 1.75f;
    }
}

void MenuCar_ScaleTo200(u8 i) {
    if (i < 2) {
        D_800F60C0[i] = 0;
        D_800F60A8[i] = 2.0f;
    }
}

void MenuCar_SetPos(u8 i, s32 a, s32 b, s32 c) {
    if (i < 2) {
        gMenuCarPos[i].x = MenuPos_ToWorldX(a);
        gMenuCarPos[i].y = MenuPos_ToWorldY(b);
        gMenuCarPos[i].z = MenuPos_ToWorldZ(c);
    }
}


extern Vec3f gMenuCarTarget[];
void MenuCar_MoveTo(s32 arg0, s32 a, s32 b, s32 c) {
    u8 i = arg0;

    if (i < 2) {
        gMenuCarTarget[i].x = MenuPos_ToWorldX(a);
        gMenuCarTarget[i].y = MenuPos_ToWorldY(b);
        gMenuCarTarget[i].z = MenuPos_ToWorldZ(c);
    }
}

INCLUDE_ASM("asm/nonmatchings/unit_53B30", MenuCar_Draw);

void MenuGlobe_SpinLarge(void) {
    D_80123E98 = 0;
    D_80123E94 = 0.75f;
}

void MenuGlobe_SpinSmall(void) {
    D_80123E98 = 0;
    D_80123E94 = 0.5f;
    D_80123E84 = 2.5307255f;
}

void MenuGlobe_Reset(void) {
    D_80123E80 = 0.0f;
    D_80123E98 = 0;
    D_80123E94 = 0.5f;
    D_80123E84 = 2.5307255f;
}

void MenuGlobe_Hide(void) {
    D_80123E94 = 0.0f;
    D_80123E98 = 0;
}

void MenuGlobe_SetPos(s32 arg0, s32 arg1, s32 arg2) {
    D_80123E74.x = MenuPos_ToWorldX(arg0);
    D_80123E74.y = MenuPos_ToWorldY(arg1);
    D_80123E74.z = MenuPos_ToWorldZ(arg2);
}

void MenuGlobe_MoveTo(s32 arg0, s32 arg1, s32 arg2) {
    D_80123E88 = MenuPos_ToWorldX(arg0);
    D_80123E8C = MenuPos_ToWorldY(arg1);
    D_80123E90 = MenuPos_ToWorldZ(arg2);
}

extern s32 D_80100BD0;
/* Draws the rotating menu model (D_80100BD0): eases its angle, scale and position towards the targets set by
 * MenuGlobe_SpinLarge..MenuGlobe_MoveTo (or spins freely when no target angle is set), then submits it. */
void MenuGlobe_Draw(s8 unused) {
    f32 m[3][4];
    f32 k;
    Vec3f d;

    Menu_InitViewMtx(m);
    k = Menu_DeltaTenths(3);
    if (D_80123E98) {
        D_80123E70 += (D_80123E84 - D_80123E70) * k;
    } else {
        D_80123E70 += Menu_DeltaSeconds(3) * 1.256636f;
        while (D_80123E70 > 6.28318f) {
            D_80123E70 -= 6.28318f;
        }
    }
    D_80123E80 += (D_80123E94 - D_80123E80) * k;
    if (D_80123E94 == 0.0f && D_80123E80 < 0.33333334f) {
        D_80123E80 = 0.0f;
    }
    d.x = D_80123E88 - D_80123E74.x;
    d.y = D_80123E8C - D_80123E74.y;
    d.z = D_80123E90 - D_80123E74.z;
    d.x *= k;
    d.y *= k;
    d.z *= k;
    D_80123E74.x += d.x;
    D_80123E74.y += d.y;
    D_80123E74.z += d.z;
    m[0][0] *= D_80123E80;
    m[0][1] *= D_80123E80;
    m[0][2] *= D_80123E80;
    m[1][0] *= D_80123E80;
    m[1][1] *= D_80123E80;
    m[1][2] *= D_80123E80;
    m[2][0] *= D_80123E80;
    m[2][1] *= D_80123E80;
    m[2][2] *= D_80123E80;
    m[0][3] += m[0][0] * D_80123E74.x + m[0][1] * D_80123E74.y + m[0][2] * D_80123E74.z;
    m[1][3] += m[1][0] * D_80123E74.x + m[1][1] * D_80123E74.y + m[1][2] * D_80123E74.z;
    m[2][3] += m[2][0] * D_80123E74.x + m[2][1] * D_80123E74.y + m[2][2] * D_80123E74.z;
    {
        f32 c = cosf(-1.2217295f);
        f32 s = sinf(-1.2217295f);
        f32 a, b;

        a = m[0][1] * c + m[0][2] * s;
        b = m[0][2] * c - m[0][1] * s;
        m[0][1] = a;
        m[0][2] = b;
        a = m[1][1] * c + m[1][2] * s;
        b = m[1][2] * c - m[1][1] * s;
        m[1][1] = a;
        m[1][2] = b;
        a = m[2][1] * c + m[2][2] * s;
        b = m[2][2] * c - m[2][1] * s;
        m[2][1] = a;
        m[2][2] = b;
    }
    {
        f32 c = cosf(D_80123E70);
        f32 s = sinf(D_80123E70);
        f32 a, b;

        a = m[0][0] * c + m[0][1] * s;
        b = m[0][1] * c - m[0][0] * s;
        m[0][0] = a;
        m[0][1] = b;
        a = m[1][0] * c + m[1][1] * s;
        b = m[1][1] * c - m[1][0] * s;
        m[1][0] = a;
        m[1][1] = b;
        a = m[2][0] * c + m[2][1] * s;
        b = m[2][1] * c - m[2][0] * s;
        m[2][0] = a;
        m[2][1] = b;
    }
    sysCameraApply(m);
    sysWorldDraw(NameList_Find(&D_80100BD0));
}

extern u8 gMenuHighlight;
/* Draws a menu: its texts (selText highlighted), sprites, buttons (selButton highlighted) and
 * items (selItem highlighted), pulsing the highlight colour over time. */
void Menu_Draw(Menu_ *m, s8 selText, s8 selButton, s32 arg3, s8 selItem, s32 arg5, s32 arg6, u8 noText) {
    u32 i;

    gMenuHighlight += (s32)(Menu_DeltaSeconds(8) * 450.0f);
    if (noText == 0) {
        Menu_DrawModels(m);
        for (i = 0; i < m->nText; i++) {
            if (selText == (s8)i) {
                Menu_DrawText(&m->text[i], 1, gMenuHighlight);
            } else {
                Menu_DrawText(&m->text[i], 0, 0);
            }
        }
    }
    for (i = 0; i < m->nSprites; i++) {
        Menu_DrawSprite(&m->sprites[i], m->sprites[i].r, m->sprites[i].g, m->sprites[i].b);
    }
    for (i = 0; i < m->nButtons; i++) {
        if (selButton == (s8)i) {
            Menu_DrawButton(&m->buttons[i], m->hiR, m->hiG, m->hiB, 1, 1, gMenuHighlight);
        } else {
            Menu_DrawButton(&m->buttons[i], m->buttons[i].r, m->buttons[i].g, m->buttons[i].b, 0, 1, gMenuHighlight);
        }
    }
    for (i = 0; i < m->nItems; i++) {
        if ((s8)i == selItem) {
            Menu_DrawItem(m, i, 1);
        } else {
            Menu_DrawItem(m, i, 0);
        }
    }
}

void Menu_SetText(Ls2C_ *m, u8 idx, void *str, s32 x, s32 y, s32 z, s32 align, s32 style, u8 sheet, u8 r, u8 g, u8 b) {
    It2C_ *item;
    char *dst;
    char *src;
    char *end;

    if (idx > (s32)(m->n - 1)) {
        return;
    }

    item = &m->it[idx];
    if (str == NULL) {
        item->text[0] = D_80100BD8;
    } else if (strlen(str) >= 25) {
        dst = item->text;
        src = str;
        end = item->text + 24;
        do {
            *dst++ = *src++;
        } while (dst < end);
    } else {
        strcpy(item->text, str);
    }

    item->x = x;
    item->y = y;
    item->z = z;
    item->r = r;
    item->g = g;
    item->b = b;
    item->align = align;
    item->style = style;
    item->sheet = sheet;
}

void MenuList_Reset(s32 arg0) {
    Ls2C_ *l = (Ls2C_ *)arg0;
    u8 i;
    u8 *src;

    i = 0;
    if (l->n != 0) {
        src = &D_80100BD8;
        do {
            l->it[i].b0 = *src;
            l->it[i].b18 = 0;
            i++;
        } while (i < l->n);
    }
}

void Menu_ResetTicker(ObjG1_ *o) {
    o->unk14 = o->unk10;
}


void Menu_DrawTicker(Scroll4E_ *t) {
    u8 *s;
    s32 x;
    s32 n;

    s = gLangText[t->idx];
    x = t->x;
    n = 0;
    for (; *s != 0; s++) {
        if (t->lo < x && x < t->hi) {
            Menu_DrawChar(*s, 1, x, t->y, 1, 0xF0, 0xF0, 0xF0);
            n++;
        }
        if (s[1] != 0) {
            x += sysFontGetCharWidth(0, *s) + Font_GetKerning(0, *s, s[1]);
        } else {
            x += sysFontGetCharWidth(0, *s);
        }
    }
    if (n == 0 && t->x < t->lo) {
        t->x = t->reset;
        return;
    }
    t->x -= Menu_DeltaSeconds(4) * 45.0f;
}

u8 Text_ButtonGlyphs(char *name, u8 *out) {
    u8 i;
    u8 j;

    for (i = 0; i < ARRAY_COUNT(D_800F61F0); i++) {
        if (strcmp(name, D_800F61F0[i].name) == 0) {
            for (j = 0; j < D_800F61F0[i].n; j++) {
                out[j] = D_800F61F0[i].v[j];
            }
            out[j] = 0;
            return 1;
        }
    }
    sprintf((char *)out, (char *)&D_80100CA0, name);
    return 0;
}

s32 CarModel_GetName(u32 arg0) {
    return (s32)gCarModelNames[arg0 & 0xFF];
}

s8 CarModel_FindByName(char *s) {
    u8 i;

    for (i = 0; i < 41; i++) {
        if (gCarModelNames[i] != NULL && strcmp(s, gCarModelNames[i]) == 0) {
            return i;
        }
    }
    return -1;
}


/* Fills a profile (Profile, profile.h) with the defaults for player i: name "Player <i+1>",
 * the initial car and track unlocks, the default key bindings and car 0x1D.
 * TODO (Clean version): [evaluated 2026-11-18: writing it through Profile fields lets gcc load
 * sysInputSaveBindings before the stores (struct stores cannot alias it), which changes the
 * ROM; the byte offsets stay. The proposed "magic" at 0x14 is really carUnlocks[0].] */
void Profile_InitDefault(void *arg0, s32 arg1) {
    u8 *o = arg0;
    s32 buf[4];
    u8 i = arg1;
    u8 j;

    sprintf((char *)buf, (char *)&D_80100CA4, D_8014122C, i + 1);
    strcpy((s32 *)(o + 8), (s32)buf);
    *(u32 *)(o + 0x14) = 0xB48EE52E;
    *(s32 *)(o + 0x18) = 0x10B;
    *(s32 *)(o + 0x1C) = 0xE3F;
    sysInputResetLayout(i);
    sysInputSaveBindings((s32)(o + 0x40), i);
    o[0x20] = 0x1D;
    o[0x21] = 0;
    o[0x22] = 0;
    o[0x23] = 0;
    for (j = 0; j < 6; j++) {
        ((s32 *)(o + 0x24))[j] = 0;
    }
    *(s32 *)(o + 0x3C) = 0;
}


typedef struct { char name[12]; u32 value; } SaveRecord;
typedef struct { SaveRecord e[5]; } SaveRecTable;
/* Save data (gActiveSave): record tables, two per track (12) and one per cup (3). */
typedef struct { u8 p0[4]; SaveRecTable highA[12]; SaveRecTable times[12]; SaveRecTable highB[3]; } SaveData;
void Menu_UpdateFade(void);
#define SAVE_DATA ((SaveData *)gActiveSave)
extern char *D_80141230;

/* TODO(match-debt): register asm for opt and ff preserves GCC 2.7.2 register allocation */
extern u8 D_80138D58;

void Save_InitDefault(void *save) {
    GameSave *g = save;
    SaveRecord a;
    SaveRecord b;
    SaveRecord c;
    u8 i;
    u8 j;
    SaveRecTable *t;
    register u8 opt asm("$4");
    register u8 ff asm("$3");

    for (i = 0; i < 12; i++) {
        j = 0;
        t = (SaveRecTable *)(i * 80 + (s32)g + 4);
        for (; j < 5; j++) {
            strcpy((s32 *)a.name, (s32)D_80141230);
            a.value = 0;
            t->e[j] = a;
        }
        j = 0;
        t = (SaveRecTable *)(i * 80 + (s32)g + 0x3C4);
        for (; j < 5; j++) {
            strcpy((s32 *)b.name, (s32)D_80141230);
            b.value = 0;
            t->e[j] = b;
        }
    }
    for (i = 0; i < 3; i++) {
        j = 0;
        t = (SaveRecTable *)(i * 80 + (s32)g + 0x784);
        for (; j < 5; j++) {
            strcpy((s32 *)c.name, (s32)D_80141230);
            c.value = 0;
            t->e[j] = c;
        }
    }
    opt = D_80138D58;
    ff = 0xFF;
    g->musicVolume = ff;
    g->voiceVolume = ff;
    g->sfxVolume = 200;
    g->pad87F = 1;
    g->records[0x874] = 0;
    g->gameMode = 2;
    g->option0 = opt;
}


void Boot_CheckSaveFound(void) {
    if (Save_Probe() != 0) {
        Fsm_Trigger(&gMenuFsm, 0x19);
    } else {
        Fsm_Trigger(&gMenuFsm, 0x1A);
    }
}


void Save_CheckLoaded(void) {
    if (gSaveLoaded != 0) {
        Fsm_Trigger(&gMenuFsm, 0x2F);
    } else {
        Fsm_Trigger(&gMenuFsm, 0x30);
    }
}


void Save_Load(void) {
    void (**fp)(s32, s32);

    Save_InitDefault(&D_80124160);
    Profile_InitDefault(&D_801249F0, 0);
    Profile_InitDefault(&D_80124B30, 1);
    if (Save_ReadAll() == 0) {
        Fsm_Trigger(&gMenuFsm, 0x23);
        return;
    }
    gActiveSave = Save_GetActiveProfile();
    gProfileP1 = (Profile *)SaveSlot_FindPlayer(0);
    gProfileP2 = (Profile *)SaveSlot_FindPlayer(1);
    if (gActiveSave == NULL) {
        gActiveSave = (GameSave *)&D_80124160;
        gProfileP1 = (Profile *)&D_801249F0;
        gProfileP2 = (Profile *)&D_80124B30;
        Fsm_Trigger(&gMenuFsm, 0x23);
        return;
    }
    if (gProfileP1 == 0) {
        gProfileP1 = (Profile *)&D_801249F0;
    }
    if (gProfileP2 == 0) {
        gProfileP2 = (Profile *)&D_80124B30;
    }
    sysAudioSetSfxVolume(0xFF);
    fp = &sysInputLoadBindings;
    (*fp)((s32)gProfileP1->bindings, 0);
    (*fp)((s32)gProfileP2->bindings, 1);
    gSaveLoaded = 1;
    Fsm_Trigger(&gMenuFsm, 0x22);
}

/* TODO (Clean version): [evaluated 2026-11-18: done with INPUT_PRESSED_ANY_PAD and the KEY_*
 * constants of include/input.h. The sound ids were left as numbers: the same id goes with
 * different keys (e.g. 0x2F with KEY_A in PakCheck), so they are not per-key names.]
 */
void PakDialog_Input(void) {
    void (**fp)(s32);

    fp = &sysInputUpdate;
    (*fp)(0);
    (*fp)(1);
    (*fp)(2);
    (*fp)(3);
    if ((u8) gPakDialogSel == 4) {
        if (INPUT_PRESSED_ANY_PAD(KEY_DOWN)) {
            sysAudioSetAmbientVolume(0xFF);
            sysSfxPlay(0x2F, 1, 1.0f, 1.0f);
            Fsm_Trigger(&gMenuFsm, 8);
        }
    } else if (INPUT_PRESSED_ANY_PAD(KEY_UP)) {
        sysAudioSetAmbientVolume(0xFF);
        sysSfxPlay(0x2E, 1, 1.0f, 1.0f);
        Fsm_Trigger(&gMenuFsm, 6);
    }
    if (INPUT_PRESSED_ANY_PAD(KEY_A)) {
        sysAudioSetAmbientVolume(0xFF);
        sysSfxPlay(0x32, 1, 1.0f, 1.0f);
        Fsm_Trigger(&gMenuFsm, 2);
    }
}

void PakNotFound_Enter(void) {
    Menu_SetText(gMenuObjs[4], 0, D_80141234, 320, 160, 0, 0, 1, 1, 255, 255, 255);
    Menu_SetText(gMenuObjs[4], 1, D_80141238, 320, 180, 0, 0, 1, 1, 255, 255, 255);
    Menu_SetText(gMenuObjs[4], 2, D_8014123C, 320, 200, 0, 0, 1, 1, 255, 255, 255);
    Menu_SetText(gMenuObjs[4], 3, D_80141240, 320, 220, 0, 0, 1, 1, 255, 255, 255);
    Menu_SetText(gMenuObjs[4], 4, D_80141244, 320, 250, 1, 0, 1, 1, 127, 127, 127);
    Menu_SetText(gMenuObjs[4], 5, D_80141248, 320, 270, 1, 0, 1, 1, 127, 127, 127);
}


void PakNoNote_Enter(void) {
    s32 buf[8];

    sprintf((char *)buf, (char *)&D_80100CAC, (s32)D_80141234, gPakErrorPort);
    Menu_SetText(gMenuObjs[4], 0, buf, 320, 150, 0, 0, 1, 1, 255, 255, 255);
    Menu_SetText(gMenuObjs[4], 1, D_8014124C, 320, 170, 0, 0, 1, 1, 255, 255, 255);
    Menu_SetText(gMenuObjs[4], 2, D_80141238, 320, 190, 0, 0, 1, 1, 255, 255, 255);
    Menu_SetText(gMenuObjs[4], 3, D_80141250, 320, 210, 0, 0, 1, 1, 255, 255, 255);
    Menu_SetText(gMenuObjs[4], 4, D_80141254, 320, 250, 0, 0, 1, 1, 127, 127, 127);
    Menu_SetText(gMenuObjs[4], 5, D_80141248, 320, 270, 0, 0, 1, 1, 127, 127, 127);
}


void PakNoSave_Enter(void) {
    Menu_SetText(gMenuObjs[4], 0, D_80141454, 320, 160, 0, 0, 1, 1, 255, 255, 255);
    Menu_SetText(gMenuObjs[4], 1, D_80141458, 320, 180, 0, 0, 1, 1, 255, 255, 255);
    Menu_SetText(gMenuObjs[4], 2, D_8014123C, 320, 200, 0, 0, 1, 1, 255, 255, 255);
    Menu_SetText(gMenuObjs[4], 3, D_80141240, 320, 220, 0, 0, 1, 1, 255, 255, 255);
    Menu_SetText(gMenuObjs[4], 4, D_80141254, 320, 250, 1, 0, 1, 1, 127, 127, 127);
    Menu_SetText(gMenuObjs[4], 5, D_80141248, 320, 270, 1, 0, 1, 1, 127, 127, 127);
}


void PakDamaged_Enter(void) {
    s32 buf[8];

    sprintf((char *)buf, (char *)&D_80100CAC, (s32)D_80141234, gPakErrorPort);
    Menu_SetText(gMenuObjs[4], 0, buf, 320, 160, 0, 0, 1, 1, 255, 255, 255);
    Menu_SetText(gMenuObjs[4], 1, (s32 *)&D_80100CB4, 320, 180, 0, 0, 1, 1, 255, 255, 255);
    Menu_SetText(gMenuObjs[4], 2, (s32 *)&D_80100CC8, 320, 230, 0, 0, 1, 1, 127, 127, 127);
    Menu_SetText(gMenuObjs[4], 3, D_80141244, 320, 210, 1, 0, 1, 1, 127, 127, 127);
    Menu_SetText(gMenuObjs[4], 4, (s32 *)&D_80100CCC, 320, 230, 0, 0, 1, 1, 127, 127, 127);
    Menu_SetText(gMenuObjs[5], 0, (s32 *)&D_80100CDC, 320, 250, 0, 0, 1, 1, 127, 127, 127);
    Menu_SetText(gMenuObjs[4], 5, (s32 *)&D_80100CF0, 320, 270, 1, 0, 1, 1, 127, 127, 127);
    Menu_StartEnter(gMenuObjs[5]);
}

void PakUnsupported_Enter(void) {
    s32 buf[8];

    sprintf((char *)buf, (char *)&D_80100CAC, (s32)D_80141234, gPakErrorPort);
    Menu_SetText(gMenuObjs[4], 0, buf, 320, 150, 0, 0, 1, 1, 255, 255, 255);
    Menu_SetText(gMenuObjs[4], 1, (s32 *)&D_80100D08, 320, 170, 0, 0, 1, 1, 255, 255, 255);
    Menu_SetText(gMenuObjs[4], 2, (s32 *)&D_80100D20, 320, 190, 0, 0, 1, 1, 255, 255, 255);
    Menu_SetText(gMenuObjs[4], 3, (s32 *)&D_80100CC8, 320, 210, 0, 0, 1, 1, 255, 255, 255);
    Menu_SetText(gMenuObjs[4], 4, D_80141244, 320, 250, 0, 0, 1, 1, 127, 127, 127);
    Menu_SetText(gMenuObjs[4], 5, (s32 *)&D_80100CF0, 320, 270, 0, 0, 1, 1, 127, 127, 127);
}


void PakRepair_Draw(void) {
    s32 none;

    Menu_SetupCamera();
    Menu_UpdateFade();
    none = -1;
    Menu_Draw((Menu_ *)gMenuObjs[4], gPakDialogSel, -1, -1, none, none, none, 0);
    if ((u8)gPakDialogSel == 4) {
        Menu_Draw((Menu_ *)gMenuObjs[5], 0, -1, -1, none, none, none, 0);
    } else {
        Menu_Draw((Menu_ *)gMenuObjs[5], -1, -1, -1, none, none, none, 0);
    }
    Menu_EndFrame();
}

void PakRepair_Input(void) {
    void (**fp)(s32);

    fp = &sysInputUpdate;
    (*fp)(0);
    (*fp)(1);
    (*fp)(2);
    (*fp)(3);
    if (INPUT_PRESSED_ANY_PAD(KEY_A)) {
        sysAudioSetAmbientVolume(0xFF);
        sysSfxPlay(0x32, 1, 1.0f, 1.0f);
        Fsm_Trigger(&gMenuFsm, 2);
    }
    if ((u8) gPakDialogSel < 5 && (INPUT_PRESSED_ANY_PAD(KEY_DOWN))) {
        sysAudioSetAmbientVolume(0xFF);
        sysSfxPlay(0x2F, 1, 1.0f, 1.0f);
        Fsm_Trigger(&gMenuFsm, 8);
    }
    if ((u8) gPakDialogSel >= 4 && (INPUT_PRESSED_ANY_PAD(KEY_UP))) {
        sysAudioSetAmbientVolume(0xFF);
        sysSfxPlay(0x2E, 1, 1.0f, 1.0f);
        Fsm_Trigger(&gMenuFsm, 6);
    }
}

void PakDialog_MoveUp(void) {
    if ((u8)gPakDialogSel >= 4) {
        gPakDialogSel = (u8)gPakDialogSel - 1;
    }
}


void PakDialog_MoveDown(void) {
    if ((u8)gPakDialogSel < 5) {
        gPakDialogSel = (u8)gPakDialogSel + 1;
    }
}


void PakDialog_Resolve3(void) {
    switch ((u8)gPakDialogSel) {
        case 4:
            Fsm_Trigger(&gMenuFsm, 0x26);
            break;
        case 5:
            Fsm_Trigger(&gMenuFsm, 0x28);
            break;
        case 3:
        default:
            Fsm_Trigger(&gMenuFsm, 0x27);
            break;
    }
}


void PakFull_Enter(void) {
    s32 buf[8];

    sprintf((char *)buf, (char *)&D_80100CA4, (s32)D_80141234, gPakErrorPort);
    Menu_SetText(gMenuObjs[4], 0, buf, 320, 160, 0, 0, 1, 1, 255, 255, 255);
    Menu_SetText(gMenuObjs[4], 1, D_80141464, 320, 180, 0, 0, 1, 1, 255, 255, 255);
    Menu_SetText(gMenuObjs[4], 2, D_80141468, 320, 200, 0, 0, 1, 1, 255, 255, 255);
    Menu_SetText(gMenuObjs[4], 3, D_8014146C, 320, 220, 0, 0, 1, 1, 255, 255, 255);
    Menu_SetText(gMenuObjs[4], 4, D_80141248, 320, 250, 1, 0, 1, 1, 127, 127, 127);
    Menu_SetText(gMenuObjs[4], 5, D_80141244, 320, 270, 1, 0, 1, 1, 127, 127, 127);
}


void PakCreateFailed_Enter(void) {
    Menu_SetText(gMenuObjs[4], 0, D_80141234, 320, 160, 0, 0, 1, 1, 255, 255, 255);
    Menu_SetText(gMenuObjs[4], 1, (s32 *)&D_80100D38, 320, 180, 0, 0, 1, 1, 255, 255, 255);
    Menu_SetText(gMenuObjs[4], 2, D_8014123C, 320, 200, 0, 0, 1, 1, 255, 255, 255);
    Menu_SetText(gMenuObjs[4], 3, D_80141240, 320, 220, 0, 0, 1, 1, 255, 255, 255);
    Menu_SetText(gMenuObjs[4], 4, D_80141248, 320, 250, 1, 0, 1, 1, 127, 127, 127);
    Menu_SetText(gMenuObjs[4], 5, D_80141244, 320, 270, 1, 0, 1, 1, 127, 127, 127);
}


void PakNoteCorrupt_Enter(void) {
    s32 buf[8];

    sprintf((char *)buf, (char *)&D_80100CA4, (s32)D_80141234, gPakErrorPort);
    Menu_SetText(gMenuObjs[4], 0, buf, 320, 160, 0, 0, 1, 1, 255, 255, 255);
    Menu_SetText(gMenuObjs[4], 1, (s32 *)&D_80100D48, 320, 180, 0, 0, 1, 1, 255, 255, 255);
    Menu_SetText(gMenuObjs[4], 2, (s32 *)&D_80100D58, 320, 200, 0, 0, 1, 1, 255, 255, 255);
    Menu_SetText(gMenuObjs[4], 3, (s32 *)&D_80100D68, 320, 220, 0, 0, 1, 1, 255, 255, 255);
    Menu_SetText(gMenuObjs[4], 4, D_80141244, 320, 250, 1, 0, 1, 1, 127, 127, 127);
    Menu_SetText(gMenuObjs[4], 5, (s32 *)&D_80100D7C, 320, 270, 1, 0, 1, 1, 127, 127, 127);
}




void PakOtherError_Enter(void) {
    Menu_SetText(gMenuObjs[4], 0, D_80141258, 320, 160, 0, 0, 1, 1, 255, 255, 255);
    Menu_SetText(gMenuObjs[4], 1, D_8014125C, 320, 180, 0, 0, 1, 1, 255, 255, 255);
    Menu_SetText(gMenuObjs[4], 2, D_8014123C, 320, 200, 0, 0, 1, 1, 255, 255, 255);
    Menu_SetText(gMenuObjs[4], 3, D_80141240, 320, 220, 0, 0, 1, 1, 255, 255, 255);
    Menu_SetText(gMenuObjs[4], 4, D_80141244, 320, 250, 1, 0, 1, 1, 127, 127, 127);
    Menu_SetText(gMenuObjs[4], 5, D_80141248, 320, 270, 1, 0, 1, 1, 127, 127, 127);
}


void PakChanged_Enter(void) {
    Menu_SetText(gMenuObjs[4], 0, D_80141470, 320, 160, 0, 0, 1, 1, 255, 255, 255);
    Menu_SetText(gMenuObjs[4], 1, D_80141474, 320, 180, 0, 0, 1, 1, 255, 255, 255);
    Menu_SetText(gMenuObjs[4], 2, D_8014123C, 320, 200, 0, 0, 1, 1, 255, 255, 255);
    Menu_SetText(gMenuObjs[4], 3, D_80141240, 320, 220, 0, 0, 1, 1, 255, 255, 255);
    Menu_SetText(gMenuObjs[4], 4, D_80141244, 320, 250, 1, 0, 1, 1, 127, 127, 127);
    Menu_SetText(gMenuObjs[4], 5, D_80141478, 320, 270, 1, 0, 1, 1, 127, 127, 127);
}


void PakNoteMissing_Enter(void) {
    Menu_SetText(gMenuObjs[4], 0, D_80141234, 320, 160, 0, 0, 1, 1, 255, 255, 255);
    Menu_SetText(gMenuObjs[4], 1, (s32 *)&D_80100D88, 320, 180, 0, 0, 1, 1, 255, 255, 255);
    Menu_SetText(gMenuObjs[4], 2, D_8014123C, 320, 200, 0, 0, 1, 1, 255, 255, 255);
    Menu_SetText(gMenuObjs[4], 3, D_80141240, 320, 220, 0, 0, 1, 1, 255, 255, 255);
    Menu_SetText(gMenuObjs[4], 4, D_80141244, 320, 250, 1, 0, 1, 1, 127, 127, 127);
    Menu_SetText(gMenuObjs[4], 5, D_80141248, 320, 270, 1, 0, 1, 1, 127, 127, 127);
}


void PakRepairOk_Enter(void) {
    s32 buf[8];

    sprintf((char *)buf, (char *)&D_80100CA4, (s32)D_80141234, gPakErrorPort);
    gPakDialogSel = 4;
    Menu_SetText(gMenuObjs[4], 0, (s32 *)&D_80100CC8, 320, 160, 0, 0, 1, 1, 255, 255, 255);
    Menu_SetText(gMenuObjs[4], 1, buf, 320, 180, 0, 0, 1, 1, 255, 255, 255);
    Menu_SetText(gMenuObjs[4], 2, (s32 *)&D_80100D94, 320, 200, 0, 0, 1, 1, 255, 255, 255);
    Menu_SetText(gMenuObjs[4], 3, (s32 *)&D_80100CC8, 320, 220, 0, 0, 1, 1, 255, 255, 255);
    Menu_SetText(gMenuObjs[4], 4, D_8014158C, 320, 250, 1, 0, 1, 1, 127, 127, 127);
    Menu_SetText(gMenuObjs[4], 5, (s32 *)&D_80100CC8, 320, 270, 1, 0, 1, 1, 127, 127, 127);
}


void PakRepairFailed_Enter(void) {
    s32 buf[8];

    sprintf((char *)buf, (char *)&D_80100CA4, (s32)D_80141234, gPakErrorPort);
    gPakDialogSel = 4;
    Menu_SetText(gMenuObjs[4], 0, (s32 *)&D_80100CC8, 320, 160, 0, 0, 1, 1, 255, 255, 255);
    Menu_SetText(gMenuObjs[4], 1, buf, 320, 180, 0, 0, 1, 1, 255, 255, 255);
    Menu_SetText(gMenuObjs[4], 2, (s32 *)&D_80100DAC, 320, 200, 0, 0, 1, 1, 255, 255, 255);
    Menu_SetText(gMenuObjs[4], 3, (s32 *)&D_80100CC8, 320, 220, 0, 0, 1, 1, 255, 255, 255);
    Menu_SetText(gMenuObjs[4], 4, D_8014158C, 320, 250, 1, 0, 1, 1, 127, 127, 127);
    Menu_SetText(gMenuObjs[4], 5, (s32 *)&D_80100CC8, 320, 270, 1, 0, 1, 1, 127, 127, 127);
}


void PakDialog_RepairDamaged(void) {
    u8 i;

    for (i = 0; i < 4; i++) {
        if (Pak_GetStatus(i) == PAK_STATUS_DAMAGED) {
            if (sysPakRepair(i) != 0) {
                Fsm_Trigger(&gMenuFsm, 0x29);
            } else {
                Fsm_Trigger(&gMenuFsm, 0x2A);
            }
            return;
        }
    }
    Fsm_Trigger(&gMenuFsm, 0x11);
}


void PakRepairResult_Input(void) {
    void (**fp)(s32) = &sysInputUpdate;

    (*fp)(0);
    (*fp)(1);
    (*fp)(2);
    (*fp)(3);
    if (INPUT_PRESSED_ANY_PAD(KEY_A)) {
        sysAudioSetAmbientVolume(0xFF);
        sysSfxPlay(0x2F, 1, 1.0f, 1.0f);
        Fsm_Trigger(&gMenuFsm, 2);
    }
}


/* TODO (Clean version): [evaluated 2026-11-18: statuses are now PAK_STATUS_* (include/pak.h).
 * A priority table would add .rodata and a nested loop, so the six scans stay for matching.]
 * Simplify 6 consecutive 4-iteration loops with a prioritized table lookup:
 *   static const struct { u8 status; u8 targetMenuState; } sStatusPriority[] = {
 *       { 4, 0x1F }, { 9, 0x20 }, { 3, 0x1E }, { 8, 0x25 }, { 2, 0x1D }, { 7, 0x24 }
 *   };
 * Also replace magic status integers with an enum (e.g., CONTROLLER_STATUS_REMOVED, MEMCARD_STATUS_CORRUPT).
 */
void Pak_DispatchStatus(void) {
    s32 st[4];
    s32 i;
    s32 found;
    s32 first;

    first = 0xFF;
    found = 0;
    for (i = 0; (u8) i < 4; i++) {
        st[(u8) i] = Pak_GetStatus(i);
        if (!(u8) found && st[(u8) i] != PAK_STATUS_NONE) {
            first = i;
            found = 1;
        }
    }
    if ((u8) first == 0xFF) {
        Fsm_Trigger(&gMenuFsm, 0x1C);
        return;
    }
    for (i = 0; (u8) i < 4; i++) {
        if (st[(u8) i] == PAK_STATUS_DAMAGED) {
            gPakErrorPort = i + 1;
            Fsm_Trigger(&gMenuFsm, 0x1f);
            return;
        }
    }
    for (i = 0; (u8) i < 4; i++) {
        if (st[(u8) i] == PAK_STATUS_RUMBLE) {
            gPakErrorPort = i + 1;
            Fsm_Trigger(&gMenuFsm, 0x20);
            return;
        }
    }
    for (i = 0; (u8) i < 4; i++) {
        if (st[(u8) i] == PAK_STATUS_NO_FILE) {
            gPakErrorPort = i + 1;
            Fsm_Trigger(&gMenuFsm, 0x1e);
            return;
        }
    }
    for (i = 0; (u8) i < 4; i++) {
        if (st[(u8) i] == PAK_STATUS_UNK8) {
            gPakErrorPort = i + 1;
            Fsm_Trigger(&gMenuFsm, 0x25);
            return;
        }
    }
    for (i = 0; (u8) i < 4; i++) {
        if (st[(u8) i] == PAK_STATUS_CHANGED) {
            gPakErrorPort = i + 1;
            Fsm_Trigger(&gMenuFsm, 0x1d);
            return;
        }
    }
    for (i = 0; (u8) i < 4; i++) {
        if (st[(u8) i] == PAK_STATUS_FULL) {
            gPakErrorPort = i + 1;
            Fsm_Trigger(&gMenuFsm, 0x24);
            return;
        }
    }
    Fsm_Trigger(&gMenuFsm, 0x21);
}

void PakCheck_Enter(void) {
    gPakDialogSel = 4;
    Menu_LoadBg(0);
    Menu_StartDim();
    Menu_StartEnter(D_800F5F18);
}

void PakDialog_SelectNone(void) {
    gPakDialogSel = 3;
}

void PakDialog_LayoutTexts(void) {
    Menu_LayoutTexts(D_800F5F18);
    Menu_LayoutTexts(D_800F5F1C);
}

void PakDialog_Draw(void) {
    Menu_SetupCamera();
    Menu_UpdateFade();
    Menu_Draw(gMenuObjs[4], gPakDialogSel, -1, -1, -1, -1, -1, 0);
    Menu_EndFrame();
}

void PakDialog_SelectTop(void) {
    gPakDialogSel = 4;
}

void PakDialog_SelectBottom(void) {
    gPakDialogSel = 5;
}

void Save_CreateNew(void) {
    Profile buf;
    u8 found;
    u8 slot;
    u8 i;
    void *p;

    found = 0;
    slot = 0;
    for (i = 0; i < 4; i++) {
        if (Pak_GetStatus(i) == 3) {
            found = Save_CreateOnPak(i);
            slot = i;
            break;
        }
    }
    if (!found) {
        Fsm_Trigger(&gMenuFsm, 0x2B);
        return;
    }
    Save_ReadAll();
    gActiveSave = (GameSave *)Save_GetProfile(slot);
    Save_InitDefault(gActiveSave);
    Profile_InitDefault(&buf, 0);
    SaveSlot_Add(slot, &buf);
    Profile_InitDefault(&buf, 1);
    SaveSlot_Add(slot, &buf);
    gProfileP1 = SaveSlot_Get(slot, 0);
    p = SaveSlot_Get(slot, 1);
    gActiveSave->player1Id = gProfileP1->id;
    gProfileP2 = p;
    gActiveSave->player2Id = ((s32 *)p)[1];
    Save_Commit();
    gSaveLoaded = 1;
    Menu_Advance();
}

void Pak_DispatchCreateError(void) {
    u8 found = 0xFF;
    u8 i;

    for (i = 0; i < 4; i++) {
        if (Pak_GetStatus(i) != 0 && Pak_GetStatus(i) != 1) {
            found = i;
            break;
        }
    }
    gPakErrorPort = found + 1;
    if (found != 0xFF && Pak_GetStatus(i) == 7) {
        Fsm_Trigger(&gMenuFsm, 0x24);
    } else {
        Fsm_Trigger(&gMenuFsm, 0x2C);
    }
}

void Save_NewGame(void) {
    gSaveLoaded = 0;
    gActiveSave = (GameSave *)&D_80124160;
    Save_InitDefault(&D_80124160);
    gProfileP1 = (Profile *)&D_801249F0;
    Profile_InitDefault(&D_801249F0, 0);
    gProfileP2 = (Profile *)&D_80124B30;
    Profile_InitDefault(&D_80124B30, 1);
    sysAudioSetSfxVolume(0xFF);
}


void Save_CommitLoaded(void) {
    s32 x;

    gSaveLoaded = 0;
    *(Big88C_ *)&D_80124160 = *(Big88C_ *)gActiveSave;
    *(Profile *)&D_801249F0 = *gProfileP1;
    *(Profile *)&D_80124B30 = *gProfileP2;
    x = D_80100DBC;
    gActiveSave = (GameSave *)&D_80124160;
    gProfileP1 = (Profile *)&D_801249F0;
    gProfileP2 = (Profile *)&D_80124B30;
    *(s32 *)&gPasswordP1 = x;
    if (gNumPlayers == 2) {
        *(s32 *)&gPasswordP2 = x;
    }
}

void PakDialog_Resolve(void) {
    if ((*(u8 *)&gPakDialogSel) == 4) {
        Fsm_Trigger(&gMenuFsm, 0x27);
    } else {
        Fsm_Trigger(&gMenuFsm, 0x26);
    }
}


void Save_ApplyProfiles(void) {
    if (gProfileP1->carUnlocks[0] == 0) {
        Profile_InitDefault(gProfileP1, 0);
    }
    if (gProfileP2->carUnlocks[0] == 0) {
        Profile_InitDefault(gProfileP2, 1);
    }
    if ((u8)Save_Commit() != 0) {
    if (gProfileP1->id == 0) {
        Fsm_Trigger(&gMenuFsm, 0x31);
    } else {
        Password_Get((s32)gProfileP1, (s32 *)&gPasswordP1);
    }
    if (gNumPlayers == 2 && gProfileP2->id == 0) {
        Fsm_Trigger(&gMenuFsm, 0x31);
    } else {
        Password_Get((s32)gProfileP2, (s32 *)&gPasswordP2);
    }
    Fsm_Trigger(&gMenuFsm, 0x33);
    } else {
        Fsm_Trigger(&gMenuFsm, 0x32);
    }
}

s32 Pak_AnyChanged(void) {
    u8 i;

    for (i = 0; i < 4; i++) {
        if (Pak_GetStatus(i) == 2) {
            return 1;
        }
    }
    return 0;
}


void PakCheck_Dispatch(void) {
    if (Pak_AnyChanged() & 0xFF) {
        Fsm_Trigger((s32) &gMenuFsm, 0x1D);
    }
    Fsm_Trigger((s32) &gMenuFsm, 0x21);
}

s32 Car_IsUnlocked(u8 i, Profile *p) {
    if (gCarModelNames[i] == NULL) {
        return 0;
    }
    if (i >= 32) {
        return (p->carUnlocks[1] >> (i - 32)) & 1;
    }
    return (p->carUnlocks[0] >> i) & 1;
}

u8 Track_IsUnlocked(u8 i) {
    u8 r;

    if (gTrackNames[(u8)(i / 3)][(u8)(i % 3)] == 0) {
        return 0;
    }
    r = 0;
    if (((gProfileP1->trackUnlocks >> i) & 1) || ((gProfileP2->trackUnlocks >> i) & 1)) {
        r = 1;
    }
    return r;
}


u8 Cup_IsUnlocked(u8 i) {
    u8 r;
    u32 w;

    switch (i) {
        case 0:
            return 1;
        case 1:
            w = gProfileP1->trackUnlocks;
            r = 0;
            if ((w >> 7) & 1) {
                r = (w >> 8) & 1;
            }
            return r;
        case 2:
            return Car_IsUnlocked(0x26, gProfileP1);
    }
    return 0;
}

/* TODO (Bug potential): Off-by-one bit shift. For id >= 32, this sets bit (id - 31) instead of (id - 32).
 * In Car_IsUnlocked, checking id >= 32 reads (bits18 >> (id - 32)) & 1.
 * As a result, when id == 32, bit 1 is set instead of bit 0, leaving entry 32 permanently locked
 * and shifting all upper unlocks by 1 bit. Furthermore, id == 62 shifts by 31, and id == 63 shifts by 32 (UB on 32-bit int).
 * Correct code:
 *   p->carUnlocks[1] |= 1 << (id - 32);
 */
void Car_Unlock(u8 id, Profile *p) {
    if (id >= 32) {
        p->carUnlocks[1] |= 1 << (id - 31);
        return;
    }
    p->carUnlocks[0] |= 1 << id;
}

void Track_Unlock(s32 id, Profile *p) {
    p->trackUnlocks |= 1 << id;
}

void Player_NextCar(u8 player) {
    Profile *p;

    if (!player) {
        p = gProfileP1;
    } else {
        p = gProfileP2;
    }
    do {
        if (p->car < 40) {
            p->car++;
        } else {
            p->car = 0;
        }
    } while (!(u8)Car_IsUnlocked(p->car, p));
}

void Player_PrevCar(u8 player) {
    Profile *p;

    if (!player) {
        p = gProfileP1;
    } else {
        p = gProfileP2;
    }
    do {
        if (p->car != 0) {
            p->car--;
        } else {
            p->car = 40;
        }
    } while (!(u8)Car_IsUnlocked(p->car, p));
}

/* TODO (Bug potential): Potential infinite loop if all tracks are locked or invalid.
 * If Track_IsUnlocked() returns 0 for all 12 indices (0..11), this loop will never terminate.
 * Propose adding a loop counter / max iterations guard:
 *   for (u8 count = 0; count < 12; count++) { ... }
 */
void Track_SelectNext(void) {
    u8 v;

    for (;;) {
        if (gProfileP1->track < 11) {
            gProfileP1->track++;
        } else {
            gProfileP1->track = 0;
        }
        if (Track_IsUnlocked(gProfileP1->track) == 0) {
            continue;
        }
        v = gProfileP1->track;
        if (gTrackNames[(u8)(v / 3)][(u8)(v % 3)] != 0) {
            break;
        }
    }
    gProfileP2->track = v;
}


void Track_SelectPrev(void) {
    u8 v;

    for (;;) {
        if (gProfileP1->track != 0) {
            gProfileP1->track--;
        } else {
            gProfileP1->track = 11;
        }
        if (Track_IsUnlocked(gProfileP1->track) == 0) {
            continue;
        }
        v = gProfileP1->track;
        if (gTrackNames[(u8)(v / 3)][(u8)(v % 3)] != 0) {
            break;
        }
    }
    gProfileP2->track = v;
}


void Song_PickRandom(void) {
    *(u8 *)&gMenuSong = sysRandRange(7);
}


void Song_PickForTrack(void) {
    gMenuSong = Music_GetSongTrack((u32) gProfileP1->track);
}

void Song_Next(void) {
    if ((u8)gMenuSong < 6) {
        gMenuSong = (u8)gMenuSong + 1;
    } else {
        gMenuSong = 0;
    }
}


void Song_Prev(void) {
    u8 v = gMenuSong;

    if (v == 0) {
        v = 6;
    } else {
        v--;
    }
    gMenuSong = v;
}


extern void (*audMusicPlayTrack)(u8);

void Music_PlaySong(void) {
    audMusicPlayTrack(*(u8 *)&gMenuSong);
}


void Music_StopTrack(void) {
    audMusicStop();
}

void Music_StartRaceTrack(void) {
    switch (gMusicMode) {
        case 0:
            Song_PickRandom();
            break;
        case 1:
            Song_PickForTrack();
            break;
        case 2:
            break;
    }
    Music_PlaySong();
}


void Music_PickMenuTrack(void) {
    u8 v;

    if (gMusicMode == 2) {
        gMenuMusicTrack = *(u8 *)&gMenuSong;
        return;
    }
    v = sysRandRange(7);
    gMenuMusicTrack = v;
    *(u8 *)&gMenuSong = v;
}

extern void (*audMusicPlayTrack)(u8);
void Music_StartMenuTrack(void) {
    u8 v = gMenuMusicStarted;

    if (v == 0) {
        gMenuMusicStarted = 1;
        audMusicPlayTrack(gMenuMusicTrack);
    }
}

void Music_StopMenuTrack(void) {
    gMenuMusicStarted = 0;
    audMusicStop();
}

extern u8 gMusicFadeVolume;
extern void (*sysAudioSetMusicVolume)(s32);
/* Steps the music volume by 8 per call: mode 0 fades out, 1 fades in, 2 moves toward half volume. */
void Music_UpdateVolumeFade(void) {
    switch ((u8)gMusicFadeMode) {
        case 0:
            if (gMusicFadeVolume > 8) {
                sysAudioSetMusicVolume((u8)(gMusicFadeVolume -= 8));
            }
            break;
        case 1:
            if (gMusicFadeVolume < 0xF7) {
                sysAudioSetMusicVolume((u8)(gMusicFadeVolume += 8));
            }
            break;
        case 2:
        default:
            if (gMusicFadeVolume >= 0x88) {
                sysAudioSetMusicVolume((u8)(gMusicFadeVolume -= 8));
            } else if (gMusicFadeVolume < 0x77) {
                sysAudioSetMusicVolume((u8)(gMusicFadeVolume += 8));
            }
            break;
    }
}

void Music_FadeOut(void) {
    gMusicFadeMode = 0;
}

void Music_FadeIn(void) {
    gMusicFadeMode = 1;
}

void Music_FadeToHalf(void) {
    gMusicFadeMode = 2;
}

void Menu_ResetAttractTimer(void) {
    D_800F6468 = sysTimeGetMillis();
}

/* TODO (Clean version / Bug check): [evaluated 2026-11-18: the overflow-safe delta changes the
 * ROM; the original comparison is the game's behaviour and stays.] Idle timeout check (30,000 ms = 30 seconds).
 * Using (now > start + 30000) causes an early trigger or miss on 32-bit timer overflow (wraps around ~49.7 days).
 * Standard delta time check is overflow-safe:
 *   #define IDLE_TIMEOUT_MS 30000
 *   if ((u32)(sysTimeGetMillis() - D_800F6468) > IDLE_TIMEOUT_MS) {
 *       Fsm_Trigger(&gMenuFsm, 0x38);
 *   }
 */
void Menu_AttractTimeout(void) {
    if ((u32)sysTimeGetMillis() > (u32)(D_800F6468 + 30000)) {
        Fsm_Trigger(&gMenuFsm, 0x38);
    }
}


void MenuObjs_Reset(void) {
    u8 i;

    Menu_FreeModels();
    for (i = 0; i < 33; i++) {
        Menu_LoadModels(gMenuObjs[i]);
    }
}


void Menu_UnloadWorlds(void) {
    Menu_FreeModels();
}

void Boot_AllocBg(void) {
    sysScreenAllocBg();
}

/* Copies language strings into the menu tables: gLangText[59..65] to the eight game-mode
 * entries D_8012406C..D_80124088 (the last one repeats entry 59) and gLangText[70..82] to
 * D_80124110[13]. (The user's TODO read D_801411CC as a calibration block: it is
 * &gLangText[59].) */
void Boot_LoadLanguage(void) {
    u8 i;

    if (!Lang_Load(&D_80100DC0)) {
        Fsm_Trigger(&gMenuFsm, 0x12);
    }
    if (!CarNames_Load(&D_80100DC8)) {
        Fsm_Trigger(&gMenuFsm, 0x12);
    }
    D_8012406C = (s32)gLangText[59];
    D_80124070 = (s32)gLangText[60];
    D_80124074 = (s32)gLangText[61];
    gGameModeNames = (s32)gLangText[62];
    D_8012407C = (s32)gLangText[63];
    D_80124080 = (s32)gLangText[64];
    D_80124084 = (s32)gLangText[65];
    D_80124088 = (s32)gLangText[59];
    for (i = 0; i < 13; i++) {
        D_80124110[i] = (s32)gLangText[i + 70];
    }
}

void Boot_LoadTuning(void) {
    if (!(Tuning_Load() & 0xFF)) {
        Fsm_Trigger((s32) &gMenuFsm, 0x12);
    }
}

void Menu_ShutdownNop(void) {
    Lang_Free();
}

void Boot_Nop(void) {

}

void Menu_ExitToGame(void) {
    if (gSaveLoaded == 0) {
        *Save_GetProfile(0) = *(Big88C_ *)&D_80124160;
    }
    Music_StopMenuTrack();
    sysAudioSetMusicVolume(gActiveSave->musicVolume);
    sysAudioSetSfxVolume(gActiveSave->sfxVolume);
    sysAudioSetVoiceVolume(gActiveSave->voiceVolume);
    sysAudioSetAmbientVolume(0xFF);
    MenuFont_FreeAll();
    Menu_FreeAll();
    sysMenuExitHook();
    sysScreenFreeBg();
    Menu_UnloadWorlds();
}

void Menu_ReturnFromGame(void) {
    MenuFont_LoadAll();
    MenuObjs_Alloc();
    MenuObjs_Reset();
    sysScreenAllocBg();
    sysAudioSetAmbientVolume(0);
    sysAudioSetSfxVolume(0xFF);
    Music_PickMenuTrack();
    Music_StartMenuTrack();
    if (gSaveLoaded == 0) {
        *(Big88C_ *)&D_80124160 = *Save_GetProfile(0);
    }
    Records_SaveRaceResult(&gRaceResult);
}


extern u8 (*sysBootCheck)(void);
void Boot_CheckSystem(void) {
    if (sysBootCheck() == 0) {
        Fsm_Trigger(&gMenuFsm, 0x12);
    }
}

void Menu_Exit(void) {
    sysMenuExit();
}

/* TODO (Clean version): [evaluated 2026-11-18: Time_Seconds/Time_Hundredths simplified. Hours and
 * minutes need the step-by-step divisions: a single ms / 3600000 or ms / 60000 changes codegen.]
 * Millisecond time formatting helpers. Propose unifying into inline helpers or a struct:
 *   typedef struct { u16 hours; u8 minutes; u8 seconds; u8 centiseconds; } FormattedTime;
 *   static inline u16 Time_GetHours(u32 ms)        { return (ms / 3600000); }
 *   static inline u16 Time_GetTotalMinutes(u32 ms) { return (ms / 60000); }
 *   static inline u32 Time_GetSeconds(u32 ms)      { return (ms / 1000) % 60; }
 *   static inline u32 Time_GetCentiseconds(u32 ms) { return (ms % 1000) / 10; }
 */
u16 Time_Hours(u32 ms) {
    u32 s = ms / 1000;
    u32 m = s / 60;
    return m / 60;
}


u16 Time_Minutes(u32 ms) {
    u32 s = ms / 1000;
    return s / 60;
}

u32 Time_Seconds(u32 ms) {
    return (u16)((ms / 1000) % 60);
}

u32 Time_Hundredths(u32 ms) {
    return (u16)((ms % 1000) / 10);
}

void Boot_CheckPAL(void) {
    if (sysIsPAL() != 0) {
        Fsm_Trigger(&gMenuFsm, 0x13);
    } else {
        Fsm_Trigger(&gMenuFsm, 0x14);
    }
}


void PalError_Draw(void) {
    Menu_SetupCamera();
    Menu_Draw(gMenuObjs[1], -1, -1, -1, -1, -1, -1, 0);
    Menu_EndFrame();
}

void Boot_CheckController(void) {
    if (sysInputIsPad1Connected() != 0) {
        Fsm_Trigger(&gMenuFsm, 0x15);
    } else {
        Fsm_Trigger(&gMenuFsm, 0x16);
    }
}


void NoController_Draw(void) {
    Menu_SetupCamera();
    Menu_Draw(gMenuObjs[2], -1, -1, -1, -1, -1, -1, 0);
    Menu_EndFrame();
}

void BootScreen_Enter(void) {
    Menu_StartEnter(gMenuObjs[0]);
}

void BootScreen_Draw(void) {
    Menu_SetupCamera();
    Menu_Draw(gMenuObjs[0], -1, 0, -1, -1, -1, -1, 0);
    Menu_EndFrame();
}

void BootScreen_Input(void) {
    Menu_HandlePad1(0x50);
}

void Legal_Enter(void) {
    Menu_LoadBg(&D_80100DD0);
}

void Legal_Draw(void) {
    Menu_SetupCamera();
    Menu_EndFrame();
}

void Menu_ResetFade(void) {
    gMenuFadeMode = 0;
    gMenuFadeLevel = 0;
    gMenuHideText = 0;
}

void Menu_StartFadeIn(void) {
    gMenuFadeMode = 1;
    gMenuHideText = 0;
}

void Menu_StartFadeOut(void) {
    gMenuFadeMode = 2;
    gMenuHideText = 1;
}

void Menu_StartDim(void) {
    gMenuFadeMode = 3;
    gMenuHideText = 1;
}

void Menu_UpdateFade(void) {
    switch (gMenuFadeMode) {
        case 1:
            if (gMenuFadeLevel < 0xE6) {
                gMenuFadeLevel += 0x19;
            } else {
                gMenuFadeLevel = 0xFF;
                gMenuFadeMode = 0;
                Menu_Advance();
            }
            sysScreenSetBgAlpha(gMenuFadeLevel);
            sysAudioSetAmbientVolume(gMenuFadeLevel);
            if (gMenuFadeLevel <= gActiveSave->musicVolume) {
                sysAudioSetMusicVolume(gMenuFadeLevel);
            } else {
                sysAudioSetMusicVolume(gActiveSave->musicVolume);
            }
            break;

        case 2:
            if (gMenuFadeLevel >= 0x1A) {
                gMenuFadeLevel -= 0x19;
            } else {
                gMenuFadeLevel = 0;
                gMenuFadeMode = 0;
                Menu_Advance();
            }
            sysScreenSetBgAlpha(gMenuFadeLevel);
            sysAudioSetAmbientVolume(gMenuFadeLevel);
            if (gMenuFadeLevel <= gActiveSave->musicVolume) {
                sysAudioSetMusicVolume(gMenuFadeLevel);
            } else {
                sysAudioSetMusicVolume(gActiveSave->musicVolume);
            }
            break;

        case 3:
            if (gMenuFadeLevel < 0x66) {
                gMenuFadeLevel += 0x19;
            } else if (gMenuFadeLevel >= 0x99) {
                gMenuFadeLevel -= 0x19;
            } else {
                gMenuFadeLevel = 0x7F;
                gMenuFadeMode = 0;
                Menu_Advance();
            }
            sysAudioSetAmbientVolume(0xFF);
            sysScreenSetBgAlpha(gMenuFadeLevel);
            break;
    }
}

void Legal_EnterAgain(void) {
    Menu_StartFadeIn();
    Menu_LoadBg(&D_80100DD0);
    Menu_StartEnter(D_800F5F20);
}

void LogoEA_Enter(void) {
    Menu_StartFadeIn();
    Menu_LoadBg(&D_80100DDC);
    Menu_StartEnter(D_800F5F20);
}

void LogoSFS_Enter(void) {
    Menu_StartFadeIn();
    Menu_LoadBg(&D_80100DE4);
    Menu_StartEnter(D_800F5F20);
}

void Logo_Draw(void) {
    Menu_SetupCamera();
    Menu_UpdateFade();
    Menu_Draw(gMenuObjs[6], -1, 0, -1, -1, -1, -1, gMenuHideText);
    Menu_EndFrame();
}

void Logo_Animate(void) {
    Menu_Animate((*(void * *)&D_800F5F20), 6U);
}

void Logo_Leave(void) {
    Menu_StartFadeOut();
}

void Splash_Enter(void) {
    Menu_LoadBg(&D_80100DEC);
    Menu_StartFadeIn();
    Menu_StartEnter(D_800F5F24);
    gSplashSeen = 0;
    D_80124C75 = 0;
}

void Splash_LayoutTexts(void) {
    Menu_LayoutTexts(D_800F5F24);
}

void Splash_Draw(void) {
    Menu_SetupCamera();
    Menu_UpdateFade();
    if (*(u8 *)&D_80124C75 == 0 && (u8)gMenuFadeLevel == 0xFF) {
        D_80124C75 = 1;
        sysSfxPlay(0x37, 1, 1.0f, 1.0f);
    }
    Menu_Draw(gMenuObjs[7], -1, 0, -1, -1, -1, -1, gMenuHideText);
    Menu_EndFrame();
}

void Splash_Input(void) {
    Menu_HandlePads(0x50, 0);
}

void Splash_Animate(void) {
    Menu_Animate((*(void * *)&D_800F5F24), 7U);
}

void Splash_Leave(void) {
    Menu_StartFadeOut();
}

void Splash_MarkSeen(void) {
    gSplashSeen = 1;
}

void Splash_SkipIfSeen(void) {
    if ((*(u8 *)&gSplashSeen) != 0) {
        Fsm_Trigger((s32) &gMenuFsm, 2);
    }
}

void Welcome_CheckNames(void) {
    s32 a[8];
    s32 b[8];

    gNameChangedP1 = 0;
    gNameChangedP2 = 0;
    Password_Get((s32)gProfileP1, a);
    Password_Get((s32)gProfileP2, b);
    if (strcmp((char *)&gPasswordP1, (char *)&D_80100CC8) == 0) {
        strcpy((s32 *)&gPasswordP1, (s32)a);
    }
    if (strcmp((char *)&gPasswordP2, (char *)&D_80100CC8) == 0) {
        strcpy((s32 *)&gPasswordP2, (s32)b);
    }
    if (strcmp((char *)&gPasswordP1, (char *)a) != 0) {
        strcpy((s32 *)&gPasswordP1, (s32)a);
        gNameChangedP1 = 1;
    }
    if (strcmp((char *)&gPasswordP2, (char *)b) != 0) {
        strcpy((s32 *)&gPasswordP2, (s32)b);
        gNameChangedP2 = 1;
    }
}

void Welcome_Dispatch(void) {
    if (gNameChangedP1 != 0 || gNameChangedP2 != 0) {
        D_80124C78 = sysTimeGetMillis();
        Fsm_Trigger(&gMenuFsm, 0x34);
    } else {
        Fsm_Trigger(&gMenuFsm, 0x35);
    }
}

void Password_Enter(void) {
    s32 a[8];
    s32 b[8];
    s32 c[8];
    u8 *src;

    Menu_StartEnter(D_800F5F14);
    Menu_LoadBg(NULL);
    Menu_StartFadeIn();
    if (gNameChangedP1 != 0) {
        sprintf((char *)a, (char *)&D_80100DF4, gProfileP1->name);
        src = &gPasswordP1;
        gNameChangedP1 = 0;
        D_80124C7E = 1;
        D_80124C7F = 0;
    } else {
        sprintf((char *)a, (char *)&D_80100DF4, gProfileP2->name);
        src = &gPasswordP2;
        gNameChangedP2 = 0;
        D_80124C7E = 0;
        D_80124C7F = 1;
    }
    strcpy(b, (s32)src);
    ((u8 *)b)[10] = 0;
    strcpy(c, (s32)(src + 10));
    ((u8 *)c)[10] = 0;
    Menu_SetText(gMenuObjs[3], 0, D_80141480, 320, 170, 0, 0, 1, 1, 255, 255, 255);
    Menu_SetText(gMenuObjs[3], 1, a, 320, 195, 0, 0, 1, 1, 255, 255, 255);
    Menu_SetText(gMenuObjs[3], 2, b, 320, 220, 0, 0, 1, 1, 255, 255, 255);
    Menu_SetText(gMenuObjs[3], 3, c, 320, 245, 0, 0, 1, 1, 255, 255, 255);
}

void Password_ResetTexts(void) {
    Menu_ResetTexts(D_800F5F14);
}

void Password_Draw(void) {
    Menu_SetupCamera();
    Menu_UpdateFade();
    Menu_Draw(gMenuObjs[3], -1, -1, -1, -1, -1, -1, gMenuHideText);
    Menu_EndFrame();
}

void Password_Input(void) {
    if ((u32)sysTimeGetMillis() > (u32)(D_80124C78 + 5000)) {
        if (D_80124C7E != 0) {
            Menu_HandlePad1(0x50);
        } else if (D_80124C7F != 0) {
            Menu_HandlePad2(0x50);
        }
    }
}

void KeyRepeat_Stamp(s32 arg0) {
    *(s32 *)((u8 *)&gKeyRepeatTime + ((arg0 & 0xFF) * 4)) = sysTimeGetMillis();
}

void KeyRepeat_ResetP1(void) {
    gKeyRepeatDelay = 0;
}

void KeyRepeat_ResetP2(void) {
    D_80124C88 = 0;
}

void KeyRepeat_Accelerate(u8 i) {
    if (((u32 *)&gKeyRepeatDelay)[i] == 0) {
        ((u32 *)&gKeyRepeatDelay)[i] = 500;
    } else {
        ((u32 *)&gKeyRepeatDelay)[i] >>= 1;
        if (((u32 *)&gKeyRepeatDelay)[i] < 250) {
            ((u32 *)&gKeyRepeatDelay)[i] = 250;
        }
    }
}


s32 KeyRepeat_IsDue(u8 i) {
    u32 now = sysTimeGetMillis();

    return ((u32 *)&gKeyRepeatTime)[i] + ((u32 *)&gKeyRepeatDelay)[i] < now;
}


void Boot_SetOnePlayer(void) {
    (*(s8 *)&gNumPlayers) = 1;
}

void MainMenu_PlaceArrow(s32 i, s32 x, s32 y, s32 c) {
    TBL_ *t = (TBL_ *)D_800F5F28;
    u8 j;

    for (j = 0; j < t->n; j++) {
        if (strcmp(t->names[j].name, D_800F646C[(u8)i]) == 0) {
            if ((x < 0) & (y < 0)) {
                t->objs[j].h30 = (u8)c;
                return;
            }
            t->objs[j].h30 = (u8)c;
            t->objs[j].x = MenuPos_ToWorldX(x);
            t->objs[j].y = MenuPos_ToWorldY(y);
            t->objs[j].x2 = t->objs[j].x;
            t->objs[j].y2 = t->objs[j].y;
            return;
        }
    }
}


void MainMenu_UpdateArrows(void) {
    switch (gMainMenuSel) {
        case 1:
            MainMenu_PlaceArrow(0, 90, 490, 100);
            MainMenu_PlaceArrow(1, 303, 490, 100);
            break;
        case 2:
            MainMenu_PlaceArrow(0, 90, 585, 100);
            MainMenu_PlaceArrow(1, 303, 585, 100);
            break;
        case 3:
            MainMenu_PlaceArrow(0, 90, 682, 100);
            MainMenu_PlaceArrow(1, 303, 682, 100);
            break;
        case 5:
            MainMenu_PlaceArrow(0, 90, 897, 100);
            MainMenu_PlaceArrow(1, 625, 899, 100);
            break;
        default:
            MainMenu_PlaceArrow(0, -1, -1, 0);
            MainMenu_PlaceArrow(1, -1, -1, 0);
            break;
    }
}

void MainMenu_Enter(void) {

    Menu_StartFadeIn();
    Menu_LoadBg((s32 *)&D_80100E08);
    MenuList_Reset(gMenuObjs[8]);
    Menu_StartEnter(gMenuObjs[8]);
    gKeyRepeatCountP1 = 0;
    gKeyRepeatCountP2 = 0;
    MenuCar_Show(0);
    MenuCar_Show(1);
    MenuCar_SetPos(0, 500, 500, 0);
    MenuCar_SetPos(1, 500, 500, 0);
    if (gMainMenuSel == 1) {
        MenuCar_ScaleTo150(0);
        MenuCar_ScaleTo150(1);
        MenuCar_MoveTo(0, 460, 480, 0);
        MenuCar_MoveTo(1, 670, 480, 0);
    } else {
        MenuCar_MoveTo(0, 450, 450, 0);
        MenuCar_MoveTo(1, 760, 450, 0);
    }
    MenuGlobe_Reset();
    MenuGlobe_SetPos(500, 500, 0);
    if (gMainMenuSel == 2) {
        MenuGlobe_SpinLarge();
        MenuGlobe_MoveTo(625, 825, 0);
    } else {
        MenuGlobe_MoveTo(675, 900, 0);
    }
    MainMenu_UpdateArrows();
    if (gSaveLoaded == 0 && gProfileChoice == 1) {
        gProfileChoice = 0;
    }
    gLoadPlayer = 0;
    Menu_ResetTicker(&D_800F6358);
}

void MainMenu_CheckPlayers(void) {
    if (gNumPlayers == 1) {
        Fsm_Trigger((s32) &gMenuFsm, 0x39);
        return;
    }
    if (gMainMenuSel == 3) {
        (*(s8 *)&gLoadPlayer) = 1;
        Fsm_Trigger((s32) &gMenuFsm, 0x3A);
        return;
    }
    Menu_Advance();
}

void MainMenu_SetTwoPlayers(void) {
    (*(s8 *)&gNumPlayers) = 2;
}

void MainMenu_SetOnePlayer(void) {
    (*(s8 *)&gNumPlayers) = 1;
}

void MenuModels_Alloc(void) {
    sysModelAllocBuffers();
}

void MenuModels_Free(void) {
    sysModelFreeBuffers();
}

void CarPreview_LoadP1(void) {
    u8 buf[0x40];
    u8 big[0x118];

    sysModelLoadAsync(0, gCarModelNames[gProfileP1->car], 0);
    sysCarInfoLoad(gCarModelNames[gProfileP1->car], big, buf);
    D_80140FD8();
    *(V12_ *)&D_801240E8 = *(V12_ *)(big + 0x80);
    D_80124C94 = 0;
}

void CarPreview_LoadP2(void) {
    u8 buf[0x40];
    u8 big[0x118];

    sysModelLoadAsync(1, gCarModelNames[gProfileP2->car], 0);
    sysCarInfoLoad(gCarModelNames[gProfileP2->car], big, buf);
    D_80140FD8();
    *(V12_ *)&D_801240F4 = *(V12_ *)(big + 0x80);
    D_80124C95 = 0;
}


void CarPreview_FreeP1(void) {
    D_80140FDC();
    sysModelFree(0);
}

void CarPreview_FreeP2(void) {
    D_80140FDC();
    sysModelFree(1);
}

void TrackPreview_Load(void) {
    s8 k = NameList_Add(&D_80100E10);
    u8 v = gProfileP1->track;

    if (k > 0) {
        sysWorldLoad(k, gTrackNames[(u8)(v / 3)][(u8)(v % 3)]);
    }
}


void TrackPreview_Free(void) {
    sysWorldFree(NameList_Find(&D_80100E10) & 0xFF);
    NameList_Remove(&D_80100E10);
}

void MainMenu_ResetTexts(void) {
    Menu_ResetTexts(D_800F5F28);
}

void MainMenu_LayoutTexts(void) {
    Menu_LayoutTexts(D_800F5F28);
}

extern s32 *D_800FA180[];
extern s32 *D_800F9FE0[][3];
extern u16 gProfileChoiceText[];
extern s32 *D_80141298;
extern s32 *D_8014129C;
extern s32 *D_80141260;

void MainMenu_SetupTexts(void) {
    s32 *p;
    u8 c;

    p = &D_800F5F28;
    Menu_SetText(*p, 0, (s32 *)gProfileP1->name, 286, 122, 0, 0, 1, 0, 255, 255, 255);
    Menu_SetText(*p, 2, D_800FA180[gProfileP1->car], 289, 238, 0, 0, 1, 1, 255, 255, 255);
    c = gProfileP1->track;
    Menu_SetText(*p, 4, D_800F9FE0[(u8)(c / 3)][(u8)(c % 3)], 384, 270, 0, 0, 1, 0, 255, 255, 255);
    Menu_SetText(*p, 5, ((s32 **)&gGameModeNames)[gActiveSave->gameMode], 76, 407, 0, 1, 0, 1, 127, 127, 127);
    Menu_SetText(*p, 6, (s32 *)gLangText[gProfileChoiceText[gProfileChoice]], 76, 305, 0, 1, 0, 1, 127, 127, 127);
    if (gActiveSave->gameMode == 4) {
        Menu_SetText(*p, 7, D_8014129C, 76, 170, 0, 1, 0, 1, 127, 127, 127);
    } else {
        Menu_SetText(*p, 7, D_80141298, 76, 170, 0, 1, 0, 1, 127, 127, 127);
    }
    if (gNumPlayers == 2) {
        Menu_SetText(gMenuObjs[8], 1, (s32 *)gProfileP2->name, 492, 122, 0, 0, 1, 0, 255, 255, 255);
        Menu_SetText(gMenuObjs[8], 3, D_800FA180[gProfileP2->car], 490, 238, 0, 0, 1, 1, 255, 255, 255);
    } else {
        Menu_SetText(gMenuObjs[8], 1, D_80141260, 500, 185, 0, 0, 1, 1, 255, 255, 255);
        Menu_SetText(gMenuObjs[8], 3, (s32 *)&D_80100CC8, 490, 238, 0, 0, 1, 1, 255, 255, 255);
    }
}

extern s32 D_801240F8;
extern s32 D_801240FC;
void MainMenu_Draw(void) {
    Menu_SetupCamera();
    Menu_UpdateFade();
    if (D_80124C94 == 0) {
        D_80124C94 = sysModelPollLoad(0);
    }
    if (D_80124C95 == 0) {
        D_80124C95 = sysModelPollLoad(1);
    }
    switch (gMainMenuSel) {
        case 0:
            Menu_Draw(gMenuObjs[8], 7, -1, -1, -1, -1, -1, (u8) gMenuHideText);
            break;
        case 1:
            Menu_Draw(gMenuObjs[8], -1, 0, -1, -1, -1, -1, (u8) gMenuHideText);
            break;
        case 2:
            Menu_Draw(gMenuObjs[8], -1, 1, -1, -1, -1, -1, (u8) gMenuHideText);
            break;
        case 3:
            Menu_Draw(gMenuObjs[8], 6, -1, -1, -1, -1, -1, (u8) gMenuHideText);
            break;
        case 4:
            Menu_Draw(gMenuObjs[8], -1, 2, -1, -1, -1, -1, (u8) gMenuHideText);
            break;
        case 5:
            Menu_Draw(gMenuObjs[8], 5, -1, -1, -1, -1, -1, (u8) gMenuHideText);
            break;
        default:
            Menu_Draw(gMenuObjs[8], -1, -1, -1, -1, -1, -1, (u8) gMenuHideText);
            break;
    }
    if (D_80124C94 != 0) {
        MenuCar_Draw(0, D_801240E8, D_801240EC, D_801240F0);
    }
    if (gNumPlayers == 2 && D_80124C95 != 0) {
        MenuCar_Draw(1, D_801240F4, D_801240F8, D_801240FC);
    }
    ((s32 (*)(void)) MenuGlobe_Draw)();
    if (gMenuHideText == 0) {
        Menu_DrawTicker(&D_800F6358);
    }
    Menu_EndFrame();
}

/* Checks the entered cheat code, then handles player 1's buttons 0x10/0x11 on this menu. */
void MainMenu_InputP1(s32 id) {
    if ((u8)Menu_HandlePad1((u16)id) && (u8)Config_CheckCheatCode((char *)Cheat_GetBufferP1(), gProfileP1)) {
        sysSfxPlay(0x34, 1, 1.0f, 1.0f);
        Cheat_ClearP1();
    }
    if (D_80124C94 && ((gMainMenuSel != 0 && gMainMenuSel < 4) || gMainMenuSel == 5)) {
        if ((u8)Menu_IsKeyHeld(0, 0x10)) {
            if ((u8)KeyRepeat_IsDue(0)) {
                Fsm_Trigger(&gMenuFsm, 10);
                KeyRepeat_Accelerate(0);
                KeyRepeat_Stamp(0);
                D_800F6474 = 1;
                gKeyRepeatCountP1++;
            }
        } else if ((u8)Menu_IsKeyHeld(0, 0x11)) {
            if ((u8)KeyRepeat_IsDue(0)) {
                Fsm_Trigger(&gMenuFsm, 12);
                KeyRepeat_Accelerate(0);
                KeyRepeat_Stamp(0);
                D_800F6474 = 1;
                gKeyRepeatCountP1++;
            }
        } else if (D_800F6474) {
            KeyRepeat_ResetP1();
            D_800F6474 = 0;
            gKeyRepeatCountP1 = 0;
        }
    }
}

/* Checks the entered cheat code, then handles player 2's buttons 0x10/0x11 on this menu. */
void MainMenu_InputP2(s32 id) {
    if ((u8)Menu_HandlePad2((u16)id) && (u8)Config_CheckCheatCode((char *)Cheat_GetBufferP2(), gProfileP2)) {
        sysSfxPlay(0x34, 1, 1.0f, 1.0f);
        Cheat_ClearP2();
    }
    if (D_80124C95 && gNumPlayers == 2 && gMainMenuSel == 1) {
        if ((u8)Menu_IsKeyHeld(1, 0x10)) {
            if ((u8)KeyRepeat_IsDue(1)) {
                Fsm_Trigger(&gMenuFsm, 11);
                KeyRepeat_Accelerate(1);
                KeyRepeat_Stamp(1);
                D_800F6475 = 1;
                gKeyRepeatCountP2++;
            }
        } else if ((u8)Menu_IsKeyHeld(1, 0x11)) {
            if ((u8)KeyRepeat_IsDue(1)) {
                Fsm_Trigger(&gMenuFsm, 13);
                KeyRepeat_Accelerate(1);
                KeyRepeat_Stamp(1);
                D_800F6475 = 1;
                gKeyRepeatCountP2++;
            }
        } else if (D_800F6475) {
            KeyRepeat_ResetP2();
            D_800F6475 = 0;
            gKeyRepeatCountP2 = 0;
        }
    }
}

void MainMenu_Input(void) {
    u16 a = 0x43;
    u16 b = 0x20;

    if (gNumPlayers == 1) {
        b = 0x50;
    }
    switch (gMainMenuSel) {
    case 2:
    case 5:
        break;
    case 3:
        a |= 0x10;
        b |= 0x10;
        break;
    case 0:
    case 1:
    case 4:
        a |= 0x10;
        break;
    }
    if (D_800F6476 == 0) {
        MainMenu_InputP1(a);
        MainMenu_InputP2(b);
        D_800F6476 = 1;
    } else {
        MainMenu_InputP2(b);
        MainMenu_InputP1(a);
        D_800F6476 = 0;
    }
}

void MainMenu_Animate(void) {
    Menu_Animate((*(void * *)&D_800F5F28), 8U);
}

void MainMenu_MoveUp(void) {
    if (gMainMenuSel == 1) {
        MenuCar_ScaleTo100(0);
        MenuCar_ScaleTo100(1);
        MenuCar_MoveTo(0, 450, 450, 0);
        MenuCar_MoveTo(1, 760, 450, 0);
    }
    if (gMainMenuSel == 2) {
        MenuGlobe_SpinSmall();
        MenuGlobe_MoveTo(675, 900, 0);
    }
    if (gMainMenuSel == 0) {
        gMainMenuSel = 5;
    } else {
        gMainMenuSel -= 1;
    }
    if (gMainMenuSel == 1) {
        MenuCar_ScaleTo150(0);
        MenuCar_ScaleTo150(1);
        MenuCar_MoveTo(0, 460, 480, 0);
        MenuCar_MoveTo(1, 670, 480, 0);
    }
    if (gMainMenuSel == 2) {
        MenuGlobe_SpinLarge();
        MenuGlobe_MoveTo(625, 825, 0);
    }
    MainMenu_UpdateArrows();
}


void MainMenu_MoveDown(void) {
    if (gMainMenuSel == 1) {
        MenuCar_ScaleTo100(0);
        MenuCar_ScaleTo100(1);
        MenuCar_MoveTo(0, 0x1C2, 0x1C2, 0);
        MenuCar_MoveTo(1, 0x2F8, 0x1C2, 0);
    }
    if (gMainMenuSel == 2) {
        MenuGlobe_SpinSmall();
        MenuGlobe_MoveTo(0x2A3, 0x384, 0);
    }
    if ((u8)gMainMenuSel < 5) {
        gMainMenuSel++;
    } else {
        gMainMenuSel = 0;
    }
    if (gMainMenuSel == 1) {
        MenuCar_ScaleTo150(0);
        MenuCar_ScaleTo150(1);
        MenuCar_MoveTo(0, 0x1CC, 0x1E0, 0);
        MenuCar_MoveTo(1, 0x29E, 0x1E0, 0);
    }
    if (gMainMenuSel == 2) {
        MenuGlobe_SpinLarge();
        MenuGlobe_MoveTo(0x271, 0x339, 0);
    }
    MainMenu_UpdateArrows();
}

void CarSelectP2_Prev(void) {
    if (gKeyRepeatCountP1 < 0x7C) {
        sysSfxPlay(0x30, 1, 1.0f, 1.0f);
        if (gMainMenuSel == 1 && gNumPlayers == 2) {
            Player_PrevCar(1);
        }
    }
}


void CarSelectP2_Next(void) {
    if (gKeyRepeatCountP1 < 0x7C) {
        sysSfxPlay(0x31, 1, 1.0f, 1.0f);
        if (gMainMenuSel == 1 && gNumPlayers == 2) {
            Player_NextCar(1);
        }
    }
}


void MainMenu_Select(void) {
    switch (gMainMenuSel) {
        case 1:
            Fsm_Trigger(&gMenuFsm, 0x3B);
            break;
        case 2:
            Fsm_Trigger(&gMenuFsm, 0x3C);
            break;
        case 3:
            Fsm_Trigger(&gMenuFsm, 0x3E);
            break;
        case 5:
            Fsm_Trigger(&gMenuFsm, 0x3D);
            break;
        default:
            Fsm_Trigger(&gMenuFsm, 0x3F);
            break;
    }
}


void CarSelectP1_Prev(void) {
    if (gKeyRepeatCountP1 < 0x7C) {
        sysSfxPlay(0x30, 1, 1.0f, 1.0f);
        Player_PrevCar(0);
    }
}


void CarSelectP1_Next(void) {
    if (gKeyRepeatCountP1 < 0x7C) {
        sysSfxPlay(0x31, 1, 1.0f, 1.0f);
        Player_NextCar(0);
    }
}


void GameMode_Prev(void) {
    s32 *p;

    sysSfxPlay(0x30, 1, 1.0f, 1.0f);
    p = &gActiveSave->gameMode;
    if (*p == 0) {
        *p = 4;
    } else {
        *p -= 1;
    }
}


void GameMode_Next(void) {
    u32 *p;

    sysSfxPlay(0x31, 1, 1.0f, 1.0f);
    p = (u32 *)&gActiveSave->gameMode;
    *p += 1;
    if (*p >= 5) {
        *p = 0;
    }
}


void Profile_Prev(void) {
    sysSfxPlay(0x30, 1, 1.0f, 1.0f);
    if (gSaveLoaded != 0) {
        if (gProfileChoice != 0) {
            gProfileChoice -= 1;
        } else {
            gProfileChoice = 2;
        }
    } else {
        do {
            if (gProfileChoice != 0) {
                gProfileChoice -= 1;
            } else {
                gProfileChoice = 2;
            }
        } while (gProfileChoice == 1);
    }
}


void Profile_Next(void) {
    sysSfxPlay(0x31, 1, 1.0f, 1.0f);
    if (gSaveLoaded != 0) {
        if (++gProfileChoice >= 3) {
            gProfileChoice = 0;
        }
    } else {
        do {
            if (++gProfileChoice >= 3) {
                gProfileChoice = 0;
            }
        } while (gProfileChoice == 1);
    }
}


void TrackSelect_Prev(void) {
    sysSfxPlay(0x30, 1, 1.0f, 1.0f);
    Track_SelectPrev();
}

void TrackSelect_Next(void) {
    sysSfxPlay(0x31, 1, 1.0f, 1.0f);
    Track_SelectNext();
}


void MainMenu_DispatchMode(void) {
    switch (gActiveSave->gameMode) {
    case 0:
        Fsm_Trigger(&(*(s32 *)&gMenuFsm), 0x40);
        break;
    case 1:
        Fsm_Trigger(&(*(s32 *)&gMenuFsm), 0x41);
        break;
    case 2:
        Fsm_Trigger(&(*(s32 *)&gMenuFsm), 0x42);
        break;
    case 3:
        Fsm_Trigger(&(*(s32 *)&gMenuFsm), 0x43);
        break;
    case 4:
        Fsm_Trigger(&(*(s32 *)&gMenuFsm), 0x44);
        break;
    default:
        Fsm_Trigger(&(*(s32 *)&gMenuFsm), 0x3F);
        break;
    }
}

void MainMenu_Dispatch(void) {
    switch (gMainMenuSel) {
    case 0:
        switch (gActiveSave->gameMode) {
        case 0:
            Fsm_Trigger(&(*(s32 *)&gMenuFsm), 0x40);
            break;
        case 1:
            Fsm_Trigger(&(*(s32 *)&gMenuFsm), 0x41);
            break;
        case 2:
            Fsm_Trigger(&(*(s32 *)&gMenuFsm), 0x42);
            break;
        case 3:
            Fsm_Trigger(&(*(s32 *)&gMenuFsm), 0x43);
            break;
        case 4:
            Fsm_Trigger(&(*(s32 *)&gMenuFsm), 0x44);
            break;
        default:
            Fsm_Trigger(&(*(s32 *)&gMenuFsm), 0x3F);
            break;
        }
        break;
    case 1:
        Fsm_Trigger(&(*(s32 *)&gMenuFsm), 0x46);
        break;
    case 3:
        (*(s8 *)&gLoadPlayer) = 0;
        Fsm_Trigger(&(*(s32 *)&gMenuFsm), 0x45);
        break;
    case 4:
        Fsm_Trigger(&(*(s32 *)&gMenuFsm), 0x47);
        break;
    default:
        Fsm_Trigger(&(*(s32 *)&gMenuFsm), 0x3F);
        break;
    }
}

void Profile_Select(void) {
    switch (gProfileChoice) {
        case 0:
            Fsm_Trigger(&gMenuFsm, 0x49);
            break;
        case 1:
            D_80124150 = gProfileP1->id;
            D_80124154 = gProfileP2->id;
            Fsm_Trigger(&gMenuFsm, 0x4A);
            break;
        case 2:
        default:
            Fsm_Trigger(&gMenuFsm, 0x4B);
            break;
    }
}


void MainMenu_Leave(void) {
    Menu_StartFadeOut();
    Menu_StartLeave(D_800F5F28);
    MenuCar_Hide(0U);
    MenuCar_Hide(1U);
    MenuCar_MoveTo(0U, 0x190, 0x190, 0);
    MenuCar_MoveTo(1U, 0x2BC, 0x190, 0);
    MenuGlobe_Hide();
    MenuGlobe_MoveTo(0x1F4, 0x1F4, 0);
}

void MainMenu_LeaveFrame(void) {
    Menu_Nop(D_800F5F28);
}

void Championship_PlaceArrow(s32 i, s32 x, s32 y, s32 c) {
    TBL_ *t = (TBL_ *)D_800F5F2C;
    u8 j;

    for (j = 0; j < t->n; j++) {
        if (strcmp(t->names[j].name, D_800F6478[(u8)i]) == 0) {
            if ((x < 0) & (y < 0)) {
                t->objs[j].h30 = (u8)c;
                return;
            }
            t->objs[j].h30 = (u8)c;
            t->objs[j].x = MenuPos_ToWorldX(x);
            t->objs[j].y = MenuPos_ToWorldY(y);
            t->objs[j].x2 = t->objs[j].x;
            t->objs[j].y2 = t->objs[j].y;
            return;
        }
    }
}


void Championship_UpdateArrows(void) {
    switch ((u8)gChampionshipSel) {
        case 2:
            Championship_PlaceArrow(0, 100, 590, 100);
            Championship_PlaceArrow(1, 303, 590, 100);
            break;
        case 3:
            Championship_PlaceArrow(0, 100, 690, 100);
            Championship_PlaceArrow(1, 303, 690, 100);
            break;
        case 5:
            Championship_PlaceArrow(0, 100, 897, 100);
            Championship_PlaceArrow(1, 620, 897, 100);
            break;
        default:
            Championship_PlaceArrow(0, -1, -1, 0);
            Championship_PlaceArrow(1, -1, -1, 0);
            break;
    }
}


/* Race position of a player (0 = first), by score with ties broken by index.
 * TODO(match-debt): the comma expression and the dead 'j = 0' only exist to match. */
s32 Cup_GetPosition(s32 player) {
    s32 rank;
    s32 i;
    u32 p;
    u32 a;
    u32 b;
    u32 j;

    rank = 0;
    i = 0;
    p = player & 0xFF;
    j = 0;
    do {
        j = i & 0xFF;
        if (j != p && ((a = gProfileP1->cupPoints[j],
                        b = gProfileP1->cupPoints[p], b < a) ||
                       ((a == b) & (j < p)))) {
            rank++;
        }
        i++;
    } while ((u32)(i & 0xFF) < 6);
    return rank & 0xFF;
}

u8 Cup_GetPositionNoTies(u8 n) {
    Profile *p = gProfileP1;
    u8 r = 0;
    u8 i;

    for (i = 0; i < 6; i++) {
        if (i != n && (u32)p->cupPoints[i] > (u32)p->cupPoints[n]) {
            r++;
        }
    }
    return r;
}


void Menu_EnterMainOrCup(void) {
    if ((*(u8 *)&gInChampionship) != 0) {
        Fsm_Trigger(&gMenuFsm, 0x36);
    } else {
        Fsm_Trigger(&gMenuFsm, 0x37);
    }
}


void Championship_Enter(void) {
    gInChampionship = 1;
    Menu_LoadBg((s32 *)&D_80100E74);
    Menu_StartFadeIn();
    gNumPlayers = 1;
    gKeyRepeatCountP1 = 0;
    MenuCar_Show(0);
    MenuCar_SetPos(0, 500, 500, 0);
    if ((u8)gChampionshipSel == 2) {
        MenuCar_ScaleTo150(0);
        MenuCar_MoveTo(0, 460, 480, 0);
    } else {
        MenuCar_MoveTo(0, 450, 450, 0);
    }
    MenuGlobe_Reset();
    MenuGlobe_SetPos(500, 500, 0);
    if ((u8)gChampionshipSel == 5) {
        MenuGlobe_SpinLarge();
        MenuGlobe_MoveTo(625, 825, 0);
    } else {
        MenuGlobe_MoveTo(675, 900, 0);
    }
    if (gSaveLoaded == 0 && (u8)gChampProfileChoice == 1) {
        gChampProfileChoice = 0;
    }
    Menu_StartEnter(D_800F5F2C);
    Menu_ResetTicker(&D_800F63A0);
    Championship_UpdateArrows();
}

void Cup_ResetProgress(void) {
    u8 i;

    gProfileP1->cupRace = 0;
    for (i = 0; i < 6; i++) {
        gProfileP1->cupPoints[i] = 0;
        D_800F60E8[i] = 0;
    }
}

void Cup_ValidateSelection(void) {
    if (((u8) gProfileP1->cup >= 3U) || ((u8) gProfileP1->cupRace >= 6U)) {
        gProfileP1->cup = 0U;
        Cup_ResetProgress();
    }
}

void CupTrackPreview_Load(void) {
    s8 k = NameList_Add(&D_80100E10);
    u8 v = D_800F60D4[gProfileP1->cup][gProfileP1->cupRace];

    if (k > 0) {
        sysWorldLoad(k, gTrackNames[(u8)(v / 3)][(u8)(v % 3)]);
    }
}


void Championship_Animate(void) {
    Menu_Animate((*(void * *)&D_800F5F2C), 9U);
}

void Championship_Draw(void) {
    Menu_SetupCamera();
    Menu_UpdateFade();
    if (D_80124C94 == 0) {
        D_80124C94 = sysModelPollLoad(0);
    }
    switch ((u8)gChampionshipSel) {
    case 3:
        Menu_Draw(gMenuObjs[9], 6, -1, -1, -1, -1, -1, gMenuHideText);
        break;
    case 4:
        Menu_Draw(gMenuObjs[9], -1, 3, -1, -1, -1, -1, gMenuHideText);
        break;
    case 5:
        Menu_Draw(gMenuObjs[9], 5, -1, -1, -1, -1, -1, gMenuHideText);
        break;
    default:
        Menu_Draw(gMenuObjs[9], -1, gChampionshipSel, -1, -1, -1, -1, gMenuHideText);
        break;
    }
    if (D_80124C94 != 0) {
        MenuCar_Draw(0, D_801240E8, D_801240EC, D_801240F0);
    }
    ((s32 (*)())MenuGlobe_Draw)();
    if (gMenuHideText == 0) {
        Menu_DrawTicker(&(*(u8 *)&D_800F63A0));
    }
    Menu_EndFrame();
}

void Championship_Input(void) {
    u16 mask;

    if (D_80124C94 != 0) {
        mask = 0x63;
        switch ((*(u8 *)&gChampionshipSel)) {
        case 0:
            mask |= 0x10;
            break;
        case 1:
            mask |= 0x10;
            break;
        case 2:
            mask |= 0x10;
            break;
        case 3:
            mask |= 0x10;
            break;
        case 4:
            mask |= 0x10;
            break;
        case 5:
            break;
        }
        Menu_HandlePad1(mask);
        if ((*(u8 *)&gChampionshipSel) == 2 || (*(u8 *)&gChampionshipSel) == 3
            || ((*(u8 *)&gChampionshipSel) == 5 && gProfileP1->cupRace == 0 && ((Cup_IsUnlocked(1) & 0xFF) || (Cup_IsUnlocked(2) & 0xFF)))) {
            if (Menu_IsKeyHeld(0, 0x10) & 0xFF) {
                if (KeyRepeat_IsDue(0) & 0xFF) {
                    Fsm_Trigger(&(*(s32 *)&gMenuFsm), 0xA);
                    KeyRepeat_Accelerate(0);
                    KeyRepeat_Stamp(0);
                    D_800F6480 = 1;
                    gKeyRepeatCountP1 += 1;
                }
            } else if (Menu_IsKeyHeld(0, 0x11) & 0xFF) {
                if (KeyRepeat_IsDue(0) & 0xFF) {
                    Fsm_Trigger(&(*(s32 *)&gMenuFsm), 0xC);
                    KeyRepeat_Accelerate(0);
                    KeyRepeat_Stamp(0);
                    D_800F6480 = 1;
                    gKeyRepeatCountP1 += 1;
                }
            } else if (D_800F6480 != 0) {
                KeyRepeat_ResetP1();
                D_800F6480 = 0;
                gKeyRepeatCountP1 = 0;
            }
        }
    }
}

void Cup_NextRace(void) {
    gProfileP1->cupRace = (u8) (gProfileP1->cupRace + 1);
}

void Championship_Nop(void) {

}

typedef struct { s32 lap[9]; s32 f24; u8 p28[2]; u8 f2A; u8 p2B; } CupRec_;
extern CupRec_ D_80123F60[];

INCLUDE_ASM("asm/nonmatchings/unit_53B30", Cup_SetupRace);

extern u8 D_80100EA8;
extern s32 D_801414E8;
extern s32 D_801414EC;
extern s32 D_801414F0;
extern s32 D_801414F4;
extern s32 D_801414F8;
extern s32 D_801414FC;
extern s32 D_800FA01C[][3];
extern s32 D_800FA04C[][3];
void Championship_SetupTexts(void) {
    s32 buf[8];
    s32 *p;
    s32 *q;
    s16 t;
    u8 i;
    u8 race;
    u8 pos;

    p = &D_800F5F2C;
    t = D_800F60D4[gProfileP1->cup][gProfileP1->cupRace];
    Menu_SetText(*p, 0, (s32 *)gProfileP1->name, 286, 122, 0, 0, 1, 0, 255, 255, 255);
    Menu_SetText(*p, 2, D_800FA180[gProfileP1->car], 282, 238, 0, 0, 1, 1, 255, 255, 255);
    Menu_SetText(*p, 4, D_800F9FE0[t / 3][t % 3], 384, 270, 0, 0, 1, 0, 255, 255, 255);
    Menu_SetText(*p, 5, ((s32 **)&D_8012406C)[gProfileP1->cup], 76, 407, 0, 1, 0, 1, 127, 127, 127);
    Menu_SetText(*p, 6, (s32 *)gLangText[gProfileChoiceText[gChampProfileChoice]], 76, 305, 0, 1, 0, 1, 127, 127, 127);
    race = gProfileP1->cupRace + 1;
    for (i = 0; i < 6; i++) {
        if (D_800F60D4[gProfileP1->cup][i] == -1) {
            break;
        }
    }
    sprintf((char *)buf, (char *)&D_80100EA8, D_801414E8, race, D_801414EC, i);
    Menu_SetText(gMenuObjs[9], 1, buf, 423, 135, 0, 0, 0, 1, 255, 255, 255);
    if (gProfileP1->cupRace == 0) {
        sprintf((char *)buf, (char *)D_801414F0);
    } else {
        pos = Cup_GetPosition(0) + 1;
        sprintf((char *)buf, (char *)&D_80100CA4, D_801414F4, pos);
    }
    q = &D_800F5F2C;
    Menu_SetText(*q, 3, buf, 420, 165, 0, 0, 0, 1, 255, 255, 255);
    sprintf((char *)buf, (char *)&D_80100EB4, D_801414F8, D_800FA01C[t / 3][t % 3]);
    Menu_SetText(*q, 7, buf, 417, 195, 0, 0, 0, 1, 255, 255, 255);
    sprintf((char *)buf, (char *)&D_80100EB4, D_801414FC, D_800FA04C[t / 3][t % 3]);
    Menu_SetText(*q, 8, buf, 414, 225, 0, 0, 0, 1, 255, 255, 255);
}

void Championship_ResetTexts(void) {
    Menu_ResetTexts(D_800F5F2C);
}

void Championship_LayoutTexts(void) {
    Menu_LayoutTexts(D_800F5F2C);
}

void ChampProfile_Select(void) {
    switch ((u8)gChampProfileChoice) {
        case 0:
            Fsm_Trigger(&gMenuFsm, 0x49);
            break;
        case 1:
            D_80124150 = gProfileP1->id;
            Fsm_Trigger(&gMenuFsm, 0x4A);
            break;
        case 2:
        default:
            Fsm_Trigger(&gMenuFsm, 0x4B);
            break;
    }
}


void Championship_ResetSel(void) {
    gChampionshipSel = 0;
    gInChampionship = 0;
}

void Championship_LeaveNop(void) {

}

void Championship_Leave(void) {
    Menu_StartLeave(D_800F5F2C);
    MenuCar_Hide(0);
    MenuGlobe_Hide();
    Menu_StartFadeOut();
}

void Championship_LeaveFrame(void) {
    Menu_Nop(D_800F5F2C);
}

void Championship_MoveUp(void) {
    if ((u8)gChampionshipSel == 2) {
        MenuCar_ScaleTo100(0);
        MenuCar_MoveTo(0, 450, 450, 0);
    }
    if ((u8)gChampionshipSel == 5) {
        MenuGlobe_SpinSmall();
        MenuGlobe_MoveTo(675, 900, 0);
    }
    {
        u8 t = gChampionshipSel;
        gChampionshipSel = t != 0 ? t - 1 : 5;
    }
    if ((u8)gChampionshipSel == 2) {
        MenuCar_ScaleTo150(0);
        MenuCar_MoveTo(0, 460, 480, 0);
    }
    if ((u8)gChampionshipSel == 5) {
        MenuGlobe_SpinLarge();
        MenuGlobe_MoveTo(625, 825, 0);
    }
    Championship_UpdateArrows();
}

void Championship_MoveDown(void) {
    if ((u8)gChampionshipSel == 2) {
        MenuCar_ScaleTo100(0);
        MenuCar_MoveTo(0, 450, 450, 0);
    }
    if ((u8)gChampionshipSel == 5) {
        MenuGlobe_SpinSmall();
        MenuGlobe_MoveTo(675, 900, 0);
    }
    {
        u8 t = gChampionshipSel;
        if (t < 5) {
            gChampionshipSel = t + 1;
        } else {
            gChampionshipSel = 0;
        }
    }
    if ((u8)gChampionshipSel == 2) {
        MenuCar_ScaleTo150(0);
        MenuCar_MoveTo(0, 460, 480, 0);
    }
    if ((u8)gChampionshipSel == 5) {
        MenuGlobe_SpinLarge();
        MenuGlobe_MoveTo(625, 825, 0);
    }
    Championship_UpdateArrows();
}

void Championship_Confirm(void) {
    switch ((*(u8 *)&gChampionshipSel)) {
    case 0:
        Fsm_Trigger(&(*(s32 *)&gMenuFsm), 0x4C);
        break;
    case 1:
        Fsm_Trigger(&(*(s32 *)&gMenuFsm), 0x4E);
        break;
    case 2:
        Fsm_Trigger(&(*(s32 *)&gMenuFsm), 0x46);
        break;
    case 3:
        Fsm_Trigger(&(*(s32 *)&gMenuFsm), 0x45);
        break;
    case 4:
        Fsm_Trigger(&(*(s32 *)&gMenuFsm), 0x47);
        break;
    default:
        Fsm_Trigger(&(*(s32 *)&gMenuFsm), 0x3F);
        break;
    }
}

void Championship_Select(void) {
    switch ((u8)gChampionshipSel) {
        case 2:
            Fsm_Trigger(&gMenuFsm, 0x3B);
            break;
        case 3:
            Fsm_Trigger(&gMenuFsm, 0x3E);
            break;
        case 5:
            Fsm_Trigger(&gMenuFsm, 0x50);
            break;
        default:
            Fsm_Trigger(&gMenuFsm, 0x3F);
            break;
    }
}


void Cup_Prev(void) {
    if (gProfileP1->cupRace == 0) {
        sysSfxPlay(0x30, 1, 1.0f, 1.0f);
        do {
            if (gProfileP1->cup != 0) {
                gProfileP1->cup--;
            } else {
                gProfileP1->cup = 2;
            }
        } while (Cup_IsUnlocked(gProfileP1->cup) == 0);
        Cup_ResetProgress();
    }
}

void Cup_Next(void) {
    if (gProfileP1->cupRace == 0) {
        sysSfxPlay(0x31, 1, 1.0f, 1.0f);
        do {
            if (gProfileP1->cup < 2) {
                gProfileP1->cup++;
            } else {
                gProfileP1->cup = 0;
            }
        } while (Cup_IsUnlocked(gProfileP1->cup) == 0);
        Cup_ResetProgress();
    }
}


void ChampCarSelect_Prev(void) {
    if (gKeyRepeatCountP1 < 0x7C) {
        sysSfxPlay(0x30, 1, 1.0f, 1.0f);
        Player_PrevCar(0);
    }
}


void ChampCarSelect_Next(void) {
    if (gKeyRepeatCountP1 < 0x7C) {
        sysSfxPlay(0x31, 1, 1.0f, 1.0f);
        Player_NextCar(0);
    }
}


void ChampProfile_Prev(void) {
    sysSfxPlay(0x30, 1, 1.0f, 1.0f);
    if (gSaveLoaded) {
        if (gChampProfileChoice) {
            gChampProfileChoice--;
        } else {
            gChampProfileChoice = 2;
        }
    } else {
        do {
            if (gChampProfileChoice) {
                gChampProfileChoice--;
            } else {
                gChampProfileChoice = 2;
            }
        } while (gChampProfileChoice == 1);
    }
}

void ChampProfile_Next(void) {
    sysSfxPlay(0x31, 1, 1.0f, 1.0f);
    if (gSaveLoaded) {
        if (++gChampProfileChoice >= 3) {
            gChampProfileChoice = 0;
        }
    } else {
        do {
            if (++gChampProfileChoice >= 3) {
                gChampProfileChoice = 0;
            }
        } while (gChampProfileChoice == 1);
    }
}

void CupResults_Input(void) {
    Menu_HandlePad1(0x50);
}

void Cup_CheckFinished(void) {
    u8 *p = (u8 *)gProfileP1;

    gCupWon = 0;
    if (p[0x23] >= 6 || D_800F60D4[p[0x22]][p[0x23]] == -1) {
        if ((u8)Cup_GetPosition(0) == 0) {
            Fsm_Trigger(&gMenuFsm, 0x52);
            gCupWon = 1;
        } else {
            Fsm_Trigger(&gMenuFsm, 0x53);
        }
    } else {
        Fsm_Trigger(&gMenuFsm, 0x54);
    }
}


void CupResults_Enter(void) {
    Menu_LoadBg(&D_80100ED4);
    Menu_StartEnter(D_800F5F34);
    Menu_StartFadeIn();
}

INCLUDE_ASM("asm/nonmatchings/unit_53B30", CupResults_SetupTexts);

void CupResults_ResetTexts(void) {
    Menu_ResetTexts(D_800F5F34);
}

void CupResults_Animate(void) {
    Menu_Animate((*(void * *)&D_800F5F34), 0xBU);
}

void CupResults_Draw(void) {
    Menu_SetupCamera();
    Menu_UpdateFade();
    Menu_Draw(gMenuObjs[11], -1, -1, -1, -1, -1, -1, gMenuHideText);
    Menu_EndFrame();
}

void CupResults_Leave(void) {
    Menu_StartFadeOut();
}

void CupResults_LeaveFrame(void) {

}

void RaceWin_Input(void) {
    if ((u32)sysTimeGetMillis() > (u32)(D_80124C98 + 5000)) {
        Menu_HandlePad1(0x50);
    }
}

void RaceWin_Enter(void) {
    Menu_LoadBg(&D_80100EFC);
    Menu_StartEnter(D_800F5F38);
    Menu_StartFadeIn();
    D_80124C98 = sysTimeGetMillis();
}

void RaceWin_SetupTexts(void) {
    s32 buf[16];
    s32 *fmt = (s32 *)&D_80100CA0;

    sprintf((char *)buf, fmt, D_80141504);
    Menu_SetText(gMenuObjs[12], 0, buf, 320, 210, 0, 2, 1, 1, 255, 255, 255);
    sprintf((char *)buf, (char *)&D_80100EB4, gProfileP1->name, D_80141508);
    Menu_SetText(gMenuObjs[12], 1, buf, 320, 240, 0, 2, 1, 1, 255, 255, 255);
    sprintf((char *)buf, fmt, D_8014150C);
    Menu_SetText(gMenuObjs[12], 2, buf, 320, 270, 0, 2, 1, 1, 255, 255, 255);
}

void RaceWin_ResetTexts(void) {
    Menu_ResetTexts(D_800F5F38);
}

void RaceWin_Animate(void) {
    Menu_Animate((*(void * *)&D_800F5F38), 0xCU);
}

void RaceWin_Draw(void) {
    Menu_SetupCamera();
    Menu_UpdateFade();
    Menu_Draw(gMenuObjs[12], -1, -1, -1, -1, -1, -1, gMenuHideText);
    Menu_EndFrame();
}

void RaceWin_Leave(void) {
    Menu_StartFadeOut();
}

void RaceWin_LeaveFrame(void) {

}

void RaceLose_Input(void) {
    if ((u32)sysTimeGetMillis() > (u32)(D_80124C98 + 5000)) {
        Menu_HandlePad1(0x50);
    }
}


void RaceLose_Enter(void) {
    Menu_LoadBg(&D_80100F04);
    Menu_StartEnter(D_800F5F3C);
    Menu_StartFadeIn();
    D_80124C98 = sysTimeGetMillis();
}

void RaceLose_SetupTexts(void) {
    s32 buf[16];

    sprintf((char *)buf, (char *)&D_80100F0C);
    Menu_SetText(gMenuObjs[13], 0, buf, 320, 210, 0, 2, 1, 1, 255, 255, 255);
    sprintf((char *)buf, (char *)&D_80100F14, gProfileP1->name);
    Menu_SetText(gMenuObjs[13], 1, buf, 320, 240, 0, 2, 1, 1, 255, 255, 255);
    sprintf((char *)buf, (char *)&D_80100F1C);
    Menu_SetText(gMenuObjs[13], 2, buf, 320, 270, 0, 2, 1, 1, 255, 255, 255);
}


void RaceLose_ResetTexts(void) {
    Menu_ResetTexts(D_800F5F3C);
}

void RaceLose_Animate(void) {
    Menu_Animate((*(void * *)&D_800F5F3C), 0xDU);
}

void RaceLose_Draw(void) {
    Menu_SetupCamera();
    Menu_UpdateFade();
    Menu_Draw(gMenuObjs[13], -1, -1, -1, -1, -1, -1, gMenuHideText);
    Menu_EndFrame();
}

void RaceLose_Leave(void) {
    Menu_StartFadeOut();
}

void RaceLose_LeaveFrame(void) {

}

void CupQuit_ResetTexts(void) {
    Menu_ResetTexts(D_800F5F30);
}

void CupQuit_Enter(void) {
    s32 *p = &D_800F5F30;

    gCupQuitChoice = 3;
    Menu_SetText(*p, 0, (s32 *)&D_80100F34, 320, 160, 0, 0, 1, 1, 255, 255, 255);
    Menu_SetText(*p, 1, (s32 *)&D_80100F48, 320, 190, 0, 0, 1, 1, 255, 255, 255);
    Menu_SetText(*p, 2, D_80141248, 320, 235, 0, 0, 1, 1, 127, 127, 127);
    Menu_SetText(*p, 3, D_80141254, 320, 265, 0, 0, 1, 1, 127, 127, 127);
    Menu_StartEnter(*p);
}

void CupQuit_Draw(void) {
    Menu_SetupCamera();
    Menu_UpdateFade();
    Menu_Draw(gMenuObjs[10], gCupQuitChoice, -1, -1, -1, -1, -1, 0);
    Menu_EndFrame();
}

void CupQuit_Confirm(void) {
    if ((u8)gCupQuitChoice == 3) {
        Menu_StartFadeIn();
        Fsm_Trigger(&gMenuFsm, 0x11);
    } else {
        Fsm_Trigger(&gMenuFsm, 0x4F);
    }
}


void CupQuit_MoveToYes(void) {
    gCupQuitChoice = 2;
}

void CupQuit_MoveToNo(void) {
    gCupQuitChoice = 3;
}

void CupQuit_Input(void) {
    if (*(u8 *)&gCupQuitChoice == 3) {
        Menu_HandlePad1(0x11);
    } else {
        Menu_HandlePad1(0x12);
    }
}


void Garage_DrawStatBars(Bars4_ s, s32 x) {
    u16 i;
    u8 r;
    u8 g;
    u8 b;

    for (i = 0; i < (u8) (s.a / 5); i++) {
        Menu_DrawChar(0x9A, 0, x + i * 3, 0x9D, 1, 0xF0, 0, 0x14);
    }
    for (i = 0; i < (u8) (s.b / 5); i++) {
        Menu_DrawChar(0x9A, 0, x + i * 3, 0xB3, 1, 0, 0xF0, 0x14);
    }
    for (i = 0; i < (u8) (s.c / 5); i++) {
        Menu_DrawChar(0x9A, 0, x + i * 3, 0xC8, 1, 0x14, 0, 0xF0);
    }
    if (s.lv >= 0) {
        switch (s.lv) {
            case 0:
                r = 0x4E;
                g = 0xB0;
                b = 0x53;
                break;
            case 1:
                r = 0xF6;
                g = 0xF6;
                b = 0x24;
                break;
            case 2:
                r = 0xF0;
                g = 0x28;
                b = 0x28;
                break;
            default:
                r = 0xF0;
                g = 0x46;
                b = 0xB4;
                break;
        }
        for (i = 0; i < (s.lv + 1) * 12; i++) {
            Menu_DrawChar(0x9A, 0, x + i * 3, 0xDF, 1, r, g, b);
        }
    }
}

/* Draws the stat bars of each player's car: one set in single-player mode, two in two-player mode. */
void Garage_DrawStats(void) {
    if (gNumPlayers == 1) {
        Garage_DrawStatBars(D_801240E0, 0x15C);
    } else {
        Garage_DrawStatBars(D_801240E0, 0x8A);
        if (gNumPlayers == 2) {
            Garage_DrawStatBars(D_801240E4, 0x15C);
        }
    }
}

void Garage_StartTimerP1(void) {
    D_800F6484 = sysTimeGetMillis();
}

void Garage_StartTimerP2(void) {
    D_800F6488 = sysTimeGetMillis();
}

void Garage_ToggleInfo(void) {
    s32 (**fp)() = &sysTimeGetMillis;

    if ((u32)(*fp)() > (u32)(D_800F6484 + 3000)) {
        Fsm_Trigger(&gMenuFsm, 0x5A);
        D_800F648C = D_800F648C == 0;
    }
    if (gNumPlayers == 2 && (u32)(*fp)() > (u32)(D_800F6488 + 3000)) {
        Fsm_Trigger(&gMenuFsm, 0x5B);
        D_800F648D = D_800F648D == 0;
    }
}


typedef struct { char s[5][20]; } CdtEntry_;
extern CdtEntry_ gCarNames[41];
extern s32 *D_80141264;
extern s32 *D_80141490;
extern s32 *D_80141494;
/* Player 1's save slot panel: the slot's record lines, or the empty-slot text. */
void Garage_SetCarTextsP1(void) {
    if (gGarageReadyP1 == 0) {
        if ((u8)Car_IsUnlocked(gGarageCarP1, gProfileP1) != 0) {
            Menu_SetText(gMenuObjs[14], 0, (s32 *)gCarNames[gGarageCarP1].s[0], 204, 360, 0, 0, 1, 1, 255, 255, 255);
            if (D_800F648C == 0) {
                Menu_SetText(gMenuObjs[14], 1, (s32 *)gCarNames[gGarageCarP1].s[1], 204, 385, 0, 0, 1, 1, 255, 255, 255);
                Menu_SetText(gMenuObjs[14], 2, (s32 *)gCarNames[gGarageCarP1].s[2], 204, 410, 0, 0, 1, 1, 255, 255, 255);
            } else {
                Menu_SetText(gMenuObjs[14], 1, (s32 *)gCarNames[gGarageCarP1].s[3], 204, 385, 0, 0, 1, 1, 255, 255, 255);
                Menu_SetText(gMenuObjs[14], 2, (s32 *)gCarNames[gGarageCarP1].s[4], 204, 410, 0, 0, 1, 1, 255, 255, 255);
            }
        } else {
            Menu_SetText(gMenuObjs[14], 0, D_80141264, 204, 360, 0, 0, 1, 1, 255, 255, 255);
            Menu_SetText(gMenuObjs[14], 1, D_80141490, 204, 385, 0, 0, 1, 1, 255, 255, 255);
            Menu_SetText(gMenuObjs[14], 2, D_80141494, 204, 410, 0, 0, 1, 1, 255, 255, 255);
        }
    }
}

/* Player 2's save slot panel: the slot's record lines, or the empty-slot text. */
void Garage_SetCarTextsP2(void) {
    if (gGarageReadyP2 == 0) {
        if ((u8)Car_IsUnlocked(gGarageCarP2, gProfileP2) != 0) {
            Menu_SetText(gMenuObjs[15], 0, (s32 *)gCarNames[gGarageCarP2].s[0], 444, 360, 0, 0, 1, 1, 255, 255, 255);
            if (D_800F648D) {
                Menu_SetText(gMenuObjs[15], 1, (s32 *)gCarNames[gGarageCarP2].s[1], 444, 385, 0, 0, 1, 1, 255, 255, 255);
                Menu_SetText(gMenuObjs[15], 2, (s32 *)gCarNames[gGarageCarP2].s[2], 444, 410, 0, 0, 1, 1, 255, 255, 255);
            } else {
                Menu_SetText(gMenuObjs[15], 1, (s32 *)gCarNames[gGarageCarP2].s[3], 444, 385, 0, 0, 1, 1, 255, 255, 255);
                Menu_SetText(gMenuObjs[15], 2, (s32 *)gCarNames[gGarageCarP2].s[4], 444, 410, 0, 0, 1, 1, 255, 255, 255);
            }
        } else {
            Menu_SetText(gMenuObjs[15], 0, D_80141264, 444, 360, 0, 0, 1, 1, 255, 255, 255);
            Menu_SetText(gMenuObjs[15], 1, D_80141490, 444, 385, 0, 0, 1, 1, 255, 255, 255);
            Menu_SetText(gMenuObjs[15], 2, D_80141494, 444, 410, 0, 0, 1, 1, 255, 255, 255);
        }
    }
}

void Garage_CheckReady(void) {
    if (((gNumPlayers == 1) && (gGarageReadyP1 != 0)) || ((gNumPlayers == 2) && (gGarageReadyP1 != 0) && (gGarageReadyP2 != 0))) {
        Fsm_Trigger((s32) &gMenuFsm, 0x59);
    }
}

void Garage_ResetTexts(void) {
    Menu_ResetTexts(D_800F5F40);
    Menu_ResetTexts(D_800F5F44);
}

void Garage_LayoutTexts(void) {
    Menu_LayoutTexts(D_800F5F40);
    Menu_LayoutTexts(D_800F5F44);
}

void Garage_DispatchPlayers(void) {
    if (gNumPlayers == 1 || *(u8 *)&gInChampionship != 0) {
        Fsm_Trigger(&gMenuFsm, 0x57);
    } else {
        Fsm_Trigger(&gMenuFsm, 0x58);
    }
}


void Garage1_Enter(void) {
    Menu_LoadBg(&D_80100F58);
    Menu_StartFadeIn();
    MenuCar_Show(0);
    MenuCar_ScaleTo200(0);
    MenuCar_MoveTo(0, 0x1A9, 0x20D, 0);
    (*(s8 *)&gGarageReadyP1) = 0;
    gGarageDirtyP1 = 1;
    gKeyRepeatCountP1 = 0;
    Menu_StartEnter(D_800F5F40);
    gGarageCarP1 = gProfileP1->car;
}

void Garage_Enter(void) {
    Menu_LoadBg(&D_80100F60);
    Menu_StartFadeIn();
    gKeyRepeatCountP1 = 0;
    gKeyRepeatCountP2 = 0;
    MenuCar_Show(0);
    MenuCar_ScaleTo175(0);
    MenuCar_MoveTo(0, 0x186, 0x262, 0);
    MenuCar_Show(1);
    MenuCar_ScaleTo175(1);
    MenuCar_MoveTo(1, 0x262, 0x262, 0);
    (*(s8 *)&gGarageReadyP1) = 0;
    (*(s8 *)&gGarageReadyP2) = 0;
    gGarageDirtyP1 = 1;
    gGarageDirtyP2 = 1;
    Menu_StartEnter(D_800F5F40);
    Menu_StartEnter(D_800F5F44);
    gGarageCarP1 = gProfileP1->car;
    gGarageCarP2 = gProfileP2->car;
}

void Garage_InputP1(u16 id) {
    if (D_80124C94) {
        Menu_HandlePad1(id);
        if ((u8)Menu_IsKeyHeld(0, 14)) {
            if ((u8)KeyRepeat_IsDue(0)) {
                Fsm_Trigger(&gMenuFsm, 6);
            KeyRepeat_Accelerate(0);
            KeyRepeat_Stamp(0);
            D_800F648E = 1;
            gKeyRepeatCountP1++;
            }
        } else if ((u8)Menu_IsKeyHeld(0, 15)) {
            if ((u8)KeyRepeat_IsDue(0)) {
                Fsm_Trigger(&gMenuFsm, 8);
            KeyRepeat_Accelerate(0);
            KeyRepeat_Stamp(0);
            D_800F648E = 1;
            gKeyRepeatCountP1++;
            }
        } else {
            KeyRepeat_ResetP1();
            D_800F648E = 0;
            gKeyRepeatCountP1 = 0;
        }
    }
}

void Garage_InputP2(u16 id) {
    if (D_80124C95) {
        Menu_HandlePad2(id);
        if ((u8)Menu_IsKeyHeld(1, 14)) {
            if ((u8)KeyRepeat_IsDue(1)) {
                Fsm_Trigger(&gMenuFsm, 7);
            KeyRepeat_Accelerate(1);
            KeyRepeat_Stamp(1);
            D_800F648F = 1;
            gKeyRepeatCountP2++;
            }
        } else if ((u8)Menu_IsKeyHeld(1, 15)) {
            if ((u8)KeyRepeat_IsDue(1)) {
                Fsm_Trigger(&gMenuFsm, 9);
            KeyRepeat_Accelerate(1);
            KeyRepeat_Stamp(1);
            D_800F648F = 1;
            gKeyRepeatCountP2++;
            }
        } else {
            if (D_800F648F) {
                KeyRepeat_ResetP2();
                D_800F648F = 0;
                gKeyRepeatCountP2 = 0;
            }
        }
    }
}

void Garage_Input(void) {
    s16 a;
    s16 b;

    if (gNumPlayers == 2) {
        b = 0x10;
        /* TODO(match-debt): branchless form of (gGarageReadyP1 == 0 ? 0x50 : 0x20); the ternary does not match. */
        a = (-(gGarageReadyP1 == 0) & 0x30) | 0x20;
        if (gGarageReadyP2 != 0) {
            b = 0x20;
        }
        if (D_800F6490 == 0) {
            Garage_InputP1(a);
            Garage_InputP2(b);
            D_800F6490 = 1;
            return;
        }
        Garage_InputP2(b);
        Garage_InputP1(a);
        D_800F6490 = 0;
        return;
    }
    if (gGarageReadyP1 == 0) {
        Garage_InputP1(0x70);
    }
}

void Garage_Animate(void) {
    Menu_Animate((*(void * *)&D_800F5F40), 0xEU);
    if (gNumPlayers == 2) {
        Menu_Animate((*(void * *)&D_800F5F44), 0xFU);
    }
}

INCLUDE_ASM("asm/nonmatchings/unit_53B30", Garage_SetupTexts);

extern s32 D_80100F68;
extern f32 D_800F6494;
extern f32 D_800F6498;
void Garage_Draw(void) {
    u8 id;
    f32 twoPi;
    s32 *p;

    Menu_SetupCamera();
    if (D_80124C94 == 0) {
        D_80124C94 = sysModelPollLoad(0);
    }
    if (D_80124C95 == 0) {
        D_80124C95 = sysModelPollLoad(1);
    }
    Menu_UpdateFade();
    if (gMenuHideText == 0) {
        Garage_DrawStats();
    }
    p = &D_800F5F40;
    Menu_Draw(*p, -1, -1, -1, -1, -1, -1, gMenuHideText);
    if ((u8)Car_IsUnlocked(gGarageCarP1, gProfileP1)) {
        if (D_80124C94) {
            MenuCar_Draw(0, D_801240E8, D_801240EC, D_801240F0);
        }
    } else if (gMenuHideText == 0) {
        id = NameList_Find(&D_80100F68);
        twoPi = 6.28318f;
        D_800F6494 += Menu_DeltaSeconds(6) * 1.256636f;
        while (twoPi < D_800F6494) {
            D_800F6494 -= twoPi;
        }
        if (gNumPlayers == 1) {
            Menu_DrawModel(id, MenuPos_ToWorldX(330), MenuPos_ToWorldY(460), 0.0f, -1.3962624f, 0.0f, D_800F6494, 2.0f);
        } else {
            Menu_DrawModel(id, MenuPos_ToWorldX(330), MenuPos_ToWorldY(640), 0.0f, -1.3962624f, 0.0f, D_800F6494, 1.5f);
        }
    }
    if (gNumPlayers == 2) {
        p = &D_800F5F44;
        Menu_Draw(*p, -1, -1, -1, -1, -1, -1, gMenuHideText);
        if ((u8)Car_IsUnlocked(gGarageCarP2, gProfileP2)) {
            if (D_80124C95) {
                MenuCar_Draw(1, D_801240F4, D_801240F8, D_801240FC);
            }
        } else if (gMenuHideText == 0) {
            id = NameList_Find(&D_80100F68);
            twoPi = 6.28318f;
            D_800F6498 += Menu_DeltaSeconds(7) * 1.256636f;
            while (twoPi < D_800F6498) {
                D_800F6498 -= twoPi;
            }
            Menu_DrawModel(id, MenuPos_ToWorldX(700), MenuPos_ToWorldY(640), 0.0f, -1.3962624f, 0.0f, D_800F6498, 1.5f);
        }
    }
    Menu_EndFrame();
}

void Garage_CancelP1(void) {
    if (gGarageReadyP1 != 0) {
        gGarageReadyP1 = 0;
        gGarageDirtyP1 = 1;
        MenuCar_ScaleTo175(0U);
        MenuCar_MoveTo(0U, 0x186, 0x262, 0);
        Menu_Advance();
        return;
    }
    Fsm_Trigger((s32) &gMenuFsm, 0x5C);
}

void Garage_CancelP2(void) {
    if (gGarageReadyP2 != 0) {
        gGarageReadyP2 = 0;
        gGarageDirtyP2 = 1;
        MenuCar_ScaleTo175(1U);
        MenuCar_MoveTo(1U, 0x262, 0x262, 0);
    }
    Menu_Advance();
}

void Garage_ConfirmP1(void) {
    if ((u8)Car_IsUnlocked(gGarageCarP1, gProfileP1) != 0) {
        gProfileP1->car = gGarageCarP1;
        gGarageReadyP1 = 1;
        if (gNumPlayers == 2) {
            MenuCar_MoveTo(0, 325, 650, 0);
            MenuCar_ScaleTo100(0);
        }
    } else {
        sysSfxPlay(0x35, 1, 1.0f, 1.0f);
        Garage_SetCarTextsP1();
    }
}

void Garage_ConfirmP2(void) {
    if (gNumPlayers != 1) {
        if ((u8)Car_IsUnlocked(gGarageCarP2, gProfileP2) != 0) {
            gProfileP2->car = gGarageCarP2;
            gGarageReadyP2 = 1;
            MenuCar_MoveTo(1, 675, 650, 0);
            MenuCar_ScaleTo100(1);
        } else {
            sysSfxPlay(0x35, 1, 1.0f, 1.0f);
            Garage_SetCarTextsP2();
        }
    }
}


void GarageCar_LoadP1(void) {
    u8 buf[0x40];
    u8 big[0x118];

    if ((u8)Car_IsUnlocked(gGarageCarP1, gProfileP1) != 0) {
        sysModelLoadAsync(0, gCarModelNames[gGarageCarP1], 1);
        sysCarInfoLoad(gCarModelNames[gGarageCarP1], big, buf);
        D_80140FD8();
        *(V12_ *)&D_801240E8 = *(V12_ *)(big + 0x80);
        D_80124C94 = 0;
        D_801240E0.a = big[0x110];
        D_801240E0.b = big[0x112];
        D_801240E0.c = big[0x111];
        D_801240E0.lv = big[0x113];
    } else {
        D_801240E0.lv = -1;
        D_801240E0.a = 0;
        D_801240E0.b = 0;
        D_801240E0.c = 0;
        D_80124C94 = 1;
    }
}

void GarageCar_FreeP1(void) {
    if (Car_IsUnlocked(gGarageCarP1, gProfileP1) & 0xFF) {
        D_80140FDC();
        sysModelFree(0);
    }
}

void GarageCar_LoadP2(void) {
    u8 buf[0x40];
    u8 big[0x118];

    if (gNumPlayers != 2) {
        return;
    }
    if ((u8)Car_IsUnlocked(gGarageCarP2, gProfileP2) != 0) {
        sysModelLoadAsync(1, gCarModelNames[gGarageCarP2], 1);
        sysCarInfoLoad(gCarModelNames[gGarageCarP2], big, buf);
        D_80140FD8();
        *(V12_ *)&D_801240F4 = *(V12_ *)(big + 0x80);
        D_80124C95 = 0;
        D_801240E4.a = big[0x110];
        D_801240E4.b = big[0x112];
        D_801240E4.c = big[0x111];
        D_801240E4.lv = big[0x113];
    } else {
        D_801240E4.lv = -1;
        D_801240E4.a = 0;
        D_801240E4.b = 0;
        D_801240E4.c = 0;
        D_80124C95 = 1;
    }
}


void GarageCar_FreeP2(void) {
    if ((gNumPlayers == 2) && (Car_IsUnlocked(gGarageCarP2, gProfileP2) & 0xFF)) {
        D_80140FDC();
        sysModelFree(1);
    }
}

void Garage_NextCarP1(void) {
    if (gGarageReadyP1 == 0 && gKeyRepeatCountP1 < 0x7C) {
        if (++gGarageCarP1 > 40) {
            gGarageCarP1 = 0;
        }
        gGarageDirtyP1 = 1;
        sysSfxPlay(0x2F, 1, 1.0f, 1.0f);
    }
}


void Garage_NextCarP2(void) {
    if (gGarageReadyP2 == 0 && *(u8 *)&gKeyRepeatCountP2 < 0x7C) {
        if (++gGarageCarP2 > 40) {
            gGarageCarP2 = 0;
        }
        gGarageDirtyP2 = 1;
        sysSfxPlay(0x2F, 1, 1.0f, 1.0f);
    }
}


void Garage_PrevCarP1(void) {
    if (gGarageReadyP1 == 0 && gKeyRepeatCountP1 < 0x7C) {
        if (gGarageCarP1 == 0) {
            gGarageCarP1 = 40;
        } else {
            gGarageCarP1 -= 1;
        }
        gGarageDirtyP1 = 1;
        sysSfxPlay(0x2E, 1, 1.0f, 1.0f);
    }
}


void Garage_PrevCarP2(void) {
    if (gGarageReadyP2 == 0 && *(u8 *)&gKeyRepeatCountP2 < 0x7C) {
        if (gGarageCarP2 == 0) {
            gGarageCarP2 = 40;
        } else {
            gGarageCarP2 -= 1;
        }
        gGarageDirtyP2 = 1;
        sysSfxPlay(0x2E, 1, 1.0f, 1.0f);
    }
}


void Garage_Leave(void) {
    MenuCar_MoveTo(0U, 0x1F4, 0x1F4, 0);
    MenuCar_Hide(0U);
    Menu_StartLeave(D_800F5F40);
    if (gNumPlayers == 2) {
        MenuCar_Hide(1U);
        MenuCar_MoveTo(1U, 0x1F4, 0x1F4, 0);
        Menu_StartLeave(D_800F5F44);
    }
    Menu_StartFadeOut();
}

void Garage_LeaveFrame(void) {
    Menu_Nop(D_800F5F40);
    if (gNumPlayers == 2) {
        Menu_Nop(D_800F5F44);
    }
}

void Options_UpdateSongTexts(void) {
    TBL_ *t = (TBL_ *)D_800F5F48;
    u8 j;

    for (j = 0; j < t->n; j++) {
        if (strcmp(t->names[j].name, D_80100F70) == 0) {
            t->objs[j].x = MenuPos_ToWorldX(gMusicMode * 185 + 460);
            t->objs[j].y = MenuPos_ToWorldY(725);
            t->objs[j].x2 = t->objs[j].x;
            t->objs[j].y2 = t->objs[j].y;
        }
    }
    if (gMusicMode == 2) {
        Menu_SetText((s32)t, 0, D_80141268, 73, 418, 0, 1, 0, 1, 127, 127, 127);
    } else {
        Menu_SetText((s32)t, 0, (s32 *)&D_80100CC8, 73, 414, 0, 1, 0, 1, 127, 127, 127);
    }
    Menu_SetText((s32)t, 1, Jukebox_GetArtist((u8)gMenuSong), 426, 385, 0, 0, 1, 1, 255, 255, 255);
    Menu_SetText((s32)t, 2, Jukebox_GetTitle((u8)gMenuSong), 426, 410, 0, 0, 1, 1, 255, 255, 255);
}

void Options_Enter(void) {
    Menu_LoadBg(&D_80100F78);
    Menu_StartFadeIn();
    Menu_StartEnter(D_800F5F48);
    Options_UpdateSongTexts();
    Menu_ResetTicker(&D_800F6370);
}

void Options_Input(void) {
    u16 a = 0x63;

    switch (gOptionsSel) {
        case 4:
        case 5:
        case 6:
            a = 0x73;
            break;
        case 7:
            if (gMusicMode != 0) {
                a = 0x67;
            }
            if (gMusicMode < 2) {
                a |= 8;
            }
            break;
        case 8:
            a = 0x6F;
            break;
    }
    Menu_HandlePad1(a);
}


void Options_Animate(void) {
    Menu_Animate((*(void * *)&D_800F5F48), 0x10U);
}

void Options_Draw(void) {
    u8 t;
    Menu_SetupCamera();
    Menu_UpdateFade();
    t = gOptionsSel;
    Menu_Draw(gMenuObjs[16], -(t != 8), (s8)t, -1, -1, -1, -1, gMenuHideText);
    if (gMenuHideText == 0) {
        Menu_DrawTicker(&D_800F6370);
    }
    Menu_EndFrame();
}

void Options_LayoutTexts(void) {
    Menu_LayoutTexts(D_800F5F48);
}

void Options_ResetTexts(void) {
    Menu_ResetTexts(D_800F5F48);
}

void Options_CheckSlider(void) {
    if (gOptionsSel < 4) {
        sysInputUpdate(0);
        if ((u8)Menu_IsKeyHeld(0, 0x10) != 0) {
            Fsm_Trigger(&gMenuFsm, 0x5D);
        } else if ((u8)Menu_IsKeyHeld(0, 0x11) != 0) {
            Fsm_Trigger(&gMenuFsm, 0x5E);
        } else {
            Fsm_Trigger(&gMenuFsm, 0x11);
        }
    }
}

void Options_PreviewVolume(void) {
    switch (gOptionsSel) {
        case 2:
            sysSfxPlay(0x38, 1, gActiveSave->sfxVolume / 255.0f, 1.0f);
            break;
        case 3:
            sysSfxPlay(0x37, 1, gActiveSave->voiceVolume / 255.0f, 1.0f);
            break;
        case 1:
            break;
    }
}


void Options_Left(void) {
    switch (gOptionsSel) {
    case 7:
        if (gMusicMode != 0) {
            gMusicMode -= 1;
            Options_UpdateSongTexts();
            return;
        }
        return;
    case 8:
        Song_Prev();
        Options_UpdateSongTexts();
        Music_StopMenuTrack();
        gMenuMusicTrack = (*(u8 *)&gMenuSong);
        Music_StartMenuTrack();
        break;
    }
}

void Options_Right(void) {
    switch (gOptionsSel) {
    case 7:
        if ((u8) gMusicMode < 2U) {
            gMusicMode += 1;
            Options_UpdateSongTexts();
            return;
        }
        return;
    case 8:
        Song_Next();
        Options_UpdateSongTexts();
        Music_StopMenuTrack();
        gMenuMusicTrack = (*(u8 *)&gMenuSong);
        Music_StartMenuTrack();
        break;
    }
}

void Options_Decrease(void) {
    switch (gOptionsSel) {
    case 0:
        if (gActiveSave->option0 > 10) {
            gActiveSave->option0 -= 10;
            sysSfxPlay(0x30, 1, 1.0f, 1.0f);
        } else {
            gActiveSave->option0 = 0;
        }
        break;
    case 1:
        if (gActiveSave->musicVolume > 10) {
            gActiveSave->musicVolume -= 10;
        } else {
            gActiveSave->musicVolume = 0;
        }
        sysAudioSetMusicVolume(gActiveSave->musicVolume);
        break;
    case 2:
        if (gActiveSave->sfxVolume > 10) {
            gActiveSave->sfxVolume -= 10;
        } else {
            gActiveSave->sfxVolume = 0;
        }
        break;
    case 3:
        if (gActiveSave->voiceVolume > 10) {
            gActiveSave->voiceVolume -= 10;
        } else {
            gActiveSave->voiceVolume = 0;
        }
        break;
    }
}

void Options_Increase(void) {
    switch (gOptionsSel) {
    case 0:
        if (gActiveSave->option0 < 245) {
            gActiveSave->option0 += 10;
            sysSfxPlay(0x31, 1, 1.0f, 1.0f);
        } else {
            gActiveSave->option0 = 255;
        }
        break;
    case 1:
        if (gActiveSave->musicVolume < 245) {
            gActiveSave->musicVolume += 10;
        } else {
            gActiveSave->musicVolume = 255;
        }
        sysAudioSetMusicVolume(gActiveSave->musicVolume);
        break;
    case 2:
        if (gActiveSave->sfxVolume < 245) {
            gActiveSave->sfxVolume += 10;
        } else {
            gActiveSave->sfxVolume = 255;
        }
        break;
    case 3:
        if (gActiveSave->voiceVolume < 245) {
            gActiveSave->voiceVolume += 10;
        } else {
            gActiveSave->voiceVolume = 255;
        }
        break;
    }
}

/* Steps the menu cursor gOptionsSel down; from 0 it wraps to 8 when gMusicMode == 2, else to 7. */
void Options_MoveUp(void) {
    if (gOptionsSel != 0) {
        gOptionsSel--;
    } else if (gMusicMode == 2) {
        gOptionsSel = 8;
    } else {
        gOptionsSel = 7;
    }
}

void Options_MoveDown(void) {
    if (gMusicMode == 2) {
        if (gOptionsSel < 8) {
            gOptionsSel++;
        } else {
            gOptionsSel = 0;
        }
    } else {
        if (gOptionsSel < 7) {
            gOptionsSel++;
        } else {
            gOptionsSel = 0;
        }
    }
}


void Options_ResetSel(void) {
    gOptionsSel = 0;
}

void Options_LayoutColorBars(void) {
    u8 c[4];
    u8 i;

    c[0] = gActiveSave->option0;
    c[1] = gActiveSave->musicVolume;
    c[2] = gActiveSave->sfxVolume;
    c[3] = gActiveSave->voiceVolume;
    for (i = 0; i < 4; i++) {
        (*(ElC05_ **)(D_800F5F48 + 0x28))[i].fC = MenuPos_ToWorldX((s32)(c[i] * 2.1568627f) + 360);
        (*(ElC05_ **)(D_800F5F48 + 0x28))[i].f10 = MenuPos_ToWorldY(i * 0x54 + 0x136);
    }
}

void Options_Select(void) {
    u8 v = gOptionsSel;

    if (v == 4) {
        Fsm_Trigger(&gMenuFsm, 0x5F);
    } else if (v == 5) {
        Fsm_Trigger(&gMenuFsm, 0x61);
    } else if (v == 6) {
        Fsm_Trigger(&gMenuFsm, 0x60);
    } else {
        Fsm_Trigger(&gMenuFsm, 0x11);
    }
}


void Options_Leave(void) {
    Menu_StartLeave(D_800F5F48);
    Menu_StartFadeOut();
}

void Options_LeaveFrame(void) {
    Menu_Nop(D_800F5F48);
}

void Credits_Enter(void) {
    Menu_LoadBg(&D_80100F80);
    Menu_StartFadeIn();
    Menu_StartEnter(D_800F5F60);
    D_800F61D6 = 0;
    Scroller_Reset(&D_800F2FA0);
    Scroller_Reset(&D_800F2FC0);
    Menu_ResetTicker(&D_800F63E8);
}

void Credits_Input(void) {
    if (*(u8 *)&D_800F61D6 == 0) {
        Menu_HandlePad1(0x62);
    } else {
        Menu_HandlePad1(0x61);
    }
}

void Credits_Animate(void) {
    Menu_Animate((*(void * *)&D_800F5F60), 0x16U);
}

void Scroller_Draw(void *, u8);
void Credits_Draw(void) {
    u8 fast;

    Menu_SetupCamera();
    Menu_UpdateFade();
    Menu_Draw((Menu_ *)gMenuObjs[22], -1, D_800F61D6, -1, -1, -1, -1, gMenuHideText);
    fast = 0;
    if ((u8)Menu_IsKeyHeld(0, 0x12) || (u8)Menu_IsKeyHeld(1, 0x12)) {
        fast = 1;
    }
    if (gMenuHideText == 0) {
        switch ((u8)D_800F61D6) {
        case 0:
            Scroller_Draw(&D_800F2FA0, fast);
            break;
        case 1:
            Scroller_Draw(&D_800F2FC0, fast);
            break;
        }
        Menu_DrawTicker((Scroll4E_ *)&D_800F63E8);
    }
    Menu_EndFrame();
}

void Credits_ShowPage1(void) {
    if ((u8)D_800F61D6 == 1) {
        D_800F61D6 = 0;
        Menu_LoadBg(((u8 *)&D_80100F80));
    }
}


void Credits_ShowPage2(void) {
    u8 v = D_800F61D6;

    if (v == 0) {
        D_800F61D6 = 1;
        Menu_LoadBg(D_80100F88);
    }
}


void Credits_Leave(void) {
    Menu_StartLeave(D_800F5F60);
    Menu_StartFadeOut();
}

void Credits_LeaveFrame(void) {
    Menu_Nop(D_800F5F60);
}

void Controls_LoadFont(void) {
    sysFontLoad(2, &D_80100F94);
}


void Controls_FreeFont(void) {
    sysFontFree(2);
}

void Controls_PlaceArrow(u8 which, u8 name, s32 x, s32 y, u8 val) {
    MnC0_ *m;
    u8 i;

    if (which == 0) {
        m = (MnC0_ *)D_800F5F4C;
    } else {
        m = (MnC0_ *)D_800F5F50;
    }
    for (i = 0; i < m->n; i++) {
        if (strcmp(m->names[i].s, D_800F649C[name]) == 0) {
            m->elems[i].h30 = val;
            m->elems[i].h32 = val;
            m->elems[i].x = MenuPos_ToWorldX(x);
            m->elems[i].y = MenuPos_ToWorldY(y);
            m->elems[i].xC = m->elems[i].x;
            m->elems[i].x10 = m->elems[i].y;
            return;
        }
    }
}

void Controls_UpdateArrowsP1(void) {
    Controls_PlaceArrow(0, 0, 200, 310, D_80124107 ? 120 : 0);
    Controls_PlaceArrow(0, 1, 250, 870, D_80124109 < 13 ? 100 : 0);
}


void Controls_UpdateArrowsP2(void) {
    if (gNumPlayers == 2) {
        Controls_PlaceArrow(1, 0, 775, 310, D_80124108 ? 120 : 0);
        if (D_8012410A < 13) {
            Controls_PlaceArrow(1, 1, 825, 870, 100);
        } else {
            Controls_PlaceArrow(1, 1, 825, 870, 0);
        }
    } else {
        Controls_PlaceArrow(1, 0, 700, 310, 0);
        Controls_PlaceArrow(1, 1, 750, 870, 0);
    }
}


void Controls_Enter(void) {
    void (**fp)(s32, s32);

    Menu_LoadBg((s32 *)&D_80100FA4);
    Menu_StartFadeIn();
    fp = &sysInputLoadBindings;
    (*fp)((s32)gProfileP1->bindings, 0);
    (*fp)((s32)gProfileP2->bindings, 1);
    gControlsRemapP1 = 0;
    gControlsRemapP2 = 0;
    Menu_StartEnter(gMenuObjs[17]);
    Menu_StartEnter(D_800F5F50);
    Menu_StartEnter(D_800F5F54);
    Menu_StartEnter(D_800F5F58);
    MenuList_Reset(gMenuObjs[17]);
    MenuList_Reset(D_800F5F54);
    MenuList_Reset(D_800F5F50);
    MenuList_Reset(D_800F5F58);
    D_8012410B = 0;
    D_8012410C = 0;
    D_80124107 = 0;
    D_80124108 = 0;
    D_80124109 = 5;
    D_8012410A = 5;
    Controls_UpdateArrowsP1();
    Controls_UpdateArrowsP2();
    Menu_ResetTicker(&D_800F63B8);
}


extern s32 D_8014126C;
extern char *(*sysInputGetKeyName)(s32 player, u8 action);
void Controls_SetupTexts(void) {
    u8 buf[32];
    u8 i;
    u8 j;
    s32 y;
    u8 g;
    void *m;
    void *t;

    j = 0;
    y = 173;
    for (i = D_80124107; i <= D_80124109; j++, i++) {
        Menu_SetText((Ls2C_ *)gMenuObjs[17], j, (void *)D_80124110[i], 138, y, 0, 0, 1, 1, 127, 127, 127);
        g = Text_ButtonGlyphs(((char *(*)(s32, s32))sysInputGetKeyName)(0, i), buf);
        if (g) {
            Menu_SetText((Ls2C_ *)gMenuObjs[19], j, buf, 250, y - 15, 0, 0, 0, 2, 127, 127, 127);
        } else {
            Menu_SetText((Ls2C_ *)gMenuObjs[19], j, buf, 270, y, 0, 0, 1, 1, 127, 127, 127);
        }
        y += 50;
    }
    if (gNumPlayers == 2) {
        j = 0;
        y = 173;
        for (i = D_80124108; i <= D_8012410A; j++, i++) {
            Menu_SetText((Ls2C_ *)gMenuObjs[18], j, (void *)D_80124110[i], 510, y, 0, 0, 1, 1, 127, 127, 127);
            g = Text_ButtonGlyphs(((char *(*)(s32, s32))sysInputGetKeyName)(1, i), buf);
            if (g) {
                Menu_SetText((Ls2C_ *)gMenuObjs[20], j, buf, 340, y - 15, 0, 0, 0, 2, 127, 127, 127);
            } else {
                Menu_SetText((Ls2C_ *)gMenuObjs[20], j, buf, 375, y, 0, 0, 1, 1, 127, 127, 127);
            }
            y += 50;
        }
    }
    if (*(u8 *)&gControlsRemapP1) {
        Menu_SetText(m = (Ls2C_ *)D_800F5F54, D_8012410B, t = (void *)D_8014126C, 270, D_8012410B * 50 + 173, 0, 0, 1, 1, 127, 127, 127);
    }
    if (*(u8 *)&gControlsRemapP2) {
        Menu_SetText(m = (Ls2C_ *)D_800F5F58, D_8012410C, t = (void *)D_8014126C, 380, D_8012410C * 50 + 173, 0, 0, 1, 1, 127, 127, 127);
    }
}

void Controls_Animate(void) {
    Menu_Animate((*(void * *)&D_800F5F4C), 0x11U);
    Menu_Animate((*(void * *)&D_800F5F54), 0x13U);
    Menu_Animate((*(void * *)&D_800F5F50), 0x12U);
    Menu_Animate((*(void * *)&D_800F5F58), 0x14U);
}

/* Draws the four option rows of this menu page (values taken modulo 5). */
void Controls_Draw(void) {
    Menu_SetupCamera();
    Menu_UpdateFade();
    Menu_Draw(gMenuObjs[17], (s8)(D_8012410B % 5), -1, -1, -1, -1, -1, gMenuHideText);
    Menu_Draw(gMenuObjs[19], (s8)(D_8012410B % 5), -1, -1, -1, -1, -1, gMenuHideText);
    Menu_Draw(gMenuObjs[18], (s8)(D_8012410C % 5), -1, -1, -1, -1, -1, gMenuHideText);
    Menu_Draw(gMenuObjs[20], (s8)(D_8012410C % 5), -1, -1, -1, -1, -1, gMenuHideText);
    if (gMenuHideText == 0) {
        Menu_DrawTicker(&D_800F63B8);
    }
    Menu_EndFrame();
}

void Controls_ScrollUp(void) {
    if (D_8012410B != 0) {
        D_8012410B -= 1;
    } else if (D_80124107 != 0) {
        D_80124107 -= 1;
        D_80124109 -= 1;
    }
    Controls_UpdateArrowsP1();
}

void Controls_ScrollDown(void) {
    if ((u8) D_8012410B < 4U) {
        D_8012410B += 1;
    } else if ((u8) D_80124109 < 0xDU) {
        D_80124109 += 1;
        D_80124107 += 1;
    }
    Controls_UpdateArrowsP1();
}

void Controls_CursorUp(void) {
    if (D_8012410C != 0) {
        D_8012410C -= 1;
    } else if (D_80124108 != 0) {
        D_80124108 -= 1;
        D_8012410A -= 1;
    }
    Controls_UpdateArrowsP2();
}

void Controls_CursorDown(void) {
    if ((u8) D_8012410C < 4U) {
        D_8012410C += 1;
    } else if ((u8) D_8012410A < 0xDU) {
        D_8012410A += 1;
        D_80124108 += 1;
    }
    Controls_UpdateArrowsP2();
}

void Controls_Leave(void) {
    void (**fp)(s32, s32);

    Menu_StartLeave(D_800F5F4C);
    Menu_StartLeave(D_800F5F50);
    Menu_StartLeave(D_800F5F54);
    Menu_StartLeave(D_800F5F58);
    Menu_StartFadeOut();
    fp = &sysInputSaveBindings;
    (*fp)((s32)gProfileP1->bindings, 0);
    (*fp)((s32)gProfileP2->bindings, 1);
}


void Controls_LeaveFrame(void) {
    Menu_Nop(D_800F5F4C);
    Menu_Nop(D_800F5F50);
    Menu_Nop(D_800F5F54);
    Menu_Nop(D_800F5F58);
}

void Controls_LayoutTexts(void) {
    Menu_LayoutTexts(D_800F5F4C);
    Menu_LayoutTexts(D_800F5F50);
    Menu_LayoutTexts(D_800F5F54);
    Menu_LayoutTexts(D_800F5F58);
}

void Controls_ResetTexts(void) {
    Menu_ResetTexts(D_800F5F4C);
    Menu_ResetTexts(D_800F5F50);
    Menu_ResetTexts(D_800F5F54);
    Menu_ResetTexts(D_800F5F58);
}

extern char *(*sysInputGetKeyName)(s32 player, u8 action);
extern s8 D_800F61E0[];

/* Two control actions bound to the same key (for player 1, and player 2 in two-player mode), unless
 * D_800F61E0 pairs them: copies both action names to D_80124CA0/D_80124CC0 and returns 1. */
u8 Controls_HasConflict(void) {
    char a[0x20];
    char b[0x20];
    u8 i;
    u8 j;
    char *(**getName)(s32, u8);

    i = 0;
    getName = &sysInputGetKeyName;
    for (; i < 13; i++) {
        for (j = i + 1; j < 13; j++) {
            strcpy((s32 *)a, (s32)(*getName)(0, i));
            strcpy((s32 *)b, (s32)(*getName)(0, j));
            if (strcmp(a, b) == 0 && D_800F61E0[i] != j) {
                strcpy((s32 *)&D_80124CA0, D_80124110[i]);
                strcpy((s32 *)&D_80124CC0, D_80124110[j]);
                return 1;
            }
            if (gNumPlayers == 2) {
                strcpy((s32 *)a, (s32)(*getName)(1, i));
                strcpy((s32 *)b, (s32)(*getName)(1, j));
                if (strcmp(a, b) == 0 && D_800F61E0[i] != j) {
                    strcpy((s32 *)&D_80124CA0, D_80124110[i]);
                    strcpy((s32 *)&D_80124CC0, D_80124110[j]);
                    return 1;
                }
            }
        }
    }
    return 0;
}

void Controls_CheckExit(void) {
    if (Controls_HasConflict() != 0) {
        sysSfxPlay(0x35, 1, 1.0f, 1.0f);
        Menu_StartDim();
        Fsm_Trigger(&gMenuFsm, 0x65);
    } else {
        Fsm_Trigger(&gMenuFsm, 0x66);
    }
}


void Controls_StartRemapP1(void) {
    gControlsRemapP1 = 1;
}

void Controls_StartRemapP2(void) {
    if (gNumPlayers == 1) {
        gNumPlayers = 2;
        Controls_UpdateArrowsP2();
        return;
    }
    gControlsRemapP2 = 1;
}

void Controls_ResetP1(void) {
    sysInputResetLayout(0);
}

void Controls_ResetP2(void) {
    sysInputResetLayout(1);
}

void Controls_RemapP1(void) {
    sysInputUpdate(0);
    if (sysInputBindKey(0, D_80124107 + D_8012410B) != 0) {
        gControlsRemapP1 = 0;
        Fsm_Trigger(&gMenuFsm, 0x64);
    }
}


void Controls_RemapP2(void) {
    sysInputUpdate(1);
    if (sysInputBindKey(1, D_80124108 + D_8012410C) != 0) {
        gControlsRemapP2 = 0;
        Fsm_Trigger(&gMenuFsm, 0x64);
    }
}


void Controls_InputP1(void) {
    s32 screen;

    if ((*(u8 *)&gControlsRemapP1) == 0) {
        screen = 0x70;
        if ((D_80124107 != 0) || (D_8012410B != 0)) {
            screen = 0x71;
        }
        if (((u8) D_80124109 < 0xDU) || ((u8) D_8012410B < 4U)) {
            screen |= 2;
        }
        Menu_HandlePad1(screen & 0xFFFF);
        return;
    }
    Controls_RemapP1();
}

void Controls_InputP2(void) {
    s32 screen;

    if ((*(u8 *)&gControlsRemapP2) == 0) {
        if (gNumPlayers == 1) {
            screen = 0x50;
        } else {
            screen = 0x10;
            if ((D_80124108 != 0) || (D_8012410C != 0)) {
                screen = 0x11;
            }
            if (((u8) D_8012410A < 0xDU) || ((u8) D_8012410C < 4U)) {
                screen |= 2;
            }
        }
        Menu_HandlePad2(screen & 0xFFFF);
        return;
    }
    Controls_RemapP2();
}

void Controls_Input(void) {
    if (D_800F64A4 == 0) {
        D_800F64A4 = 1;
        Controls_InputP1();
        Controls_InputP2();
        return;
    }
    D_800F64A4 = 0;
    Controls_InputP2();
    Controls_InputP1();
}

void ControlsError_Enter(void) {
    s32 buf[8];

    sprintf((char *)buf, (char *)&D_80100FAC);
    Menu_SetText(gMenuObjs[21], 0, buf, 320, 165, 0, 0, 1, 1, 255, 255, 255);
    sprintf((char *)buf, (char *)&D_80100FB4, (s32)&D_80124CA0);
    Menu_SetText(gMenuObjs[21], 1, buf, 320, 195, 0, 0, 1, 1, 255, 255, 255);
    sprintf((char *)buf, (char *)&D_80100FBC, (s32)&D_80124CC0);
    Menu_SetText(gMenuObjs[21], 2, buf, 320, 225, 0, 0, 1, 1, 255, 255, 255);
    sprintf((char *)buf, (char *)&D_80100FC4);
    Menu_SetText(gMenuObjs[21], 3, buf, 320, 255, 0, 0, 1, 1, 255, 255, 255);
    Menu_StartEnter(gMenuObjs[21]);
}

void ControlsError_ResetTexts(void) {
    Menu_ResetTexts(D_800F5F5C);
}

void ControlsError_Draw(void) {
    Menu_SetupCamera();
    Menu_UpdateFade();
    Menu_Draw(gMenuObjs[21], -1, -1, -1, -1, -1, -1, 0);
    Menu_EndFrame();
}

void ControlsConflict_Input(void) {
    Menu_HandlePad1(0x50);
    if (gNumPlayers == 2) {
        Menu_HandlePad2(0x50);
    }
}

void Controls_FadeIn(void) {
    Menu_StartFadeIn();
}

void Keyboard_ClearText(void) {
    u8 i;

    for (i = 0; i < 20; i++) {
        (&gKeyboardText)[i] = 0;
    }
}

extern s32 D_80100FD4;
extern s32 gSaveIds[];
/* First Controller Pak that is ready, or 0xFF. */
static inline s32 LoadGame_FirstReadyPak(void) {
    s32 i;
    s32 sel = 0xFF;

    for (i = 0; (u8)i < 4; i++) {
        if (Pak_GetStatus(i) == 0) {
            sel = i;
            break;
        }
    }
    return sel;
}

void LoadGame_Enter(void) {
    s32 i;
    s32 sel;

    Menu_LoadBg((s32 *)&D_80100FD4);
    Menu_StartFadeIn();
    Menu_StartEnter(D_800F5F64);
    Menu_StartEnter(D_800F5F68);
    Menu_StartEnter(D_800F5F6C);
    Menu_StartEnter(D_800F5F70);
    Menu_StartEnter(D_800F5F74);
    D_801240B2 = 0;
    D_801240B3 = 0;
    D_801240B4 = 0;
    D_801240B5 = 0;
    D_801240B6 = 0;
    gDeleteConfirmSel = 0;
    gRenamePending = 0;
    gLoadPakP1 = 0xFF;
    gLoadPakP2 = 0xFF;
    gLoadSlotP1 = 0;
    gLoadSlotP2 = 0;
    if (Save_ReadAll() == 0 || (u8)(sel = LoadGame_FirstReadyPak()) == 0xFF) {
        Fsm_Trigger(&gMenuFsm, 0x68);
        return;
    }
        gLoadPakP1 = sel;
        gLoadPakP2 = sel;
        if (gProfileP1->saveId != 0) {
            for (i = 0; (u8) i < 4; i++) {
                if (gSaveIds[(u8) i] == gProfileP1->saveId && Pak_GetStatus(i) == 0) {
                    gLoadPakP1 = i;
                    break;
                }
            }
        }
        if (gProfileP2->saveId != 0) {
            for (i = 0; (u8) i < 4; i++) {
                if (gSaveIds[(u8) i] == gProfileP2->saveId && Pak_GetStatus(i) == 0) {
                    gLoadPakP2 = i;
                    break;
                }
            }
        }
        gLoadPlayer = 0;
        Menu_ResetTicker(&D_800F63D0);
}

extern s32 *D_80124090[];
extern s32 *D_801240A0[];
void LoadGame_SetupTexts(void) {
    s32 buf[6];
    u8 i;
    u8 n;

    if (Pak_GetStatus(gLoadPakP1) == 0) {
        n = SaveSlot_GetCount(gLoadPakP1);
        ((s32 (*)(s32 *, s32 *, s32 *, s32))sprintf)(buf, (s32 *)&D_80100CA4, ((s32 **)&D_80141230)[16], gLoadPakP1 + 1);
        Menu_SetText(gMenuObjs[23], 0, buf, 0xC0, 0xC4, 0, 0, 1, 1, 0x7F, 0x7F, 0x7F);
        for (i = 0; i < 4; i++) {
            if (i < n) {
                s32 *s = (s32 *)((u8 *)SaveSlot_Get(gLoadPakP1, i) + 8);
                Menu_SetText(gMenuObjs[23], i + 1, s, 0xC0, i * 46 + 0xEE, 0, 0, 1, 1, 0x7F, 0x7F, 0x7F);
                D_80124090[i] = s;
            } else {
                Menu_SetText(gMenuObjs[23], i + 1, ((s32 **)&D_80141230)[0], 0xC0, i * 46 + 0xEE, 0, 0, 1, 1, 0x7F, 0x7F, 0x7F);
                D_80124090[i] = ((s32 **)&D_80141230)[0];
            }
        }
    }
    if (gNumPlayers != 2) {
        for (i = 0; i < 6; i++) {
            Menu_SetText(gMenuObjs[24], i, (s32 *)&D_80100CC8, 0x1CC, i * 42 + 0xC4, 0, 0, 1, 1, 0x7F, 0x7F, 0x7F);
        }
        return;
    }
    if (Pak_GetStatus(gLoadPakP2) == 0) {
        n = SaveSlot_GetCount(gLoadPakP2);
        ((s32 (*)(s32 *, s32 *, s32 *, s32))sprintf)(buf, (s32 *)&D_80100CA4, ((s32 **)&D_80141230)[16], gLoadPakP2 + 1);
        Menu_SetText(gMenuObjs[24], 0, buf, 0x1CC, 0xC4, 0, 0, 1, 1, 0x7F, 0x7F, 0x7F);
        for (i = 0; i < 4; i++) {
            if (i < n) {
                s32 *s = (s32 *)((u8 *)SaveSlot_Get(gLoadPakP2, i) + 8);
                Menu_SetText(gMenuObjs[24], i + 1, s, 0x1CC, i * 46 + 0xEE, 0, 0, 1, 1, 0x7F, 0x7F, 0x7F);
                D_801240A0[i] = s;
            } else {
                Menu_SetText(gMenuObjs[24], i + 1, ((s32 **)&D_80141230)[0], 0x1CC, i * 46 + 0xEE, 0, 0, 1, 1, 0x7F, 0x7F, 0x7F);
                D_801240A0[i] = ((s32 **)&D_80141230)[0];
            }
        }
    }
}

void LoadGame_PrevPakP1(void) {
    if (gLoadSlotP1 == 0) {
        do {
            gLoadPakP1 = (gLoadPakP1 - 1) & 3;
        } while (Pak_GetStatus(gLoadPakP1) != 0);
    }
}

void LoadGame_NextPakP1(void) {
    if (gLoadSlotP1 == 0) {
        do {
            gLoadPakP1 = (gLoadPakP1 + 1) & 3;
        } while (Pak_GetStatus(gLoadPakP1) != 0);
    }
}

void LoadGame_PrevPakP2(void) {
    if (gLoadSlotP2 == 0 && gNumPlayers == 2) {
        do {
            gLoadPakP2 = (gLoadPakP2 - 1) & 3;
        } while (Pak_GetStatus(gLoadPakP2) != 0);
    }
}

void LoadGame_NextPakP2(void) {
    if (gLoadSlotP2 == 0 && gNumPlayers == 2) {
        do {
            gLoadPakP2 = (gLoadPakP2 + 1) & 3;
        } while (Pak_GetStatus(gLoadPakP2) != 0);
    }
}

void LoadGame_LayoutTexts(void) {
    Menu_LayoutTexts(D_800F5F64);
    Menu_LayoutTexts(D_800F5F68);
}

void LoadGame_Animate(void) {
    Menu_Animate((*(void * *)&D_800F5F64), 0x17U);
    if (gNumPlayers == 2) {
        Menu_Animate((*(void * *)&D_800F5F68), 0x18U);
    }
    if ((*(u8 *)&D_801240B2) != 0) {
        Menu_Animate(D_800F5F6C, 0x19U);
    }
}

void LoadGame_Draw(void) {
    Menu_SetupCamera();
    Menu_UpdateFade();
    if (*(u8 *)&D_801240B2) {
        Menu_Draw((s32)D_800F5F6C, -1, D_801240B3, -1, -1, -1, -1, 0);
    } else if (*(u8 *)&D_801240B4) {
        if (*(u8 *)&D_801240B6) {
            Menu_Draw(D_800F5F74, -1, *(s8 *)&gDeleteConfirmSel, -1, -1, -1, -1, 0);
        } else {
            Menu_Draw(D_800F5F70, -1, *(s8 *)&D_801240B5, -1, -1, -1, -1, 0);
        }
    } else {
        Menu_Draw(D_800F5F64, *(s8 *)&gLoadSlotP1, -1, -1, -1, -1, -1, gMenuHideText);
        if (gNumPlayers == 2) {
            Menu_Draw(D_800F5F68, *(s8 *)&gLoadSlotP2, -1, -1, -1, -1, -1, gMenuHideText);
        }
        if (gMenuHideText == 0) {
            Menu_DrawTicker(&D_800F63D0);
        }
    }
    Menu_EndFrame();
}

void LoadGame_MoveUpP1(void) {
    if (gLoadSlotP1 != 0) {
        gLoadSlotP1 -= 1;
    }
}

void LoadGame_MoveDownP1(void) {
    if ((u8) gLoadSlotP1 < 4U) {
        gLoadSlotP1 += 1;
    }
}

void LoadGame_MoveUpP2(void) {
    if (gLoadSlotP2 != 0) {
        gLoadSlotP2 -= 1;
    }
}

void LoadGame_MoveDownP2(void) {
    if ((u8) gLoadSlotP2 < 4U) {
        gLoadSlotP2 += 1;
    }
}

void LoadConfirm_Input(void) {
    u32 screen;

    screen = 0x31;
    if ((*(u8 *)&D_801240B3) == 0) {
        screen = 0x32;
    }
    if (gLoadPlayer == 0) {
        Menu_HandlePad1(screen);
        return;
    }
    Menu_HandlePad2(screen);
}

void LoadConfirm_MoveTop(void) {
    D_801240B3 = 0;
}

void LoadConfirm_MoveBottom(void) {
    D_801240B3 = 1;
}

void LoadConfirm_Select(void) {
    if ((*(u8 *)&D_801240B3) == 0) {
        Keyboard_ClearText();
        Fsm_Trigger(&gMenuFsm, 0x6E);
    } else {
        Fsm_Trigger(&gMenuFsm, 0x6F);
    }
}


/* TODO(match-debt): the local pointer to gLoadSlotP2 is only needed to match (forces a reload on one path). */
void LoadGame_Input(void) {
    u8 *sel2;
    s32 a;
    s32 b;

    a = 0x2C;
    if (gLoadSlotP1 != 0) {
        a = 0x31;
    }
    if (gLoadSlotP1 < 4) {
        a |= 2;
    }
    Menu_HandlePad1(a & 0xFFFF);
    sel2 = &gLoadSlotP2;
    b = 0x50;
    if (gNumPlayers != 1) {
        b = 0x11;
        if (*sel2 == 0) {
            b = 0xC;
        }
        if (gLoadSlotP2 < 4) {
            b |= 2;
        }
    }
    Menu_HandlePad2(b & 0xFFFF);
}

void LoadGame_SelectP1(void) {
    u8 n = gLoadSlotP1;

    if (n == 0) {
        Fsm_Trigger(&gMenuFsm, 0x69);
    } else if (SaveSlot_GetCount(gLoadPakP1) < n) {
        gLoadPlayer = 0;
        Fsm_Trigger(&gMenuFsm, 0x6C);
    } else {
        gLoadPlayer = 0;
        Fsm_Trigger(&gMenuFsm, 0x6B);
    }
}


void LoadGame_SelectP2(void) {
    u8 n;

    if (gNumPlayers == 1) {
        gNumPlayers = 2;
        Fsm_Trigger(&gMenuFsm, 0x6D);
        return;
    }
    n = gLoadSlotP2;
    if (n == 5) {
        Fsm_Trigger(&gMenuFsm, 0x6A);
    } else if (n == 0) {
        Fsm_Trigger(&gMenuFsm, 0x69);
    } else if (SaveSlot_GetCount(gLoadPakP2) < n) {
        gLoadPlayer = 1;
        Fsm_Trigger(&gMenuFsm, 0x6C);
    } else {
        gLoadPlayer = 1;
        Fsm_Trigger(&gMenuFsm, 0x6B);
    }
}


void LoadOptions_Open(void) {
    Menu_StartDim();
    D_801240B4 = 1;
}

void DeleteConfirm_Open(void) {
    Menu_StartDim();
    D_801240B6 = 1;
}

void LoadConfirm_Open(void) {
    Menu_StartDim();
    D_801240B2 = 1;
}

void LoadGame_ApplySlot(void) {
    if (gLoadPlayer == 0) {
        D_80124150 = (*(s32 *)((u8 *)(SaveSlot_Get((u32) gLoadPakP1, gLoadSlotP1 - 1)) + 0x4));
    } else {
        D_80124154 = (*(s32 *)((u8 *)(SaveSlot_Get((u32) gLoadPakP2, gLoadSlotP2 - 1)) + 0x4));
    }
    Menu_Advance();
}

extern u32 D_80124A04[3]; /* P1 default profile unlocks */
extern u32 D_80124B44[3]; /* P2 default profile unlocks */
void LoadGame_SyncProfiles(void) {
    Profile a;
    Profile b;
    s32 i;
    s32 j;
    Profile *p;

    a = *gProfileP1;
    b = *gProfileP2;
    gProfileP1 = NULL;
    gProfileP2 = NULL;
    for (i = 0; (u8) i < 4; i++) {
        for (j = 0; (u8) j < 4; j++) {
            p = SaveSlot_Get((u8) i, (u8) j);
            if (p != NULL) {
                if (p->id == D_80124150) {
                    gProfileP1 = SaveSlot_Get((u8) i, (u8) j);
                    gActiveSave->player1Id = gProfileP1->id;
                } else if (p->id == D_80124154) {
                    gProfileP2 = SaveSlot_Get((u8) i, (u8) j);
                    gActiveSave->player2Id = gProfileP2->id;
                }
            }
        }
    }
    if (gProfileP1 == NULL) {
        Profile_InitDefault(&D_801249F0, 0);
        if ((D_80124A04[0] | a.carUnlocks[0]) != D_80124A04[0] || (D_80124A04[1] | a.carUnlocks[1]) != D_80124A04[1] || (D_80124A04[2] | a.trackUnlocks) != D_80124A04[2]) {
            D_80124A04[0] |= a.carUnlocks[0];
            D_80124A04[1] |= a.carUnlocks[1];
            D_80124A04[2] |= a.trackUnlocks;
            *(s32 *) &gPasswordP1 = D_80100DBC;
        }
        gProfileP1 = (Profile *)&D_801249F0;
        gActiveSave->player1Id = 0;
    }
    if (gProfileP2 == NULL) {
        Profile_InitDefault(&D_80124B30, 1);
        if ((D_80124B44[0] | b.carUnlocks[0]) != D_80124B44[0] || (D_80124B44[1] | b.carUnlocks[1]) != D_80124B44[1] || (D_80124B44[2] | b.trackUnlocks) != D_80124B44[2]) {
            D_80124B44[0] |= b.carUnlocks[0];
            D_80124B44[1] |= b.carUnlocks[1];
            D_80124B44[2] |= b.trackUnlocks;
            *(s32 *) &gPasswordP2 = D_80100DBC;
        }
        gProfileP2 = (Profile *)&D_80124B30;
        gActiveSave->player2Id = 0;
    }
}

void LoadGame_Leave(void) {
    Menu_StartLeave(D_800F5F64);
    Menu_StartLeave(D_800F5F68);
    Menu_StartFadeOut();
}

void LoadGame_LeaveFrame(void) {
    Menu_Nop(D_800F5F64);
    Menu_Nop(D_800F5F68);
}

void LoadOptions_Input(void) {
    s32 base;
    s32 screen;

    base = -(D_801240B5 != 0) & 0x31;
    screen = base | 0x30;
    if ((u8) D_801240B5 < 2U) {
        screen = base | 0x32;
    }
    if (gLoadPlayer == 0) {
        Menu_HandlePad1(screen);
        return;
    }
    Menu_HandlePad2(screen);
}

void LoadOptions_MoveUp(void) {
    if (D_801240B5 != 0) {
        D_801240B5 -= 1;
    }
}

void LoadOptions_MoveDown(void) {
    if ((u8) D_801240B5 < 2U) {
        D_801240B5 += 1;
    }
}

void LoadOptions_Select(void) {
    sysScreenSetBgAlpha(0xFF);
    switch (D_801240B5) {
        case 0:
            Fsm_Trigger(&gMenuFsm, 0x70);
            break;
        case 1:
            Fsm_Trigger(&gMenuFsm, 0x72);
            break;
        case 2:
            Fsm_Trigger(&gMenuFsm, 0x71);
            break;
        default:
            Fsm_Trigger(&gMenuFsm, 0x69);
            break;
    }
}


void LoadGame_CopyEntryName(void) {
    u8 pak;
    u8 entry;
    void *rec;

    if (gLoadPlayer == 0) {
        entry = gLoadSlotP1;
        pak = gLoadPakP1;
    } else {
        entry = gLoadSlotP2;
        pak = gLoadPakP2;
    }
    rec = SaveSlot_Get((u32) pak, entry - 1);
    D_80124CE0 = rec;
    strcpy(&gKeyboardText, rec + 8);
    gRenamePending = 1;
}

void LoadGame_CheckRename(void) {
    if ((*(u8 *)&gRenamePending) != 0) {
        Fsm_Trigger(&gMenuFsm, 0x67);
    } else {
        Fsm_Trigger(&gMenuFsm, 0x11);
    }
}


void Keyboard_SaveName(void) {
    DEBUG_NOP();
    strcpy(*(s32 *)&D_80124CE0 + 8, &gKeyboardText);
    if (Save_Commit() & 0xFF) {
        Menu_Advance();
        return;
    }
    Fsm_Trigger((s32)&gMenuFsm, 0x68);
}

void DeleteConfirm_MoveTop(void) {
    (*(s8 *)&gDeleteConfirmSel) = 0;
}

void DeleteConfirm_MoveBottom(void) {
    (*(s8 *)&gDeleteConfirmSel) = 1;
}

void DeleteConfirm_Input(void) {
    s32 screen;

    screen = 0x31;
    if (gDeleteConfirmSel == 0) {
        screen = 0x32;
    }
    if (gLoadPlayer == 0) {
        Menu_HandlePad1(screen);
        return;
    }
    Menu_HandlePad2(screen);
}

void DeleteConfirm_Apply(void) {
    if (gDeleteConfirmSel == 0) {
        Fsm_Trigger(&gMenuFsm, 0x73);
    } else {
        Fsm_Trigger(&gMenuFsm, 0x74);
    }
}


void LoadGame_DeleteSelected(void) {
    u8 pak;
    u8 entry;

    if (gLoadPlayer == 0) {
        entry = gLoadSlotP1;
        pak = gLoadPakP1;
    } else {
        entry = gLoadSlotP2;
        pak = gLoadPakP2;
    }
    SaveSlot_Delete(pak, entry - 1);
    Save_Commit();
}

void Keyboard_OpenPlayerName(void) {
    s32 player;
    u8 n;

    n = Keyboard_LoadCharset((s32)&D_80100FDC);
    D_801240D6 = n;
    if (!n) {
        Fsm_Trigger((s32)&gMenuFsm, 0x12);
    }
    Keyboard_ClearText();
    if (gLoadPlayer == 0) {
        player = (s32)gProfileP1;
    } else {
        player = (s32)gProfileP2;
    }
    strcpy(&gKeyboardText, player + 8);
    D_801240D7 = 10;
    D_801240D8 = 0;
    D_801240DD = D_801240D6 + 1;
    D_801240DC = D_801240D6;
    D_801240DE = D_801240D6 + 2;
    D_801240D6 += 3;
}

void Keyboard_OpenKeepText(void) {
    u8 n;

    n = Keyboard_LoadCharset((s32)&D_80100FDC);
    D_801240D6 = n;
    if (!n) {
        Fsm_Trigger((s32)&gMenuFsm, 0x12);
    }
    D_801240D7 = 10;
    D_801240D8 = 2;
    D_801240DD = D_801240D6 + 1;
    D_801240DC = D_801240D6;
    D_801240DE = D_801240D6 + 2;
    D_801240D6 += 3;
}

void Keyboard_OpenPassword(void) {
    u8 n;

    n = Keyboard_LoadCharset((s32)&D_80100FE4);
    D_801240D6 = n;
    if (!n) {
        Fsm_Trigger((s32)&gMenuFsm, 0x12);
    }
    Keyboard_ClearText();
    D_801240D7 = 20;
    D_801240D8 = 3;
    *(u8 *)&D_801240DD = 0xFF;
    D_801240DC = D_801240D6;
    D_801240DE = D_801240D6 + 1;
    D_801240D6 += 2;
}

void Keyboard_OpenNewName(void) {
    u8 n;

    n = Keyboard_LoadCharset((s32)&D_80100FDC);
    D_801240D6 = n;
    if (!n) {
        Fsm_Trigger((s32)&gMenuFsm, 0x12);
    }
    Keyboard_ClearText();
    D_801240D7 = 10;
    D_801240D8 = 1;
    D_801240DD = D_801240D6 + 1;
    D_801240DC = D_801240D6;
    D_801240DE = D_801240D6 + 2;
    D_801240D6 += 3;
}

void Keyboard_TrimText(void) {
    u8 r = 10;
    u8 i;

    for (i = 0; i < 10; i++) {
        if ((&gKeyboardText)[i] != 0x20) {
            r = i;
        }
    }
    if (r < 8) {
        (&gKeyboardText)[r + 1] = 0;
    }
}


void Register_PlaceCursor(s32 x, s32 y) {
    MnC3_ *m;
    u8 i;

    m = (MnC3_ *)D_800F5F7C;
    for (i = 0; i < m->n; i++) {
        if (strcmp(m->names[i].s, D_80100FEC) == 0) {
            m->elems[i].xC = MenuPos_ToWorldX((x + 5) / 640.0f * 1000.0f);
            m->elems[i].x10 = MenuPos_ToWorldY((y + 10) / 480.0f * 1000.0f);
            m->elems[i].x = m->elems[i].xC;
            m->elems[i].y = m->elems[i].x10;
            return;
        }
    }
}

void Register_LayoutTexts(void) {
    Menu_LayoutTexts(D_800F5F7C);
}

typedef struct { u8 p[4]; } ObjG1d_;
extern s32 D_80100FF8;
extern s32 *D_80141274;
extern s32 *D_80141498;
/* Opens the name entry screen: clears the name buffer after its terminator and draws the prompt. */
void Register_Enter(void) {
    u8 i;
    u8 end;

    end = 0;
    for (i = 0; i < 20; i++) {
        if ((&gKeyboardText)[i] == 0) {
            end = 1;
        }
        if (end) {
            (&gKeyboardText)[i] = 0;
        }
    }
    Menu_LoadBg((s32 *)&D_80100FF8);
    Menu_StartFadeIn();
    Menu_StartEnter(gMenuObjs[29]);
    D_801240D5 = 0;
    gRegisterKey = 0;
    Register_PlaceCursor(D_800F610C[0], D_800F6170[0]);
    if (D_801240D8 != 3) {
        if (gLoadPlayer == 0) {
            Menu_SetText(gMenuObjs[29], 0, ((s32 **)&D_80141230)[18], 80, 190, 0, 0, 0, 1, 255, 255, 255);
        } else {
            Menu_SetText(gMenuObjs[29], 0, ((s32 **)&D_80141230)[19], 80, 190, 0, 0, 0, 1, 255, 255, 255);
        }
        Menu_SetText(gMenuObjs[29], 1, (s32 *)&gKeyboardText, 180, 400, 0, 0, 0, 1, 255, 255, 255);
        Menu_SetText(gMenuObjs[29], 2, (s32 *)&D_80100CC8, 180, 400, 0, 0, 0, 1, 255, 255, 255);
    } else {
        Menu_SetText(gMenuObjs[29], 0, D_80141498, 73, 190, 0, 0, 0, 1, 255, 255, 255);
        Menu_SetText(gMenuObjs[29], 1, (s32 *)&gKeyboardText, 180, 400, 0, 0, 0, 1, 255, 255, 255);
        Menu_SetText(gMenuObjs[29], 2, (s32 *)&D_80100CC8, 180, 400, 0, 0, 0, 1, 255, 255, 255);
    }
    Menu_SetText(gMenuObjs[29], 3, ((s32 **)&D_80141230)[17], D_800F616C, D_800F61D0, 0, 0, 1, 1, 255, 255, 255);
    Menu_ResetTicker(&D_800F6400);
}

void Register_ApplyChoice(void) {
    Profile l;
    s32 p;

    switch ((u32)D_801240D8) {
        case 0:
            if (gLoadPlayer == 0) {
                p = gProfileP1;
            } else {
                p = gProfileP2;
            }
            strcpy((s32 *)(p + 8), (s32)&gKeyboardText);
            Fsm_Trigger(&gMenuFsm, 0x75);
            break;
        case 1:
            if (gLoadPlayer == 0) {
                Profile_InitDefault(&l, 0);
                l.carUnlocks[0] |= gProfileP1->carUnlocks[0];
                l.carUnlocks[1] |= gProfileP1->carUnlocks[1];
                l.trackUnlocks |= gProfileP1->trackUnlocks;
                strcpy((s32 *)l.name, (s32)&gKeyboardText);
                SaveSlot_Add(gLoadPakP1, &l);
            } else {
                Profile_InitDefault(&l, 0);
                l.carUnlocks[0] |= gProfileP2->carUnlocks[0];
                l.carUnlocks[1] |= gProfileP2->carUnlocks[1];
                l.trackUnlocks |= gProfileP2->trackUnlocks;
                strcpy((s32 *)l.name, (s32)&gKeyboardText);
                SaveSlot_Add(gLoadPakP2, &l);
            }
            if ((u8)Save_Commit() != 0) {
                Fsm_Trigger(&gMenuFsm, 0x76);
            } else {
                Menu_StartDim();
                Fsm_Trigger(&gMenuFsm, 0x68);
            }
            break;
        case 2:
            Fsm_Trigger(&gMenuFsm, 0x76);
            break;
        default:
            Fsm_Trigger(&gMenuFsm, 0x77);
            break;
    }
}

void Register_Cancel(void) {
    switch ((u32)D_801240D8) {
        case 1:
            Fsm_Trigger(&gMenuFsm, 0x76);
            break;
        case 2:
            gRenamePending = 0;
            Fsm_Trigger(&gMenuFsm, 0x76);
            break;
        case 0:
        default:
            Fsm_Trigger(&gMenuFsm, 0x75);
            break;
    }
}

void Register_Input(void) {
    u16 f = 0x60;
    u8 n;

    if (D_801240D5 < *(u8 *)&D_801240D7 || gRegisterKey == D_801240DC || gRegisterKey == D_801240D6 - 1) {
        f |= 0x10;
    }
    if (gRegisterKey != 0) {
        f |= 4;
    }
    if (gRegisterKey < D_801240D6 - 1) {
        f |= 8;
    }
    if (gRegisterKey != D_801240D6 - 1) {
        f |= 2;
    }
    if (gRegisterKey >= 8) {
        f |= 1;
    }
    if (gLoadPlayer == 0) {
        Menu_HandlePad1(f);
    } else {
        Menu_HandlePad2(f);
    }
}


void Register_Animate(void) {
    Menu_Animate((*(void * *)&D_800F5F7C), 0x1DU);
}

void Register_Draw(void) {
    u8 i;

    Menu_SetupCamera();
    Menu_UpdateFade();
    Menu_Draw(gMenuObjs[29], -1, -1, -1, -1, -1, -1, gMenuHideText);
    if (gMenuHideText == 0) {
        for (i = 0; i < D_801240D6 - 1; i++) {
            if (i == D_801240DC) {
                sysFontDrawChar(1, 0x9B, D_800F610C[i], D_800F6170[i], i == gRegisterKey);
            } else if (i == *(u8 *)&D_801240DD) {
                sysFontDrawChar(1, 0x9A, D_800F610C[i], D_800F6170[i], i == gRegisterKey);
            } else {
                sysFontDrawChar(1, D_801410A8[i], D_800F610C[i], D_800F6170[i], i == gRegisterKey);
            }
        }
        Menu_DrawTicker(&D_800F6400);
    }
    Menu_EndFrame();
}

void Register_MoveLeft(void) {
    if (gRegisterKey != 0) {
        gRegisterKey--;
        Register_PlaceCursor(D_800F610C[gRegisterKey], D_800F6170[gRegisterKey]);
    }
}

void Register_MoveRight(void) {
    u8 i = gRegisterKey;
    u8 n = D_801240D6;

    if (i == n - 2) {
        gRegisterKey = i + 1;
        Register_PlaceCursor(D_800F616C, D_800F61D0);
    } else if (i < n - 1) {
        i++;
        gRegisterKey = i;
        Register_PlaceCursor(D_800F610C[i], D_800F6170[i]);
    }
}


void Register_MoveUp(void) {
    if (gRegisterKey == D_801240D6 - 1) {
        gRegisterKey = D_801240DF;
    } else if (gRegisterKey - 8 >= 0) {
        gRegisterKey -= 8;
    }
    Register_PlaceCursor(D_800F610C[gRegisterKey], D_800F6170[gRegisterKey]);
}

void Register_MoveDown(void) {
    u8 i = gRegisterKey;
    u8 n = D_801240D6;

    if (i + 8 >= n - 1) {
        D_801240DF = i;
        gRegisterKey = n - 1;
        Register_PlaceCursor(D_800F616C, D_800F61D0);
    } else if (i + 8 < n) {
        i += 8;
        gRegisterKey = i;
        Register_PlaceCursor(D_800F610C[i], D_800F6170[i]);
    }
}


extern char *D_80141278;
extern char *D_8014127C;
void Register_PressKey(void) {
    u8 buf[0x10];
    u8 i;
    s32 *p;

    if (gRegisterKey == D_801240DC) {
        if (D_801240D5 == 0) {
            D_801240D5 = strlen((char *)&gKeyboardText);
        }
        if (D_801240D5 != 0) {
            D_801240D5--;
            (&gKeyboardText)[D_801240D5] = 0;
        }
    } else if (gRegisterKey == *(u8 *)&D_801240DD) {
        if (strcmp((char *)&gKeyboardText, D_80141278) == 0 || strcmp((char *)&gKeyboardText, D_8014127C) == 0) {
            if (D_801240D5 == 0) {
                memset(&gKeyboardText, 0, 10);
            }
        } else {
            D_801240D5 = strlen((char *)&gKeyboardText);
        }
        if (D_801240D5 < *(u8 *)&D_801240D7) {
            (&gKeyboardText)[D_801240D5] = ' ';
            D_801240D5++;
        }
    } else if (gRegisterKey == *(u8 *)&D_801240DE) {
        Fsm_Trigger(&gMenuFsm, 0x78);
        return;
    } else {
        if (strcmp((char *)&gKeyboardText, D_80141278) == 0 || strcmp((char *)&gKeyboardText, D_8014127C) == 0) {
            if (D_801240D5 == 0) {
                memset(&gKeyboardText, 0, 10);
            }
        } else {
            D_801240D5 = strlen((char *)&gKeyboardText);
        }
        if (D_801240D5 < *(u8 *)&D_801240D7) {
            (&gKeyboardText)[D_801240D5] = D_801410A8[gRegisterKey];
            D_801240D5++;
        }
    }
    if (*(u8 *)&D_801240D7 >= 11) {
        for (i = 0; i < 10; i++) {
            if (i < D_801240D5) {
                buf[i] = (&gKeyboardText)[i];
            } else {
                buf[i] = 0;
            }
        }
        buf[10] = 0;
        Menu_SetText(gMenuObjs[29], 1, (s32 *)buf, 180, 390, 0, 0, 0, 1, 255, 255, 255);
        for (i = 10; i < 20; i++) {
            if (i < D_801240D5) {
                buf[i - 10] = (&gKeyboardText)[i];
            } else {
                buf[i - 10] = 0;
            }
        }
        buf[10] = 0;
        Menu_SetText(gMenuObjs[29], 2, (s32 *)buf, 180, 410, 0, 0, 0, 1, 255, 255, 255);
    } else {
        p = &D_800F5F7C;
        Menu_SetText(*p, 1, (s32 *)&gKeyboardText, 180, 400, 0, 0, 0, 1, 255, 255, 255);
        Menu_SetText(*p, 2, (s32 *)&D_80100CC8, 180, 400, 0, 0, 0, 1, 255, 255, 255);
    }
    Fsm_Trigger(&gMenuFsm, 0x79);
}

void Register_Leave(void) {
    Menu_StartFadeOut();
    Menu_Nop(D_800F5F7C);
}

void Register_Confirm(void) {
    s32 buf[6];

    if (Password_Apply(&gKeyboardText, gLoadPlayer == 0 ? gProfileP1 : gProfileP2) == 0) {
        if (gLoadPlayer == 0) {
            Password_Get((s32)gProfileP1, buf);
            strcpy((s32 *)&gPasswordP1, (s32)buf);
        } else {
            Password_Get((s32)gProfileP2, buf);
            strcpy((s32 *)&gPasswordP2, (s32)buf);
        }
        Fsm_Trigger(&gMenuFsm, 0x7A);
    } else {
        sysSfxPlay(0x35, 1, 1.0f, 1.0f);
        Fsm_Trigger(&gMenuFsm, 0x7B);
    }
}

void RegisterError_Enter(void) {
    Menu_StartDim();
    Menu_StartEnter(D_800F5F78);
}

void RegisterError_Draw(void) {
    s32 m;
    s32 *q;
    q = &D_800F5F78;
    Menu_SetupCamera();
    m = -1;
    Menu_UpdateFade();
    Menu_Draw(gMenuObjs[29], -1, -1, -1, m, m, m, 1);
    Menu_Draw(*q, -1, -1, -1, m, m, m, 0);
    Menu_EndFrame();
}

void RegisterError_Input(void) {
    if (gLoadPlayer == 0) {
        Menu_HandlePad1(0x30);
        return;
    }
    Menu_HandlePad2(0x30);
}

void HighScores_PlaceArrow(s32 i, s32 x, s32 y, s32 c) {
    TBL_ *t = (TBL_ *)D_800F5F80;
    u8 j;

    for (j = 0; j < t->n; j++) {
        if (strcmp(t->names[j].name, D_800F64A8[(u8)i]) == 0) {
            if ((x < 0) & (y < 0)) {
                t->objs[j].h30 = (u8)c;
                return;
            }
            t->objs[j].h30 = (u8)c;
            t->objs[j].x = MenuPos_ToWorldX(x);
            t->objs[j].y = MenuPos_ToWorldY(y);
            t->objs[j].x2 = t->objs[j].x;
            t->objs[j].y2 = t->objs[j].y;
            return;
        }
    }
}


void HighScores_UpdateArrows(void) {
    switch (gHighScoresPage) {
        case 0:
            HighScores_PlaceArrow(0, 80, 390, 100);
            HighScores_PlaceArrow(1, 425, 387, 100);
            break;
        case 1:
            HighScores_PlaceArrow(0, 80, 480, 100);
            HighScores_PlaceArrow(1, 425, 477, 100);
            break;
        default:
            HighScores_PlaceArrow(0, -1, -1, 0);
            HighScores_PlaceArrow(1, -1, -1, 0);
            break;
    }
}

extern u8 D_80101000;
extern u8 gHighScoresCategory;
extern u8 D_80124101;
extern u8 D_80124102;
extern ObjG1_ D_800F6388;
void MenuList_Reset(s32);
void HighScores_Enter(void) {

    Menu_LoadBg((s32 *)&D_80101000);
    Menu_StartFadeIn();
    Menu_StartEnter(gMenuObjs[30]);
    MenuList_Reset(gMenuObjs[30]);
    gHighScoresAutoCycle = 0;
    gHighScoresCycleTicks = 0;
    gHighScoresPage = 0;
    gHighScoresCategory = 0;
    D_80124101 = 0;
    D_80124102 = 0;
    HighScores_UpdateArrows();
    Menu_ResetTicker(&D_800F6388);
}

void HighScores_ResetTexts(void) {
    Menu_ResetTexts(D_800F5F80);
}

void HighScores_Animate(void) {
    Menu_Animate((*(void * *)&D_800F5F80), 0x1EU);
}

void HighScores_AutoCycleTick(void) {
    if (gHighScoresAutoCycle != 0) {
        Menu_TickEverySecond();
    }
}

void HighScores_Input(void) {
    if (gHighScoresPage == 0) {
        Menu_HandlePad1(0x6E);
    } else {
        Menu_HandlePad1(0x6D);
    }
}

void HighScores_PickRandom(void) {
    s32 t;

    gHighScoresCategory = sysRandRange(3);
    t = gHighScoresCategory;
    if (t >= 0) {
        if (t < 2) {
            do {
            } while (!Track_IsUnlocked(D_80124101 = sysRandRange(12)));
            return;
        }
    }
    do {
    } while (!Cup_IsUnlocked(D_80124102 = sysRandRange(3)));
}

void HighScores_StartAutoCycle(void) {
    if (gHighScoresAutoCycle == 0) {
        HighScores_PickRandom();
        gHighScoresAutoCycle = 1;
        (*(s8 *)&gHighScoresCycleTicks) = 0;
    }
}

void HighScores_StopAutoCycle(void) {
    (*(s8 *)&gHighScoresAutoCycle) = 0;
}

void HighScores_AutoCycle(void) {
    if (gHighScoresAutoCycle != 0) {
        if (++gHighScoresCycleTicks >= 20) {
            HighScores_PickRandom();
            gHighScoresCycleTicks = 0;
        }
    }
}

void HighScores_PageLeft(void) {
    if (gHighScoresPage != 0) {
        gHighScoresPage -= 1;
    }
    HighScores_UpdateArrows();
}

void HighScores_PageRight(void) {
    if (gHighScoresPage == 0) {
        gHighScoresPage += 1;
    }
    HighScores_UpdateArrows();
}

extern s32 gTrackNames[][3];
extern u8 gHighScoresPage;
extern u8 gHighScoresCategory;
extern u8 D_80124101;
extern u8 D_80124102;
u8 Track_IsUnlocked(u8 i);
u8 Cup_IsUnlocked(u8 i);
void HighScores_PrevCategory(void) {
    s32 m;

    switch (gHighScoresPage) {
        case 0:
            if (gHighScoresCategory != 0) {
                gHighScoresCategory--;
            } else {
                gHighScoresCategory = 2;
            }
            break;
        case 1:
            /* TODO(match-debt): the redundant m >= 0 test on a u8 is needed for the bltz/slti pair. */
            m = gHighScoresCategory;
            if (m >= 0) {
                if (m < 2) {
                    do {
                        if (D_80124101 != 0) {
                            D_80124101--;
                        } else {
                            D_80124101 = 11;
                        }
                    } while (!Track_IsUnlocked(D_80124101) || gTrackNames[(u8)(D_80124101 / 3)][(u8)(D_80124101 % 3)] == 0);
                    return;
                }
            }
            do {
                if (D_80124102 != 0) {
                    D_80124102--;
                } else {
                    D_80124102 = 2;
                }
            } while (!Cup_IsUnlocked(D_80124102));
            break;
    }
}

/* Moves the cursor of menu page gHighScoresPage to the next selectable entry, wrapping around. */
void HighScores_NextCategory(void) {
    s32 m;

    switch (gHighScoresPage) {
        case 0:
            if (gHighScoresCategory < 2) {
                gHighScoresCategory++;
            } else {
                gHighScoresCategory = 0;
            }
            break;
        case 1:
            /* TODO(match-debt): the redundant m >= 0 test on a u8 is needed for the bltz/slti pair. */
            m = gHighScoresCategory;
            if (m >= 0) {
                if (m < 2) {
                    do {
                        do {
                            if (D_80124101 < 11) {
                                D_80124101++;
                            } else {
                                D_80124101 = 0;
                            }
                        } while (!Track_IsUnlocked(D_80124101));
                    } while (gTrackNames[(u8)(D_80124101 / 3)][(u8)(D_80124101 % 3)] == 0);
                    return;
                }
            }
            do {
                if (D_80124102 < 2) {
                    D_80124102++;
                } else {
                    D_80124102 = 0;
                }
            } while (!Cup_IsUnlocked(D_80124102));
            break;
    }
}

extern u8 gHighScoresCategory;
extern u8 D_80124102;
extern u16 D_800F61D8[];
extern u8 D_80101008, D_80100EEC, D_80101010;
void HighScores_SetupTexts(void) {
    s32 a[8];
    s32 b[8];
    u32 sel;
    u8 i;
    s32 y;
    u32 v;
    s32 m;
    s32 s;

    switch (gHighScoresCategory) {
    case 0:
    case 1:
        sel = D_80124101;
        break;
    case 2:
    default:
        sel = D_80124102;
        break;
    }
    sprintf((char *)a, (char *)&D_80100CA0, gLangText[D_800F61D8[gHighScoresCategory]]);
    if (gHighScoresCategory == 2) {
        sprintf((char *)b, (char *)&D_80100CA0, ((s32 *)&D_8012406C)[sel]);
    } else {
        sprintf((char *)b, (char *)&D_80100CA0, D_800F9FE0[sel / 3][sel % 3]);
    }
    Menu_SetText((Ls2C_ *)gMenuObjs[30], 0, a, 0x44, 0xA0, 0, 0, 0, 1, 0x7F, 0x7F, 0x7F);
    Menu_SetText((Ls2C_ *)gMenuObjs[30], 1, b, 0x44, 0xCD, 0, 0, 0, 1, 0x7F, 0x7F, 0x7F);
    y = 0xA0;
    for (i = 0; i < 5; i++) {
        switch (gHighScoresCategory) {
        case 0:
            sprintf((char *)a, (char *)&D_80101008, i + 1, SAVE_DATA->highA[sel].e[i].name);
            sprintf((char *)b, (char *)&D_80101010, SAVE_DATA->highA[sel].e[i].value);
            break;
        case 1:
            sprintf((char *)a, (char *)&D_80101008, i + 1, SAVE_DATA->times[sel].e[i].name);
            m = (u16)Time_Minutes(SAVE_DATA->times[sel].e[i].value);
            s = (u16)Time_Seconds(SAVE_DATA->times[sel].e[i].value);
            sprintf((char *)b, (char *)&D_80100EEC, m, s, (u16)Time_Hundredths(SAVE_DATA->times[sel].e[i].value));
            break;
        case 2:
            sprintf((char *)a, (char *)&D_80101008, i + 1, SAVE_DATA->highB[sel].e[i].name);
            sprintf((char *)b, (char *)&D_80101010, SAVE_DATA->highB[sel].e[i].value);
            break;
        }
        if (gHighScoresAutoCycle == 0 || i < gHighScoresCycleTicks) {
            Menu_SetText((Ls2C_ *)gMenuObjs[30], (u8)(i + 2), a, 0x177, y, 0, 0, 1, 1, 0xFF, 0xFF, 0xFF);
            Menu_SetText((Ls2C_ *)gMenuObjs[30], (u8)(i + 7), b, 0x217, y, 0, 0, 1, 1, 0xFF, 0xFF, 0xFF);
        } else {
            Menu_SetText((Ls2C_ *)gMenuObjs[30], (u8)(i + 2), (s32 *)&D_80100CC8, 0x177, y, 0, 0, 1, 1, 0xFF, 0xFF, 0xFF);
            Menu_SetText((Ls2C_ *)gMenuObjs[30], (u8)(i + 7), (s32 *)&D_80100CC8, 0x217, y, 0, 0, 1, 1, 0xFF, 0xFF, 0xFF);
        }
        y += 0x2E;
    }
}

void HighScores_Draw(void) {
    Menu_SetupCamera();
    Menu_UpdateFade();
    Menu_Draw(gMenuObjs[30], gHighScoresPage, -1, -1, -1, -1, -1, gMenuHideText);
    if (!gMenuHideText) {
        Menu_DrawTicker((Scroll4E_ *)&D_800F6388);
    }
    Menu_EndFrame();
}

void HighScores_Leave(void) {
    Menu_StartLeave(D_800F5F80);
    Menu_StartFadeOut();
}

void HighScores_LeaveFrame(void) {
    Menu_Nop(D_800F5F80);
}

/* Adds car and track unlock bits to profile p. */
static inline void Profile_AddUnlocks(Profile *p, u32 car0, u32 car1, u32 track) {
    p->trackUnlocks |= track;
    p->carUnlocks[0] |= car0;
    p->carUnlocks[1] |= car1;
}

/* After winning a cup: records the score and unlocks the next cup's cars and tracks, noting the new
 * ones in D_80124CE4/CEC/CF4 (and the next cup in D_80124CFC). */
void Cup_AwardWin(void) {
    if (gCupWon) {
        gCupWon = 0;
        Records_InsertHighB(gProfileP1->name, gProfileP1->cup, gProfileP1->cupPoints[0]);
        switch (gProfileP1->cup) {
            case 0:
                if (!Cup_IsUnlocked(1)) {
                    D_80124CFC = 1;
                }
                D_80124CF4 |= ~gProfileP1->trackUnlocks & 0x180;
                D_80124CE4 |= ~gProfileP1->carUnlocks[0] & 0x200081;
                Profile_AddUnlocks(gProfileP1, 0x200081, 0, 0x180);
                break;
            case 1:
                if (!Cup_IsUnlocked(2)) {
                    D_80124CFC = 2;
                }
                D_80124CEC |= ~gProfileP1->carUnlocks[1] & 0xC0;
                Profile_AddUnlocks(gProfileP1, 0, 0xC0, 0);
                break;
            case 2:
                D_80124CE4 |= ~gProfileP1->carUnlocks[0];
                D_80124CEC |= ~gProfileP1->carUnlocks[1] & 0x1FF;
                Profile_AddUnlocks(gProfileP1, 0xFFFFFFFF, 0x1FF, 0);
                break;
        }
    }
}

void Unlocks_CheckNew(void) {
    if (D_80124CE4 != 0 || D_80124CE8 != 0 || D_80124CEC != 0 || D_80124CF0 != 0 || D_80124CF4 != 0 ||
        D_80124CF8 != 0 || D_80124CFC != 0) {
        Fsm_Trigger(&gMenuFsm, 0x55);
    } else {
        Fsm_Trigger(&gMenuFsm, 0x56);
    }
}


s32 Bits_PopLowest(u32 *m) {
    u32 v = *m;
    u8 i = 0;

    while (!(v & 1) && i < 32) {
        v >>= 1;
        i++;
    }
    if (i < 32) {
        *m &= ~(1 << i);
        return (s8)i;
    }
    return -1;
}


void Unlocks_Enter(void) {
    Menu_StartEnter(D_800F5F88);
    Menu_LoadBg(&D_80101014);
    Menu_StartFadeIn();
}

void Unlocks_FreeModel(void) {
    if ((D_80124CFD != -1) || (D_80124CFF != D_80124CFD)) {
        sysModelFree(0);
        return;
    }
    if ((D_80124CFE != D_80124CFF) || (D_80124D00 != D_80124CFE)) {
        sysWorldFree(NameList_Find(&D_80100E10) & 0xFF);
        NameList_Remove(&D_80100E10);
    }
}

void Unlocks_ShowCar(u8 i) {
    u8 buf[0x40];
    u8 big[0x118];

    sysModelLoadAsync(0, gCarModelNames[i], 1);
    sysCarInfoLoad(gCarModelNames[i], big, buf);
    D_80140FD8();
    *(V12c_ *)&D_801240E8 = *(V12c_ *)(big + 0x80);
    D_80124C94 = 0;
    MenuCar_Show(0);
    MenuCar_ScaleTo200(0);
    MenuCar_SetPos(0, 500, 500, 0);
    MenuCar_MoveTo(0, 510, 525, 0);
}

void Unlocks_ShowTrack(u8 i) {
    sysWorldLoad(NameList_Add(&D_80100E10), gTrackNames[(u8)(i / 3)][(u8)(i % 3)]);
    MenuGlobe_Reset();
    MenuGlobe_SpinLarge();
    MenuGlobe_SetPos(500, 500, 0);
    MenuGlobe_MoveTo(525, 425, 0);
}


extern s32 D_8014149C;
void Unlocks_ShowNext(void) {

    D_80124CFD = -1;
    D_80124CFF = -1;
    D_80124CFE = -1;
    D_80124D00 = -1;
    D_80124D01 = -1;
    if (D_80124CE4 != 0 || D_80124CEC != 0) {
        if (D_80124CE4 != 0) {
            D_80124CFD = Bits_PopLowest((u32 *)&D_80124CE4);
        } else {
            D_80124CFD = Bits_PopLowest((u32 *)&D_80124CEC) + 0x20;
        }
        Unlocks_ShowCar(D_80124CFD);
        Menu_SetText(gMenuObjs[32], 0, (s32 *)gProfileP1->name, 0x140, 0x12C, 0, 2, 1, 1, 0xFF, 0xFF, 0xFF);
        Menu_SetText(gMenuObjs[32], 1, (s32 *)D_8014149C, 0x140, 0x154, 0, 2, 1, 1, 0xFF, 0xFF, 0xFF);
        Menu_SetText(gMenuObjs[32], 2, (s32 *)D_800FA180[D_80124CFD], 0x140, 0x17C, 0, 2, 1, 1, 0xFF, 0xFF, 0xFF);
        Fsm_Trigger(&gMenuFsm, 0x86);
    } else if (D_80124CF4 != 0) {
        D_80124CFE = Bits_PopLowest((u32 *)&D_80124CF4);
        Unlocks_ShowTrack(D_80124CFE);
        Menu_SetText(gMenuObjs[32], 0, (s32 *)gProfileP1->name, 0x140, 0x12C, 0, 2, 1, 1, 0xFF, 0xFF, 0xFF);
        Menu_SetText(gMenuObjs[32], 1, (s32 *)D_8014149C, 0x140, 0x154, 0, 2, 1, 1, 0xFF, 0xFF, 0xFF);
        Menu_SetText(gMenuObjs[32], 2, (s32 *)D_800F9FE0[D_80124CFE / 3][D_80124CFE % 3], 0x140, 0x17C, 0, 2, 1, 1, 0xFF, 0xFF, 0xFF);
        Fsm_Trigger(&gMenuFsm, 0x86);
    } else if (D_80124CE8 != 0 || D_80124CF0 != 0) {
        if (D_80124CE8 != 0) {
            D_80124CFF = Bits_PopLowest((u32 *)&D_80124CE8);
        } else {
            D_80124CFF = Bits_PopLowest((u32 *)&D_80124CF0) + 0x20;
        }
        Unlocks_ShowCar(D_80124CFF);
        Menu_SetText(gMenuObjs[32], 0, (s32 *)gProfileP2->name, 0x140, 0x12C, 0, 2, 1, 1, 0xFF, 0xFF, 0xFF);
        Menu_SetText(gMenuObjs[32], 1, (s32 *)D_8014149C, 0x140, 0x154, 0, 2, 1, 1, 0xFF, 0xFF, 0xFF);
        Menu_SetText(gMenuObjs[32], 2, (s32 *)D_800FA180[D_80124CFF], 0x140, 0x17C, 0, 2, 1, 1, 0xFF, 0xFF, 0xFF);
        Fsm_Trigger(&gMenuFsm, 0x86);
    } else if (D_80124CF8 != 0) {
        D_80124D00 = Bits_PopLowest((u32 *)&D_80124CF8);
        Unlocks_ShowTrack(D_80124D00);
        Menu_SetText(gMenuObjs[32], 0, (s32 *)gProfileP2->name, 0x140, 0x12C, 0, 2, 1, 1, 0xFF, 0xFF, 0xFF);
        Menu_SetText(gMenuObjs[32], 1, (s32 *)D_8014149C, 0x140, 0x154, 0, 2, 1, 1, 0xFF, 0xFF, 0xFF);
        Menu_SetText(gMenuObjs[32], 2, (s32 *)D_800F9FE0[D_80124D00 / 3][D_80124D00 % 3], 0x140, 0x17C, 0, 2, 1, 1, 0xFF, 0xFF, 0xFF);
        Fsm_Trigger(&gMenuFsm, 0x86);
    } else if (D_80124CFC != 0) {
        D_80124D01 = D_80124CFC;
        D_80124CFC = 0;
        Menu_SetText(gMenuObjs[32], 0, (s32 *)gProfileP1->name, 0x140, 0xD2, 0, 2, 1, 1, 0xFF, 0xFF, 0xFF);
        Menu_SetText(gMenuObjs[32], 1, (s32 *)D_8014149C, 0x140, 0xF0, 0, 2, 1, 1, 0xFF, 0xFF, 0xFF);
        Menu_SetText(gMenuObjs[32], 2, ((s32 **)&D_8012406C)[D_80124D01], 0x140, 0x10E, 0, 2, 1, 1, 0xFF, 0xFF, 0xFF);
        Fsm_Trigger(&gMenuFsm, 0x86);
    }
    Fsm_Trigger(&gMenuFsm, 0x11);
}

void Unlocks_ResetTexts(void) {
    Menu_ResetTexts(D_800F5F88);
}

void Unlocks_Animate(void) {
    Menu_Animate((*(void * *)&D_800F5F88), 0x20U);
}

void Unlocks_Draw(void) {
    Menu_SetupCamera();
    Menu_UpdateFade();
    Menu_Draw(gMenuObjs[32], -1, -1, -1, -1, -1, -1, gMenuHideText);
    if (D_80124CFD != -1 || D_80124CFF != D_80124CFD) {
        if (D_80124C94 == 0) {
            D_80124C94 = sysModelPollLoad(0);
        } else {
            MenuCar_Draw(0, D_801240E8, D_801240EC, D_801240F0);
        }
    } else if (D_80124CFE != D_80124CFF || D_80124D00 != D_80124CFE) {
        MenuGlobe_Draw(D_80124CFE);
    }
    Menu_EndFrame();
}

void Unlocks_Input(void) {
    if ((D_80124CFD != -1) || (D_80124CFE != D_80124CFD) || (D_80124D01 != D_80124CFE)) {
        Menu_HandlePad1(0x50);
        return;
    }
    if ((D_80124CFF != D_80124D01) || (D_80124D00 != D_80124CFF)) {
        Menu_HandlePad2(0x50);
    }
}

void Unlocks_Leave(void) {
    Menu_StartLeave(D_800F5F88);
    Menu_StartFadeOut();
}

void Unlocks_LeaveNop(void) {

}

/* Inserts (name, value) into track's high-value record table (highA); an entry with the placeholder name is overwritten. */
void Records_InsertHighA(char *name, u32 value, u8 track) {
    SaveRecTable tmp;
    u8 i;

    for (i = 0; i < 5; i++) {
        if (strcmp(SAVE_DATA->highA[track].e[i].name, D_80141230) == 0) {
            strcpy((s32 *)SAVE_DATA->highA[track].e[i].name, (s32)name);
            SAVE_DATA->highA[track].e[i].value = value;
            return;
        }
        if (SAVE_DATA->highA[track].e[i].value < value) {
            tmp = SAVE_DATA->highA[track];
            memcpy(&SAVE_DATA->highA[track].e[i + 1], &tmp.e[i], (4 - i) * sizeof(SaveRecord));
            strcpy((s32 *)SAVE_DATA->highA[track].e[i].name, (s32)name);
            SAVE_DATA->highA[track].e[i].value = value;
            return;
        }
    }
}

/* Inserts (name, value) into track's best-time table (times, lower is better); an entry with the placeholder name is overwritten. */
void Records_InsertTime(char *name, u32 value, u8 track) {
    SaveRecTable tmp;
    u8 i;

    for (i = 0; i < 5; i++) {
        if (strcmp(SAVE_DATA->times[track].e[i].name, D_80141230) == 0) {
            strcpy((s32 *)SAVE_DATA->times[track].e[i].name, (s32)name);
            SAVE_DATA->times[track].e[i].value = value;
            return;
        }
        if (value < SAVE_DATA->times[track].e[i].value) {
            tmp = SAVE_DATA->times[track];
            memcpy(&SAVE_DATA->times[track].e[i + 1], &tmp.e[i], (4 - i) * sizeof(SaveRecord));
            strcpy((s32 *)SAVE_DATA->times[track].e[i].name, (s32)name);
            SAVE_DATA->times[track].e[i].value = value;
            return;
        }
    }
}

/* Inserts (name, value) into track's second high-value record table (highB); an entry with the placeholder name is overwritten. */
void Records_InsertHighB(char *name, u8 track, u32 value) {
    SaveRecTable tmp;
    u8 i;

    for (i = 0; i < 5; i++) {
        if (strcmp(SAVE_DATA->highB[track].e[i].name, D_80141230) == 0) {
            strcpy((s32 *)SAVE_DATA->highB[track].e[i].name, (s32)name);
            SAVE_DATA->highB[track].e[i].value = value;
            return;
        }
        if (SAVE_DATA->highB[track].e[i].value < value) {
            tmp = SAVE_DATA->highB[track];
            memcpy(&SAVE_DATA->highB[track].e[i + 1], &tmp.e[i], (4 - i) * sizeof(SaveRecord));
            strcpy((s32 *)SAVE_DATA->highB[track].e[i].name, (s32)name);
            SAVE_DATA->highB[track].e[i].value = value;
            return;
        }
    }
}

void Records_AddRaceTimes(RaceSt_ *rs) {
    u8 slot;

    slot = D_80123EBF + D_80123EBE * 3 - 1;
    Records_InsertHighA(gProfileP1->name, rs->res[0].raceTime, slot);
    if (gNumPlayers == 2) {
        Records_InsertHighA(gProfileP2->name, rs->res[1].raceTime, slot);
    }
}

void Records_AddLapTimes(RaceSt_ *rs) {
    u8 slot;

    slot = D_80123EBF + D_80123EBE * 3 - 1;
    if (rs->res[0].lap == D_80123EC1) {
        Records_InsertTime(gProfileP1->name, rs->res[0].lapTime, slot);
    }
    if (gNumPlayers == 2 && rs->res[1].lap == D_80123EC1) {
        Records_InsertTime(gProfileP2->name, rs->res[1].lapTime, slot);
    }
}

/* Records the race times and stores which cars/tracks this race unlocked for each player
 * (D_80124CE4..CF8, shown by Unlocks_ShowNext) before adding them to the profiles. */
void Records_SaveRaceResult(void *arg) {
    RaceSt_ *rs = arg;
    Profile *p1;
    Profile *p2;
    u32 p1Cars1;

    if (rs->outcome == 1) {
        if (gRaceSetup.flags & 2) {
            Records_AddRaceTimes(rs);
        }
        Records_AddLapTimes(rs);
    }

    p1 = gProfileP1;
    p1Cars1 = p1->carUnlocks[1];
    p2 = gProfileP2;
    D_80124CE4 = rs->carUnlocksP1[0] & ~p1->carUnlocks[0];
    D_80124CEC = rs->carUnlocksP1[1] & ~p1Cars1;
    D_80124CF4 = rs->trackUnlocksP1 & ~p1->trackUnlocks;
    D_80124CE8 = rs->carUnlocksP2[0] & ~p2->carUnlocks[0];
    D_80124CF0 = rs->carUnlocksP2[1] & ~p2->carUnlocks[1];
    D_80124CF8 = rs->trackUnlocksP2 & ~p2->trackUnlocks;
    p1->carUnlocks[0] |= rs->carUnlocksP1[0];
    p1->carUnlocks[1] |= rs->carUnlocksP1[1];
    p1->trackUnlocks |= rs->trackUnlocksP1;
    p2->carUnlocks[0] |= rs->carUnlocksP2[0];
    p2->carUnlocks[1] |= rs->carUnlocksP2[1];
    p2->trackUnlocks |= rs->trackUnlocksP2;
}

void Help_Enter(void) {
    s32 buf[8];

    sprintf((char *)buf, (char *)&D_80101020, sysRandRange(10));
    Menu_LoadBg(buf);
    Menu_StartFadeIn();
    D_80124D04 = sysTimeGetMillis();
    D_80124D08 = 0;
    D_80124D09 = 0;
}


void Help_Draw(void) {
    Menu_SetupCamera();
    Menu_UpdateFade();
    if ((u8)gMenuFadeLevel == 0xFF && *(u8 *)&D_80124D08 == 0) {
        D_80124D08 = 1;
        sysSfxPlay(0x36, 1, 1.0f, 1.0f);
    }
    Menu_EndFrame();
}


void Help_Leave(void) {
    Menu_StartFadeOut();
    D_80124D09 = 1;
}

void Help_Timeout10s(void) {
    if ((u32)sysTimeGetMillis() > (u32)(D_80124D04 + 10000)) {
        Fsm_Trigger(&gMenuFsm, 0x7D);
    }
}


void Help_Input(void) {
    if (*(u8 *)&D_80124D09 == 0 && (u32)sysTimeGetMillis() > (u32)(D_80124D04 + 3000)) {
        Menu_HandlePad1(0x50);
        if (gNumPlayers == 2) {
            Menu_HandlePad2(0x50);
        }
    }
}


void Race_StartForMode(void) {
    switch (gActiveSave->gameMode) {
    case 0:
    case 1:
        Fsm_Trigger(&(*(s32 *)&gMenuFsm), 0x7E);
        break;
    case 2:
        Fsm_Trigger(&(*(s32 *)&gMenuFsm), 0x7F);
        break;
    case 3:
        Fsm_Trigger(&(*(s32 *)&gMenuFsm), 0x80);
        break;
    case 4:
    default:
        Fsm_Trigger(&(*(s32 *)&gMenuFsm), 0x81);
        break;
    }
}

void Demo_Input(void) {
    Menu_HandlePad1(0x50);
}

void Race_StartDemo(void) {
    u8 i;
    u8 r;
    s32 (**rnd)(s32);

    Menu_FreeBg();
    rnd = &sysRandRange;
    do {
        do {
            gRaceSetup.cup = (*rnd)(4);
            gRaceSetup.track = (*rnd)(3) + 1;
        } while ((u8)Track_IsUnlocked(gRaceSetup.cup * 3 + (gRaceSetup.track - 1)) == 0);
    } while (gTrackNames[gRaceSetup.cup][gRaceSetup.track - 1] == 0);
    strcpy((s32 *)gRaceSetup.name, D_800F9FD0[gRaceSetup.cup]);
    gRaceSetup.count = 6;
    gRaceSetup.f19 = 1;
    gRaceSetup.f9C = D_80138D58;
    gRaceSetup.f1A = ((u8 *)D_800FA010)[gRaceSetup.track - 1];
    memset(D_80124D10, 1, 0x29);
    for (i = 0; i < gRaceSetup.count; i++) {
        r = sysRandRange(0x29);
        do {
            r++;
            if (r >= 0x29) {
                r = 0;
            }
        } while ((u8)Car_IsUnlocked(r, gProfileP1) == 0 || D_80124D10[r] == 0);
        D_80124D10[r] = 0;
        gRaceSetup.p[i].grid = i;
        strcpy((s32 *)gRaceSetup.p[i].name, (s32 *)gCarModelNames[r]);
        gRaceSetup.p[i].kind = 0;
        gRaceSetup.p[i].player = 0;
    }
    gRaceSetup.timeLimit = 60000;
    gRaceSetup.flags = 5;
    if ((u8)sysGameStartRace(&gRaceSetup) == 0) {
        Fsm_Trigger(&gMenuFsm, 0x82);
    }
    Music_StartRaceTrack();
}

void Race_ReturnDemo(void) {
    if (sysGameEndRace(&gRaceResult) == 0) {
        Fsm_Trigger(&gMenuFsm, 0x82);
    }
    Music_StopTrack();
}


void Demo_RunRace(void) {
    if (sysGameUpdate() != 0) {
        Fsm_Trigger(&gMenuFsm, 0x11);
    }
}


void Race_CopyPlayerNames(void) {
    strcpy(&D_80138D20, gProfileP1->name);
    strcpy(&D_801345B8, gProfileP2->name);
}

s32 Cheat_GetColorShiftFlag(u32 arg0) {
    return (arg0 >> 3) & 0x10;
}

s32 Cheat_GetMirrorFlag(u32 arg0) {
    return (arg0 >> 2) & 0x40;
}

void Race_StartPractice(void) {
    s32 one;
    s32 f;
    RaceSetup *g;

    Menu_FreeBg();
    Race_CopyPlayerNames();
    one = 1;
    gRaceSetup.count = gNumPlayers;
    strcpy((s32 *)gRaceSetup.name, D_800F9FD0[gProfileP1->track / 3]);
    gRaceSetup.cup = gProfileP1->track / 3;
    gRaceSetup.track = gProfileP1->track % 3 + 1;
    gRaceSetup.f19 = 2;
    gRaceSetup.f1A = D_800FA010[(u8)(gProfileP1->track / 3)][(u8)gRaceSetup.track - 1];
    strcpy((s32 *)gRaceSetup.p[0].name, (s32 *)gCarModelNames[gProfileP1->car]);
    gRaceSetup.p[0].car = gProfileP1->car;
    gRaceSetup.p[0].grid = 0;
    gRaceSetup.p[0].kind = one;
    gRaceSetup.p[0].player = 0;
    if (gRaceSetup.count == 2) {
        strcpy((s32 *)gRaceSetup.p[1].name, (s32 *)gCarModelNames[gProfileP2->car]);
        gRaceSetup.p[1].car = gProfileP2->car;
        gRaceSetup.p[1].grid = 1;
        gRaceSetup.p[1].kind = one;
        gRaceSetup.p[1].player = 1;
    }
    gRaceSetup.f9C = gActiveSave->option0;
    if (gProfileP1->cheats != 0) {
        gRaceSetup.cheats = gProfileP1->cheats;
    } else if (gProfileP2->cheats != 0) {
        gRaceSetup.cheats = gProfileP2->cheats;
    } else {
        gRaceSetup.cheats = 0;
    }
    gRaceSetup.timeLimit = 1800000;
    if (gActiveSave->gameMode == 0) {
        gRaceSetup.flags = 12;
    } else {
        gRaceSetup.flags = 6;
    }
    f = Cheat_GetColorShiftFlag(gProfileP1->cheats) | Cheat_GetColorShiftFlag(gProfileP2->cheats) |
        Cheat_GetMirrorFlag(gProfileP1->cheats) | Cheat_GetMirrorFlag(gProfileP2->cheats);
    g = &gRaceSetup;
    g->flags |= f;
    if ((u8)sysGameStartRace(g) == 0) {
        Fsm_Trigger(&gMenuFsm, 0x82);
    }
    Music_StartRaceTrack();
}

void Race_ReturnPractice(void) {
    u32 *p = (u32 *)&gRaceResult;

    sysGameEndRace(p);
    switch (*p) {
        case 1:
            Fsm_Trigger(&gMenuFsm, 0x83);
            break;
        case 2:
            Fsm_Trigger(&gMenuFsm, 0x84);
            break;
        case 3:
            Fsm_Trigger(&gMenuFsm, 0x85);
            break;
        default:
            Fsm_Trigger(&gMenuFsm, 0x82);
            break;
    }
    Music_StopTrack();
    gProfileP1->cheats = 0;
    gProfileP2->cheats = 0;
}


void Practice_RunRace(void) {
    if (sysGameUpdate() != 0) {
        Fsm_Trigger(&gMenuFsm, 0x11);
    }
}


extern u8 D_80124D0A[];
void Race_StartExhibition(void) {
    u8 i;
    u8 r;
    u8 k;
    s32 a;
    s32 b;
    s32 c;
    RaceSetup *g;

    Menu_FreeBg();
    Race_CopyPlayerNames();
    strcpy((s32 *)gRaceSetup.name, D_800F9FD0[(u8)(gProfileP1->track / 3)]);
    gRaceSetup.cup = gProfileP1->track / 3;
    gRaceSetup.track = gProfileP1->track % 3 + 1;
    gRaceSetup.f19 = 4;
    gRaceSetup.f1A = D_800FA010[(u8)(gProfileP1->track / 3)][gRaceSetup.track - 1];
    memset(D_80124D10, 1, 0x29);
    memset(D_80124D0A, 1, 6);
    strcpy((s32 *)gRaceSetup.p[0].name, (s32 *)gCarModelNames[gProfileP1->car]);
    gRaceSetup.p[0].car = gProfileP1->car;
    gRaceSetup.p[0].kind = 1;
    gRaceSetup.p[0].player = 0;
    D_80124D10[gProfileP1->car] = 0;
    do {
        k = sysRandRange(6);
    } while (D_80124D0A[k] == 0);
    gRaceSetup.p[0].grid = k;
    D_80124D0A[k] = 0;
    if (gNumPlayers == 2) {
        strcpy((s32 *)gRaceSetup.p[1].name, (s32 *)gCarModelNames[gProfileP2->car]);
        gRaceSetup.p[1].car = gProfileP2->car;
        gRaceSetup.p[1].kind = 1;
        gRaceSetup.p[1].player = 1;
        D_80124D10[gProfileP2->car] = 0;
        gRaceSetup.p[0].grid = 0;
        gRaceSetup.p[1].grid = 1;
        gRaceSetup.count = 2;
    } else {
        gRaceSetup.count = 6;
    }
    for (i = gNumPlayers; i < gRaceSetup.count; i++) {
        r = sysRandRange(0x29);
        do {
            r++;
            if (r >= 0x29) {
                r = 0;
            }
        } while ((u8)Car_IsUnlocked(r, gProfileP1) == 0 || D_80124D10[r] == 0);
        D_80124D10[r] = 0;
        do {
            k = sysRandRange(6);
        } while (D_80124D0A[k] == 0);
        gRaceSetup.p[i].grid = k;
        D_80124D0A[k] = 0;
        strcpy((s32 *)gRaceSetup.p[i].name, (s32 *)gCarModelNames[r]);
        gRaceSetup.p[i].car = r;
        gRaceSetup.p[i].kind = 0;
        gRaceSetup.p[i].player = 0;
    }
    gRaceSetup.timeLimit = 0;
    gRaceSetup.f9C = gActiveSave->option0;
    if (gProfileP1->cheats != 0) {
        gRaceSetup.cheats = gProfileP1->cheats;
    } else if (gProfileP2->cheats != 0) {
        gRaceSetup.cheats = gProfileP2->cheats;
    } else {
        gRaceSetup.cheats = 0;
    }
    g = &gRaceSetup;
    g->flags = 0x20;
    a = Cheat_GetColorShiftFlag(gProfileP1->cheats);
    b = Cheat_GetColorShiftFlag(gProfileP2->cheats);
    c = Cheat_GetMirrorFlag(gProfileP1->cheats);
    g->flags |= a | b | c | Cheat_GetMirrorFlag(gProfileP2->cheats);
    if ((u8)sysGameStartRace(&gRaceSetup) == 0) {
        Fsm_Trigger(&gMenuFsm, 0x82);
    }
    Music_StartRaceTrack();
}

void Race_ReturnExhibition(void) {
    u32 *p = (u32 *)&gRaceResult;

    sysGameEndRace(p);
    switch (*p) {
        case 1:
            Fsm_Trigger(&gMenuFsm, 0x83);
            break;
        case 2:
            Fsm_Trigger(&gMenuFsm, 0x84);
            break;
        case 3:
            Fsm_Trigger(&gMenuFsm, 0x85);
            break;
        default:
            Fsm_Trigger(&gMenuFsm, 0x82);
            break;
    }
    Music_StopTrack();
    gProfileP1->cheats = 0;
    gProfileP2->cheats = 0;
}


void Exhibition_RunRace(void) {
    if (sysGameUpdate() != 0) {
        Fsm_Trigger(&gMenuFsm, 0x11);
    }
}


void Race_StartAirtime(void) {
    s32 a;
    s32 b;
    s32 c;
    RaceSetup *g;

    Menu_FreeBg();
    Race_CopyPlayerNames();
    strcpy((s32 *)gRaceSetup.name, D_800F9FD0[gProfileP1->track / 3]);
    gRaceSetup.cup = gProfileP1->track / 3;
    gRaceSetup.track = gProfileP1->track % 3 + 1;
    gRaceSetup.f19 = 2;
    gRaceSetup.f1A = D_800FA010[(u8)(gProfileP1->track / 3)][(u8)gRaceSetup.track - 1];
    gRaceSetup.count = gNumPlayers;
    strcpy((s32 *)gRaceSetup.p[0].name, (s32 *)gCarModelNames[gProfileP1->car]);
    gRaceSetup.p[0].car = gProfileP1->car;
    gRaceSetup.p[0].kind = 1;
    gRaceSetup.p[0].player = 0;
    gRaceSetup.p[0].grid = 0;
    if (gRaceSetup.count == 2) {
        strcpy((s32 *)gRaceSetup.p[1].name, (s32 *)gCarModelNames[gProfileP2->car]);
        gRaceSetup.p[1].car = gProfileP2->car;
        gRaceSetup.p[1].kind = 1;
        gRaceSetup.p[1].player = 1;
        gRaceSetup.p[1].grid = 1;
    }
    gRaceSetup.timeLimit = 180000;
    gRaceSetup.f9C = gActiveSave->option0;
    if (gProfileP1->cheats != 0) {
        gRaceSetup.cheats = gProfileP1->cheats;
    } else if (gProfileP2->cheats != 0) {
        gRaceSetup.cheats = gProfileP2->cheats;
    } else {
        gRaceSetup.cheats = 0;
    }
    g = &gRaceSetup;
    g->flags = 6;
    a = Cheat_GetColorShiftFlag(gProfileP1->cheats);
    b = Cheat_GetColorShiftFlag(gProfileP2->cheats);
    c = Cheat_GetMirrorFlag(gProfileP1->cheats);
    g->flags |= a | b | c | Cheat_GetMirrorFlag(gProfileP2->cheats);
    if ((u8)sysGameStartRace(&gRaceSetup) == 0) {
        Fsm_Trigger(&gMenuFsm, 0x82);
    }
    Music_StartRaceTrack();
}

void Race_ReturnAirtime(void) {
    u32 *p = (u32 *)&gRaceResult;

    sysGameEndRace(p);
    switch (*p) {
        case 1:
            Fsm_Trigger(&gMenuFsm, 0x83);
            break;
        case 2:
            Fsm_Trigger(&gMenuFsm, 0x84);
            break;
        case 3:
            Fsm_Trigger(&gMenuFsm, 0x85);
            break;
        default:
            Fsm_Trigger(&gMenuFsm, 0x82);
            break;
    }
    Music_StopTrack();
    gProfileP1->cheats = 0;
    gProfileP2->cheats = 0;
}


void Airtime_RunRace(void) {
    if (sysGameUpdate() != 0) {
        Fsm_Trigger(&gMenuFsm, 0x11);
    }
}


u32 Car_WrapIndex(u32 arg0) {
    return ((arg0 & 0xFF) % 41) & 0xFF;
}

void Race_StartCup(void) {
    s32 buf[4];
    u8 i;
    u8 r;
    u8 c;
    s32 a;
    s32 b;
    s32 d;
    RaceSetup *g;

    Menu_FreeBg();
    Race_CopyPlayerNames();
    memset(D_80124D10, 1, 0x29);
    strcpy((s32 *)gRaceSetup.p[0].name, (s32 *)gCarModelNames[gProfileP1->car]);
    gRaceSetup.p[0].car = gProfileP1->car;
    gRaceSetup.p[0].kind = 1;
    gRaceSetup.p[0].player = 0;
    gRaceSetup.p[0].grid = Cup_GetPosition(0);
    D_80124D10[gProfileP1->car] = 0;
    if (gProfileP1->cup == 2) {
        gRaceSetup.count = 3;
        gRaceSetup.f9C = 0xFF;
        gRaceSetup.p[0].grid = 2;
        memcpy(&gRaceSetup.p[1], D_80101044, 9);
        gRaceSetup.p[1].car = 0x26;
        gRaceSetup.p[1].kind = 0;
        gRaceSetup.p[1].player = 0;
        gRaceSetup.p[1].grid = 0;
        memcpy(&gRaceSetup.p[2], D_80101050, 9);
        gRaceSetup.p[2].car = 0x27;
        gRaceSetup.p[2].kind = 0;
        gRaceSetup.p[2].player = 0;
        gRaceSetup.p[2].grid = 1;
    } else {
        gRaceSetup.count = 6;
        gRaceSetup.f9C = gActiveSave->option0;
        for (i = 1; i < 6; i++) {
            r = (u8)Car_WrapIndex(((u8 *)gProfileP1 + i)[7]);
            do {
                r++;
                if (r >= 0x29) {
                    r = 0;
                }
            } while ((u8)Car_IsUnlocked(r, gProfileP1) == 0 || D_80124D10[r] == 0);
            D_80124D10[r] = 0;
            strcpy((s32 *)gRaceSetup.p[i].name, (s32 *)gCarModelNames[r]);
            gRaceSetup.p[i].car = r;
            gRaceSetup.p[i].kind = 0;
            gRaceSetup.p[i].player = 0;
            gRaceSetup.p[i].grid = Cup_GetPosition(i);
        }
    }
    c = D_800F60D4[gProfileP1->cup][gProfileP1->cupRace];
    gRaceSetup.cup = c / 3;
    gRaceSetup.track = c % 3 + 1;
    strcpy((s32 *)gRaceSetup.name, D_800F9FD0[gRaceSetup.cup]);
    gRaceSetup.f19 = 6;
    gRaceSetup.f1A = D_800FA010[gRaceSetup.cup][gRaceSetup.track - 1];
    gRaceSetup.timeLimit = 0;
    if (gProfileP1->cheats != 0) {
        gRaceSetup.cheats = gProfileP1->cheats;
    } else if (gProfileP2->cheats != 0) {
        gRaceSetup.cheats = gProfileP2->cheats;
    } else {
        gRaceSetup.cheats = 0;
    }
    g = &gRaceSetup;
    g->flags = 0x20;
    a = Cheat_GetColorShiftFlag(gProfileP1->cheats);
    b = Cheat_GetColorShiftFlag(gProfileP2->cheats);
    d = Cheat_GetMirrorFlag(gProfileP1->cheats);
    g->flags |= a | b | d | Cheat_GetMirrorFlag(gProfileP2->cheats);
    if (gProfileP1->cup == 1) {
        g->flags |= 0x100;
        sprintf((char *)buf, (char *)&D_8010105C, gProfileP1->cupRace + 1);
        strcpy((s32 *)g->sub, (s32)buf);
    } else if (gProfileP1->cup == 2) {
        g->flags |= 0x100;
        sprintf((char *)buf, (char *)&D_80101064);
        strcpy((s32 *)g->sub, (s32)buf);
    }
    if ((u8)sysGameStartRace(&gRaceSetup) == 0) {
        Fsm_Trigger(&gMenuFsm, 0x82);
    }
    Music_StartRaceTrack();
}


void Race_ReturnCup(void) {
    u8 *lap;
    s32 ofs;
    CupRec_ *recs;
    u8 i;
    s32 *res;

    for (i = 0, lap = &D_80123EC1, recs = D_80123F60; i < 6; i++) {
        /* TODO(match-debt): recs[i].lap[*lap] adds the terms in another order; the row offset has to be
           computed first and added to &recs->lap[*lap] as bytes. */
        ofs = i * sizeof(CupRec_);
        D_80123F60[i].f24 = 0;
        *(s32 *)((s32)&recs->lap[*lap] + ofs) = 0;
        D_80123F60[i].f2A = 0;
    }
    res = (s32 *)&gRaceResult;
    sysGameEndRace(res);
    switch ((u32)*res) {
    case 1:
        Fsm_Trigger(&gMenuFsm, 0x83);
        break;
    case 2:
        Fsm_Trigger(&gMenuFsm, 0x84);
        break;
    case 3:
        Fsm_Trigger(&gMenuFsm, 0x85);
        break;
    default:
        Fsm_Trigger(&gMenuFsm, 0x82);
        break;
    }
    Music_StopTrack();
    gProfileP1->cheats = 0;
    gProfileP2->cheats = 0;
}

void Cup_RunRace(void) {
    if (sysGameUpdate() != 0) {
        Fsm_Trigger(&gMenuFsm, 0x11);
    }
}

