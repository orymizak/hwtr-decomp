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
void func_80060C30(void);
void Audio_Init(void);
void World_Init(void);
void func_8006B2A0(void);
void func_8006C468(void);
void func_8006DA88(void);
void func_8006DC90(void);
void Font_Init(void);
void GfxPools_Init(void);
void func_80077FB0(void);
void func_80077FD0(void);
void func_8007BE30(void);
void func_8007F420(void);
void func_80087720(void);
void func_8008777C(void);
void nuGfxFuncSet(void (*callback)(s32));
void nuGfxPreNMIFuncSet(void (*callback)(void));
void nuGfxDisplayOff(void);
void nuGfxDisplayOn(void);

void func_80078BA4(s32 arg0);
void func_80078BD0(void);

inline void Video_Init(void) {
    u32 i;

    for (i = 0; i < FRAMEBUFFER_WORDS; i++) {
        FRAMEBUFFER[i] = 0;
    }
    osViGetCurrentFramebuffer();
    osViSwapBuffer(FRAMEBUFFER);
    /* TODO (Clean version):
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
    func_80060C30();
    switch (osTvType) {
        case OS_TV_NTSC:
            gViModeIndex = 0;
            break;
        case OS_TV_PAL:
            gViModeIndex = 14;
            break;
        case OS_TV_MPAL:
            gViModeIndex = 28;
            break;
    }
    /* TODO (Clean version):
    Magic offsets like `14` and `28` used to index VI modes based on TV type 
    should be defined using an enum or offset constants:
    #define VI_MODE_NTSC_OFFSET 0
    #define VI_MODE_PAL_OFFSET  14
    #define VI_MODE_MPAL_OFFSET 28
    */
    Video_Init();
    func_8006B2A0();
    func_80077FB0();
    func_8007F420();
    Memory_InitMain();
    GfxPools_Init();
    FileTable_Init();
    Audio_Init();
    World_Init();
    Font_Init();
    func_8007BE30();
    func_80077FD0();
    nuGfxPreNMIFuncSet(func_80078BD0);
    func_80087720();
    func_8006DA88();
    func_8006DC90();
    nuGfxFuncSet(func_80078BA4);
    nuGfxDisplayOn();
}

void func_80078BA4(s32 arg0) {
    if (arg0 < D_800F2190) {
        func_8008777C();
    }
}

void func_80078BD0(void) {
    func_8006C468();
    nuGfxDisplayOff();
}
