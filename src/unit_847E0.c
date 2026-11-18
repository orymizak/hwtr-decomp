#include "common.h"
#include "include_asm.h"
#include "PRinternal/piint.h"
#include "PR/ultraerror.h"

// TODO: this comes from a header
#ident "$Revision: 1.17 $"
#include "PR/os_internal.h"
#include "PRinternal/piint.h"
#include "PRinternal/piint.h"
#include "PR/ultraerror.h"
#include "assert.h"




















// Adjust line numbers to match assert

// TODO: this comes from a header
#ident "$Revision: 1.17 $"


s32 __osPiRawStartDma(s32 direction, u32 devAddr, void* dramAddr, u32 size) {
    register u32 stat;


    WAIT_ON_IOBUSY(stat);

    IO_WRITE(PI_DRAM_ADDR_REG, osVirtualToPhysical(dramAddr));
    IO_WRITE(PI_CART_ADDR_REG, K1_TO_PHYS((u32)osRomBase | devAddr));

    switch (direction) {
        case OS_READ:
            IO_WRITE(PI_WR_LEN_REG, size - 1);
            break;
        case OS_WRITE:
            IO_WRITE(PI_RD_LEN_REG, size - 1);
            break;
        default:
            return -1;
    }
    return 0;
}


OSMesgQueue* osPiGetCmdQueue(void) {
    if (!__osPiDevMgr.active) {
        return NULL;
    } else {
        return __osPiDevMgr.cmdQueue;
    }
}


s32 __osEPiRawReadIo(OSPiHandle* pihandle, u32 devAddr, u32* data) {
    register u32 stat;
    register u32 domain;

    assert(data != NULL);

    EPI_SYNC(pihandle, stat, domain);
    *data = IO_READ(pihandle->baseAddress | devAddr);

    return 0;
}
