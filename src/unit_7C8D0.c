#include "common.h"
#include "include_asm.h"
#include "audio/synthInternals.h"
#include <libaudio.h>
#include <os_internal.h>
#include <ultraerror.h>


void *alHeapDBAlloc(u8 *file, s32 line, ALHeap *hp, s32 num, s32 size)
{
    s32 bytes;
    u8 *ptr = 0;

    bytes = (num*size + AL_CACHE_ALIGN) & ~AL_CACHE_ALIGN;
    
    
    if ((hp->cur + bytes) <= (hp->base + hp->len)) {

        ptr = hp->cur;
        hp->cur += bytes;


    } else {
    }

    return ptr;
}
