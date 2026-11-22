#include "common.h"

extern u32 __OSGlobalIntMask;
extern void *__osRunningThread;

u32 __osDisableInt(void) {
    register u32 ret asm("$2");
    asm volatile(
        ".set noreorder\n\t"
        ".set noat\n\t"
        "lui $t2, %%hi(__OSGlobalIntMask)\n\t"
        "addiu $t2, $t2, %%lo(__OSGlobalIntMask)\n\t"
        "lw $t3, 0($t2)\n\t"
        "andi $t3, $t3, 0xFF00\n\t"
        "mfc0 $t0, $12\n\t"
        "addiu $at, $zero, -2\n\t"
        "and $t1, $t0, $at\n\t"
        "mtc0 $t1, $12\n\t"
        "andi $2, $t0, 1\n\t"
        "lw $t0, 0($t2)\n\t"
        "andi $t0, $t0, 0xFF00\n\t"
        "beq $t0, $t3, 1f\n\t"
        "lui $t2, %%hi(__osRunningThread)\n\t"
        "addiu $t2, $t2, %%lo(__osRunningThread)\n\t"
        "lw $t1, 0x118($t2)\n\t"
        "andi $t2, $t1, 0xFF00\n\t"
        "and $t2, $t2, $t0\n\t"
        "lui $at, 0xFFFF\n\t"
        "ori $at, $at, 0x00FF\n\t"
        "and $t1, $t1, $at\n\t"
        "or $t1, $t1, $t2\n\t"
        "addiu $at, $zero, -2\n\t"
        "and $t1, $t1, $at\n\t"
        "mtc0 $t1, $12\n\t"
        "nop\n\t"
        "nop\n"
        "1:\n\t"
        ".set at\n\t"
        ".set reorder"
        : "=r"(ret)
        :
        : "t0", "t1", "t2", "t3"
    );
    return ret;
}

void __osRestoreInt(u32 val) {
    asm volatile(
        ".set noreorder\n\t"
        "mfc0 $t0, $12\n\t"
        "or $t0, $t0, %0\n\t"
        "mtc0 $t0, $12\n\t"
        "nop\n\t"
        "nop\n\t"
        ".set reorder"
        :
        : "r"(val)
        : "t0"
    );
}
