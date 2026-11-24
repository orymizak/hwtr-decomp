#include "common.h"
#include "ultra64.h"
#include "include_asm.h"

typedef struct {
    /* 0x00 */ u8 width;
    /* 0x01 */ u8 height;
    /* 0x02 */ u8 format;
    /* 0x03 */ u8 unk03;
    /* 0x04 */ void *data;
} Texture; /* size = 0x8 */

extern s32 D_800F2190;
extern s32 D_800F2194;
extern s32 D_800F2198;

void srand(u32 seed);
u32 rand(void);
void *Memory_Alloc(u32 size);
void Memory_EnableSecondary(void);
void Memory_DisableSecondary(void);
s32 File_GetSize(char *name);
s32 File_Load(char *name, void *dst);
void GfxPools_Shutdown(void);
void Video_SetLowRes(void);
void Video_SetHighRes(void);

s32 Rand_Seed(void) {
    srand(osGetCount());
    return 1;
}

u32 Time_GetMillis(void) {
    u64 time = osGetTime() * 64 / 3000;

    return time / 1000;
}

u32 Rand_Range(u32 max) {
    return rand() % max;
}

s32 Video_EnterRaceMode(void) {
    D_800F2190 = D_800F2194;
    Video_SetLowRes();
    Memory_EnableSecondary();
    srand(osGetCount());
    return 1;
}

s32 Video_LeaveRaceMode(void) {
    D_800F2190 = D_800F2198;
    GfxPools_Shutdown();
    Memory_DisableSecondary();
    Video_SetHighRes();
    return 1;
}

inline s32 Math_Log2(u32 value) {
    s32 log2 = 0;

    value >>= 1;
    while (value != 0) {
        value >>= 1;
        log2++;
    }
    return log2;
}

void Texture_Relocate(Texture *texture) {
    texture->data = texture + 1;
}

Texture *Texture_Load(char *name) {
    Texture *texture = Memory_Alloc(File_GetSize(name));

    File_Load(name, texture);
    texture->data = texture + 1;
    return texture;
}

Texture *Texture_Create(s32 width, s32 height) {
    Texture *texture = Memory_Alloc(width * height * 2 + sizeof(Texture));

    texture->width = width;
    texture->height = height;
    texture->format = 0;
    texture->unk03 = 0;
    texture->data = texture + 1;
    return texture;
}

/* Emits the commands that load texture t into TMEM (tile 0, wrapping, masks from its size):
 * format 0 RGBA16, 1 I8, 3 RGBA32, 4 CI8 (the image follows a palette of n entries). */
Gfx *Gfx_LoadTexture(Gfx *g, Texture *t) {
    s32 ms;
    s32 mt;
    u32 w;
    u32 h;
    s16 *pal;

    ms = Math_Log2(t->width);
    mt = Math_Log2(t->height);
    w = t->width;
    h = t->height;
    if (t->format == 1) {
        gDPLoadTextureBlock(g++, t->data, G_IM_FMT_I, G_IM_SIZ_8b, w, h, 0, G_TX_WRAP, G_TX_WRAP, ms, mt,
                            G_TX_NOLOD, G_TX_NOLOD);
    } else if (t->format == 0) {
        gDPLoadTextureBlock(g++, t->data, G_IM_FMT_RGBA, G_IM_SIZ_16b, w, h, 0, G_TX_WRAP, G_TX_WRAP, ms, mt,
                            G_TX_NOLOD, G_TX_NOLOD);
    } else if (t->format == 3) {
        gDPLoadTextureBlock(g++, t->data, G_IM_FMT_RGBA, G_IM_SIZ_32b, w, h, 0, G_TX_WRAP, G_TX_WRAP, ms, mt,
                            G_TX_NOLOD, G_TX_NOLOD);
    } else if (t->format == 4) {
        pal = t->data;
        gDPLoadTextureBlock(g++, (u8 *)t->data + pal[0] * 2 + 8, G_IM_FMT_CI, G_IM_SIZ_8b, w, h, 0,
                            G_TX_WRAP, G_TX_WRAP, ms, mt, G_TX_NOLOD, G_TX_NOLOD);
    }
    return g;
}

typedef struct { u8 w; u8 h; u8 fmt; u8 pad; void *data; } Img7F_;
/* Fills an image with one of its texels (the first one for 16-bit images). */
void Image_FillSolid(Img7F_ *img) {
    u32 n;
    u32 mid;
    u32 i;

    if (img == NULL || img->data == NULL) {
        return;
    }
    n = img->w * img->h;
    mid = (n + img->h) / 4;
    switch (img->fmt) {
        case 0: {
            u16 *p = img->data;
            u16 v = p[0];

            for (i = 0; i < n; i++) {
                p[i] = v;
            }
            break;
        }
        case 1: {
            u8 *p = img->data;
            u8 v = p[mid];

            for (i = 0; i < n; i++) {
                p[i] = v;
            }
            break;
        }
        case 2:
            break;
        case 3: {
            u32 *p = img->data;
            u32 v = p[mid];

            for (i = 0; i < n; i++) {
                p[i] = v;
            }
            break;
        }
    }
}
