#include "common.h"
#include "include_asm.h"
s32 Fsm_Update(void *);

s32 Fsm_Reset(s32 *);
extern s32 gPauseFsm;
extern u8 gPauseQuit;
extern u8 gPauseRestart;
extern u8 gPauseUnusedFlag;

void Pause_Init(void) {
    gPauseQuit = 0;
    gPauseRestart = 0;
    gPauseUnusedFlag = 0;
    Fsm_Reset(&gPauseFsm);
}

void Pause_End(void) {

}

void Pause_RequestQuit(void) {
    gPauseQuit = 1;
}

void Pause_RequestRestart(void) {
    gPauseRestart = 1;
}

    /* TODO (Bug check): gPauseUnusedFlag is set here and cleared in Pause_Init, but is never checked in Pause_Update or anywhere else in this unit. Check if there was a missing state condition (e.g. returning 4). */
void Pause_SetUnusedFlag(void) {
    gPauseUnusedFlag = 1;
}

s32 Pause_Update(void) {
    if (Fsm_Update(&gPauseFsm) == 0) {
        return 0;
    }
    if (gPauseQuit != 0) {
        return 3;
    }
    if (gPauseRestart != 0) {
        return 2;
    }
    return 1;
}

