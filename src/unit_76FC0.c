#include "common.h"
#include "ultra64.h"
#include "include_asm.h"
s32 nuContDataLock();
s32 nuContDataUnLock();
extern u8 nuContData;



void nuContDataGet(s32 arg0, s32 arg1) {
    nuContDataLock();
    bcopy((arg1 * 6) + &nuContData, arg0, 6);
    nuContDataUnLock();
}
