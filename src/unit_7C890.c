#include "common.h"
#include "include_asm.h"
typedef struct { u8 *base; u8 *cur; s32 len; s32 count; } ALHeap_;



void alHeapInit(ALHeap_ *hp, u8 *base, s32 len) {
    s32 extraAlign = 16 - ((s32)base & 15);

    if (extraAlign != 16) {
        hp->base = base + extraAlign;
    } else {
        hp->base = base;
    }
    hp->len = len;
    hp->count = 0;
    hp->cur = hp->base;
}
