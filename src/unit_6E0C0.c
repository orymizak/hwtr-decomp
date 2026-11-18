#include "common.h"
#include "ultra64.h"
/* This libultra predates the osInitialize() macro of newer SDK headers. */
#undef osInitialize
void osInitialize(void);
#include "include_asm.h"
extern OSThread gIdleThread;
extern u8 D_80127900; /* top of the idle thread stack */
void Boot_IdleThread(void *arg);



void main(void) {
    osInitialize();
    osCreateThread(&gIdleThread, 1, Boot_IdleThread, 0, &D_80127900, 0xA);
    osStartThread(&gIdleThread);
}

extern void (*gIdleCallback)(void);
extern OSThread gMainThread;
extern u8 D_80143840; /* top of the main thread stack */
void nuPiInit(void);
void nuScCreateScheduler(s32, s32);
void Game_Init(void);
void Boot_IdleThread(void *arg) {
    gIdleCallback = 0;
    nuPiInit();
    nuScCreateScheduler(2, 1);
    osViSetSpecialFeatures(OS_VI_GAMMA_OFF | OS_VI_GAMMA_DITHER_OFF | OS_VI_DIVOT_ON | OS_VI_DITHER_FILTER_ON);
    osCreateThread(&gMainThread, 3, Game_Init, 0, &D_80143840, 10);
    osStartThread(&gMainThread);
    osSetThreadPri(&gIdleThread, 0);
    while (1) {
        /* TODO (Bug potential): 'gIdleCallback' is polled in a tight loop without volatile qualification, OS event wait, or yield. */
        if (gIdleCallback != 0) {
            gIdleCallback();
        }
    }
}
