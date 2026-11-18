#include "common.h"
#include "include_asm.h"
extern char D_800FF790[];

#define FONT_NO_GLYPH 0xFF
#define FONT_SPACE 0x2E

/* Virtual UI space: 1000x1000 units centred on (500, 500), y up, mapped to 90% x 95% of
   the screen. Double literals as in the original arithmetic. */
#define UI_SIZE 1000.0
#define UI_CENTER 500.0f
#define UI_SCALE_X 0.9
#define UI_SCALE_Y 0.95

typedef struct {
    /* 0x00 */ u8 width;
    /* 0x01 */ u8 height;
    /* 0x02 */ u8 format;
    /* 0x03 */ u8 unk03;
    /* 0x04 */ u32 data;
} Glyph; /* size = 0x8 */

typedef struct {
    /* 0x000 */ u8 glyphIndex[0x100];
    /* 0x100 */ Glyph *glyphs[0x100];
    /* 0x500 */ u8 glyphData[1];
} Font;

typedef struct {
    /* 0x00 */ s32 unk00[8];
    /* 0x20 */ Font *fonts[8];
} FontTable;

extern FontTable D_801415C0;
extern u16 gScreenWidth;
extern u16 gScreenHeight;

void *Memory_Alloc(u32 size);
void Memory_Free(void *ptr);
s32 File_Load(char *name, void *dst);
s32 File_GetSize(char *name);
char *strcpy(char *dst, const char *src);
char *strcat(char *dst, const char *src);
void Texture_Relocate(Glyph *glyph);
void Draw2D_Begin();
void Image_DrawRGBA16Scaled(s32 x, s32 y, u32 data, u8 width, s32 height, f32 scaleX, f32 scaleY);
void Image_DrawRGBA16(s32 x, s32 y, u32 data, u8 width, s32 height);

s32 Font_Init(void) {
    s32 i;

    for (i = 0; i < 8; i++) {
        D_801415C0.unk00[i] = 0;
        D_801415C0.fonts[i] = NULL;
    }
    return 1;
}

s32 Font_Load(u8 id, char *name) {
    char buf[0x40];
    Font *f;
    Glyph *g;
    u32 n;
    u32 i;

    strcpy(buf, name);
    strcat(buf, D_800FF790);
    f = Memory_Alloc(File_GetSize(buf));
    File_Load(buf, f);
    D_801415C0.fonts[id] = f;
    g = (Glyph *)f->glyphData;
    n = 0;
    for (i = 0; i < 0x100; i++) {
        n += f->glyphIndex[i] != 0xFF;
    }
    for (i = 0; i < n; i++) {
        Texture_Relocate(g);
        f->glyphs[i] = g;
        if (g->format == 0) {
            g = (Glyph *)(g->data + g->width * g->height * 2);
        } else {
            g = (Glyph *)(g->data + g->width * g->height);
        }
        g = (Glyph *)(((u32)g + 7) & ~7);
    }
    return 1;
}

s32 Font_Free(u8 id) {
    if (D_801415C0.fonts[id] != NULL) {
        Memory_Free(D_801415C0.fonts[id]);
        D_801415C0.fonts[id] = NULL;
    }
    return 1;
}

inline f32 Font_GetCharWidthF(u8 id, u8 c) {
    Font *font = D_801415C0.fonts[id];
    u8 index = font->glyphIndex[c];

    if (index == FONT_NO_GLYPH) {
        return 1.0f;
    }
    return font->glyphs[index]->width;
}

u32 Font_GetCharWidth(u8 id, u8 c) {
    return Font_GetCharWidthF(id, c);
}

u16 Font_GetCharWidth16(u8 id, u8 c) {
    return Font_GetCharWidthF(id, c);
}

    /* TODO(match-debt): empty loop needed to match (scheduling barrier). Assumed to be a debug assert
    * compiled out in release builds; verify against a real cause when one turns up. */
#define FONT_ASSERT(cond) do { } while (0)

s32 Font_DrawCharScaled(u8 id, u8 c, f32 x, f32 y, f32 scale) {
    Font *font;
    u8 index;
    Glyph *glyph;

    if (c != FONT_SPACE) {
        font = D_801415C0.fonts[id];
        index = font->glyphIndex[c];
        if (index != 0xFF) {
            glyph = font->glyphs[index];
            FONT_ASSERT(glyph != NULL);
            x = (x - UI_CENTER) * gScreenWidth * UI_SCALE_X / UI_SIZE + gScreenWidth / 2;
            y = (UI_CENTER - y) * gScreenHeight * UI_SCALE_Y / UI_SIZE + gScreenHeight / 2;
            Draw2D_Begin();
            Image_DrawRGBA16Scaled(x, y, glyph->data, glyph->width, glyph->height, scale, scale);
        }
    }
    return 0;
}

inline u8 Font_DrawChar(u8 id, u8 c, s32 x, s32 y) {
    Font *font = D_801415C0.fonts[id];
    u8 index = font->glyphIndex[c];
    Glyph *glyph;

    if (index == FONT_NO_GLYPH) {
        return 0;
    }
    glyph = font->glyphs[index];
    Draw2D_Begin();
    Image_DrawRGBA16(x, y, glyph->data, glyph->width, glyph->height);
    return glyph->width;
}

u8 Font_DrawChar2(u8 id, u8 c, s32 x, s32 y) {
    return Font_DrawChar(id, c, x, y);
}
