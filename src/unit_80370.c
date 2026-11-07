#include "common.h"
#include "include_asm.h"



INCLUDE_ASM("asm/nonmatchings/unit_80370", __osTimerServicesInit);

INCLUDE_ASM("asm/nonmatchings/unit_80370", __osTimerInterrupt);

INCLUDE_ASM("asm/nonmatchings/unit_80370", __osSetTimerIntr);

INCLUDE_ASM("asm/nonmatchings/unit_80370", __osInsertTimer);
