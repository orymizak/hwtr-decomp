#include "common.h"
#include "include_asm.h"
typedef struct {
    /* 0x00 */ u32 size;
    /* 0x04 */ u8 unk04[0x1C];
} HeapBlock; /* size = HEAP_HDR_SIZE */

typedef struct {
    /* 0x00 */ HeapBlock *first;
} Heap;

/* Each block starts with a HEAP_HDR_SIZE-byte header; size is the payload size, bit 0 = in use.
   A header with size 0 ends the heap. */
#define HEAP_HDR_SIZE 0x20
#define HEAP_USED 1
#define HEAP_SIZE(size) ((size) & ~HEAP_USED)

void Heap_Clear(Heap *heap) {
    heap->first = NULL;
}

INCLUDE_ASM("asm/nonmatchings/heap", Heap_Alloc);

s32 Heap_Free(Heap *heap, void *ptr) {
    HeapBlock *cur;
    HeapBlock *prev;
    HeapBlock *b;
    u32 n;

    if (heap->first == NULL) {
        return 0;
    }
    if (ptr == NULL) {
        return 0;
    }
    b = (HeapBlock *)((u8 *)ptr - HEAP_HDR_SIZE);
    cur = heap->first;
    prev = cur;
    while (cur != b) {
        prev = cur;
        cur = (HeapBlock *)((u8 *)cur + HEAP_SIZE(cur->size) + HEAP_HDR_SIZE);
    }
    if (cur != heap->first) {
        if (!(prev->size & HEAP_USED)) {
            prev->size += HEAP_HDR_SIZE + HEAP_SIZE(cur->size);
            cur = prev;
        } else {
            cur->size &= ~HEAP_USED;
        }
    } else {
        prev->size &= ~HEAP_USED;
    }
    prev = (HeapBlock *)(HEAP_SIZE(cur->size) + (u32)cur);
    n = prev[1].size;
    if (n != 0 && !(n & HEAP_USED)) {
        cur->size += HEAP_HDR_SIZE + HEAP_SIZE(n);
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
        while HEAP_SIZE(b->size) {
            b = (HeapBlock *)((u8 *)b + HEAP_SIZE(b->size) + HEAP_HDR_SIZE);
        }
        b->size = (base - (u32)b - HEAP_HDR_SIZE) | HEAP_USED;
    } else {
        heap->first = (HeapBlock *)base;
    }
    b = (HeapBlock *)base;
    b->size = size - 2 * HEAP_HDR_SIZE; /* header + end marker */
    ((HeapBlock *)((u8 *)b + b->size))[1].size = 0;
    /* TODO (Clean version / Bug check): [evaluated 2026-11-18: the header size and bit 0 are now
    HEAP_HDR_SIZE/HEAP_USED/HEAP_SIZE. Point 1: `size == 0` alone changes the ROM, so the
    original test (maybe a mangled `size < 0x40`) stays. Point 2 kept as is.]
    1. The condition `if (size >= 0xFFFFFFC0 && size == 0)` contains a logical contradiction 
    (a value cannot be both greater than or equal to a huge threshold and equal to 0). 
    This is likely a flawed check for underflow or invalid region sizes and should be 
    cleaned up (e.g., `if (size == 0)` or checking for negative/wrapped sizes).
    2. Setting boundary sentinels via raw array offset indexing past the block payload 
    (`((HeapBlock *)((u8 *)b + b->size))[1].size = 0`) should be encapsulated into an 
    explicit helper function or macro like `Heap_SetBoundaryFooter(b, 0)`.
    */
}
