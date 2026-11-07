#include "common.h"
#include "include_asm.h"
s32 __osSiDeviceBusy(void);



s32 __osSiRawReadIo(u32 devAddr, u32 *data) {
    if (__osSiDeviceBusy()) {
        return -1;
    }
    *data = *(volatile u32 *)(devAddr | 0xA0000000);
    return 0;
}
