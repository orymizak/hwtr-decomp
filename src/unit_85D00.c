#include "common.h"

void osUnmapTLBAll(void) {
    asm volatile(
        ".set noreorder\n\t"
        "mfc0 $t0, $10\n\t"
        "addiu $t1, $zero, 0x1E\n\t"
        "lui $t2, 0x8000\n\t"
        "mtc0 $t2, $10\n\t"
        "mtc0 $zero, $2\n\t"
        "mtc0 $zero, $3\n"
        "1:\n\t"
        "mtc0 $t1, $0\n\t"
        "nop\n\t"
        "tlbwi\n\t"
        "nop\n\t"
        "nop\n\t"
        "addi $t1, $t1, -1\n\t"
        "bgez $t1, 1b\n\t"
        "nop\n\t"
        "mtc0 $t0, $10\n\t"
        ".set reorder"
        : : : "t0", "t1", "t2"
    );
}
