#include "common.h"
#include "include_asm.h"

#define CHAR_SPACE 1
#define CHAR_FIRST 0xF
#define CHAR_COUNT 0x33
#define CHAR_UNKNOWN 0x38
#define SCREEN_WIDTH 640
#define GLYPH_WIDTH 16

extern u16 *D_8014EB38;
extern u16 *D_80138E38;

INCLUDE_ASM("asm/nonmatchings/debug_text", Debug_InitFont);

INCLUDE_ASM("asm/nonmatchings/debug_text", Debug_FormatNumber);

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
                    pixel[0x281] = shadow;
                }
                pixel++;
            }
            pixel += SCREEN_WIDTH - GLYPH_WIDTH;
            glyph++;
        }
        D_80138E38 += GLYPH_WIDTH;
    }
}

INCLUDE_ASM("asm/nonmatchings/debug_text", Debug_DrawString);

INCLUDE_ASM("asm/nonmatchings/debug_text", func_80065314);
