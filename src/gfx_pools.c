#include "common.h"
#include "gfx.h"
#include "PR/os_convert.h"
#include "include_asm.h"


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

extern VtxPool gVtxPools[2];
extern MtxPool gMtxPools[2];
extern Gfx D_80134FD0[2][0x301];
extern Vtx D_801347C0[2][0x40];
extern Mtx D_801480A0[2][0x80];
extern Vtx *gVtxHead;
extern Mtx *gMtxHead;

void *Memory_Alloc(u32 size);
void Memory_Free(void *ptr);
void Gfx_SendTask(Gfx *list, s32 size, s32 arg2, s32 arg3);
void Gfx_WaitTasks(void);
s32 Screen_FlushFrame(void);

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
    gSPEndDisplayList(cur);
    if (chunk == NULL) {
        pool->ptr = pool->chunks[pool->chunkCount - 1];
    } else {
        /* TODO (Clean version): [evaluated 2026-11-18: design note on the original fallback; no
        change possible while matching]
        When allocation fails (`chunk == NULL`), the pool falls back by resetting 
        the pointer to the last valid chunk. A clean implementation should handle 
        out-of-memory errors gracefully using explicit error codes or panic handlers 
        rather than silently failing back to a restricted buffer boundary.
        */
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

/* Builds a display list that calls every chunk of the pool and passes it on. */
inline void GfxPool_Submit(GfxPool *pool, s32 arg) {
    Gfx *g = pool->head;
    u32 i;

    for (i = 0; i < pool->chunkCount; i++) {
        gSPDisplayList(g++, OS_K0_TO_PHYSICAL(pool->chunks[i]));
    }
    gSPEndDisplayList(g++);
    Gfx_SendTask(pool->head, (g - pool->head) * sizeof(Gfx), 0, arg);
}

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
            pool->end = chunk + size;
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

__asm__(
    ".text\n"
    ".globl GfxPools_Init\n"
    ".type GfxPools_Init, @function\n"
    "GfxPools_Init:\n"
    ".set noreorder\n"
    ".set noat\n"
    "addiu      $a0, $zero, 0x300\n"
    "lui        $v1, %hi(D_80134FD0)\n"
    "addiu      $v1, $v1, %lo(D_80134FD0)\n"
    "addiu      $v0, $v1, 0x1800\n"
    "addiu      $a2, $zero, 0x1\n"
    "lui        $at, %hi(D_8014C744)\n"
    "sw         $v0, %lo(D_8014C744)($at)\n"
    "addiu      $v0, $v1, 0x1808\n"
    "lui        $at, %hi(D_8014C950)\n"
    "sw         $v1, %lo(D_8014C950)($at)\n"
    "lui        $at, %hi(gGfxPools)\n"
    "sw         $v1, %lo(gGfxPools)($at)\n"
    "addiu      $v1, $v1, 0x3008\n"
    "lui        $at, %hi(D_8014CA54)\n"
    "sw         $a0, %lo(D_8014CA54)($at)\n"
    "lui        $at, %hi(D_8014CD74)\n"
    "sw         $a0, %lo(D_8014CD74)($at)\n"
    "addiu      $a0, $zero, 0x40\n"
    "lui        $at, %hi(D_8014CC70)\n"
    "sw         $v0, %lo(D_8014CC70)($at)\n"
    "lui        $at, %hi(D_8014CA60)\n"
    "sw         $v0, %lo(D_8014CA60)($at)\n"
    "lui        $v0, %hi(D_801347C0)\n"
    "addiu      $v0, $v0, %lo(D_801347C0)\n"
    "lui        $at, %hi(D_8014CA64)\n"
    "sw         $v1, %lo(D_8014CA64)($at)\n"
    "addiu      $v1, $v0, 0x400\n"
    "lui        $at, %hi(D_80140E50)\n"
    "sw         $v0, %lo(D_80140E50)($at)\n"
    "lui        $at, %hi(gVtxPools)\n"
    "sw         $v0, %lo(gVtxPools)($at)\n"
    "addiu      $v0, $v0, 0x800\n"
    "lui        $at, %hi(D_80140ED0)\n"
    "sw         $a0, %lo(D_80140ED0)($at)\n"
    "lui        $at, %hi(D_80140F60)\n"
    "sw         $a0, %lo(D_80140F60)($at)\n"
    "addiu      $a0, $zero, 0x80\n"
    "lui        $at, %hi(D_80140EDC)\n"
    "sw         $v0, %lo(D_80140EDC)($at)\n"
    "lui        $v0, %hi(D_801480A0)\n"
    "addiu      $v0, $v0, %lo(D_801480A0)\n"
    "lui        $at, %hi(D_80140E4C)\n"
    "sw         $v1, %lo(D_80140E4C)($at)\n"
    "lui        $at, %hi(D_80140EE0)\n"
    "sw         $v1, %lo(D_80140EE0)($at)\n"
    "lui        $at, %hi(D_80140ED8)\n"
    "sw         $v1, %lo(D_80140ED8)($at)\n"
    "addiu      $v1, $v0, 0x2000\n"
    "lui        $at, %hi(D_8014EB60)\n"
    "sw         $v0, %lo(D_8014EB60)($at)\n"
    "lui        $at, %hi(gMtxPools)\n"
    "sw         $v0, %lo(gMtxPools)($at)\n"
    "addiu      $v0, $v0, 0x4000\n"
    "lui        $at, %hi(D_8014C954)\n"
    "sw         $zero, %lo(D_8014C954)($at)\n"
    "lui        $at, %hi(D_8014CA58)\n"
    "sw         $a2, %lo(D_8014CA58)($at)\n"
    "lui        $at, %hi(D_8014CC74)\n"
    "sw         $zero, %lo(D_8014CC74)($at)\n"
    "lui        $at, %hi(D_8014CD78)\n"
    "sw         $a2, %lo(D_8014CD78)($at)\n"
    "lui        $at, %hi(D_80140E54)\n"
    "sw         $zero, %lo(D_80140E54)($at)\n"
    "lui        $at, %hi(D_80140ED4)\n"
    "sw         $a2, %lo(D_80140ED4)($at)\n"
    "lui        $at, %hi(D_80140EE4)\n"
    "sw         $zero, %lo(D_80140EE4)($at)\n"
    "lui        $at, %hi(D_80140F64)\n"
    "sw         $a2, %lo(D_80140F64)($at)\n"
    "lui        $at, %hi(D_8014EBE0)\n"
    "sw         $a0, %lo(D_8014EBE0)($at)\n"
    "lui        $at, %hi(D_8014EB64)\n"
    "sw         $zero, %lo(D_8014EB64)($at)\n"
    "lui        $at, %hi(D_8014EB5C)\n"
    "sw         $v1, %lo(D_8014EB5C)($at)\n"
    "lui        $at, %hi(D_8014EBE4)\n"
    "sw         $a2, %lo(D_8014EBE4)($at)\n"
    "lui        $at, %hi(D_8014EC70)\n"
    "sw         $a0, %lo(D_8014EC70)($at)\n"
    "lui        $at, %hi(D_8014EBF0)\n"
    "sw         $v1, %lo(D_8014EBF0)($at)\n"
    "lui        $at, %hi(D_8014EBF4)\n"
    "sw         $zero, %lo(D_8014EBF4)($at)\n"
    "lui        $at, %hi(D_8014EBEC)\n"
    "sw         $v0, %lo(D_8014EBEC)($at)\n"
    "lui        $v0, %hi(gGfxPools)\n"
    "lw         $v0, %lo(gGfxPools)($v0)\n"
    "lui        $a0, %hi(gVtxPools)\n"
    "lw         $a0, %lo(gVtxPools)($a0)\n"
    "lui        $a1, %hi(gMtxPools)\n"
    "lw         $a1, %lo(gMtxPools)($a1)\n"
    "lui        $at, %hi(D_8014EC74)\n"
    "sw         $a2, %lo(D_8014EC74)($at)\n"
    "lui        $at, %hi(D_8014EBE8)\n"
    "sw         $v1, %lo(D_8014EBE8)($at)\n"
    "lui        $at, %hi(gGfxPoolIndex)\n"
    "sw         $zero, %lo(gGfxPoolIndex)($at)\n"
    "lui        $at, %hi(gDisplayListHead)\n"
    "sw         $v0, %lo(gDisplayListHead)($at)\n"
    "lui        $at, %hi(gVtxHead)\n"
    "sw         $a0, %lo(gVtxHead)($at)\n"
    "lui        $at, %hi(gMtxHead)\n"
    "jr         $ra\n"
    " sw        $a1, %lo(gMtxHead)($at)\n"
    ".set at\n"
    ".set reorder\n"
);

void GfxPools_ReserveVtx(u32 required) {
    VtxPool *pool = &gVtxPools[gGfxPoolIndex];

    gVtxPools[gGfxPoolIndex].ptr = gVtxHead;
    VtxPool_Reserve(pool, required);
    gVtxHead = gVtxPools[gGfxPoolIndex].ptr;
}

void GfxPools_Swap(s32 arg0) {
    gGfxPools[gGfxPoolIndex].ptr = gDisplayListHead;
    GfxPool_Submit(&gGfxPools[gGfxPoolIndex], arg0);
    gGfxPoolIndex ^= 1;
    GfxPool_Reset(&gGfxPools[gGfxPoolIndex]);
    VtxPool_Reset(&gVtxPools[gGfxPoolIndex]);
    MtxPool_Reset(&gMtxPools[gGfxPoolIndex]);
    gDisplayListHead = gGfxPools[gGfxPoolIndex].ptr;
    gVtxHead = gVtxPools[gGfxPoolIndex].ptr;
    gMtxHead = gMtxPools[gGfxPoolIndex].ptr;
}

void GfxPools_Shutdown(void) {
    Gfx_WaitTasks();
    Screen_FlushFrame();
    Gfx_WaitTasks();
    GfxPool_Reset(&gGfxPools[0]);
    /* TODO (Clean version): [evaluated 2026-11-18: both calls are in the ROM; removing one
    changes it]
    The function `Gfx_WaitTasks()` is called twice consecutively around `Screen_FlushFrame()`. 
    This redundancy is likely a legacy artifact of hardware state synchronization 
    or pipeline flushing, but could be cleaned up into a single pre/post sequence if safe.
    */
    VtxPool_Reset(&gVtxPools[0]);
    MtxPool_Reset(&gMtxPools[0]);
    GfxPool_Reset(&gGfxPools[1]);
    VtxPool_Reset(&gVtxPools[1]);
    MtxPool_Reset(&gMtxPools[1]);
}
