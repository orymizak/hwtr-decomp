#include "common.h"
#include "include_asm.h"
typedef struct { u8 *base; u8 *cur; s32 len; s32 count; } ALHeap_;



void *alHeapDBAlloc(u8 *file, s32 line, ALHeap_ *hp, s32 num, s32 size) {
    s32 bytes;
    u8 *ptr = 0;

    bytes = (num * size + 0xF) & ~0xF;
    if ((hp->cur + bytes) <= (hp->base + hp->len)) {
        ptr = hp->cur;
        hp->cur += bytes;
    }
    return ptr;
}
