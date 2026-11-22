#include "common.h"

void osMapTLBRdb(void) {
    asm volatile(
        ".set noreorder\n\t"
        "mfc0 $t0, $10\n\t"
        "addiu $t1, $zero, 0x1F\n\t"
        "mtc0 $t1, $0\n\t"
        "mtc0 $zero, $5\n\t"
        "addiu $t2, $zero, 0x17\n\t"
        "lui $t1, 0xC000\n\t"
        "mtc0 $t1, $10\n\t"
        "lui $t1, 0x8000\n\t"
        "srl $t3, $t1, 6\n\t"
        "or $t3, $t3, $t2\n\t"
        "mtc0 $t3, $2\n\t"
        "addiu $t1, $zero, 1\n\t"
        "mtc0 $t1, $3\n\t"
        "nop\n\t"
        "tlbwi\n\t"
        "nop\n\t"
        "nop\n\t"
        "nop\n\t"
        "nop\n\t"
        "mtc0 $t0, $10\n\t"
        ".set reorder"
        : : : "t0", "t1", "t2", "t3"
    );
}
