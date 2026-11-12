#include "common.h"
#include "include_asm.h"
s32 osCreateThread(void *,s32, void *,s32, void *, s32);
s32 osInitialize();
s32 osStartThread(void *);
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
void func_800D6200(void);
void func_800D5560(s32, s32);
void osViSetSpecialFeatures(u32);
void osSetThreadPri(void *, s32);
void Game_Init(void);
void func_800CD524(void *arg) {
    D_801455B4 = 0;
    func_800D6200();
    func_800D5560(2, 1);
    osViSetSpecialFeatures(0x5A);
    osCreateThread(&D_80125750, 3, Game_Init, 0, &D_80143840, 10);
    osStartThread(&D_80125750);
    osSetThreadPri(&D_801255A0, 0);
    while (1) {
        if (D_801455B4 != 0) {
            D_801455B4();
        }
    }
}
