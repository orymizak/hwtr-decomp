#include "common.h"
#include "ultra64.h"

typedef struct {
    void *first;
} Heap;

extern Heap gSecondaryHeap;
extern Heap gMainHeap;
extern Heap gExpansionHeap;
extern u8 D_8014ECD0[];
extern u8 gSecondaryHeapOn;
extern u8 gExpansionHeapOn;

void Heap_Clear(Heap *heap);
void *Heap_Alloc(Heap *heap, u32 size);
s32 Heap_Free(Heap *heap, void *ptr);
void Heap_AddRegion(Heap *heap, void *start, u32 size);

/* RDRAM layout of the heaps. */
#define MAIN_HEAP_END 0x8023E000         /* main heap: end of BSS up to here */
#define SECONDARY_HEAP_START 0x80263800  /* optional second region, toggled at run time */
#define SECONDARY_HEAP_SIZE 0x151800
#define RAM_END_4MB 0x803FFFFF           /* last byte of the base 4 MB */
#define BASE_RAM_SIZE 0x400000           /* 4 MB; anything above is the Expansion Pak */
#define EXPANSION_RAM_START 0x80400000

void Memory_InitMain(void) {
    Heap_Clear(&gMainHeap);
    Heap_AddRegion(&gMainHeap, D_8014ECD0, MAIN_HEAP_END - (u32)D_8014ECD0);
}

inline void Memory_InitSecondary(void) {
    Heap_Clear(&gSecondaryHeap);
    Heap_AddRegion(&gSecondaryHeap, (void *)SECONDARY_HEAP_START, SECONDARY_HEAP_SIZE);
}

inline void Memory_InitExpansion(u32 memSize) {
    if (memSize > BASE_RAM_SIZE) {
        Heap_Clear(&gExpansionHeap);
        Heap_AddRegion(&gExpansionHeap, (void *)EXPANSION_RAM_START, memSize - BASE_RAM_SIZE);
    }
}

void Memory_DisableSecondary(void) {
    gSecondaryHeapOn = 0;
}

void Memory_EnableSecondary(void) {
    Memory_InitSecondary();
    gSecondaryHeapOn = 1;
}

void Memory_DetectExpansion(void) {
    u32 memSize = osGetMemSize();

    if (memSize > BASE_RAM_SIZE) {
        Memory_InitExpansion(memSize);
        gExpansionHeapOn = 1;
    } else {
        gExpansionHeapOn = 0;
    }
}

    /* TODO (Clean version): [evaluated 2026-11-18: design note; a heap list would change the
    ROM. The flags are now gSecondaryHeapOn/gExpansionHeapOn]
    The tiered fallback allocator checks global flags (`gSecondaryHeapOn`, `gExpansionHeapOn`) inline. 
    A cleaner design would iterate through an array or list of active heap pools 
    registered during initialization, avoiding rigid hardcoded conditional chains.
    */
void *Memory_Alloc(u32 size) {
    void *ptr = Heap_Alloc(&gMainHeap, size);

    if (gSecondaryHeapOn == 1 && ptr == NULL) {
        ptr = Heap_Alloc(&gSecondaryHeap, size);
    }
    if (gExpansionHeapOn == 1 && ptr == NULL) {
        ptr = Heap_Alloc(&gExpansionHeap, size);
    }
    return ptr;
}

void Memory_Free(void *ptr) {
    if (((u32)ptr - SECONDARY_HEAP_START > SECONDARY_HEAP_SIZE) && ((u32)ptr <= RAM_END_4MB)) {
        Heap_Free(&gMainHeap, ptr);
    } else if (!((u32)ptr - SECONDARY_HEAP_START > SECONDARY_HEAP_SIZE)) {
        Heap_Free(&gSecondaryHeap, ptr);
    } else {
        Heap_Free(&gExpansionHeap, ptr);
    }
}
