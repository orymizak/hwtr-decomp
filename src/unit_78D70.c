#include "common.h"
#include "include_asm.h"
typedef struct PiHandle2_ { struct PiHandle2_ *next; u8 type; u8 latency; u8 pageSize; u8 relDuration; u8 pulse; u8 domain; u8 pad[2]; u32 baseAddress; u32 speed; u8 transferInfo[0x60]; } PiHandle2_;
extern PiHandle2_ D_80141600;
extern s32 D_800FB2D0;
extern PiHandle2_ *D_800FB2BC;
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
        return &D_80141600;
    }
    D_800FB2D0 = 0;
    D_80141600.type = 0;
    D_80141600.baseAddress = 0xB0000000;
    D_80141600.domain = 0;
    D_80141600.speed = 0;
    bzero(&D_80141600.transferInfo, 0x60);
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
    value = *(volatile u32 *)(D_80141600.baseAddress | 0xA0000000);
    D_80141600.latency = value & 0xFF;
    D_80141600.pageSize = (value >> 0x10) & 0xF;
    D_80141600.relDuration = (value >> 0x14) & 0xF;
    D_80141600.pulse = (value >> 8) & 0xFF;
    *(volatile u32 *)0xA4600014 = latency;
    *(volatile u32 *)0xA460001C = pageSize;
    *(volatile u32 *)0xA4600020 = relDuration;
    *(volatile u32 *)0xA4600018 = pulse;
    saveMask = __osDisableInt();
    D_80141600.next = D_800FB2BC;
    D_800FB2BC = &D_80141600;
    __osRestoreInt(saveMask);
    __osPiRelAccess();
    return &D_80141600;
}
