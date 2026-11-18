#include "common.h"
#include "ultra64.h"

extern void *sysBootCheck;
extern void *sysMenuExit;
extern void *sysGameStartRace;
extern void *sysGameEndRace;
extern void *sysGameUpdate;
extern void *sysSfxPlay;
extern void *D_80140FB0;
extern void *sysModelFree;
extern void *sysCarInfoLoad;
extern void *sysModelLoadAsync;
extern void *sysModelPollLoad;
extern void *sysModelAllocBuffers;
extern void *sysModelFreeBuffers;
extern void *sysModelDrawWithLookAt;
extern void *sysModelRotateAllParts;
extern void *sysFileLoad;
extern void *sysFileGetSize;
extern void *sysPakOpen;
extern void *sysPakGetState;
extern void *sysSaveRead;
extern void *sysSaveWrite;
extern void *sysPakRepair;
extern void *sysFontLoad;
extern void *sysFontFree;
extern void *sysFontGetCharWidth;
extern void *sysFontDrawChar;
extern void *sysDraw2DSetColor;
extern void *sysScreenSetClearColor;
extern void *sysScreenSetBgAlpha;
extern void *sysScreenLoadBg;
extern void *sysScreenBeginFrame;
extern void *sysScreenEndFrame;
extern void *sysCameraApply;
extern void *sysCameraSetPerspective;
extern void *sysWorldLoad;
extern void *sysWorldFree;
extern void *sysWorldDraw;
extern void *sysTimeGetMillis;
extern void *sysRandRange;
extern void *sysAudioGetMusicVolume;
extern void *sysAudioSetMusicVolume;
extern void *sysAudioGetSfxVolume;
extern void *sysAudioSetSfxVolume;
extern void *sysAudioGetVoiceVolume;
extern void *sysAudioSetVoiceVolume;
extern void *sysAudioSetAmbientVolume;
extern void *sysHookA;
extern void *sysHookB;
extern void *sysMenuExitHook;
extern void *sysIsPAL;
extern void *sysScreenAllocBg;
extern void *sysScreenFreeBg;
extern s32 osTvType; /* osTvType */

void Model_LoadAsync();
void Model_PollLoad();
void Model_AllocBuffers();
void Model_FreeBuffers();
void Model_Free();
void Model_DrawWithLookAt();
void Model_RotateAllParts();
void CarInfo_Load();
void File_Load();
void File_GetSize();
void Pak_Open();
void Pak_GetState();
void Save_Read();
void Save_Write();
void Pak_Repair();
void Font_Load();
void Font_Free();
void Font_GetCharWidth();
void Font_DrawChar2();
void Draw2D_SetColor();
void Screen_SetClearColor();
void Screen_SetBgAlpha();
void Screen_LoadBg();
void Screen_BeginFrameDefault();
void Camera_SetPerspective();
void Camera_Apply();
void Screen_AllocBg();
void Screen_FreeBg();
void World_Load();
void World_Free();
void World_DrawSlot();
void Time_GetMillis();
void Rand_Range();
void Game_StartRace();
void Game_EndRace();
void Game_Update();
void Audio_GetMusicVolume();
void Audio_SetMusicVolume();
void Audio_GetSfxVolume();
void Audio_SetSfxVolume();
void Audio_GetVoiceVolume();
void Audio_SetVoiceVolume();
void Sfx_Play();
void Audio_SetAmbientVolume();
u8 Screen_EndFrame(void);

s32 Sys_BootCheck(void);
s32 Sys_MenuExit(void);
s32 Sys_ScreenEndFrame(void);
s32 Sys_HookA_Nop(void);
s32 Sys_HookB_Nop(void);
s32 Sys_MenuExitHookNop(void);
s32 System_IsPAL(void);

s32 Sys_StubTrue(void) {
    return 1;
}

    /* TODO (Clean version): [evaluated 2026-11-18: the sys* pointers are separate globals in
    .bss, assigned one by one in the ROM; a struct would change addresses and code]
    Populating a massive list of global `void*` variables with raw function addresses 
    bypasses type safety. A clean implementation should use a properly typed 
    plugin interface struct or a static dispatch table with explicit function signatures:
    typedef struct {
        void (*initAudio)(void);
        void (*updateInput)(void);
        // ...
    } SystemInterface;
    */
s32 Sys_InitTable(void) {
    sysModelLoadAsync = &Model_LoadAsync;
    sysModelPollLoad = &Model_PollLoad;
    sysModelAllocBuffers = &Model_AllocBuffers;
    sysModelFreeBuffers = &Model_FreeBuffers;
    sysModelFree = &Model_Free;
    sysModelDrawWithLookAt = &Model_DrawWithLookAt;
    sysModelRotateAllParts = &Model_RotateAllParts;
    sysCarInfoLoad = &CarInfo_Load;
    sysFileLoad = &File_Load;
    sysFileGetSize = &File_GetSize;
    sysPakOpen = &Pak_Open;
    sysPakGetState = &Pak_GetState;
    sysSaveRead = &Save_Read;
    sysSaveWrite = &Save_Write;
    sysPakRepair = &Pak_Repair;
    sysFontLoad = &Font_Load;
    sysFontFree = &Font_Free;
    sysFontGetCharWidth = &Font_GetCharWidth;
    sysFontDrawChar = &Font_DrawChar2;
    sysDraw2DSetColor = &Draw2D_SetColor;
    sysScreenSetClearColor = &Screen_SetClearColor;
    sysScreenSetBgAlpha = &Screen_SetBgAlpha;
    sysScreenLoadBg = &Screen_LoadBg;
    sysScreenBeginFrame = &Screen_BeginFrameDefault;
    sysScreenEndFrame = &Sys_ScreenEndFrame;
    sysCameraSetPerspective = &Camera_SetPerspective;
    sysCameraApply = &Camera_Apply;
    sysIsPAL = &System_IsPAL;
    sysScreenAllocBg = &Screen_AllocBg;
    sysScreenFreeBg = &Screen_FreeBg;
    sysWorldLoad = &World_Load;
    sysWorldFree = &World_Free;
    sysWorldDraw = &World_DrawSlot;
    sysTimeGetMillis = &Time_GetMillis;
    sysRandRange = &Rand_Range;
    sysBootCheck = &Sys_BootCheck;
    sysMenuExit = &Sys_MenuExit;
    sysGameStartRace = &Game_StartRace;
    sysGameEndRace = &Game_EndRace;
    sysGameUpdate = &Game_Update;
    sysAudioGetMusicVolume = &Audio_GetMusicVolume;
    sysAudioSetMusicVolume = &Audio_SetMusicVolume;
    sysAudioGetSfxVolume = &Audio_GetSfxVolume;
    sysAudioSetSfxVolume = &Audio_SetSfxVolume;
    sysAudioGetVoiceVolume = &Audio_GetVoiceVolume;
    sysAudioSetVoiceVolume = &Audio_SetVoiceVolume;
    sysHookA = &Sys_HookA_Nop;
    sysHookB = &Sys_HookB_Nop;
    sysMenuExitHook = &Sys_MenuExitHookNop;
    sysSfxPlay = &Sfx_Play;
    sysAudioSetAmbientVolume = &Audio_SetAmbientVolume;
    return 1;
}

    /* TODO (Clean version): [evaluated 2026-11-18: same as Sys_InitTable; a memset changes the ROM]
    Manually zeroing out dozens of individual global function pointers during teardown 
    is error-prone and verbose. This should be handled by resetting a registered 
    subsystem interface instance (e.g., `memset(&sysInterface, 0, sizeof(sysInterface))`).
    */
s32 Sys_ClearTable(void) {
    D_80140FB0 = 0;
    sysModelFree = 0;
    sysModelDrawWithLookAt = 0;
    sysFileLoad = 0;
    sysFileGetSize = 0;
    sysIsPAL = 0;
    sysScreenSetClearColor = 0;
    sysScreenLoadBg = 0;
    sysScreenBeginFrame = 0;
    sysScreenEndFrame = 0;
    sysCameraApply = 0;
    sysCameraSetPerspective = 0;
    sysWorldLoad = 0;
    sysWorldFree = 0;
    sysWorldDraw = 0;
    sysTimeGetMillis = 0;
    sysRandRange = 0;
    sysGameStartRace = 0;
    sysGameEndRace = 0;
    sysGameUpdate = 0;
    sysHookA = 0;
    sysHookB = 0;
    sysMenuExitHook = 0;
    sysAudioSetAmbientVolume = 0;
    return 1;
}

s32 Sys_BootCheck(void) {
    return 1;
}

s32 Sys_MenuExit(void) {
    return 1;
}

s32 Sys_ScreenEndFrame(void) {
    return Screen_EndFrame();
}

s32 Sys_HookA_Nop(void) {
    return 1;
}

s32 Sys_HookB_Nop(void) {
    return 1;
}

s32 Sys_MenuExitHookNop(void) {
    return 1;
}

s32 System_IsPAL(void) {
    return osTvType == OS_TV_PAL;
}
