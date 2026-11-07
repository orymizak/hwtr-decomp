#include "common.h"
#include "include_asm.h"

#define G_DL_BRANCH 0xDE000000
#define G_ENDDL 0xDF000000

typedef struct {
    u32 w0;
    u32 w1;
} Gfx;

typedef struct {
    u8 data[0x10];
} Vtx;

typedef struct {
    u8 data[0x40];
} Mtx;

typedef struct {
    /* 0x000 */ Gfx *ptr;
    /* 0x004 */ Gfx *end;
    /* 0x008 */ Gfx head[0x41];
    /* 0x210 */ Gfx *chunks[0x41];
    /* 0x314 */ u32 capacity;
    /* 0x318 */ u32 chunkCount;
    /* 0x31C */ u8 unk31C[4];
} GfxPool; /* size = 0x320 */

typedef struct {
    /* 0x00 */ Vtx *ptr;
    /* 0x04 */ Vtx *end;
    /* 0x08 */ Vtx *chunks[0x20];
    /* 0x88 */ u32 capacity;
    /* 0x8C */ u32 chunkCount;
} VtxPool; /* size = 0x90 */

typedef struct {
    /* 0x00 */ Mtx *ptr;
    /* 0x04 */ Mtx *end;
    /* 0x08 */ Mtx *chunks[0x20];
    /* 0x88 */ u32 capacity;
    /* 0x8C */ u32 chunkCount;
} MtxPool; /* size = 0x90 */

extern GfxPool D_8014C740[2];
extern VtxPool D_80140E48[2];
extern MtxPool D_8014EB58[2];
extern Gfx D_80134FD0[2][0x301];
extern Vtx D_801347C0[2][0x40];
extern Mtx D_801480A0[2][0x80];
extern s32 D_8014C540;
extern Gfx *D_8014EC78;
extern Vtx *D_8014445C;
extern Mtx *D_8014108C;

void *Memory_Alloc(u32 size);
void Memory_Free(void *ptr);
void func_8006FA7C(Gfx *list, s32 size, s32 arg2, s32 arg3);
void func_80071924(void);
s32 func_800784F0(void);

inline void GfxPool_Init(GfxPool *pool, u32 capacity, Gfx *buffer) {
    pool->capacity = capacity;
    pool->chunkCount = 1;
    pool->chunks[0] = buffer;
    pool->chunks[1] = NULL;
    pool->end = pool->chunks[0] + pool->capacity;
    pool->ptr = pool->chunks[0];
}

void GfxPool_Grow(GfxPool *pool, u32 required) {
    u32 size = pool->capacity;
    Gfx *chunk;
    Gfx *cur;

    if (size < required) {
        size = required;
    }
    chunk = Memory_Alloc((size + 1) * sizeof(Gfx));
    cur = pool->ptr;
    cur->w0 = G_ENDDL;
    cur->w1 = 0;
    if (chunk == NULL) {
        pool->ptr = pool->chunks[pool->chunkCount - 1];
    } else {
        pool->chunks[pool->chunkCount] = chunk;
        pool->chunkCount++;
        pool->chunks[pool->chunkCount] = NULL;
        pool->ptr = chunk;
        pool->end = chunk + size;
    }
}

inline void GfxPool_Reset(GfxPool *pool) {
    u32 i;

    for (i = 1; i < pool->chunkCount; i++) {
        Memory_Free(pool->chunks[i]);
    }
    pool->chunkCount = 1;
    pool->ptr = pool->chunks[0];
    pool->end = pool->chunks[0] + pool->capacity;
}

INCLUDE_ASM("asm/nonmatchings/gfx_pools", GfxPool_Submit);

inline void VtxPool_Init(VtxPool *pool, u32 capacity, Vtx *buffer) {
    pool->capacity = capacity;
    pool->chunkCount = 1;
    pool->chunks[0] = buffer;
    pool->chunks[1] = NULL;
    pool->end = pool->chunks[0] + pool->capacity;
    pool->ptr = pool->chunks[0];
}

inline void VtxPool_Reserve(VtxPool *pool, u32 required) {
    u32 size;
    u32 bytes;
    Vtx *chunk;

    if (pool->ptr + required >= pool->end) {
        size = pool->capacity;
        if (size < required) {
            size = required;
        }
        bytes = size * sizeof(Vtx);
        chunk = Memory_Alloc(bytes);
        if (chunk == NULL) {
            pool->ptr = pool->chunks[pool->chunkCount - 1];
        } else {
            pool->chunks[pool->chunkCount] = chunk;
            pool->chunkCount++;
            pool->chunks[pool->chunkCount] = NULL;
            pool->ptr = chunk;
            pool->end = (Vtx *)((u8 *)chunk + bytes);
        }
    }
}

inline void VtxPool_Reset(VtxPool *pool) {
    u32 i;

    for (i = 1; i < pool->chunkCount; i++) {
        Memory_Free(pool->chunks[i]);
    }
    pool->chunkCount = 1;
    pool->ptr = pool->chunks[0];
    pool->end = pool->chunks[0] + pool->capacity;
}

inline void MtxPool_Init(MtxPool *pool, u32 capacity, Mtx *buffer) {
    pool->capacity = capacity;
    pool->chunkCount = 1;
    pool->chunks[0] = buffer;
    pool->chunks[1] = NULL;
    pool->end = pool->chunks[0] + pool->capacity;
    pool->ptr = pool->chunks[0];
}

void MtxPool_Grow(MtxPool *pool, u32 required) {
    u32 size = pool->capacity;
    u32 bytes;
    Mtx *chunk;

    if (size < required) {
        size = required;
    }
    bytes = size * sizeof(Mtx);
    chunk = Memory_Alloc(bytes);
    if (chunk == NULL) {
        pool->ptr = pool->chunks[pool->chunkCount - 1];
    } else {
        pool->chunks[pool->chunkCount] = chunk;
        pool->chunkCount++;
        pool->chunks[pool->chunkCount] = NULL;
        pool->ptr = chunk;
        pool->end = (Mtx *)((u8 *)chunk + bytes);
    }
}

inline void MtxPool_Reset(MtxPool *pool) {
    u32 i;

    for (i = 1; i < pool->chunkCount; i++) {
        Memory_Free(pool->chunks[i]);
    }
    pool->chunkCount = 1;
    pool->ptr = pool->chunks[0];
    pool->end = pool->chunks[0] + pool->capacity;
}

INCLUDE_ASM("asm/nonmatchings/gfx_pools", GfxPools_Init);

void GfxPools_ReserveVtx(u32 required) {
    VtxPool *pool = &D_80140E48[D_8014C540];

    D_80140E48[D_8014C540].ptr = D_8014445C;
    VtxPool_Reserve(pool, required);
    D_8014445C = D_80140E48[D_8014C540].ptr;
}

INCLUDE_ASM("asm/nonmatchings/gfx_pools", GfxPools_Swap);

void GfxPools_Shutdown(void) {
    func_80071924();
    func_800784F0();
    func_80071924();
    GfxPool_Reset(&D_8014C740[0]);
    VtxPool_Reset(&D_80140E48[0]);
    MtxPool_Reset(&D_8014EB58[0]);
    GfxPool_Reset(&D_8014C740[1]);
    VtxPool_Reset(&D_80140E48[1]);
    MtxPool_Reset(&D_8014EB58[1]);
}
