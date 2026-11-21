#include "common.h"
#include "ultra64.h"
#include "include_asm.h"

extern s32 D_80127BA0;
extern s32 D_80127BA4;
extern s32 D_800FAEC0;
extern s32 D_80127BB0;
extern s32 D_80127BB4;
extern s32 D_80127BB8;
extern u32 D_80127BBC;

void SndHeap_Init(s32 arg0, s32 arg1) {
    Snd_MemSet((void *) arg0, 0U, arg1);
    alHeapInit(&(*(u8 *)&D_80127BA0), (u32) arg0, (u32) arg1);
}

void SndHeap_Alloc(s32 arg0) {
    alHeapDBAlloc(0, 0, &(*(u8 *)&D_80127BA0), 1, arg0);
}

s32 SndHeap_GetUsed(void) {
    return D_80127BA4 - D_80127BA0;
}

s32 *SndHeap_Get(void) {
    return &D_80127BA0;
}

void Snd_MemSet(void *arg0, u8 arg1, s32 arg2) {
    u8 unused[1];
    u8 *p = arg0;
    s32 i;

    for (i = arg2 - 1; i != -1; i--) {
        *p++ = arg1;
    }
}

void Snd_MemMove(u8 *dst, u8 *src, s32 n) {
    if (src < dst) {
        dst += n;
        src += n;
        for (n--; n != -1; n--) {
            *--dst = *--src;
        }
    } else {
        for (n--; n != -1; n--) {
            *dst++ = *src++;
        }
    }
}

s32 Snd_InitFrameSize(s32 a0, s32 a1, u32 a2, s32 a3) {
    u32 samples;
    u32 extra;
    s32 step;
    s32 frame;

    asm("nop");
    samples = ((u32)(((a0 * a1) + a2) - 1)) / a2;
    frame = ((samples / 184) + 1) * 184;
    extra = ((u32)(frame * a3)) / 100;
    D_80127BB0 = frame;
    step = 184;
    D_80127BB4 = frame - 184;
    D_80127BB8 = frame + step;
    D_80127BBC = extra;
    return frame + (extra + step);
}

s32 Snd_GetFrameSamples(u32 arg0) {
    if ((u32) (D_80127BBC + 0xB8) < arg0) {
        if (D_800FAEC0 != 0) {
            D_800FAEC0 = 0;
            return D_80127BB4;
        }
        goto block_7;
    }
    if (arg0 < (u32) D_80127BBC) {
        if (D_800FAEC0 != 0) {
            D_800FAEC0 = 0;
            return D_80127BB8;
        }
        goto block_7;
    }
    D_800FAEC0 = 1;
block_7:
    return D_80127BB0;
}
