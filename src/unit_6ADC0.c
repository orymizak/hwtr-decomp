#include "common.h"
#include "include_asm.h"
s32 Fsm_Update(void *);

s32 Fsm_Reset(s32 *);
extern s32 gPauseFsm;
extern s8 gPauseQuit;
extern s8 gPauseRestart;
extern s8 gPauseUnusedFlag;

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
    /* TODO (Clean version): Declare gPauseQuit..62 as u8 (or a state flags struct) to eliminate pointer-cast dereferences:
       if (gPauseQuit) {
           return 3;
       }
       if (gPauseRestart) {
           return 2;
       }
    */
    if (*(u8 *)&gPauseQuit != 0) {
        return 3;
    }
    if (*(u8 *)&gPauseRestart != 0) {
        return 2;
    }
    return 1;
}

