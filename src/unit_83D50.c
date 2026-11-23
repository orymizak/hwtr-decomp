#include "common.h"
#include "include_asm.h"



INCLUDE_ASM("asm/nonmatchings/unit_83D50", __osException);

INCLUDE_ASM("asm/nonmatchings/unit_83D50", send_mesg);

INCLUDE_ASM("asm/nonmatchings/unit_83D50", __osEnqueueAndYield);

INCLUDE_ASM("asm/nonmatchings/unit_83D50", __osEnqueueThread);

#include "ultra64.h"

/* TODO(match-debt): the original is handwritten assembly (exceptasm.s); the register pins
 * reproduce its v0/t9 choice, which GCC would not pick on its own. */
OSThread *__osPopThread(OSThread **threadQueue) {
    register OSThread *t asm("$2");
    register OSThread *next asm("$25");

    t = *threadQueue;
    next = t->next;
    *threadQueue = next;
    return t;
}

INCLUDE_ASM("asm/nonmatchings/unit_83D50", __osDispatchThread);
