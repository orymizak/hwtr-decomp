#include "common.h"
#include "include_asm.h"
typedef struct PiHandle_ { struct PiHandle_ *next; u8 type; u8 latency; u8 pageSize; u8 relDuration; u8 pulse; u8 domain; u8 pad[2]; u32 baseAddress; } PiHandle_;
extern PiHandle_ *D_800FB2C0[];
extern u32 D_80000308;
u32 osVirtualToPhysical(void *);
extern s32 D_800FB2A0;
extern void *D_800FB2A8;



s32 __osPiRawStartDma(s32 direction, u32 devAddr, void *dramAddr, u32 size) {
    register u32 stat;

    while (stat = *(volatile u32 *)0xA4600010, stat & 3) {
    }
    *(volatile u32 *)0xA4600000 = osVirtualToPhysical(dramAddr);
    *(volatile u32 *)0xA4600004 = (D_80000308 | devAddr) & 0x1FFFFFFF;
    switch (direction) {
    case 0:
        *(volatile u32 *)0xA460000C = size - 1;
        break;
    case 1:
        *(volatile u32 *)0xA4600008 = size - 1;
        break;
    default:
        return -1;
    }
    return 0;
}

void *osPiGetCmdQueue(void) {
    if (!D_800FB2A0) {
        return NULL;
    }
    return D_800FB2A8;
}

s32 __osEPiRawReadIo(PiHandle_ *pihandle, u32 devAddr, u32 *data) {
    u32 stat;
    u32 domain;

    while (stat = *(volatile u32 *)0xA4600010, stat & 3) {
    }
    domain = pihandle->domain;
    if (D_800FB2C0[domain]->type != pihandle->type) {
        PiHandle_ *cHandle = D_800FB2C0[domain];

        if (domain == 0) {
            if (cHandle->latency != pihandle->latency) {
                *(volatile u32 *)0xA4600014 = pihandle->latency;
            }
            if (cHandle->pageSize != pihandle->pageSize) {
                *(volatile u32 *)0xA460001C = pihandle->pageSize;
            }
            if (cHandle->relDuration != pihandle->relDuration) {
                *(volatile u32 *)0xA4600020 = pihandle->relDuration;
            }
            if (cHandle->pulse != pihandle->pulse) {
                *(volatile u32 *)0xA4600018 = pihandle->pulse;
            }
        } else {
            if (cHandle->latency != pihandle->latency) {
                *(volatile u32 *)0xA4600024 = pihandle->latency;
            }
            if (cHandle->pageSize != pihandle->pageSize) {
                *(volatile u32 *)0xA460002C = pihandle->pageSize;
            }
            if (cHandle->relDuration != pihandle->relDuration) {
                *(volatile u32 *)0xA4600030 = pihandle->relDuration;
            }
            if (cHandle->pulse != pihandle->pulse) {
                *(volatile u32 *)0xA4600028 = pihandle->pulse;
            }
        }
        cHandle->type = pihandle->type;
        cHandle->latency = pihandle->latency;
        cHandle->pageSize = pihandle->pageSize;
        cHandle->relDuration = pihandle->relDuration;
        cHandle->pulse = pihandle->pulse;
    }
    *data = *(volatile u32 *)((pihandle->baseAddress | devAddr) | 0xA0000000);
    return 0;
}

