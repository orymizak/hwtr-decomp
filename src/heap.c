#include "common.h"
#include "include_asm.h"

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

INCLUDE_ASM("asm/nonmatchings/heap", Heap_Free);

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
}
