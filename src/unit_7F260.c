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
typedef struct { s16 retraceMsg; u8 p2[0x1E]; s16 prenmiMsg; u8 p22[0x1E]; u8 interruptQ[0x18]; void *intBuf[8]; u8 cmdQ[0x18]; void *cmdMsgBuf[8]; u8 thread[0x1B0]; OSScClient_ *clientList; OSScTask_ *audioListHead; OSScTask_ *gfxListHead; OSScTask_ *audioListTail; OSScTask_ *gfxListTail; OSScTask_ *curRSPTask; OSScTask_ *curRDPTask; u32 frameCount; s32 doAudio; } OSSched_;
typedef struct { u8 b[0x50]; } OSViMode_;
extern OSViMode_ D_800FB650[];
void osCreateMesgQueue(void *mq, void **msg, s32 count);
void osCreateViManager(s32 pri);
void osViSetMode(OSViMode_ *mode);
void osSetEventMesg(s32 e, void *mq, void *msg);
void osViSetEvent(void *mq, void *msg, u32 retraceCount);
void osCreateThread(void *t, s32 id, void (*entry)(void *), void *arg, void *sp, s32 pri);
void osStartThread(void *t);
s32 osRecvMesg(void *mq, void **msg, s32 flag);
s32 osSpTaskYielded(void *task);
void __scMain(void *arg);
void __scHandleRetrace(OSSched_ *sc);
void __scHandleRSP(OSSched_ *sc);
void __scHandleRDP(s32 arg0);
s32 __scTaskComplete(void *sc, OSScTask_ *t);
void __scAppendList(OSSchedT_ *sc, OSScTask_ *t);
void __scExec(OSSchedT_ *sc, OSScTask_ *sp, OSScTask_ *dp);
void __scYield(void *arg0);
u32 osVirtualToPhysical(void *);
void osWritebackDCache(void *, s32);
void osInvalDCache(void *, s32);
s32 __scSchedule(OSSched_ *sc, OSScTask_ **sp, OSScTask_ **dp, s32 availRCP);

s32 osSetIntMask(s32);
s32 osViGetCurrentFramebuffer(void);
s32 osViGetNextFramebuffer(void);
s32 osSpTaskYield(void *);

void osCreateScheduler(OSSched_ *sc, void *stack, s32 priority, u8 mode, u8 numFields) {
    sc->curRSPTask = 0;
    sc->curRDPTask = 0;
    sc->clientList = 0;
    sc->frameCount = 0;
    sc->audioListHead = 0;
    sc->gfxListHead = 0;
    sc->audioListTail = 0;
    sc->gfxListTail = 0;
    sc->retraceMsg = 1;
    sc->prenmiMsg = 4;
    osCreateMesgQueue(&sc->interruptQ, sc->intBuf, 8);
    osCreateMesgQueue(&sc->cmdQ, sc->cmdMsgBuf, 8);
    osCreateViManager(0xFE);
    osViSetMode(&D_800FB650[mode]);
    osViBlack(1);
    osSetEventMesg(4, &sc->interruptQ, (void *)667);
    osSetEventMesg(9, &sc->interruptQ, (void *)668);
    osSetEventMesg(14, &sc->interruptQ, (void *)669);
    osViSetEvent(&sc->interruptQ, (void *)666, numFields);
    osCreateThread(&sc->thread, 4, __scMain, (void *)sc, stack, priority);
    osStartThread(&sc->thread);
}

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

void __scMain(void *arg) {
    void *msg;
    OSSched_ *sc = (OSSched_ *)arg;
    OSScClient_ *client;

    while (1) {
        osRecvMesg(&sc->interruptQ, &msg, 1);
        switch ((s32)msg) {
            case 666:
                __scHandleRetrace(sc);
                break;
            case 667:
                __scHandleRSP(sc);
                break;
            case 668:
                __scHandleRDP(sc);
                break;
            case 669:
                for (client = sc->clientList; client != 0; client = client->next) {
                    osSendMesg(client->msgQ, &sc->prenmiMsg, 0);
                }
                break;
        }
    }
}

void __scHandleRetrace(OSSched_ *sc) {
    OSScTask_ *rspTask;
    OSScClient_ *client;
    s32 i;
    s32 state;
    OSScTask_ *sp = 0;
    OSScTask_ *dp = 0;

    sc->frameCount++;
    while (osRecvMesg(&sc->cmdQ, (void **)&rspTask, 0) != -1) {
        __scAppendList(sc, rspTask);
    }
    if (sc->doAudio && sc->curRSPTask) {
        __scYield(sc);
    } else {
        state = (sc->curRSPTask == 0) * 2;
    if (sc->curRDPTask == 0) {
        state |= 1;
    }
        if (__scSchedule(sc, &sp, &dp, state) != state)
            __scExec(sc, sp, dp);
    }
    for (client = sc->clientList; client != 0; client = client->next) {
        osSendMesg(client->msgQ, &sc->retraceMsg, 0);
    }
}

void __scHandleRSP(OSSched_ *sc) {
    OSScTask_ *t, *sp = 0, *dp = 0;
    s32 state;

    t = sc->curRSPTask;
    sc->curRSPTask = 0;
    if ((t->state & 0x10) && osSpTaskYielded(&t->type)) {
        t->state |= 0x20;
        if ((t->flags & 7) == 3) {
            t->next = sc->gfxListHead;
            sc->gfxListHead = t;
            if (sc->gfxListTail == 0)
                sc->gfxListTail = t;
        }
    } else {
        t->state &= ~2;
        __scTaskComplete(sc, t);
    }
    state = (sc->curRSPTask == 0) * 2;
    if (sc->curRDPTask == 0) {
        state |= 1;
    }
    if ((__scSchedule(sc, &sp, &dp, state)) != state)
        __scExec(sc, sp, dp);
}

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

s32 __scSchedule(OSSched_ *sc, OSScTask_ **sp, OSScTask_ **dp, s32 availRCP) {
    s32 avail = availRCP;
    OSScTask_ *gfx = sc->gfxListHead;
    OSScTask_ *audio = sc->audioListHead;
    s32 ready;
    s32 cur;

    if (sc->doAudio && (avail & 2)) {
        if (gfx && (gfx->flags & 0x10)) {
            *sp = gfx;
            avail &= ~2;
        } else {
            *sp = audio;
            avail &= ~2;
            sc->doAudio = 0;
            sc->audioListHead = sc->audioListHead->next;
            if (sc->audioListHead == 0)
                sc->audioListTail = 0;
        }
    } else {
        if (gfx != 0) {
            cur = osViGetCurrentFramebuffer();
            ready = (s32)gfx & -(cur == osViGetNextFramebuffer());
        } else {
            ready = 0;
        }
        if (ready) {
            switch (gfx->flags & 7) {
                case 3:
                    if (gfx->state & 0x20) {
                        if (avail & 2) {
                            *sp = gfx;
                            avail &= ~2;
                            if (gfx->state & 1) {
                                *dp = gfx;
                                avail &= ~1;
                            }
                            sc->gfxListHead = sc->gfxListHead->next;
                            if (sc->gfxListHead == 0)
                                sc->gfxListTail = 0;
                        }
                    } else {
                        if (avail == 3) {
                            *sp = *dp = gfx;
                            avail &= ~3;
                            sc->gfxListHead = sc->gfxListHead->next;
                            if (sc->gfxListHead == 0)
                                sc->gfxListTail = 0;
                        }
                    }
                    break;
                case 7:
                case 6:
                case 2:
                    if (gfx->state & 2) {
                        if (avail & 2) {
                            *sp = gfx;
                            avail &= ~2;
                        }
                    } else if (gfx->state & 1) {
                        if (avail & 1) {
                            *dp = gfx;
                            avail &= ~1;
                            sc->gfxListHead = sc->gfxListHead->next;
                            if (sc->gfxListHead == 0)
                                sc->gfxListTail = 0;
                        }
                    }
                    break;
                case 5:
                case 1:
                default:
                    break;
            }
        }
    }
    if (avail != availRCP)
        avail = __scSchedule(sc, sp, dp, avail);
    return avail;
}

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
