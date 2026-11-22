#include "common.h"
#include "include_asm.h"



void bzero(void *s, u32 n) {
    asm volatile(
        ".set noreorder\n\t"
        ".set noat\n\t"
        "negu $v1, $a0\n\t"
        "slti $at, $a1, 12\n\t"
        "bnez $at, 3f\n\t"
        "nop\n\t"
        "andi $v1, $v1, 3\n\t"
        "beqz $v1, 1f\n\t"
        "subu $a1, $a1, $v1\n\t"
        "swl $zero, 0($a0)\n\t"
        "addu $a0, $a0, $v1\n"
        "1:\n\t"
        "addiu $at, $zero, -32\n\t"
        "and $a3, $a1, $at\n\t"
        "beqz $a3, 2f\n\t"
        "subu $a1, $a1, $a3\n\t"
        "addu $a3, $a3, $a0\n"
        "4:\n\t"
        "sw $zero, 0($a0)\n\t"
        "sw $zero, 4($a0)\n\t"
        "sw $zero, 8($a0)\n\t"
        "sw $zero, 12($a0)\n\t"
        "addiu $a0, $a0, 32\n\t"
        "sw $zero, -16($a0)\n\t"
        "sw $zero, -12($a0)\n\t"
        "sw $zero, -8($a0)\n\t"
        "bne $a0, $a3, 4b\n\t"
        "sw $zero, -4($a0)\n"
        "2:\n\t"
        "addiu $at, $zero, -4\n\t"
        "and $a3, $a1, $at\n\t"
        "beqz $a3, 3f\n\t"
        "subu $a1, $a1, $a3\n\t"
        "addu $a3, $a3, $a0\n"
        "5:\n\t"
        "addiu $a0, $a0, 4\n\t"
        "bne $a0, $a3, 5b\n\t"
        "sw $zero, -4($a0)\n"
        "3:\n\t"
        "blez $a1, 6f\n\t"
        "nop\n\t"
        "addu $a1, $a1, $a0\n"
        "7:\n\t"
        "addiu $a0, $a0, 1\n\t"
        "bne $a0, $a1, 7b\n\t"
        "sb $zero, -1($a0)\n"
        "6:\n\t"
        ".set at\n\t"
        ".set reorder"
        : : : "v1", "a3"
    );
}

const char *strchr(const char *s, s32 c) {
    const char ch = c;

    while (*s != ch) {
        if (*s++ == 0) {
            return 0;
        }
    }
    return s;
}


s32 strlen(const char *s) {
    const char *sc = s;

    while (*sc != 0) {
        sc++;
    }
    return sc - s;
}

void *memcpy(void *s1, const void *s2, u32 n) {
    char *su1 = (char *)s1;
    const char *su2 = (const char *)s2;

    while (n > 0) {
        *su1 = *su2;
        su1++;
        su2++;
        n--;
    }
    return s1;
}
