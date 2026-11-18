#include "common.h"
#include "ultra64.h"

/* Cached ROM DMA buffer (sample data for the synthesizer). The in-use list is sorted by address. */
typedef struct AudDmaBuffer_s {
    /* 0x00 */ struct AudDmaBuffer_s *prev;
    /* 0x04 */ struct AudDmaBuffer_s *next;
    /* 0x08 */ s32 frame;      /* frames left before it can be reused */
    /* 0x0C */ u32 addr;       /* ROM address of the cached data */
    /* 0x10 */ u8 *ptr;        /* the buffer */
} AudDmaBuffer;
#include "include_asm.h"
extern AudDmaBuffer *D_80127990; /* buffers in use */
extern AudDmaBuffer *D_80127994; /* free buffers */
extern s32 D_801279A8;
extern u8 D_801279B0;

AudDmaBuffer *func_800D1D9C();
u32 func_800D1D30(u32 addr);

extern void *D_801279C8;
extern s32 D_801279A4;
extern OSIoMesg *D_8012799C;
extern AudDmaBuffer *D_80127998;
extern void *D_801279A0;
void *SndHeap_Alloc(s32);
void func_800D2450(void *, u8, s32);
s32 *func_800D1D24(void);
/* Sets up the ROM DMA cache: n buffers of size bytes in a doubly linked free list, plus the PI
 * message queue; returns the cached-DMA callback. */
void *func_800D1AE0(s32 n, s32 size) {
    s32 i;

    D_801279C8 = osCartRomInit();
    D_8012799C = SndHeap_Alloc(n * 48);
    D_801279A0 = SndHeap_Alloc(n * 8);
    D_80127998 = SndHeap_Alloc(n * 20);
    func_800D2450(D_80127998, 0, n * 20);
    for (i = 0; i < n - 1; i++) {
        D_80127998[i].next = &D_80127998[i] + 1;
        D_80127998[i].next->prev = &D_80127998[i];
        D_80127998[i].ptr = (s32)SndHeap_Alloc(size);
        D_80127998[i].addr = -1;
    }
    D_80127998[i].ptr = (s32)SndHeap_Alloc(size);
    D_80127998[i].addr = -1;
    D_801279A4 = size;
    D_801279A8 = 0;
    D_80127990 = NULL;
    D_80127994 = D_80127998;
    osCreateMesgQueue(&D_801279B0, D_801279A0, n * 2);
    return func_800D1D24;
}

/* Once per frame: drains the finished DMA messages and frees the buffers not used for 2 frames. */
void func_800D1C54(void) {
    s32 msg;
    AudDmaBuffer *b;
    AudDmaBuffer *next;

    if (D_801279A8 != 0) {
        do {
            osRecvMesg(&D_801279B0, (OSMesg *)&msg, OS_MESG_NOBLOCK);
        } while (--D_801279A8 != 0);
    }
    b = D_80127990;
    if (b != NULL) {
        do {
            if (--b->frame == 0) {
                /* unused for long enough: unlink it and put it on the free list */
                next = b->next;
                if (next != NULL) {
                    next->prev = b->prev;
                }
                if (b->prev != NULL) {
                    b->prev->next = b->next;
                } else {
                    D_80127990 = b->next;
                }
                b->prev = NULL;
                {
                    AudDmaBuffer *free = D_80127994;

                    D_80127994 = b;
                    b->next = free;
                }
                b = next;
            } else {
                b = b->next;
            }
        } while (b != NULL);
    }
}

s32 *func_800D1D24(void) {
    return (s32 *)func_800D1D30;
}

u32 func_800D1D30(u32 addr) {
    AudDmaBuffer *s = func_800D1D9C(addr);

    if (s == 0) {
        return osVirtualToPhysical((void *)addr);
    }
    if ((addr & 0xFF000000) == 0xFF000000) {
        addr &= 0xFFFFFF;
        addr += 0x140000;
    }
    return osVirtualToPhysical((void *)(s->ptr + addr - s->addr));
}

extern void *D_80138144;
extern s32 D_80145564;

AudDmaBuffer *func_800D1D9C(u32 addr, s32 len) {
    void *h;
    AudDmaBuffer *dp;
    AudDmaBuffer *last;
    AudDmaBuffer *fp;
    AudDmaBuffer *n;
    u32 end;
    OSIoMesg *m;

    if ((addr & 0xFF000000) == 0xFF000000) {
        h = D_80138144;
        addr &= 0xFFFFFF;
        addr += 0x140000;
    } else {
        if (D_80145564 & 1) {
            return NULL;
        }
        h = D_801279C8;
    }
    end = addr + len;
    last = NULL;
    for (dp = (D_80127990); dp != NULL; dp = dp->next) {
        if (addr < dp->addr) {
            break;
        }
        if (!(dp->addr + D_801279A4 < end)) {
            dp->frame = 2;
            return dp;
        }
        last = dp;
    }
    fp = (D_80127994);
    if (fp == NULL) {
        return D_80127990;
    }
    D_80127994 = fp->next;
    if (last != NULL) {
        n = last->next;
        fp->next = n;
        if (n != NULL) {
            n->prev = fp;
        }
        fp->prev = last;
        last->next = fp;
    } else {
        fp->prev = NULL;
        fp->next = (D_80127990);
        if ((D_80127990) != NULL) {
            (D_80127990)->prev = fp;
        }
        D_80127990 = fp;
    }
    fp->addr = addr & ~1;
    fp->frame = 2;
    m = &D_8012799C[D_801279A8++];
    m->hdr.pri = OS_MESG_PRI_NORMAL;
    m->hdr.retQueue = &D_801279B0;
    m->dramAddr = fp->ptr;
    m->devAddr = fp->addr;
    m->size = D_801279A4;
    osEPiStartDma(h, m, OS_READ);
    return fp;
}

