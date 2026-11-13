#include "common.h"
#include "include_asm.h"
typedef struct { u32 inst1; u32 inst2; u32 inst3; u32 inst4; } ExcVecI_;
extern ExcVecI_ __osException;
extern u32 D_8014C6F8;
extern u64 D_800FC7F0;
extern s32 D_800FC7F8;
extern s32 D_8000030C;
extern s32 D_80000300;
extern u8 D_8000031C[];
u32 __osGetSR(void);
void __osSetSR(u32);
void __osSetFpcCsr(u32);
void __osSetWatchLo(u32);
s32 __osSiRawReadIo(u32, u32 *);
s32 __osSiRawWriteIo(u32, u32);
void osWritebackDCache(void *, s32);
void osInvalICache(void *, s32);
void func_800E5100(void);
void func_800E50A0(void);
void bzero(void *, s32);
u32 __osGetCause(void);
extern s8 D_80138024;
extern s8 D_80138025;
extern s8 D_80138026;
extern s8 D_80138027;
extern s8 D_80138028;
extern s8 D_801417C4;
extern s8 D_801417C5;
extern s8 D_801417C6;
extern s8 D_801417C7;
extern s8 D_801417C8;



u32 osGetMemSize(void) {
    volatile u32 *ptr;
    u32 size = 0x400000;
    u32 data0;
    u32 data1;

    while (size < 0x800000) {
        ptr = (volatile u32 *)(0xA0000000 + size);
        data0 = *ptr;
        data1 = ptr[0x100000 / 4 - 1];
        *ptr ^= ~0;
        ptr[0x100000 / 4 - 1] ^= ~0;
        if ((*ptr != (data0 ^ ~0)) || (ptr[0x100000 / 4 - 1] != (data1 ^ ~0))) {
            return size;
        }
        *ptr = data0;
        ptr[0x100000 / 4 - 1] = data1;
        size += 0x100000;
    }
    return size;
}

void __createSpeedParam(void);
void osInitialize(void) {
    u32 pifdata;

    D_8014C6F8 = 1;
    __osSetSR(__osGetSR() | 0x20000000);
    __osSetFpcCsr(0x01000800);
    __osSetWatchLo(0x04900000);
    while (__osSiRawReadIo(0x1FC007FC, &pifdata)) {
    }
    while (__osSiRawWriteIo(0x1FC007FC, pifdata | 8)) {
    }
    *(ExcVecI_ *)0x80000000 = __osException;
    *(ExcVecI_ *)0x80000080 = __osException;
    *(ExcVecI_ *)0x80000100 = __osException;
    *(ExcVecI_ *)0x80000180 = __osException;
    osWritebackDCache((void *)0x80000000, 0x190);
    osInvalICache((void *)0x80000000, 0x190);
    __createSpeedParam();
    func_800E5100();
    func_800E50A0();
    D_800FC7F0 = D_800FC7F0 * 3 / 4;
    if (D_8000030C == 0) {
        bzero(D_8000031C, 0x40);
    }
    if (D_80000300 == 0) {
        D_800FC7F8 = 0x2F5B2D2;
    } else if (D_80000300 == 2) {
        D_800FC7F8 = 0x2E6025C;
    } else {
        D_800FC7F8 = 0x2E6D354;
    }
    if (__osGetCause() & 0x1000) {
        while (1) {
        }
    }
    *(volatile u32 *)0xA4500008 = 1;
    *(volatile u32 *)0xA4500010 = 0x3FFF;
    *(volatile u32 *)0xA4500014 = 0xF;
}

void __createSpeedParam(void) {
    D_801417C4 = 7;
    D_80138024 = 7;
    D_801417C5 = (s8) *(volatile s32 *)0xA4600014;
    D_801417C8 = (s8) *(volatile s32 *)0xA4600018;
    D_801417C6 = (s8) *(volatile s32 *)0xA460001C;
    D_801417C7 = (s8) *(volatile s32 *)0xA4600020;
    D_80138025 = (s8) *(volatile s32 *)0xA4600024;
    D_80138028 = (s8) *(volatile s32 *)0xA4600028;
    D_80138026 = (s8) *(volatile s32 *)0xA460002C;
    D_80138027 = (s8) *(volatile s32 *)0xA4600030;
}
