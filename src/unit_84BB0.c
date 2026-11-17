#include "common.h"
#include "include_asm.h"



s32 __osAiDeviceBusy(void) {
    register s32 status = *(volatile s32 *)0xA450000C;

    if (status & 0x80000000) {
        return 1;
    } else {
        return 0;
    }
}

    /* TODO validar si este código es equivalente:
    s32 __osAiDeviceBusy(void) {
        int zero = 0;
        // Nota: Se compara contra 'zero' para forzar la instrucción MIPS de signo original (matching)
        return (*((volatile s32 *)AI_STATUS_REG)) < zero;
    }
    */