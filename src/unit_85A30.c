#include "common.h"
#include "include_asm.h"
#include "PR/os_internal.h"
#include "assert.h"
#include "PRinternal/siint.h"

s32 __osSiRawReadIo(u32 devAddr, u32* data) {
    assert((devAddr & 0x3) == 0);
    assert(data != NULL);
    
    if (__osSiDeviceBusy()) {
        return -1;
    }

    *data = IO_READ(devAddr);
    return 0;
}
