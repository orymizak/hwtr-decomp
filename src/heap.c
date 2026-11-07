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

INCLUDE_ASM("asm/nonmatchings/heap", Heap_AddRegion);
