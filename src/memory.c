#include "common.h"

typedef struct {
    void *first;
} Heap;

extern Heap D_80138B18;
extern Heap D_80138D2C;
extern Heap D_8014C0A0;
extern u8 D_8014ECD0[];
extern u8 D_800F21A0;
extern u8 D_800F21A1;

void Heap_Clear(Heap *heap);
void *Heap_Alloc(Heap *heap, u32 size);
s32 Heap_Free(Heap *heap, void *ptr);
void Heap_AddRegion(Heap *heap, void *start, u32 size);
u32 osGetMemSize(void);

void Memory_InitMain(void) {
    Heap_Clear(&D_80138D2C);
    Heap_AddRegion(&D_80138D2C, D_8014ECD0, 0x8023E000 - (u32)D_8014ECD0);
}

inline void Memory_InitSecondary(void) {
    Heap_Clear(&D_80138B18);
    Heap_AddRegion(&D_80138B18, (void *)0x80263800, 0x151800);
}

    /* TODO (Clean version):
    The threshold `0x400000` (4 MB) represents the standard N64 base RAM size, 
    and anything above it targets the Expansion Pak. These magic numbers should 
    be defined using descriptive macros:
    #define N64_BASE_RAM_SIZE (4 * 1024 * 1024)
    #define EXPANSION_PAK_START 0x80400000
    */
inline void Memory_InitExpansion(u32 memSize) {
    if (memSize > 0x400000) {
        Heap_Clear(&D_8014C0A0);
        Heap_AddRegion(&D_8014C0A0, (void *)0x80400000, memSize - 0x400000);
    }
}

void Memory_DisableSecondary(void) {
    D_800F21A0 = 0;
}

void Memory_EnableSecondary(void) {
    Memory_InitSecondary();
    D_800F21A0 = 1;
}

void Memory_DetectExpansion(void) {
    u32 memSize = osGetMemSize();

    if (memSize > 0x400000) {
        Memory_InitExpansion(memSize);
        D_800F21A1 = 1;
    } else {
        D_800F21A1 = 0;
    }
}

    /* TODO (Clean version):
    The tiered fallback allocator checks global flags (`D_800F21A0`, `D_800F21A1`) inline. 
    A cleaner design would iterate through an array or list of active heap pools 
    registered during initialization, avoiding rigid hardcoded conditional chains.
    */
void *Memory_Alloc(u32 size) {
    void *ptr = Heap_Alloc(&D_80138D2C, size);

    if (D_800F21A0 == 1 && ptr == NULL) {
        ptr = Heap_Alloc(&D_80138B18, size);
    }
    if (D_800F21A1 == 1 && ptr == NULL) {
        ptr = Heap_Alloc(&D_8014C0A0, size);
    }
    return ptr;
}

    /* TODO (Clean version):
    Hardcoded memory address ranges (`0x80263800`, `0x151800`, `0x803FFFFF`) 
    used to route `Memory_Free` to the correct heap are error-prone. 
    They should be defined using clear boundary constants or encapsulated 
    into explicit memory pool descriptor ranges:
    #define SECONDARY_HEAP_START 0x80263800
    #define SECONDARY_HEAP_SIZE  0x151800
    */
void Memory_Free(void *ptr) {
    if (((u32)ptr - 0x80263800 > 0x151800) && ((u32)ptr <= 0x803FFFFF)) {
        Heap_Free(&D_80138D2C, ptr);
    } else if (!((u32)ptr - 0x80263800 > 0x151800)) {
        Heap_Free(&D_80138B18, ptr);
    } else {
        Heap_Free(&D_8014C0A0, ptr);
    }
}
