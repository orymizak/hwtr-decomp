#include "common.h"
#include "include_asm.h"
typedef struct DmaBuf_ { struct DmaBuf_ *next; struct DmaBuf_ *prev; s32 count; } DmaBuf_;
s32 osRecvMesg(void *, void *,s32);
extern void **D_80127990;
extern void **D_80127994;
extern s32 D_801279A8;
extern u8 D_801279B0;

typedef struct { u8 p0[0xC]; u32 unkC; u32 unk10; } Seg_;
Seg_ *func_800D1D9C();
u32 osVirtualToPhysical(void *);
u32 func_800D1D30(u32 addr);

typedef struct DmaC_ { struct DmaC_ *prev; struct DmaC_ *next; s32 frame; u32 addr; s32 ptr; } DmaC_;
typedef struct { u8 p0[2]; u8 pri; u8 p3; void *retQueue; s32 dramAddr; u32 devAddr; s32 size; s32 piHandle; } IoMesgD_;
extern void *D_801279C8;
extern s32 D_801279A4;
extern IoMesgD_ *D_8012799C;
extern DmaC_ *D_80127998;
extern void *D_801279A0;
void *osCartRomInit(void);
void *func_800D23FC(s32);
void func_800D2450(void *, u8, s32);
void osCreateMesgQueue(void *, void *, s32);
s32 *func_800D1D24(void);
/* Sets up the ROM DMA cache: n buffers of size bytes in a doubly linked free list, plus the PI
 * message queue; returns the cached-DMA callback. */
void *func_800D1AE0(s32 n, s32 size) {
    s32 i;

    D_801279C8 = osCartRomInit();
    D_8012799C = func_800D23FC(n * 48);
    D_801279A0 = func_800D23FC(n * 8);
    D_80127998 = func_800D23FC(n * 20);
    func_800D2450(D_80127998, 0, n * 20);
    for (i = 0; i < n - 1; i++) {
        D_80127998[i].next = &D_80127998[i] + 1;
        D_80127998[i].next->prev = &D_80127998[i];
        D_80127998[i].ptr = (s32)func_800D23FC(size);
        D_80127998[i].addr = -1;
    }
    D_80127998[i].ptr = (s32)func_800D23FC(size);
    D_80127998[i].addr = -1;
    D_801279A4 = size;
    D_801279A8 = 0;
    D_80127990 = NULL;
    D_80127994 = (void **)D_80127998;
    osCreateMesgQueue(&D_801279B0, D_801279A0, n * 2);
    return func_800D1D24;
}

void func_800D1C54(void) {
    s32 msg;
    DmaBuf_ *b;
    DmaBuf_ *prev;

    if (D_801279A8 != 0) {
        do {
            osRecvMesg(&D_801279B0, &msg, 0);
        } while (--D_801279A8 != 0);
    }
    b = (DmaBuf_ *)D_80127990;
    if (b != NULL) {
        do {
            if (--b->count == 0) {
                prev = b->prev;
                if (prev != NULL) {
                    prev->next = b->next;
                }
                if (b->next != NULL) {
                    b->next->prev = b->prev;
                } else {
                    D_80127990 = (void **)b->prev;
                }
                b->next = NULL;
                {
                    DmaBuf_ *free = (DmaBuf_ *)D_80127994;

                    D_80127994 = (void **)b;
                    b->prev = free;
                }
                b = prev;
            } else {
                b = b->prev;
            }
        } while (b != NULL);
    }
}

s32 *func_800D1D24(void) {
    return (s32 *)func_800D1D30;
}

u32 func_800D1D30(u32 addr) {
    Seg_ *s = func_800D1D9C(addr);

    if (s == 0) {
        return osVirtualToPhysical((void *)addr);
    }
    if ((addr & 0xFF000000) == 0xFF000000) {
        addr &= 0xFFFFFF;
        addr += 0x140000;
    }
    return osVirtualToPhysical((void *)(s->unk10 + addr - s->unkC));
}

extern void *D_80138144;
extern s32 D_80145564;
s32 osEPiStartDma(void *, IoMesgD_ *, s32);

Seg_ *func_800D1D9C(u32 addr, s32 len) {
    void *h;
    DmaC_ *dp;
    DmaC_ *last;
    DmaC_ *fp;
    DmaC_ *n;
    u32 end;
    IoMesgD_ *m;

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
    for (dp = ((DmaC_ *)D_80127990); dp != NULL; dp = dp->next) {
        if (addr < dp->addr) {
            break;
        }
        if (!(dp->addr + D_801279A4 < end)) {
            dp->frame = 2;
            return (Seg_ *)dp;
        }
        last = dp;
    }
    fp = ((DmaC_ *)D_80127994);
    if (fp == NULL) {
        return (Seg_ *)D_80127990;
    }
    D_80127994 = (void **)fp->next;
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
        fp->next = ((DmaC_ *)D_80127990);
        if (((DmaC_ *)D_80127990) != NULL) {
            ((DmaC_ *)D_80127990)->prev = fp;
        }
        D_80127990 = (void **)fp;
    }
    fp->addr = addr & ~1;
    fp->frame = 2;
    m = &D_8012799C[D_801279A8++];
    m->pri = 0;
    m->retQueue = &D_801279B0;
    m->dramAddr = fp->ptr;
    m->devAddr = fp->addr;
    m->size = D_801279A4;
    osEPiStartDma(h, m, 0);
    return (Seg_ *)fp;
}

