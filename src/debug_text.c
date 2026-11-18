#include "common.h"
#include "include_asm.h"
typedef struct { u8 start; u8 count; } FontRange_;
extern FontRange_ D_800F01D0[];

#define CHAR_SPACE 1
#define CHAR_FIRST 0xF
#define CHAR_COUNT 0x33
#define CHAR_UNKNOWN 0x38
#define SCREEN_WIDTH 640
#define GLYPH_WIDTH 16
#define SHADOW_OFFSET_PIXELS (SCREEN_WIDTH + 1) /* one pixel down and right */

extern u16 *D_8014EB38;
extern u16 *D_80138E38;

void Debug_InitFont(u16 *buf) {
    u32 *rom = (u32 *)0xB0000B74;
    u32 bits = rom[-1];
    s32 bit = 0;
    u32 i;
    s32 n;
    s32 j;
    s32 k;
    u16 *dst;

    D_8014EB38 = buf;
    for (n = 0x33F; n >= 0; n--) {
        *buf++ = 0;
    }
    for (i = 0; i < 3; i++) {
        dst = &D_8014EB38[D_800F01D0[i].start * 16 + 2];
        for (j = 0; j < D_800F01D0[i].count; j++) {
            for (k = 0; k < 14; k++) {
                bit += 13;
                if (bit & ~31) {
                    bit &= 31;
                    *dst = (bits << bit) & 0x1FFF;
                    bits = *rom++;
                    if (bit == 0) {
                        goto next;
                    }
                }
                *dst |= (bits >> (32 - bit)) & 0x1FFF;
            next:
                dst++;
            }
            dst += 2;
            bit += 2;
        }
    }

    /* TODO (Clean version):
    Bit-level font unpacking logic (relying on bit-shifts and gotos) is highly 
    dependent on the raw ROM asset structure. A clean version would extract 
    font assets into a standard format or use a dedicated decompression parser, 
    but this is kept as-is for matching purposes.
    */
}

void Debug_FormatNumber(u16 n, u8 *out) {
    s32 d;
    u8 c;
    u8 c2;

    if (n >= 1000) {
        n = 999;
    }
    d = (u16)(n / 100);
    c = d + 0xF;
    if (n >= 100) {
        out[0] = d + 0x10;
    } else {
        out[0] = c;
    }
    d = (u16)(n % 100) / 10;
    c2 = d + 0xF;
    if (n >= 10) {
        out[1] = d + 0x10;
    } else {
        out[1] = c2;
    }
    out[2] = n % 10 + 0x10;

    /* TODO (Clean version): [evaluated 2026-11-18: the ternary/hundreds-tens form
    changes codegen (63 words differ); the current form is kept for matching]
    Can be simplified using direct modular arithmetic and a cleaner string 
    formatting approach (e.g., custom sprintf or hundreds/tens/units logic):
    
    void Clean_Debug_FormatNumber(u16 n, u8 *out) {
        if (n > 999) n = 999;
        out[0] = (n >= 100) ? ((n / 100) + 0x10) : ((n / 100) + 0xF);
        out[1] = (n >= 10)  ? (((n / 10) % 10) + 0x10) : (((n / 10) % 10) + 0xF);
        out[2] = (n % 10) + 0x10;
    }
    */
}

inline void Debug_DrawChar(s32 c, u16 color, u16 shadow) {
    u16 *pixel;
    u16 *glyph;
    s32 row;
    s32 bit;

    if (c == CHAR_SPACE) {
        D_80138E38 += 5;
    } else {
        if ((u32)(c - CHAR_FIRST) >= CHAR_COUNT) {
            c = CHAR_UNKNOWN;
        }
        pixel = D_80138E38;
        glyph = D_8014EB38 + (c - CHAR_FIRST) * GLYPH_WIDTH;
        for (row = 0; row < GLYPH_WIDTH; row++) {
            for (bit = GLYPH_WIDTH - 1; bit >= 0; bit--) {
                if ((*glyph >> bit) & 1) {
                    pixel[0] = color;
                    pixel[SHADOW_OFFSET_PIXELS] = shadow;
                }
                pixel++;
            }
            pixel += SCREEN_WIDTH - GLYPH_WIDTH;
            glyph++;
        }
        D_80138E38 += GLYPH_WIDTH;
    }
}

void Debug_DrawString(u8 *s, s32 n, u16 color, u16 shadow) {
    s32 i;

    for (i = n - 1; i != -1; i--) {
        if (*s == 0) {
            return;
        }
        Debug_DrawChar(*s++, color, shadow);
    }
    /* TODO (Clean version): [evaluated 2026-11-18: forward and while(n--) loops
    change codegen (46 words differ); the countdown is kept for matching]
    A standard forward loop with a size limit is much more readable:
    for (s32 i = 0; i < n && *s != '\0'; i++) {
        Debug_DrawChar(*s++, color, shadow);
    }
    */
}


void Debug_DrawChars(u8 *s, s32 n, u16 color, u16 shadow) {
    s32 i;

    for (i = n - 1; i != -1; i--) {
        if (*s != 0) {
            Debug_DrawChar(*s++, color, shadow);
        } else {
            s++;
            Debug_DrawChar(CHAR_FIRST, color, shadow);
        }
    }
    /* TODO (Clean version): [evaluated 2026-11-18: forward loop and a single call
    with a ternary change codegen (70 words differ); kept for matching]
    Similar to Debug_DrawString, this reverse countdown loop can be refactored 
    into a cleaner forward loop with explicit bounds and string termination checks.
    */
}

