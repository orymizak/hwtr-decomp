#include "common.h"
#include "include_asm.h"
s32 __osSiDeviceBusy(void);



s32 __osSiRawWriteIo(u32 devAddr, u32 data) {
    if (__osSiDeviceBusy()) {
        return -1;
    }
    *(volatile u32 *)(devAddr | 0xA0000000) = data;
    return 0;
}
