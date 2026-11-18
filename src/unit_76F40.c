#include "common.h"
#include "ultra64.h"
#include "include_asm.h"
extern u8 nuContWaitMesgQ;
s32 nuSiSendMesg(s32, s32);



void nuContDataRead(s32 arg0) {
    nuSiSendMesg(0x101, arg0);
}

void nuContDataReadWait(void) {
    osRecvMesg(&nuContWaitMesgQ, 0, 1);
}
