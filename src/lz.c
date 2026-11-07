#include "common.h"

typedef struct {
    /* 0x00 */ u32 size;
    /* 0x04 */ union {
        u16 windowBits;
        u8 bytes[2];
    } u;
    /* 0x06 */ u8 pad;
    /* 0x07 */ u8 lengthBits;
} LZHeader; /* size = 0x8 */

extern u16 D_800F2650[];
extern u16 D_800F2660[];
extern u8 D_8010C400;

u8 *LZ_AdvanceBits(int n, u8 *p);

s32 LZ_Decompress(u8 *src, u8 *dst) {
    u16 length;
    u16 distance;
    u8 totalBits;
    u8 lengthBits;
    s32 unused2;
    u32 i;
    u32 j;
    u16 unused;
    u8 *in;
    u8 *out;
    u16 prefix;
    u8 flag;
    u16 value;
    LZHeader *hdr;

    hdr = (LZHeader *)src;
    in = src + 8;
    out = dst;
    unused = 0;
    D_8010C400 = 0;
    totalBits = hdr->lengthBits + hdr->u.bytes[1];
    lengthBits = hdr->lengthBits;
    prefix = (1 << hdr->u.windowBits) + 1;
    for (i = 0; i < prefix; i++) {
        *out++ = *in++;
    }
    D_8010C400 = 0;
    while ((u32)(out - dst) < hdr->size) {
        flag = *in & (1 << D_8010C400);
        in = LZ_AdvanceBits(1, in);
        if (flag) {
            value = ((in[0] | ((in[1] << 8) | (in[2] << 16))) >> D_8010C400) & D_800F2660[totalBits];
            in = LZ_AdvanceBits(totalBits, in);
            distance = (value >> lengthBits) + 1;
            length = value << (16 - lengthBits);
            length = length >> (16 - lengthBits);
            length++;
            if (length == 0) {
                while (1);
            }
            for (j = 0; j < length; j++) {
                *out++ = *(out - (distance + 1));
            }
        } else {
            *out++ = ((in[0] | (in[1] << 8)) & D_800F2650[D_8010C400]) >> D_8010C400;
            in = LZ_AdvanceBits(8, in);
        }
    }
    return hdr->size;
}

/*
 * El C de esta función aún difiere en la asignación de registros: el original usa $a2 como
 * temporal porque mantiene $a1 ocupado. Equivale a:
 *
 * u8 *LZ_AdvanceBits(register int n, register u8 *p) {
 *     D_8010C400 += n;
 *     while (D_8010C400 >= 8) {
 *         D_8010C400 -= 8;
 *         if (D_8010C400 >= 0x10) {
 *             while (1);
 *         }
 *         p++;
 *     }
 *     return p;
 * }
 */
__asm__(
    ".text\n"
    ".set noreorder\n"
    ".set noat\n"
    ".globl LZ_AdvanceBits\n"
    "LZ_AdvanceBits:\n"
    "    addiu $29, $29, -8\n"
    "    sw    $30, 0($29)\n"
    "    addu  $30, $29, $0\n"
    "    addu  $3, $4, $0\n"
    "    addu  $4, $5, $0\n"
    "    addu  $5, $3, $0\n"
    "    lui   $2, %hi(D_8010C400)\n"
    "    lbu   $2, %lo(D_8010C400)($2)\n"
    "    addu  $6, $2, $5\n"
    "    lui   $1, %hi(D_8010C400)\n"
    "    sb    $6, %lo(D_8010C400)($1)\n"
    ".LAdvanceLoop:\n"
    "    lui   $2, %hi(D_8010C400)\n"
    "    lbu   $2, %lo(D_8010C400)($2)\n"
    "    sltiu $6, $2, 8\n"
    "    beq   $6, $0, .LAdvanceBody\n"
    "    nop\n"
    "    j     .LAdvanceDone\n"
    "    nop\n"
    ".LAdvanceBody:\n"
    "    lui   $2, %hi(D_8010C400)\n"
    "    lbu   $2, %lo(D_8010C400)($2)\n"
    "    addiu $6, $2, -8\n"
    "    lui   $1, %hi(D_8010C400)\n"
    "    sb    $6, %lo(D_8010C400)($1)\n"
    "    lui   $2, %hi(D_8010C400)\n"
    "    lbu   $2, %lo(D_8010C400)($2)\n"
    "    sltiu $6, $2, 16\n"
    "    bne   $6, $0, .LAdvanceNext\n"
    "    nop\n"
    ".LAdvanceHang:\n"
    "    j     .LAdvanceHang2\n"
    "    nop\n"
    "    j     .LAdvanceNext\n"
    "    nop\n"
    ".LAdvanceHang2:\n"
    "    j     .LAdvanceHang\n"
    "    nop\n"
    ".LAdvanceNext:\n"
    "    j     .LAdvanceLoop\n"
    "    addiu $4, $4, 1\n"
    ".LAdvanceDone:\n"
    "    j     .LAdvanceRet\n"
    "    addu  $2, $4, $0\n"
    ".LAdvanceRet:\n"
    "    addu  $29, $30, $0\n"
    "    lw    $30, 0($29)\n"
    "    jr    $31\n"
    "    addiu $29, $29, 8\n"
    ".set at\n"
    ".set reorder\n"
);
