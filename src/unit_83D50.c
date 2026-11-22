#include "common.h"
#include "ultra64.h"
#include "include_asm.h"



INCLUDE_ASM("asm/nonmatchings/unit_83D50", __osException);

INCLUDE_ASM("asm/nonmatchings/unit_83D50", send_mesg);

INCLUDE_ASM("asm/nonmatchings/unit_83D50", __osEnqueueAndYield);

void __osEnqueueThread(OSThread **queue, OSThread *thread) {
    asm volatile(
        ".set noreorder\n\t"
        ".set noat\n\t"
        "addu $t9, %0, $zero\n\t"
        "lw $t8, 0(%0)\n\t"
        "lw $t7, 4(%1)\n\t"
        "lw $t6, 4($t8)\n\t"
        "slt $at, $t6, $t7\n\t"
        "bnez $at, 1f\n\t"
        "nop\n"
        "2:\n\t"
        "addu $t9, $t8, $zero\n\t"
        "lw $t8, 0($t8)\n\t"
        "lw $t6, 4($t8)\n\t"
        "slt $at, $t6, $t7\n\t"
        "beqz $at, 2b\n\t"
        "nop\n"
        "1:\n\t"
        "lw $t8, 0($t9)\n\t"
        "sw $t8, 0(%1)\n\t"
        "sw %1, 0($t9)\n\t"
        ".set at\n\t"
        ".set reorder"
        : : "r"(queue), "r"(thread)
        : "t6", "t7", "t8", "t9"
    );
    thread->queue = queue;
}

OSThread *__osPopThread(OSThread **threadQueue) {
    register OSThread *t asm("$2");
    register OSThread *next asm("$25");

    t = *threadQueue;
    next = t->next;
    *threadQueue = next;
    return t;
}

INCLUDE_ASM("asm/nonmatchings/unit_83D50", __osDispatchThread);
