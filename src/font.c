#include "common.h"
#include "include_asm.h"
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

INCLUDE_ASM("asm/nonmatchings/font", Font_Load);

s32 Font_Free(u32 arg0) {
  s32 *temp_s0;
  s32 temp_a0;
  ;
  temp_a0 = *temp_s0;
  if ((*(&D_801415E0[arg0 & 0xFF])) != 0) {
    Memory_Free((u32) (*(&D_801415E0[arg0 & 0xFF])));
    *(&D_801415E0[arg0 & 0xFF]) = 0;
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

INCLUDE_ASM("asm/nonmatchings/font", func_8006F528);

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
}
