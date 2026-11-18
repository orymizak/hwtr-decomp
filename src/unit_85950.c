#include "common.h"
#include "include_asm.h"
#include "PR/os_internal.h"
#include "PR/rcp.h"
#include "PRinternal/osint.h"
#include "assert.h"

u32 __osSpGetStatus(void) {
    return IO_READ(SP_STATUS_REG);
}

void __osSpSetStatus(u32 data) {
    IO_WRITE(SP_STATUS_REG, data);
}

s32 __osSpSetPc(u32 pc) {
    register u32 status = IO_READ(SP_STATUS_REG);

    if (!(status & SP_STATUS_HALT)) {
        return -1;
    }
    IO_WRITE(SP_PC_REG, pc);

    return 0;
}

s32 __osSpRawStartDma(s32 direction, u32 devAddr, void* dramAddr, u32 size) {
    assert(((u32)devAddr & 0x7) == 0);
    assert(((u32)dramAddr & 0x7) == 0);
    assert(((u32)size & 0x7) == 0);

    if (__osSpDeviceBusy()) {
        return -1;
    }

    IO_WRITE(SP_MEM_ADDR_REG, devAddr);
    IO_WRITE(SP_DRAM_ADDR_REG, osVirtualToPhysical(dramAddr));

    if (direction == OS_READ) {
        IO_WRITE(SP_WR_LEN_REG, size - 1);
    } else {
        IO_WRITE(SP_RD_LEN_REG, size - 1);
    }

    return 0;
}
