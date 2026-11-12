#include "common.h"
#include "include_asm.h"
s32 osRecvMesg(void *, void *,s32);
extern void **D_80127990;
extern void **D_80127994;
extern s32 D_801279A8;
extern u8 D_801279B0;

typedef struct { u8 p0[0xC]; u32 unkC; u32 unk10; } Seg_;
Seg_ *func_800D1D9C();
u32 osVirtualToPhysical(void *);
u32 func_800D1D30(u32 addr);

INCLUDE_ASM("asm/nonmatchings/unit_726E0", func_800D1AE0);

void func_800D1C54(void) {
    s32 sp10;
    s32 temp_v0;
    s32 temp_v0_2;
    void **temp_a1;
    void **temp_v0_3;
    void **var_v1;
    void *temp_a0;

    if (D_801279A8 != 0) {
        do {
            osRecvMesg(&D_801279B0, &sp10, 0);
            temp_v0 = D_801279A8 - 1;
            D_801279A8 = temp_v0;
        } while (temp_v0 != 0);
    }
    var_v1 = D_80127990;
    if (var_v1 != NULL) {
        do {
            temp_v0_2 = (*(s32 *)((u8 *)(var_v1) + 0x8)) - 1;
            (*(s32 *)((u8 *)(var_v1) + 0x8)) = temp_v0_2;
            if (temp_v0_2 == 0) {
                temp_a1 = (*(s32 *)((u8 *)(var_v1) + 0x4));
                if (temp_a1 != NULL) {
                    *temp_a1 = (*(s32 *)((u8 *)(var_v1) + 0x0));
                }
                temp_a0 = (*(s32 *)((u8 *)(var_v1) + 0x0));
                if (temp_a0 != NULL) {
                    (*(s32 *)((u8 *)(temp_a0) + 0x4)) = (void **) (*(s32 *)((u8 *)(var_v1) + 0x4));
                } else {
                    D_80127990 = (*(s32 *)((u8 *)(var_v1) + 0x4));
                }
                temp_v0_3 = D_80127994;
                (*(s32 *)((u8 *)(var_v1) + 0x0)) = NULL;
                D_80127994 = var_v1;
                (*(s32 *)((u8 *)(var_v1) + 0x4)) = temp_v0_3;
                var_v1 = temp_a1;
            } else {
                var_v1 = (*(s32 *)((u8 *)(var_v1) + 0x4));
            }
        } while (var_v1 != NULL);
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

typedef struct DmaC_ { struct DmaC_ *prev; struct DmaC_ *next; s32 frame; u32 addr; s32 ptr; } DmaC_;
typedef struct { u8 p0[2]; u8 pri; u8 p3; void *retQueue; s32 dramAddr; u32 devAddr; s32 size; s32 piHandle; } IoMesgD_;
extern void *D_80138144;
extern s32 D_80145564;
extern void *D_801279C8;
extern s32 D_801279A4;
extern IoMesgD_ *D_8012799C;
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

