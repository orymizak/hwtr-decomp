#include "common.h"
#include "include_asm.h"
extern s32 D_800FB5E4;
extern s32 D_800FB5E8;
void osWritebackDCacheAll(void);
void osSpTaskLoad(void *task);
void osSpTaskStartGo(void *task);
s32 osDpSetNextBuffer(void *bufPtr, u64 size);
extern s32 D_800FB5EC;
s32 osSendMesg(void *mq, void *msg, s32 flag);
void osViBlack(u8 active);
void osViSwapBuffer(void *frameBufPtr);
typedef struct OSScTask_ { struct OSScTask_ *next; u32 state; u32 flags; void *framebuffer; u32 type; u32 pad14[9]; void *output_buff; u64 *output_buff_size; u32 pad40[4]; void *msgQ; void *msg; } OSScTask_;
typedef struct { u8 pad[0x260]; void *clientList; OSScTask_ *audioListHead; OSScTask_ *gfxListHead; OSScTask_ *audioListTail; OSScTask_ *gfxListTail; OSScTask_ *curRSPTask; OSScTask_ *curRDPTask; u32 frameCount; s32 doAudio; } OSSchedT_;
typedef struct OSScClient_ { struct OSScClient_ *next; void *msgQ; } OSScClient_;
typedef struct { u8 pad[0x260]; OSScClient_ *clientList; } OSSchedC_;
u32 osVirtualToPhysical(void *);
void osWritebackDCache(void *, s32);
void osInvalDCache(void *, s32);
u32 __scSchedule(s32, void *, void *, u32);

s32 osSetIntMask(s32);
s32 osViGetCurrentFramebuffer();
s32 osViGetNextFramebuffer();
s32 osSpTaskYield(void *);

INCLUDE_ASM("asm/nonmatchings/unit_7F260", osCreateScheduler);

void osScAddClient(void *arg0, void *arg1, s32 arg2) {
    s32 temp_v0;

    temp_v0 = osSetIntMask(1);
    *(s32 *)((u8 *)arg1 + 0x4) = arg2;
    *(s32 *)((u8 *)arg1 + 0x0) = (void *) *(s32 *)((u8 *)arg0 + 0x260);
    *(s32 *)((u8 *)arg0 + 0x260) = arg1;
    osSetIntMask(temp_v0);
}

void osScRemoveClient(OSSchedC_ *sc, OSScClient_ *c) {
    OSScClient_ *client = sc->clientList;
    OSScClient_ *prev = 0;
    s32 mask;

    mask = osSetIntMask(1);
    while (client != 0) {
        if (client == c) {
            if (prev)
                prev->next = c->next;
            else
                sc->clientList = c->next;
            break;
        }
        prev = client;
        client = client->next;
    }
    osSetIntMask(mask);
}


s32 osScGetCmdQ(s32 arg0) {
    return arg0 + 0x78;
}

INCLUDE_ASM("asm/nonmatchings/unit_7F260", __scMain);

INCLUDE_ASM("asm/nonmatchings/unit_7F260", __scHandleRetrace);

INCLUDE_ASM("asm/nonmatchings/unit_7F260", __scHandleRSP);

void __scHandleRDP(s32 arg0) {
    s32 sp10;
    s32 sp14;
    u32 var_s0;
    void *temp_a1;

    temp_a1 = (*(s32 *)((u8 *)(arg0) + 0x278));
    sp10 = 0;
    sp14 = 0;
    (*(s32 *)((u8 *)(arg0) + 0x278)) = NULL;
    (*(s32 *)((u8 *)(temp_a1) + 0x4)) = (s32) ((*(s32 *)((u8 *)(temp_a1) + 0x4)) & ~1);
    __scTaskComplete(arg0, (s32) temp_a1);
    var_s0 = ((*(s32 *)((u8 *)(arg0) + 0x274)) == 0) * 2;
    if ((*(s32 *)((u8 *)(arg0) + 0x278)) == NULL) {
        var_s0 |= 1;
    }
    if (__scSchedule(arg0, &sp10, &sp14, var_s0) != var_s0) {
        __scExec(arg0, sp10, sp14);
    }
}

s32 __scTaskReady(s32 arg0) {
    s32 temp_s0;

    if (arg0 != 0) {
        temp_s0 = osViGetCurrentFramebuffer();
        return arg0 & -(temp_s0 == osViGetNextFramebuffer());
    }
    return 0;
}

s32 __scTaskComplete(void *sc, OSScTask_ *t) {
    s32 rv;

    if ((t->state & 3) == 0) {
        rv = osSendMesg(t->msgQ, t->msg, 1);
        if (t->type == 1) {
            if ((t->flags & 0x40) && (t->flags & 0x20)) {
                if (D_800FB5EC) {
                    osViBlack(0);
                    D_800FB5EC = 0;
                }
                osViSwapBuffer(t->framebuffer);
            }
        }
        return 1;
    }
    return 0;
}


void __scAppendList(OSSchedT_ *sc, OSScTask_ *t) {
    long type = t->type;

    if (type == 2) {
        if (sc->audioListTail)
            sc->audioListTail->next = t;
        else
            sc->audioListHead = t;
        sc->audioListTail = t;
        sc->doAudio = 1;
    } else {
        if (sc->gfxListTail)
            sc->gfxListTail->next = t;
        else
            sc->gfxListHead = t;
        sc->gfxListTail = t;
    }
    t->next = 0;
    t->state = t->flags & 3;
}


void __scExec(OSSchedT_ *sc, OSScTask_ *sp, OSScTask_ *dp) {
    s32 rv;

    if (sp) {
        if (sp->type == 2) {
            osWritebackDCacheAll();
        }
        sp->state &= ~(0x10 | 0x20);
        osSpTaskLoad(&sp->type);
        osSpTaskStartGo(&sp->type);
        sc->curRSPTask = sp;
        if (sp == dp)
            sc->curRDPTask = dp;
    }
    if (dp && (dp != sp)) {
        rv = osDpSetNextBuffer(dp->output_buff, *dp->output_buff_size);
        D_800FB5E4 = 1;
        D_800FB5E8 = 0;
        sc->curRDPTask = dp;
    }
}

void __scYield(void *arg0) {
    void *temp_a0;

    temp_a0 = *(s32 *)((u8 *)arg0 + 0x274);
    if (*(s32 *)((u8 *)temp_a0 + 0x10) == 1) {
        *(s32 *)((u8 *)temp_a0 + 0x4) = (s32) (*(s32 *)((u8 *)temp_a0 + 0x4) | 0x10);
        osSpTaskYield(temp_a0);
    }
}

INCLUDE_ASM("asm/nonmatchings/unit_7F260", __scSchedule);

s32 __osSiRawStartDma(s32 direction, void *dramAddr) {
    if (*(volatile u32 *)0xA4800018 & 3) {
        return -1;
    }
    if (direction == 1) {
        osWritebackDCache(dramAddr, 64);
    }
    *(volatile u32 *)0xA4800000 = osVirtualToPhysical(dramAddr);
    if (direction == 0) {
        *(volatile u32 *)0xA4800004 = 0x1FC007C0;
    } else {
        *(volatile u32 *)0xA4800010 = 0x1FC007C0;
    }
    if (direction == 0) {
        osInvalDCache(dramAddr, 64);
    }
    return 0;
}
