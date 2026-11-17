#include "common.h"
#include "include_asm.h"
typedef struct { u32 type; u32 flags; } OSTaskHdr_;
s32 __osSpGetStatus(void);



u32 osSpTaskYielded(OSTaskHdr_ *tp) {
    u32 status;
    u32 result;

    status = __osSpGetStatus();
    result = (status & 0x100) ? 1 : 0;
    if (status & 0x80) {
        tp->flags |= result;
        tp->flags &= ~2;
    }
    return result;
}
