#include "common.h"
#include "include_asm.h"
typedef struct { u32 size; u8 unk04[0x1C]; } HBlk_;
typedef struct { HBlk_ *first; } HHeap_;

typedef struct {
    /* 0x00 */ u32 size;
    /* 0x04 */ u8 unk04[0x1C];
} HeapBlock; /* size = 0x20 */

typedef struct {
    /* 0x00 */ HeapBlock *first;
} Heap;

void Heap_Clear(Heap *heap) {
    heap->first = NULL;
}

INCLUDE_ASM("asm/nonmatchings/heap", Heap_Alloc);

s32 Heap_Free(HHeap_ *heap, void *ptr) {
    HBlk_ *cur;
    HBlk_ *prev;
    HBlk_ *b;
    u32 n;
    /* TODO (Clean version):
    Heap block headers and footers use custom tag bits (e.g., bit 0 for free/allocated status) 
    and explicit 0x20 byte offsets. This raw footer inspection logic (`prev[1].size`) 
    should be encapsulated into helper macros or inline functions for safety and clarity:
    #define HEAP_BLOCK_HEADER_SIZE 0x20
    #define HEAP_IS_BUSY(size) ((size) & 1)
    #define HEAP_GET_SIZE(size) ((size) & ~1)
    */

    if (heap->first == NULL) {
        return 0;
    }
    if (ptr == NULL) {
        return 0;
    }
    b = (HBlk_ *)((u8 *)ptr - 0x20);
    cur = heap->first;
    prev = cur;
    while (cur != b) {
        prev = cur;
        cur = (HBlk_ *)((u8 *)cur + (cur->size & ~1) + 0x20);
    }
    if (cur != heap->first) {
        if (!(prev->size & 1)) {
            prev->size += 0x20 + (cur->size & ~1);
            cur = prev;
        } else {
            cur->size &= ~1;
        }
    } else {
        prev->size &= ~1;
    }
    prev = (HBlk_ *)((cur->size & ~1) + (u32)cur);
    n = prev[1].size;
    if (n != 0 && !(n & 1)) {
        cur->size += 0x20 + (n & ~1);
    }
    return 0;
}

void Heap_AddRegion(Heap *heap, u32 base, u32 size) {
    HeapBlock *b;
    u32 orig = base;

    base = (base + 31) & ~31;
    size = (size - (base - orig)) & ~31;
    if (size >= 0xFFFFFFC0 && size == 0) {
        return;
    }
    if (heap->first != NULL) {
        b = heap->first;
        while (b->size & ~1) {
            b = (HeapBlock *)((u8 *)b + (b->size & ~1) + 0x20);
        }
        b->size = (base - (u32)b - 0x20) | 1;
    } else {
        heap->first = (HeapBlock *)base;
    }
    b = (HeapBlock *)base;
    b->size = size - 0x40;
    ((HeapBlock *)((u8 *)b + b->size))[1].size = 0;
    /* TODO (Clean version / Bug check):
    1. The condition `if (size >= 0xFFFFFFC0 && size == 0)` contains a logical contradiction 
    (a value cannot be both greater than or equal to a huge threshold and equal to 0). 
    This is likely a flawed check for underflow or invalid region sizes and should be 
    cleaned up (e.g., `if (size == 0)` or checking for negative/wrapped sizes).
    2. Setting boundary sentinels via raw array offset indexing past the block payload 
    (`((HeapBlock *)((u8 *)b + b->size))[1].size = 0`) should be encapsulated into an 
    explicit helper function or macro like `Heap_SetBoundaryFooter(b, 0)`.
    */
}
