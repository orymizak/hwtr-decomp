#ifndef GFX_H
#define GFX_H

#include "PR/mbi.h"

/* Display-list pool: a fixed head buffer plus chunks allocated on demand (one pool per frame). */
typedef struct {
    /* 0x000 */ Gfx *ptr;
    /* 0x004 */ Gfx *end;
    /* 0x008 */ Gfx head[0x41];
    /* 0x210 */ Gfx *chunks[0x41];
    /* 0x314 */ u32 capacity;
    /* 0x318 */ u32 chunkCount;
    /* 0x31C */ u8 unk31C[4];
} GfxPool; /* size = 0x320 */

extern GfxPool gGfxPools[2]; /* display-list pools, one per frame */
extern s32 gGfxPoolIndex;        /* index of the current frame's pool */
extern Gfx *gDisplayListHead;       /* display-list write pointer */

void GfxPool_Grow(GfxPool *pool, u32 required);

/* Makes room for n more commands in the current pool before writing them.
 * n is not parenthesized in the pointer sum: GFX_RESERVE(n + 8) must stay (gfx + n) + 8 to match. */
#define GFX_RESERVE(n)                                                       \
    if ((u32)(gDisplayListHead + n) >= (u32)gGfxPools[gGfxPoolIndex].end) {          \
        gGfxPools[gGfxPoolIndex].ptr = gDisplayListHead;                             \
        GfxPool_Grow(&gGfxPools[gGfxPoolIndex], (n));                          \
        gDisplayListHead = gGfxPools[gGfxPoolIndex].ptr;                             \
    }

#endif
