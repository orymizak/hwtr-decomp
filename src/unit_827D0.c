#include "common.h"
#include "include_asm.h"
typedef struct { s32 status; } OSPfs_;
s32 __osGetId(OSPfs_ *pfs);



s32 osPfsRepairId(OSPfs_ *pfs) {
    s32 ret = 0;

    if ((pfs->status & (1 | 4))) {
        ret = __osGetId(pfs);
        if (ret == 0) {
            pfs->status &= ~4;
        }
    } else {
        ret = 5;
    }
    return ret;
}
