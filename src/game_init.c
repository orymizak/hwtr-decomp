#include "common.h"
#include "ultra64.h"

#define FRAMEBUFFER ((s32 *)0x80300000)
#define FRAMEBUFFER_WORDS 0x25800

#define OS_TV_PAL 0
#define OS_TV_NTSC 1
#define OS_TV_MPAL 2

#define VI_MODE_SIZE 0x50

typedef struct {
    /* 0x00 */ u8 data[VI_MODE_SIZE];
} ViMode;

extern s32 osTvType; /* osTvType */
extern ViMode D_800FB920[];
extern s32 gViModeIndex;
extern s32 D_800F2190;

s32 FileTable_Init(void);
void Memory_InitMain(void);
void Audio_InitDriver(void);
void Audio_Init(void);
void World_Init(void);
void Input_Init(void);
void Input_RumbleStopAll(void);
void Explosions_Init(void);
void EnvMap_Init(void);
void Font_Init(void);
void GfxPools_Init(void);
void Screen_InitGfx(void);
void Screen_Init(void);
void World_InitSlots(void);
void Rand_Seed(void);
void Game_InitMenus(void);
void Game_MenuFrame(void);
void nuGfxFuncSet(void (*callback)(s32));
void nuGfxPreNMIFuncSet(void (*callback)(void));
void nuGfxDisplayOff(void);
void nuGfxDisplayOn(void);

void Gfx_FrameCallback(s32 arg0);
void Game_Shutdown(void);

inline void Video_Init(void) {
    u32 i;

    for (i = 0; i < FRAMEBUFFER_WORDS; i++) {
        FRAMEBUFFER[i] = 0;
    }
    osViGetCurrentFramebuffer();
    osViSwapBuffer(FRAMEBUFFER);
    /* TODO (Clean version): [evaluated 2026-11-18: memset changes the ROM (the loop stores words
    inline); the double swap is in the ROM too]
    Manually looping through the raw framebuffer array using a macro pointer 
    can be replaced with `memset(FRAMEBUFFER, 0, FRAMEBUFFER_WORDS * sizeof(s32))`.
    Additionally, calling `osViGetCurrentFramebuffer()` and swapping buffers twice 
    is a boilerplate N64 initialization sequence to flush the VI pipeline, which 
    could be wrapped in a descriptive helper function like `Video_ClearAndFlushBuffers()`.
    */
    osViSwapBuffer(FRAMEBUFFER);
    osViGetCurrentMode();
    osViSetMode(&D_800FB920[gViModeIndex]);
    osViBlack(1);
}

void Game_Init(void) {
    Audio_InitDriver();
    switch (osTvType) {
        case OS_TV_NTSC:
            gViModeIndex = OS_VI_NTSC_LPN1;
            break;
        case OS_TV_PAL:
            gViModeIndex = OS_VI_PAL_LPN1;
            break;
        case OS_TV_MPAL:
            gViModeIndex = OS_VI_MPAL_LPN1;
            break;
    }
    Video_Init();
    Input_Init();
    Screen_InitGfx();
    Rand_Seed();
    Memory_InitMain();
    GfxPools_Init();
    FileTable_Init();
    Audio_Init();
    World_Init();
    Font_Init();
    World_InitSlots();
    Screen_Init();
    nuGfxPreNMIFuncSet(Game_Shutdown);
    Game_InitMenus();
    Explosions_Init();
    EnvMap_Init();
    nuGfxFuncSet(Gfx_FrameCallback);
    nuGfxDisplayOn();
}

void Gfx_FrameCallback(s32 arg0) {
    if (arg0 < D_800F2190) {
        Game_MenuFrame();
    }
}

void Game_Shutdown(void) {
    Input_RumbleStopAll();
    nuGfxDisplayOff();
}
