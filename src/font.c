#include "common.h"
#include "include_asm.h"
typedef struct { u8 width; u8 height; u8 format; u8 unk03; u32 data; } GlyphFL_;
typedef struct { u8 glyphIndex[0x100]; GlyphFL_ *glyphs[0x100]; u8 glyphData[1]; } FontFL_;
extern char D_800FF790[];
extern s32 D_801415E0[];

#define FONT_NO_GLYPH 0xFF
#define FONT_SPACE 0x2E

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
extern u16 D_800F20FA;
extern u16 D_800F20FC;

void *Memory_Alloc(u32 size);
void Memory_Free(void *ptr);
s32 File_Load(char *name, void *dst);
s32 File_GetSize(char *name);
char *strcpy(char *dst, const char *src);
char *strcat(char *dst, const char *src);
void func_8007F570(Glyph *glyph);
void func_80076BCC();
void func_80076D6C(s32 x, s32 y, u32 data, u8 width, s32 height, f32 scaleX, f32 scaleY);
void func_80076CF8(s32 x, s32 y, u32 data, u8 width, s32 height);

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
    FontFL_ *f;
    GlyphFL_ *g;
    u32 n;
    u32 i;

    strcpy(buf, name);
    strcat(buf, D_800FF790);
    f = Memory_Alloc(File_GetSize(buf));
    File_Load(buf, f);
    D_801415E0[id] = (s32)f;
    g = (GlyphFL_ *)f->glyphData;
    n = 0;
    for (i = 0; i < 0x100; i++) {
        n += f->glyphIndex[i] != 0xFF;
    }
    for (i = 0; i < n; i++) {
        func_8007F570(g);
        f->glyphs[i] = g;
        if (g->format == 0) {
            g = (GlyphFL_ *)(g->data + g->width * g->height * 2);
        } else {
            g = (GlyphFL_ *)(g->data + g->width * g->height);
        }
        g = (GlyphFL_ *)(((u32)g + 7) & ~7);
    }
    return 1;
    /* TODO (Clean version):
    Duplicate struct definitions (`GlyphFL_` / `FontFL_` vs `Glyph` / `Font`) 
    are used here because file-loading structures often contain flat layout 
    differences compared to active runtime structures. Unifying them into a 
    single robust asset loader struct with explicit alignment macros would 
    improve maintainability.
    */
}

s32 Font_Free(u8 id) {
    if (D_801415E0[id] != 0) {
        Memory_Free(D_801415E0[id]);
        D_801415E0[id] = 0;
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

s32 func_8006F528(u8 id, u8 c, f32 x, f32 y, f32 scale) {
    Font *font;
    u8 index;
    Glyph *glyph;

    if (c != 0x2E) {
        font = D_801415C0.fonts[id];
        index = font->glyphIndex[c];
        if (index != 0xFF) {
            glyph = font->glyphs[index];
            FONT_ASSERT(glyph != NULL);
            x = (x - 500.0f) * D_800F20FA * 0.9 / 1000.0 + D_800F20FA / 2;
            y = (500.0f - y) * D_800F20FC * 0.95 / 1000.0 + D_800F20FC / 2;
            func_80076BCC();
            func_80076D6C(x, y, glyph->data, glyph->width, glyph->height, scale, scale);
        }
    }
    return 0;
    /* TODO (Clean version):
    Magic screen coordinate constants (like 500.0f, 1000.0, 0.9, 0.95) 
    should be encapsulated into UI scaling configuration macros or viewport structs:
    #define SCREEN_VIRTUAL_WIDTH 1000.0f
    #define SCREEN_HALF_WIDTH (SCREEN_VIRTUAL_WIDTH / 2.0f)
    */
}

u8 Font_DrawChar(u8 id, u8 c, s32 x, s32 y) {
    Font *font = D_801415C0.fonts[id];
    u8 index = font->glyphIndex[c];
    Glyph *glyph;

    if (index == FONT_NO_GLYPH) {
        return 0;
    }
    glyph = font->glyphs[index];
    func_80076BCC();
    func_80076CF8(x, y, glyph->data, glyph->width, glyph->height);
    return glyph->width;
}

u8 Font_DrawChar2(u8 id, u8 c, s32 x, s32 y) {
    Font *font = D_801415C0.fonts[id];
    u8 index = font->glyphIndex[c];
    Glyph *glyph;

    if (index != FONT_NO_GLYPH) {
        glyph = font->glyphs[index];
        func_80076BCC();
        func_80076CF8(x, y, glyph->data, glyph->width, glyph->height);
        return glyph->width;
    }
    return 0;
    /* TODO (Clean version):
    `Font_DrawChar2` duplicates the core logic of `Font_DrawChar` with an inverted 
    conditional structure. They could share a single internal helper function 
    to reduce code duplication while preserving the exported function symbols.
    */
}
