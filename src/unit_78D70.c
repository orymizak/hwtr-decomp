#include "common.h"
#include "include_asm.h"
typedef struct PiHandle2_ { struct PiHandle2_ *next; u8 type; u8 latency; u8 pageSize; u8 relDuration; u8 pulse; u8 domain; u8 pad[2]; u32 baseAddress; u32 speed; u8 transferInfo[0x60]; } PiHandle2_;
extern PiHandle2_ __CartRomHandle;
extern s32 D_800FB2D0;
extern PiHandle2_ *__osPiTable;
void __osPiGetAccess(void);
void __osPiRelAccess(void);
void bzero(void *p, s32 n);
u32 __osDisableInt(void);
void __osRestoreInt(u32);



PiHandle2_ *osCartRomInit(void) {
    u32 value = 0;
    u32 saveMask;
    register u32 stat;
    u32 latency;
    u32 pulse;
    u32 pageSize;
    u32 relDuration;

    __osPiGetAccess();
    if (!D_800FB2D0) {
        __osPiRelAccess();
        return &__CartRomHandle;
    }
    D_800FB2D0 = 0;
    __CartRomHandle.type = 0;
    __CartRomHandle.baseAddress = 0xB0000000;
    __CartRomHandle.domain = 0;
    __CartRomHandle.speed = 0;
    bzero(&__CartRomHandle.transferInfo, 0x60);
    stat = *(volatile u32 *)0xA4600010;
    while (stat & 3)
        stat = *(volatile u32 *)0xA4600010;
    latency = *(volatile u32 *)0xA4600014;
    pageSize = *(volatile u32 *)0xA460001C;
    relDuration = *(volatile u32 *)0xA4600020;
    pulse = *(volatile u32 *)0xA4600018;
    *(volatile u32 *)0xA4600014 = 0xFF;
    *(volatile u32 *)0xA460001C = 0;
    *(volatile u32 *)0xA4600020 = 3;
    *(volatile u32 *)0xA4600018 = 0xFF;
    value = *(volatile u32 *)(__CartRomHandle.baseAddress | 0xA0000000);
    __CartRomHandle.latency = value & 0xFF;
    __CartRomHandle.pageSize = (value >> 0x10) & 0xF;
    __CartRomHandle.relDuration = (value >> 0x14) & 0xF;
    __CartRomHandle.pulse = (value >> 8) & 0xFF;
    *(volatile u32 *)0xA4600014 = latency;
    *(volatile u32 *)0xA460001C = pageSize;
    *(volatile u32 *)0xA4600020 = relDuration;
    *(volatile u32 *)0xA4600018 = pulse;
    saveMask = __osDisableInt();
    __CartRomHandle.next = __osPiTable;
    __osPiTable = &__CartRomHandle;
    __osRestoreInt(saveMask);
    __osPiRelAccess();
    return &__CartRomHandle;
}
