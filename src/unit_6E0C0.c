#include "common.h"
#include "include_asm.h"
s32 osCreateThread(void *,s32, void *,s32, void *, s32);
s32 osInitialize();
s32 osStartThread(void *);
    /* TODO (Clean version):
    D_801255A0 and D_80125750 are OSThread instances; D_80127900 and D_80143840 are thread stacks:
    extern OSThread sIdleThread; // D_801255A0
    extern u64 sIdleStack[];     // D_80127900
    extern OSThread sMainThread; // D_80125750
    extern u64 sMainStack[];     // D_80143840
    */
extern u8 D_801255A0;
extern u8 D_80127900;
void func_800CD524(void *arg);



void main(void) {
    osInitialize();
    osCreateThread(&D_801255A0, 1, func_800CD524, 0, &D_80127900, 0xA);
    osStartThread(&D_801255A0);
}

extern void (*D_801455B4)(void);
extern u8 D_80125750;
extern u8 D_80143840;
void nuPiInit(void);
void nuScCreateScheduler(s32, s32);
void osViSetSpecialFeatures(u32);
void osSetThreadPri(void *, s32);
void Game_Init(void);
void func_800CD524(void *arg) {
    D_801455B4 = 0;
    nuPiInit();
    nuScCreateScheduler(2, 1);
    /* TODO (Clean version): 0x5A is OS_VI_DITHER_ON | OS_VI_GAMMA_ON | OS_VI_GAMMA_DITHER_ON | OS_VI_DIVOT_ON */
    osViSetSpecialFeatures(0x5A);
    osCreateThread(&D_80125750, 3, Game_Init, 0, &D_80143840, 10);
    osStartThread(&D_80125750);
    osSetThreadPri(&D_801255A0, 0);
    while (1) {
        /* TODO (Bug potential): 'D_801455B4' is polled in a tight loop without volatile qualification, OS event wait, or yield. */
        if (D_801455B4 != 0) {
            D_801455B4();
        }
    }
}
