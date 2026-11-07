#include "common.h"
#include "include_asm.h"
typedef struct { u16 type; u8 pri; u8 status; void *retQueue; void *dramAddr; u32 devAddr; u32 size; void *piHandle; } IoMesgE_;
extern s32 D_800FB2A0;
s32 osJamMesg(void *, void *, s32);
s32 osSendMesg(void *, void *, s32);
void *osPiGetCmdQueue(void);



s32 osEPiStartDma(void *pihandle, IoMesgE_ *mb, s32 direction) {
    register s32 ret;

    if (!D_800FB2A0) {
        return -1;
    }
    mb->piHandle = pihandle;
    if (direction == 0) {
        mb->type = 15;
    } else {
        mb->type = 16;
    }
    if (mb->pri == 1) {
        ret = osJamMesg(osPiGetCmdQueue(), mb, 0);
    } else {
        ret = osSendMesg(osPiGetCmdQueue(), mb, 0);
    }
    return ret;
}
