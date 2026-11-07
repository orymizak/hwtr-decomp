#include "common.h"
#include "include_asm.h"
typedef struct { u16 type; u8 pri; u8 status; void *retQueue; void *dramAddr; u32 devAddr; u32 size; void *piHandle; } IoMesg_;
s32 osJamMesg(void *, void *, s32);
void *osPiGetCmdQueue(void);
s32 osSendMesg(void *, void *, s32);
extern s32 D_800FB2A0;



s32 osPiStartDma(IoMesg_ *mb, s32 priority, s32 direction, u32 devAddr, void *dramAddr, u32 size, void *mq) {
    s32 ret;

    if (D_800FB2A0 == 0) {
        return -1;
    }
    if (direction == 0) {
        mb->type = 0xB;
    } else {
        mb->type = 0xC;
    }
    mb->pri = priority;
    mb->retQueue = mq;
    mb->dramAddr = dramAddr;
    mb->devAddr = devAddr;
    mb->size = size;
    mb->piHandle = NULL;
    if (priority == 1) {
        ret = osJamMesg(osPiGetCmdQueue(), mb, 0);
    } else {
        ret = osSendMesg(osPiGetCmdQueue(), mb, 0);
    }
    return ret;
}
