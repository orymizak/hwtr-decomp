#include "common.h"
#include "include_asm.h"
s32 osCreateThread(void *,s32, void *,s32, void *, s32);
s32 osInitialize();
s32 osStartThread(void *);
    /* TODO (Clean version):
    gIdleThread and gMainThread are OSThread instances; D_80127900 and D_80143840 are thread stacks:
    extern OSThread sIdleThread; // gIdleThread
    extern u64 sIdleStack[];     // D_80127900
    extern OSThread sMainThread; // gMainThread
    extern u64 sMainStack[];     // D_80143840
    */
extern u8 gIdleThread;
extern u8 D_80127900;
void Boot_IdleThread(void *arg);



void main(void) {
    osInitialize();
    osCreateThread(&gIdleThread, 1, Boot_IdleThread, 0, &D_80127900, 0xA);
    osStartThread(&gIdleThread);
}

extern void (*gIdleCallback)(void);
extern u8 gMainThread;
extern u8 D_80143840;
void nuPiInit(void);
void nuScCreateScheduler(s32, s32);
void osViSetSpecialFeatures(u32);
void osSetThreadPri(void *, s32);
void Game_Init(void);
void Boot_IdleThread(void *arg) {
    gIdleCallback = 0;
    nuPiInit();
    nuScCreateScheduler(2, 1);
    /* TODO (Clean version): 0x5A is OS_VI_DITHER_ON | OS_VI_GAMMA_ON | OS_VI_GAMMA_DITHER_ON | OS_VI_DIVOT_ON */
    osViSetSpecialFeatures(0x5A);
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
