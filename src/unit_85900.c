#include "common.h"
#include "PR/os_internal.h"
#include "PR/rcp.h"
#include "PR/sptask.h"
#include "PRinternal/osint.h"

u32 __osSetFpcCsr(u32 val) {
    register u32 prev asm("$2");
    asm volatile("cfc1 %0, $31\n\tctc1 %1, $31" : "=&r"(prev) : "r"(val));
    asm("nop");
    return prev;
}

void __osSetSR(u32 val) {
    asm("mtc0 %0, $12\n\tnop" : : "r"(val));
    asm("nop");
}

void __osSetWatchLo(u32 val) {
    asm("mtc0 %0, $18\n\tnop" : : "r"(val));
    asm("nop");
}

int __osSpDeviceBusy(void) {
    register u32 stat = IO_READ(SP_STATUS_REG);

    if (stat & (SP_STATUS_DMA_BUSY | SP_STATUS_DMA_FULL | SP_STATUS_IO_FULL)) {
        return TRUE;
    } else {
        return FALSE;
    }
}
