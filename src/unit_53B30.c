#include "common.h"
#include "ultra64.h"
#include "math.h"
#include "include_asm.h"
/* TODO(match-debt): empty loop needed to match (scheduling barrier). Assumed to be a debug hook
 * compiled out in release builds; verify against a real cause when one turns up. */
#define DEBUG_NOP() do { } while (0)

/* TODO (Clean version): Unify Plr53_ with the actual player-data struct at gProfileP1.
 * Fields bits14/bits18 are a split 64-bit unlock mask; consider:
 *   typedef struct { u8 p0[0x14]; u64 unlockBits; s32 raceUnlocks; u8 sel; } Plr53_;
 * or use helper accessors:
 *   #define PLR_IS_UNLOCKED(p, id) \
 *       ((id) >= 32 ? ((p)->bits18 >> ((id)-32)) & 1 : ((p)->bits14 >> (id)) & 1)
 */
/* Player menu state: unlock bitmasks and the selected entry. */
typedef struct { u8 p0[0x14]; s32 bits14; s32 bits18; s32 bits1C; u8 sel; } Plr53_;
/* Per-player race result block (0x2C) inside the race state. */
typedef struct { s32 lapTime; u8 lap; u8 p5[3]; s32 raceTime; u8 pC[0x20]; } RaceRes_;
typedef struct { u8 p0[0x3C]; RaceRes_ res[2]; } RaceSt_;
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
typedef struct { u8 pad[0x23]; u8 b23; s32 w24[6]; } Blk23_;
extern s32 D_800F60E8[];
u8 Save_Probe(void);
void KeyRepeat_Accelerate(u8 i);
extern u8 gSaveLoaded;
extern u8 D_80100F88[];
typedef struct { u8 p0[0x10]; s32 unk10; f32 unk14; } ObjG1_;
typedef struct { s32 d[0x88C / 4]; } Big88C_;
typedef struct { u8 a; u8 pad; s16 x; s16 y; u8 pad2[4]; } E0A4_;
typedef struct { s32 d[0x50]; } Big140_;
typedef struct { char *name; u8 pad[0xC]; } N10_;
typedef struct { f32 x; f32 y; f32 z; f32 x2; f32 y2; u8 pad[0x1C]; u16 h30; u16 h32; } E34b_;
typedef struct { u8 pad[0x20]; u8 n; u8 p[3]; N10_ *names; E34b_ *objs; } TBL_;
typedef struct { u8 pad[8]; char name[0xC]; s32 w14; s32 w18; s32 w1C; u8 pad2[0x120]; } E140b_;
typedef struct { u16 idx; u8 pad[2]; s32 lo; s32 hi; s32 y; s32 reset; f32 x; } Scroll4E_;
typedef struct { u8 a; u8 b; u8 c; s8 lv; } Bars4_;
typedef struct { char name[0xC]; s32 f0C; u8 f10; u8 f11; u8 f12; u8 p13; } Pl7A_;
typedef struct { s32 flags; char name[9]; char sub[9]; u8 trk; u8 sb; u8 cnt; u8 f19; u8 f1A; u8 p1B; s32 f1C; s32 f20; Pl7A_ p[6]; u8 f9C; } Race7A_;
typedef struct { u8 b0; u8 p1[0x17]; u8 b18; u8 p19[0x13]; } It2C_;
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
extern s32 D_801411CC[];
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
extern Nm5010_ D_800F61F0[];
void Menu_DrawModel(u8, f32, f32, f32, f32, f32, f32, f32);
void Menu_InitViewMtx(f32 (*m)[4]);
extern void (*sysCameraApply)(f32 (*)[4]);
extern u8 D_800F648F;
extern u8 D_800F648E;
void HighScores_PickRandom(void);
extern u8 D_80100BD8;
void MenuList_Reset(s32 arg0);
s32 Car_IsUnlocked(u8 i, s32 p);
extern void (*sysWorldDraw)(u8);
extern void (*sysFontLoad)(s32, void *);
extern u8 D_80100F94;
void Music_PlaySong(void);
extern s32 (*sysScreenEndFrame)();
s32 Menu_EndFrame();
s32 Menu_LoadBg(s32 *a);
extern Race7A_ gRaceSetup;
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
s32 SaveSlot_Add(u32 i, E140b_ *src);
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
void Menu_SetText(s32, s32, s32 *, s32, s32, s32, s32, s32, s32, s32, s32, s32);
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
s32 Password_Apply(void *, s32);
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
extern u8 *gActiveSave;
extern u8 D_80123EBE;
extern u8 D_80123EBF;
extern u8 D_80123EC1;
s32 MenuCar_Draw(s32, s32, s32, s32);
void MenuGlobe_Draw(s8);
typedef struct { u8 p0[0x2C]; } MnText_;
typedef struct { u8 p0[0x10]; u8 r; u8 g; u8 b; u8 p13[5]; } MnSprite_;
typedef struct {
    u8 nText; u8 p1[3]; MnText_ *text;
    u8 nSprites; u8 p9[3]; MnSprite_ *sprites;
    u8 nButtons; u8 p11[3]; MnSprite_ *buttons;
    u8 nItems; u8 p19[0x13];
    u8 hiR; u8 hiG; u8 hiB;
} Menu_;
void Menu_Draw(Menu_ *m, s8 selText, s8 selButton, s32 arg3, s8 selItem, s32 arg5, s32 arg6, u8 noText);
s32 Menu_UpdateFade();
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
s32 MenuCar_MoveTo(s32,s32,s32,s32);
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
extern u8 gCarModelNames;
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
s32 func_80086F78();
extern s32 (*sysMenuExit)();
s32 Menu_StartEnter(s32);
extern s32 gMenuObjs[];
s32 Menu_HandlePad1(s32);
extern s32 D_80100DD0;
extern s8 gMenuFadeMode;
extern s8 gMenuFadeLevel;
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
extern s32 gProfileP1;
extern u8 gNumPlayers;
extern u8 gGarageCarP2;
extern s32 gProfileP2;
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

/* TODO (Clean version): Perspective projection setup with magic constants:
 *   nearClip = 48.0f, farClip = 5000.0f, fovY = 60.0 deg (1.0471967f rad = M_PI / 3.0f), aspect = 4:3 (1.3333334f).
 * Propose:
 *   #define FOV_60_DEGREES  1.0471967f
 *   #define ASPECT_4_3      (4.0f / 3.0f)
 *   sysCameraSetPerspective(48.0f, 5000.0f, FOV_60_DEGREES, ASPECT_4_3);
 */
void Menu_SetupCamera(void) {
    sysScreenSetClearColor(0, 0, 0);
    sysScreenBeginFrame();
    sysCameraSetPerspective(48.0f, 5000.0f, 1.0471967f, 1.3333334f);
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

/* TODO (Clean version): Replace raw byte indexing on 't' with a typed UI component header:
 *   typedef struct {
 *       u8 pad0[0x18];
 *       u8 count;             // 0x18
 *       u8 pad19[3];
 *       E0A4_ *entries;       // 0x1C
 *       u8 pad20[8];
 *       u8 sheet;             // 0x28
 *       u8 r, g, b;           // 0x29, 0x2A, 0x2B
 *   } UIComponent_;
 */
void Menu_DrawText(u8 *t, u8 mode, u8 c) {
    u8 r = 0;
    u8 g = 0;
    u8 b = 0;
    s32 i;

    if (mode == 0) {
        r = t[0x29];
        g = t[0x2A];
        b = t[0x2B];
    }
    for (i = 0; i < t[0x18]; i++) {
        E0A4_ *e = &(*(E0A4_ **)(t + 0x1C))[i];

        if (mode != 0) {
            Menu_DrawChar(e->a, t[0x28], e->x, e->y, mode, c, c, c);
        } else {
            Menu_DrawChar(e->a, t[0x28], e->x, e->y, 0, r, g, b);
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

/* TODO (Clean version): Unify per-player camera arrays into a struct and define angle constant:
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
    D_800F60B0[i] = 2.2689264f;
    D_800F60B8[i] = 2.2689264f;
    D_800F60A0[i] = 1.0f;
    D_800F60A8[i] = 1.0f;
}

s32 MenuCar_ResetAngle(u8 i) {
    D_800F60B8[i] = 2.2689264f;
    D_800F60B0[i] = 2.2689264f;
    return 1;
}

void MenuCar_Show(u8 i) {
    if (i < 2) {
        D_800F60A0[i] = 0.0f;
        D_800F60C0[i] = 0;
        D_800F60A8[i] = 1.0f;
        D_800F60B8[i] = 2.2689264f;
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
        D_800F60B8[i] = 2.2689264f;
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
/* TODO (Bug potential): Missing return value in non-void function. Return type is s32 but control
 * reaches end without returning a value. Either change signature to void MenuCar_MoveTo(...) or return 0.
 */
s32 MenuCar_MoveTo(s32 arg0, s32 a, s32 b, s32 c) {
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

/* TODO (Clean version): Type punning integer store to clear an f32 variable (D_80123E94 is f32,
 * assigned 0.75f and 0.5f in MenuGlobe_SpinLarge/510). Propose clean assignment:
 *   D_80123E94 = 0.0f;
 *   D_80123E98 = 0;
 */
void MenuGlobe_Hide(void) {
    (*(s32 *)&D_80123E94) = 0;
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

INCLUDE_ASM("asm/nonmatchings/unit_53B30", Menu_SetText);

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

/* TODO (Clean version): Fix sprintf prototype and remove int-pointer casts:
 *   int sprintf(char *str, const char *format, ...);
 *   sprintf((char *)out, (const char *)&D_80100CA0, name);
 * Also replace magic bound 18 with ARRAY_COUNT(D_800F61F0).
 */
s32 Text_ButtonGlyphs(char *name, u8 *out) {
    u8 i;
    u8 j;

    for (i = 0; i < 18; i++) {
        if (strcmp(name, D_800F61F0[i].name) == 0) {
            for (j = 0; j < D_800F61F0[i].n; j++) {
                out[j] = D_800F61F0[i].v[j];
            }
            out[j] = 0;
            return 1;
        }
    }
    sprintf((s32 *)out, (s32 *)&D_80100CA0, (s32)name);
    return 0;
}

/* TODO (Clean version): gCarModelNames is an array of 41 string pointers (char *table[41]).
 * Declare properly as:
 *   extern const char *gCarModelNames[41];
 * And refactor accessors cleanly without raw byte pointer arithmetic:
 *   const char *CarModel_GetName(u8 id) { return gCarModelNames[id]; }
 *   s8 CarModel_FindByName(const char *s) {
 *       for (u8 i = 0; i < 41; i++) {
 *           if (gCarModelNames[i] != NULL && strcmp(s, gCarModelNames[i]) == 0) return i;
 *       }
 *       return -1;
 *   }
 */
s32 CarModel_GetName(u32 arg0) {
    return *(s32 *)((u8 *)&gCarModelNames + ((arg0 & 0xFF) * 4));
}

s8 CarModel_FindByName(char *s) {
    u8 i;

    for (i = 0; i < 41; i++) {
        if (((char **)&gCarModelNames)[i] != NULL && strcmp(s, ((char **)&gCarModelNames)[i]) == 0) {
            return i;
        }
    }
    return -1;
}


/* TODO (Clean version): Structure definition for 0x140-byte player profile / save record:
 *   typedef struct {
 *       u8 pad0[8];
 *       char name[12];           // 0x08 (default "Player %d")
 *       u32 magic;               // 0x14 (0xB48EE52E)
 *       s32 unlockMaskLo;        // 0x18 (default 0x10B)
 *       s32 unlockMaskHi;        // 0x1C (default 0xE3F)
 *       u8 defaultCar;           // 0x20 (default 0x1D)
 *       u8 selectedCourse;       // 0x21
 *       u8 pad22[2];
 *       s32 records[6];          // 0x24..0x38
 *       s32 totalTime;           // 0x3C
 *       u8 controllerCfg[0x100]; // 0x40
 *   } PlayerProfile; // sizeof = 0x140
 */
void Profile_InitDefault(void *arg0, s32 arg1) {
    u8 *o = arg0;
    s32 buf[4];
    u8 i = arg1;
    u8 j;

    ((s32 (*)(s32 *, s32 *, s32, s32))sprintf)(buf, (s32 *)&D_80100CA4, D_8014122C, i + 1);
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


INCLUDE_ASM("asm/nonmatchings/unit_53B30", Save_InitDefault);

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
    gProfileP1 = SaveSlot_FindPlayer(0);
    gProfileP2 = SaveSlot_FindPlayer(1);
    if (gActiveSave == NULL) {
        gActiveSave = &D_80124160;
        gProfileP1 = (s32)&D_801249F0;
        gProfileP2 = (s32)&D_80124B30;
        Fsm_Trigger(&gMenuFsm, 0x23);
        return;
    }
    if (gProfileP1 == 0) {
        gProfileP1 = (s32)&D_801249F0;
    }
    if (gProfileP2 == 0) {
        gProfileP2 = (s32)&D_80124B30;
    }
    sysAudioSetSfxVolume(0xFF);
    fp = &sysInputLoadBindings;
    (*fp)(gProfileP1 + 0x40, 0);
    (*fp)(gProfileP2 + 0x40, 1);
    gSaveLoaded = 1;
    Fsm_Trigger(&gMenuFsm, 0x22);
}

/* TODO (Clean version): Consolidate repeated 4-controller button polling with a helper macro:
 *   #define CHECK_ANY_CONTROLLER_BTN(btn) \
 *       (Input_PressedOnce(0, (btn)) || Input_PressedOnce(1, (btn)) || \
 *        Input_PressedOnce(2, (btn)) || Input_PressedOnce(3, (btn)))
 * And define named constants for button events (0xE, 0xF, 0x12) and SFX IDs (0x2E, 0x2F, 0x32).
 */
void PakDialog_Input(void) {
    void (**fp)(s32);

    fp = &sysInputUpdate;
    (*fp)(0);
    (*fp)(1);
    (*fp)(2);
    (*fp)(3);
    if ((u8) gPakDialogSel == 4) {
        if (Input_PressedOnce(0, 0xF) || Input_PressedOnce(1, 0xF) || Input_PressedOnce(2, 0xF) || Input_PressedOnce(3, 0xF)) {
            sysAudioSetAmbientVolume(0xFF);
            sysSfxPlay(0x2F, 1, 1.0f, 1.0f);
            Fsm_Trigger(&gMenuFsm, 8);
        }
    } else if (Input_PressedOnce(0, 0xE) || Input_PressedOnce(1, 0xE) || Input_PressedOnce(2, 0xE) || Input_PressedOnce(3, 0xE)) {
        sysAudioSetAmbientVolume(0xFF);
        sysSfxPlay(0x2E, 1, 1.0f, 1.0f);
        Fsm_Trigger(&gMenuFsm, 6);
    }
    if (Input_PressedOnce(0, 0x12) || Input_PressedOnce(1, 0x12) || Input_PressedOnce(2, 0x12) || Input_PressedOnce(3, 0x12)) {
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

    ((s32 (*)(s32 *, s32 *, s32, s32))sprintf)(buf, (s32 *)&D_80100CAC, (s32)D_80141234, gPakErrorPort);
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

    ((s32 (*)(s32 *, s32 *, s32, s32))sprintf)(buf, (s32 *)&D_80100CAC, (s32)D_80141234, gPakErrorPort);
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

    ((s32 (*)(s32 *, s32 *, s32, s32))sprintf)(buf, (s32 *)&D_80100CAC, (s32)D_80141234, gPakErrorPort);
    Menu_SetText(gMenuObjs[4], 0, buf, 320, 150, 0, 0, 1, 1, 255, 255, 255);
    Menu_SetText(gMenuObjs[4], 1, (s32 *)&D_80100D08, 320, 170, 0, 0, 1, 1, 255, 255, 255);
    Menu_SetText(gMenuObjs[4], 2, (s32 *)&D_80100D20, 320, 190, 0, 0, 1, 1, 255, 255, 255);
    Menu_SetText(gMenuObjs[4], 3, (s32 *)&D_80100CC8, 320, 210, 0, 0, 1, 1, 255, 255, 255);
    Menu_SetText(gMenuObjs[4], 4, D_80141244, 320, 250, 0, 0, 1, 1, 127, 127, 127);
    Menu_SetText(gMenuObjs[4], 5, (s32 *)&D_80100CF0, 320, 270, 0, 0, 1, 1, 127, 127, 127);
}


INCLUDE_ASM("asm/nonmatchings/unit_53B30", PakRepair_Draw);

void PakRepair_Input(void) {
    void (**fp)(s32);

    fp = &sysInputUpdate;
    (*fp)(0);
    (*fp)(1);
    (*fp)(2);
    (*fp)(3);
    if (Input_PressedOnce(0, 0x12) || Input_PressedOnce(1, 0x12) || Input_PressedOnce(2, 0x12) || Input_PressedOnce(3, 0x12)) {
        sysAudioSetAmbientVolume(0xFF);
        sysSfxPlay(0x32, 1, 1.0f, 1.0f);
        Fsm_Trigger(&gMenuFsm, 2);
    }
    if ((u8) gPakDialogSel < 5 && (Input_PressedOnce(0, 0xF) || Input_PressedOnce(1, 0xF) || Input_PressedOnce(2, 0xF) || Input_PressedOnce(3, 0xF))) {
        sysAudioSetAmbientVolume(0xFF);
        sysSfxPlay(0x2F, 1, 1.0f, 1.0f);
        Fsm_Trigger(&gMenuFsm, 8);
    }
    if ((u8) gPakDialogSel >= 4 && (Input_PressedOnce(0, 0xE) || Input_PressedOnce(1, 0xE) || Input_PressedOnce(2, 0xE) || Input_PressedOnce(3, 0xE))) {
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

    ((s32 (*)(s32 *, s32 *, s32, s32))sprintf)(buf, (s32 *)&D_80100CA4, (s32)D_80141234, gPakErrorPort);
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

    ((s32 (*)(s32 *, s32 *, s32, s32))sprintf)(buf, (s32 *)&D_80100CA4, (s32)D_80141234, gPakErrorPort);
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

    ((s32 (*)(s32 *, s32 *, s32, s32))sprintf)(buf, (s32 *)&D_80100CA4, (s32)D_80141234, gPakErrorPort);
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

    ((s32 (*)(s32 *, s32 *, s32, s32))sprintf)(buf, (s32 *)&D_80100CA4, (s32)D_80141234, gPakErrorPort);
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
        if (Pak_GetStatus(i) == 4) {
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
    if (Input_PressedOnce(0, 18) != 0 || Input_PressedOnce(1, 18) != 0 || Input_PressedOnce(2, 18) != 0 ||
        Input_PressedOnce(3, 18) != 0) {
        sysAudioSetAmbientVolume(0xFF);
        sysSfxPlay(0x2F, 1, 1.0f, 1.0f);
        Fsm_Trigger(&gMenuFsm, 2);
    }
}


/* TODO (Clean version): Simplify 6 consecutive 4-iteration loops with a prioritized table lookup:
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
        if (!(u8) found && st[(u8) i] != 1) {
            first = i;
            found = 1;
        }
    }
    if ((u8) first == 0xFF) {
        Fsm_Trigger(&gMenuFsm, 0x1C);
        return;
    }
    for (i = 0; (u8) i < 4; i++) {
        if (st[(u8) i] == 4) {
            gPakErrorPort = i + 1;
            Fsm_Trigger(&gMenuFsm, 0x1f);
            return;
        }
    }
    for (i = 0; (u8) i < 4; i++) {
        if (st[(u8) i] == 9) {
            gPakErrorPort = i + 1;
            Fsm_Trigger(&gMenuFsm, 0x20);
            return;
        }
    }
    for (i = 0; (u8) i < 4; i++) {
        if (st[(u8) i] == 3) {
            gPakErrorPort = i + 1;
            Fsm_Trigger(&gMenuFsm, 0x1e);
            return;
        }
    }
    for (i = 0; (u8) i < 4; i++) {
        if (st[(u8) i] == 8) {
            gPakErrorPort = i + 1;
            Fsm_Trigger(&gMenuFsm, 0x25);
            return;
        }
    }
    for (i = 0; (u8) i < 4; i++) {
        if (st[(u8) i] == 2) {
            gPakErrorPort = i + 1;
            Fsm_Trigger(&gMenuFsm, 0x1d);
            return;
        }
    }
    for (i = 0; (u8) i < 4; i++) {
        if (st[(u8) i] == 7) {
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

/* TODO (Clean version): Avoid raw offset indexing into game state gActiveSave.
 * Offsets 0x884 and 0x888 correspond to player profile IDs/hashes in the 0x88C save header:
 *   typedef struct {
 *       u8 data[0x884];
 *       s32 player1Id; // 0x884
 *       s32 player2Id; // 0x888
 *   } GameSaveData_;
 * Also replace ((s32 *)p)[1] with player2->id.
 */
void Save_CreateNew(void) {
    E140b_ buf;
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
    gActiveSave = (u8 *)Save_GetProfile(slot);
    Save_InitDefault(gActiveSave);
    Profile_InitDefault(&buf, 0);
    SaveSlot_Add(slot, &buf);
    Profile_InitDefault(&buf, 1);
    SaveSlot_Add(slot, &buf);
    gProfileP1 = (s32)SaveSlot_Get(slot, 0);
    p = SaveSlot_Get(slot, 1);
    *(s32 *)(gActiveSave + 0x884) = *(s32 *)(gProfileP1 + 4);
    gProfileP2 = (s32)p;
    *(s32 *)(gActiveSave + 0x888) = ((s32 *)p)[1];
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
    gActiveSave = &D_80124160;
    Save_InitDefault(&D_80124160);
    gProfileP1 = (s32)&D_801249F0;
    Profile_InitDefault(&D_801249F0, 0);
    gProfileP2 = (s32)&D_80124B30;
    Profile_InitDefault(&D_80124B30, 1);
    sysAudioSetSfxVolume(0xFF);
}


void Save_CommitLoaded(void) {
    s32 x;

    gSaveLoaded = 0;
    *(Big88C_ *)&D_80124160 = *(Big88C_ *)gActiveSave;
    *(Big140_ *)&D_801249F0 = *(Big140_ *)gProfileP1;
    *(Big140_ *)&D_80124B30 = *(Big140_ *)gProfileP2;
    x = D_80100DBC;
    gActiveSave = &D_80124160;
    gProfileP1 = (s32)&D_801249F0;
    gProfileP2 = (s32)&D_80124B30;
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
    if (((s32 *)gProfileP1)[5] == 0) {
        Profile_InitDefault((void *)gProfileP1, 0);
    }
    if (((s32 *)gProfileP2)[5] == 0) {
        Profile_InitDefault((void *)gProfileP2, 1);
    }
    if ((u8)Save_Commit() != 0) {
    if (((s32 *)gProfileP1)[1] == 0) {
        Fsm_Trigger(&gMenuFsm, 0x31);
    } else {
        Password_Get(gProfileP1, (s32 *)&gPasswordP1);
    }
    if (gNumPlayers == 2 && ((s32 *)gProfileP2)[1] == 0) {
        Fsm_Trigger(&gMenuFsm, 0x31);
    } else {
        Password_Get(gProfileP2, (s32 *)&gPasswordP2);
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

s32 Car_IsUnlocked(u8 i, s32 p) {
    if (((s32 *)&gCarModelNames)[i] == 0) {
        return 0;
    }
    if (i >= 32) {
        return (*(u32 *)(p + 0x18) >> (i - 32)) & 1;
    }
    return (*(u32 *)(p + 0x14) >> i) & 1;
}

u8 Track_IsUnlocked(u8 i) {
    u8 r;

    if (gTrackNames[(u8)(i / 3)][(u8)(i % 3)] == 0) {
        return 0;
    }
    r = 0;
    if (((((u32 *)gProfileP1)[7] >> i) & 1) || ((((u32 *)gProfileP2)[7] >> i) & 1)) {
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
            w = ((u32 *)gProfileP1)[7];
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
 *   p->bits18 |= 1 << (id - 32);
 */
void Car_Unlock(u8 id, Plr53_ *p) {
    if (id >= 32) {
        p->bits18 |= 1 << (id - 31);
        return;
    }
    p->bits14 |= 1 << id;
}

void Track_Unlock(s32 id, Plr53_ *p) {
    p->bits1C |= 1 << id;
}

void Player_NextCar(u8 player) {
    Plr53_ *p;

    if (!player) {
        p = (Plr53_ *)gProfileP1;
    } else {
        p = (Plr53_ *)gProfileP2;
    }
    do {
        if (p->sel < 40) {
            p->sel++;
        } else {
            p->sel = 0;
        }
    } while (!(u8)Car_IsUnlocked(p->sel, (s32)p));
}

void Player_PrevCar(u8 player) {
    Plr53_ *p;

    if (!player) {
        p = (Plr53_ *)gProfileP1;
    } else {
        p = (Plr53_ *)gProfileP2;
    }
    do {
        if (p->sel != 0) {
            p->sel--;
        } else {
            p->sel = 40;
        }
    } while (!(u8)Car_IsUnlocked(p->sel, (s32)p));
}

/* TODO (Bug potential): Potential infinite loop if all tracks are locked or invalid.
 * If Track_IsUnlocked() returns 0 for all 12 indices (0..11), this loop will never terminate.
 * Propose adding a loop counter / max iterations guard:
 *   for (u8 count = 0; count < 12; count++) { ... }
 */
void Track_SelectNext(void) {
    u8 v;

    for (;;) {
        if (((u8 *)gProfileP1)[0x21] < 11) {
            ((u8 *)gProfileP1)[0x21]++;
        } else {
            ((u8 *)gProfileP1)[0x21] = 0;
        }
        if (Track_IsUnlocked(((u8 *)gProfileP1)[0x21]) == 0) {
            continue;
        }
        v = ((u8 *)gProfileP1)[0x21];
        if (gTrackNames[(u8)(v / 3)][(u8)(v % 3)] != 0) {
            break;
        }
    }
    ((u8 *)gProfileP2)[0x21] = v;
}


void Track_SelectPrev(void) {
    u8 v;

    for (;;) {
        if (((u8 *)gProfileP1)[0x21] != 0) {
            ((u8 *)gProfileP1)[0x21]--;
        } else {
            ((u8 *)gProfileP1)[0x21] = 11;
        }
        if (Track_IsUnlocked(((u8 *)gProfileP1)[0x21]) == 0) {
            continue;
        }
        v = ((u8 *)gProfileP1)[0x21];
        if (gTrackNames[(u8)(v / 3)][(u8)(v % 3)] != 0) {
            break;
        }
    }
    ((u8 *)gProfileP2)[0x21] = v;
}


void Song_PickRandom(void) {
    *(u8 *)&gMenuSong = sysRandRange(7);
}


void Song_PickForTrack(void) {
    gMenuSong = Music_GetSongTrack((u32) (*(u8 *)((u8 *)((*(void * *)&gProfileP1)) + 0x21)));
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

INCLUDE_ASM("asm/nonmatchings/unit_53B30", Music_UpdateVolumeFade);

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

/* TODO (Clean version / Bug check): Idle timeout check (30,000 ms = 30 seconds).
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

/* TODO (Clean version): D_801411CC is a loaded configuration/calibration block.
 * Contiguous scalar globals D_8012406C..D_80124088 (8 words) and D_80124110 (13 words)
 * should be mapped to structured settings:
 *   typedef struct { s32 val[8]; } SystemSettings;
 *   typedef struct { s32 val[13]; } AudioCalibrationSettings;
 */
void Boot_LoadLanguage(void) {
    u8 i;

    if (!Lang_Load(&D_80100DC0)) {
        Fsm_Trigger(&gMenuFsm, 0x12);
    }
    if (!CarNames_Load(&D_80100DC8)) {
        Fsm_Trigger(&gMenuFsm, 0x12);
    }
    D_8012406C = D_801411CC[0];
    D_80124070 = D_801411D0;
    D_80124074 = D_801411D4;
    gGameModeNames = D_801411D8;
    D_8012407C = D_801411DC;
    D_80124080 = D_801411E0;
    D_80124084 = D_801411E4;
    D_80124088 = D_801411CC[0];
    for (i = 0; i < 13; i++) {
        D_80124110[i] = D_801411CC[i + 11];
    }
}

void Boot_LoadTuning(void) {
    if (!(Tuning_Load() & 0xFF)) {
        Fsm_Trigger((s32) &gMenuFsm, 0x12);
    }
}

void Menu_ShutdownNop(void) {
    func_80086F78();
}

void Boot_Nop(void) {

}

void Menu_ExitToGame(void) {
    if (gSaveLoaded == 0) {
        *Save_GetProfile(0) = *(Big88C_ *)&D_80124160;
    }
    Music_StopMenuTrack();
    sysAudioSetMusicVolume(gActiveSave[0x87D]);
    sysAudioSetSfxVolume(gActiveSave[0x87C]);
    sysAudioSetVoiceVolume(gActiveSave[0x880]);
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

/* TODO (Clean version): Millisecond time formatting helpers. Propose unifying into inline helpers or a struct:
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

u32 Time_Seconds(u32 arg0) {
    return ((arg0 / 1000) % 60) & 0xFFFF;
}

u32 Time_Hundredths(u32 arg0) {
    return ((arg0 % 1000) / 10) & 0xFFFF;
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

INCLUDE_ASM("asm/nonmatchings/unit_53B30", Menu_UpdateFade);

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
    Password_Get(gProfileP1, a);
    Password_Get(gProfileP2, b);
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
        sprintf(a, (s32 *)&D_80100DF4, gProfileP1 + 8);
        src = &gPasswordP1;
        gNameChangedP1 = 0;
        D_80124C7E = 1;
        D_80124C7F = 0;
    } else {
        sprintf(a, (s32 *)&D_80100DF4, gProfileP2 + 8);
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

    sysModelLoadAsync(0, ((char **)&gCarModelNames)[((u8 *)gProfileP1)[0x20]], 0);
    sysCarInfoLoad(((char **)&gCarModelNames)[((u8 *)gProfileP1)[0x20]], big, buf);
    D_80140FD8();
    *(V12_ *)&D_801240E8 = *(V12_ *)(big + 0x80);
    D_80124C94 = 0;
}

void CarPreview_LoadP2(void) {
    u8 buf[0x40];
    u8 big[0x118];

    sysModelLoadAsync(1, ((char **)&gCarModelNames)[((u8 *)gProfileP2)[0x20]], 0);
    sysCarInfoLoad(((char **)&gCarModelNames)[((u8 *)gProfileP2)[0x20]], big, buf);
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
    u8 v = ((u8 *)gProfileP1)[0x21];

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

INCLUDE_ASM("asm/nonmatchings/unit_53B30", MainMenu_SetupTexts);

INCLUDE_ASM("asm/nonmatchings/unit_53B30", MainMenu_Draw);

/* Checks the entered cheat code, then handles player 1's buttons 0x10/0x11 on this menu. */
void MainMenu_InputP1(s32 id) {
    if ((u8)Menu_HandlePad1((u16)id) && (u8)Config_CheckCheatCode((char *)Cheat_GetBufferP1(), (void *)gProfileP1)) {
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
    if ((u8)Menu_HandlePad2((u16)id) && (u8)Config_CheckCheatCode((char *)Cheat_GetBufferP2(), (void *)gProfileP2)) {
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
    p = (s32 *)(gActiveSave + 0x878);
    if (*p == 0) {
        *p = 4;
    } else {
        *p -= 1;
    }
}


void GameMode_Next(void) {
    u32 *p;

    sysSfxPlay(0x31, 1, 1.0f, 1.0f);
    p = (u32 *)(gActiveSave + 0x878);
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
    switch (*(s32 *)(gActiveSave + 0x878)) {
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
        switch (*(s32 *)(gActiveSave + 0x878)) {
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
            D_80124150 = ((s32 *)gProfileP1)[1];
            D_80124154 = ((s32 *)gProfileP2)[1];
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
        if (j != p && ((a = *(s32 *)((u8 *)(j * 4 + gProfileP1) + 0x24),
                        b = *(s32 *)((u8 *)(p * 4 + gProfileP1) + 0x24), b < a) ||
                       ((a == b) & (j < p)))) {
            rank++;
        }
        i++;
    } while ((u32)(i & 0xFF) < 6);
    return rank & 0xFF;
}

u8 Cup_GetPositionNoTies(u8 n) {
    Blk23_ *p = (Blk23_ *)gProfileP1;
    u8 r = 0;
    u8 i;

    for (i = 0; i < 6; i++) {
        if (i != n && (u32)p->w24[i] > (u32)p->w24[n]) {
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

    ((Blk23_ *)gProfileP1)->b23 = 0;
    for (i = 0; i < 6; i++) {
        ((Blk23_ *)gProfileP1)->w24[i] = 0;
        D_800F60E8[i] = 0;
    }
}

void Cup_ValidateSelection(void) {
    if (((u8) (*(u8 *)((u8 *)((*(void * *)&gProfileP1)) + 0x22)) >= 3U) || ((u8) (*(u8 *)((u8 *)((*(void * *)&gProfileP1)) + 0x23)) >= 6U)) {
        (*(u8 *)((u8 *)((*(void * *)&gProfileP1)) + 0x22)) = 0U;
        Cup_ResetProgress();
    }
}

void CupTrackPreview_Load(void) {
    s8 k = NameList_Add(&D_80100E10);
    u8 v = D_800F60D4[((u8 *)gProfileP1)[0x22]][((u8 *)gProfileP1)[0x23]];

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
            || ((*(u8 *)&gChampionshipSel) == 5 && (*(u8 * *)&gProfileP1)[0x23] == 0 && ((Cup_IsUnlocked(1) & 0xFF) || (Cup_IsUnlocked(2) & 0xFF)))) {
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
    (*(u8 *)((u8 *)((*(void * *)&gProfileP1)) + 0x23)) = (u8) ((*(u8 *)((u8 *)((*(void * *)&gProfileP1)) + 0x23)) + 1);
}

void Championship_Nop(void) {

}

INCLUDE_ASM("asm/nonmatchings/unit_53B30", Cup_SetupRace);

INCLUDE_ASM("asm/nonmatchings/unit_53B30", Championship_SetupTexts);

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
            D_80124150 = ((s32 *)gProfileP1)[1];
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
    if (((u8 *)gProfileP1)[0x23] == 0) {
        sysSfxPlay(0x30, 1, 1.0f, 1.0f);
        do {
            if (((u8 *)gProfileP1)[0x22] != 0) {
                ((u8 *)gProfileP1)[0x22]--;
            } else {
                ((u8 *)gProfileP1)[0x22] = 2;
            }
        } while (Cup_IsUnlocked(((u8 *)gProfileP1)[0x22]) == 0);
        Cup_ResetProgress();
    }
}

void Cup_Next(void) {
    if (((u8 *)gProfileP1)[0x23] == 0) {
        sysSfxPlay(0x31, 1, 1.0f, 1.0f);
        do {
            if (((u8 *)gProfileP1)[0x22] < 2) {
                ((u8 *)gProfileP1)[0x22]++;
            } else {
                ((u8 *)gProfileP1)[0x22] = 0;
            }
        } while (Cup_IsUnlocked(((u8 *)gProfileP1)[0x22]) == 0);
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

    sprintf(buf, fmt, D_80141504);
    Menu_SetText(gMenuObjs[12], 0, buf, 320, 210, 0, 2, 1, 1, 255, 255, 255);
    ((s32 (*)(s32 *, s32 *, s32, s32))sprintf)(buf, (s32 *)&D_80100EB4, gProfileP1 + 8, D_80141508);
    Menu_SetText(gMenuObjs[12], 1, buf, 320, 240, 0, 2, 1, 1, 255, 255, 255);
    sprintf(buf, fmt, D_8014150C);
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

    ((s32 (*)(s32 *, s32 *))sprintf)(buf, (s32 *)&D_80100F0C);
    Menu_SetText(gMenuObjs[13], 0, buf, 320, 210, 0, 2, 1, 1, 255, 255, 255);
    sprintf(buf, (s32 *)&D_80100F14, gProfileP1 + 8);
    Menu_SetText(gMenuObjs[13], 1, buf, 320, 240, 0, 2, 1, 1, 255, 255, 255);
    ((s32 (*)(s32 *, s32 *))sprintf)(buf, (s32 *)&D_80100F1C);
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
    gGarageCarP1 = (*(u8 *)((u8 *)((*(void * *)&gProfileP1)) + 0x20));
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
    gGarageCarP1 = (*(u8 *)((u8 *)((*(void * *)&gProfileP1)) + 0x20));
    gGarageCarP2 = (*(u8 *)((u8 *)((*(void * *)&gProfileP2)) + 0x20));
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

INCLUDE_ASM("asm/nonmatchings/unit_53B30", Garage_Draw);

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
        ((u8 *)gProfileP1)[0x20] = gGarageCarP1;
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
            ((u8 *)gProfileP2)[0x20] = gGarageCarP2;
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
        sysModelLoadAsync(0, ((char **)&gCarModelNames)[gGarageCarP1], 1);
        sysCarInfoLoad(((char **)&gCarModelNames)[gGarageCarP1], big, buf);
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
        sysModelLoadAsync(1, ((char **)&gCarModelNames)[gGarageCarP2], 1);
        sysCarInfoLoad(((char **)&gCarModelNames)[gGarageCarP2], big, buf);
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
            sysSfxPlay(0x38, 1, gActiveSave[0x87C] / 255.0f, 1.0f);
            break;
        case 3:
            sysSfxPlay(0x37, 1, gActiveSave[0x880] / 255.0f, 1.0f);
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
        if (gActiveSave[0x87E] > 10) {
            gActiveSave[0x87E] -= 10;
            sysSfxPlay(0x30, 1, 1.0f, 1.0f);
        } else {
            gActiveSave[0x87E] = 0;
        }
        break;
    case 1:
        if (gActiveSave[0x87D] > 10) {
            gActiveSave[0x87D] -= 10;
        } else {
            gActiveSave[0x87D] = 0;
        }
        sysAudioSetMusicVolume(gActiveSave[0x87D]);
        break;
    case 2:
        if (gActiveSave[0x87C] > 10) {
            gActiveSave[0x87C] -= 10;
        } else {
            gActiveSave[0x87C] = 0;
        }
        break;
    case 3:
        if (gActiveSave[0x880] > 10) {
            gActiveSave[0x880] -= 10;
        } else {
            gActiveSave[0x880] = 0;
        }
        break;
    }
}

void Options_Increase(void) {
    switch (gOptionsSel) {
    case 0:
        if (gActiveSave[0x87E] < 245) {
            gActiveSave[0x87E] += 10;
            sysSfxPlay(0x31, 1, 1.0f, 1.0f);
        } else {
            gActiveSave[0x87E] = 255;
        }
        break;
    case 1:
        if (gActiveSave[0x87D] < 245) {
            gActiveSave[0x87D] += 10;
        } else {
            gActiveSave[0x87D] = 255;
        }
        sysAudioSetMusicVolume(gActiveSave[0x87D]);
        break;
    case 2:
        if (gActiveSave[0x87C] < 245) {
            gActiveSave[0x87C] += 10;
        } else {
            gActiveSave[0x87C] = 255;
        }
        break;
    case 3:
        if (gActiveSave[0x880] < 245) {
            gActiveSave[0x880] += 10;
        } else {
            gActiveSave[0x880] = 255;
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

    c[0] = gActiveSave[0x87E];
    c[1] = gActiveSave[0x87D];
    c[2] = gActiveSave[0x87C];
    c[3] = gActiveSave[0x880];
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

INCLUDE_ASM("asm/nonmatchings/unit_53B30", Credits_Draw);

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
    (*fp)(gProfileP1 + 0x40, 0);
    (*fp)(gProfileP2 + 0x40, 1);
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


INCLUDE_ASM("asm/nonmatchings/unit_53B30", Controls_SetupTexts);

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
    (*fp)(gProfileP1 + 0x40, 0);
    (*fp)(gProfileP2 + 0x40, 1);
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

INCLUDE_ASM("asm/nonmatchings/unit_53B30", Controls_HasConflict);

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

    ((s32 (*)(s32 *, s32 *))sprintf)(buf, (s32 *)&D_80100FAC);
    Menu_SetText(gMenuObjs[21], 0, buf, 320, 165, 0, 0, 1, 1, 255, 255, 255);
    sprintf(buf, (s32 *)&D_80100FB4, (s32)&D_80124CA0);
    Menu_SetText(gMenuObjs[21], 1, buf, 320, 195, 0, 0, 1, 1, 255, 255, 255);
    sprintf(buf, (s32 *)&D_80100FBC, (s32)&D_80124CC0);
    Menu_SetText(gMenuObjs[21], 2, buf, 320, 225, 0, 0, 1, 1, 255, 255, 255);
    ((s32 (*)(s32 *, s32 *))sprintf)(buf, (s32 *)&D_80100FC4);
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

INCLUDE_ASM("asm/nonmatchings/unit_53B30", LoadGame_Enter);

INCLUDE_ASM("asm/nonmatchings/unit_53B30", LoadGame_SetupTexts);

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

INCLUDE_ASM("asm/nonmatchings/unit_53B30", LoadGame_SyncProfiles);

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
        player = gProfileP1;
    } else {
        player = gProfileP2;
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

INCLUDE_ASM("asm/nonmatchings/unit_53B30", Register_Enter);

void Register_ApplyChoice(void) {
    E140b_ l;
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
                l.w14 |= ((E140b_ *)gProfileP1)->w14;
                l.w18 |= ((E140b_ *)gProfileP1)->w18;
                l.w1C |= ((E140b_ *)gProfileP1)->w1C;
                strcpy((s32 *)l.name, (s32)&gKeyboardText);
                SaveSlot_Add(gLoadPakP1, &l);
            } else {
                Profile_InitDefault(&l, 0);
                l.w14 |= ((E140b_ *)gProfileP2)->w14;
                l.w18 |= ((E140b_ *)gProfileP2)->w18;
                l.w1C |= ((E140b_ *)gProfileP2)->w1C;
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


INCLUDE_ASM("asm/nonmatchings/unit_53B30", Register_PressKey);

void Register_Leave(void) {
    Menu_StartFadeOut();
    Menu_Nop(D_800F5F7C);
}

void Register_Confirm(void) {
    s32 buf[6];

    if (Password_Apply(&gKeyboardText, gLoadPlayer == 0 ? gProfileP1 : gProfileP2) == 0) {
        if (gLoadPlayer == 0) {
            Password_Get(gProfileP1, buf);
            strcpy((s32 *)&gPasswordP1, (s32)buf);
        } else {
            Password_Get(gProfileP2, buf);
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

INCLUDE_ASM("asm/nonmatchings/unit_53B30", HighScores_PrevCategory);

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

INCLUDE_ASM("asm/nonmatchings/unit_53B30", HighScores_SetupTexts);

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

INCLUDE_ASM("asm/nonmatchings/unit_53B30", Cup_AwardWin);

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

    sysModelLoadAsync(0, ((char **)&gCarModelNames)[i], 1);
    sysCarInfoLoad(((char **)&gCarModelNames)[i], big, buf);
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


INCLUDE_ASM("asm/nonmatchings/unit_53B30", Unlocks_ShowNext);

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

typedef struct { char name[12]; u32 value; } SaveRecord;
typedef struct { SaveRecord e[5]; } SaveRecTable;
/* Save data (gActiveSave): per-track record tables, 12 tracks. */
typedef struct { u8 p0[4]; SaveRecTable highA[12]; SaveRecTable times[12]; SaveRecTable highB[12]; } SaveData;
#define SAVE_DATA ((SaveData *)gActiveSave)
extern char *D_80141230;
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
    Records_InsertHighA(gProfileP1 + 8, rs->res[0].raceTime, slot);
    if (gNumPlayers == 2) {
        Records_InsertHighA(gProfileP2 + 8, rs->res[1].raceTime, slot);
    }
}

void Records_AddLapTimes(RaceSt_ *rs) {
    u8 slot;

    slot = D_80123EBF + D_80123EBE * 3 - 1;
    if (rs->res[0].lap == D_80123EC1) {
        Records_InsertTime(gProfileP1 + 8, rs->res[0].lapTime, slot);
    }
    if (gNumPlayers == 2 && rs->res[1].lap == D_80123EC1) {
        Records_InsertTime(gProfileP2 + 8, rs->res[1].lapTime, slot);
    }
}

INCLUDE_ASM("asm/nonmatchings/unit_53B30", Records_SaveRaceResult);

void Help_Enter(void) {
    s32 buf[8];

    sprintf(buf, &D_80101020, sysRandRange(10));
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
    switch (*(s32 *)(gActiveSave + 0x878)) {
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

INCLUDE_ASM("asm/nonmatchings/unit_53B30", Race_StartDemo);

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
    strcpy(&D_80138D20, gProfileP1 + 8);
    strcpy(&D_801345B8, gProfileP2 + 8);
}

s32 func_800C6CC0(u32 arg0) {
    return (arg0 >> 3) & 0x10;
}

s32 func_800C6CCC(u32 arg0) {
    return (arg0 >> 2) & 0x40;
}

INCLUDE_ASM("asm/nonmatchings/unit_53B30", Race_StartPractice);

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
    ((s32 *)gProfileP1)[15] = 0;
    ((s32 *)gProfileP2)[15] = 0;
}


void Practice_RunRace(void) {
    if (sysGameUpdate() != 0) {
        Fsm_Trigger(&gMenuFsm, 0x11);
    }
}


INCLUDE_ASM("asm/nonmatchings/unit_53B30", Race_StartExhibition);

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
    ((s32 *)gProfileP1)[15] = 0;
    ((s32 *)gProfileP2)[15] = 0;
}


void Exhibition_RunRace(void) {
    if (sysGameUpdate() != 0) {
        Fsm_Trigger(&gMenuFsm, 0x11);
    }
}


INCLUDE_ASM("asm/nonmatchings/unit_53B30", Race_StartAirtime);

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
    ((s32 *)gProfileP1)[15] = 0;
    ((s32 *)gProfileP2)[15] = 0;
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
    Race7A_ *g;

    Menu_FreeBg();
    Race_CopyPlayerNames();
    memset(D_80124D10, 1, 0x29);
    strcpy((s32 *)gRaceSetup.p[0].name, ((s32 *)&gCarModelNames)[((u8 *)gProfileP1)[0x20]]);
    gRaceSetup.p[0].f10 = ((u8 *)gProfileP1)[0x20];
    gRaceSetup.p[0].f0C = 1;
    gRaceSetup.p[0].f11 = 0;
    gRaceSetup.p[0].f12 = Cup_GetPosition(0);
    D_80124D10[((u8 *)gProfileP1)[0x20]] = 0;
    if (((u8 *)gProfileP1)[0x22] == 2) {
        gRaceSetup.cnt = 3;
        gRaceSetup.f9C = 0xFF;
        gRaceSetup.p[0].f12 = 2;
        memcpy(&gRaceSetup.p[1], D_80101044, 9);
        gRaceSetup.p[1].f10 = 0x26;
        gRaceSetup.p[1].f0C = 0;
        gRaceSetup.p[1].f11 = 0;
        gRaceSetup.p[1].f12 = 0;
        memcpy(&gRaceSetup.p[2], D_80101050, 9);
        gRaceSetup.p[2].f10 = 0x27;
        gRaceSetup.p[2].f0C = 0;
        gRaceSetup.p[2].f11 = 0;
        gRaceSetup.p[2].f12 = 1;
    } else {
        gRaceSetup.cnt = 6;
        gRaceSetup.f9C = gActiveSave[0x87E];
        for (i = 1; i < 6; i++) {
            r = (u8)Car_WrapIndex(((u8 *)gProfileP1 + i)[7]);
            do {
                r++;
                if (r >= 0x29) {
                    r = 0;
                }
            } while ((u8)Car_IsUnlocked(r, gProfileP1) == 0 || D_80124D10[r] == 0);
            D_80124D10[r] = 0;
            strcpy((s32 *)gRaceSetup.p[i].name, ((s32 *)&gCarModelNames)[r]);
            gRaceSetup.p[i].f10 = r;
            gRaceSetup.p[i].f0C = 0;
            gRaceSetup.p[i].f11 = 0;
            gRaceSetup.p[i].f12 = Cup_GetPosition(i);
        }
    }
    c = D_800F60D4[((u8 *)gProfileP1)[0x22]][((u8 *)gProfileP1)[0x23]];
    gRaceSetup.trk = c / 3;
    gRaceSetup.sb = c % 3 + 1;
    strcpy((s32 *)gRaceSetup.name, D_800F9FD0[gRaceSetup.trk]);
    gRaceSetup.f19 = 6;
    gRaceSetup.f1A = D_800FA010[gRaceSetup.trk][gRaceSetup.sb - 1];
    gRaceSetup.f20 = 0;
    if (*(s32 *)(gProfileP1 + 0x3C) != 0) {
        gRaceSetup.f1C = *(s32 *)(gProfileP1 + 0x3C);
    } else if (*(s32 *)(gProfileP2 + 0x3C) != 0) {
        gRaceSetup.f1C = *(s32 *)(gProfileP2 + 0x3C);
    } else {
        gRaceSetup.f1C = 0;
    }
    g = &gRaceSetup;
    g->flags = 0x20;
    a = func_800C6CC0(*(s32 *)(gProfileP1 + 0x3C));
    b = func_800C6CC0(*(s32 *)(gProfileP2 + 0x3C));
    d = func_800C6CCC(*(s32 *)(gProfileP1 + 0x3C));
    g->flags |= a | b | d | func_800C6CCC(*(s32 *)(gProfileP2 + 0x3C));
    if (((u8 *)gProfileP1)[0x22] == 1) {
        g->flags |= 0x100;
        sprintf(buf, (s32 *)&D_8010105C, ((u8 *)gProfileP1)[0x23] + 1);
        strcpy((s32 *)g->sub, (s32)buf);
    } else if (((u8 *)gProfileP1)[0x22] == 2) {
        g->flags |= 0x100;
        ((s32 (*)(s32 *, s32 *))sprintf)(buf, (s32 *)&D_80101064);
        strcpy((s32 *)g->sub, (s32)buf);
    }
    if ((u8)sysGameStartRace(&gRaceSetup) == 0) {
        Fsm_Trigger(&gMenuFsm, 0x82);
    }
    Music_StartRaceTrack();
}

INCLUDE_ASM("asm/nonmatchings/unit_53B30", Race_ReturnCup);

void Cup_RunRace(void) {
    if (sysGameUpdate() != 0) {
        Fsm_Trigger(&gMenuFsm, 0x11);
    }
}

