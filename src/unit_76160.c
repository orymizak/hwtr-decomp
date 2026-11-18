#include "common.h"
#include "ultra64.h"
#include "nusys.h"
#include "include_asm.h"
typedef struct { u8 p0[8]; s16 count; } RetraceCfg_;
typedef struct { u8 b[0x10]; } ClientW_;
typedef struct { u8 b[0x10]; } ClientR_;
typedef struct { u8 p0[0xC]; void *fb; } FrameR_;
extern u8 D_8012DBE0;
extern u8 D_8012FD90;
void nuGfxThread(void *arg);

extern s32 D_8014D304;
extern s32 D_8014D33C;
extern s8 D_8014D97D;
extern u8 D_8014D97C;
typedef struct ClientS_ { struct ClientS_ *next; void *mq; s16 mask; } ClientS_;
typedef struct { s16 retraceMsg; s16 prenmiMsg; OSMesgQueue audioRequestMQ; void *audioBuf[8]; OSMesgQueue gfxMQ; void *gfxBuf[8]; OSMesgQueue retraceMQ; void *retraceBuf[8]; OSMesgQueue rspMQ; void *rspBuf[8]; OSMesgQueue rdpMQ; void *rdpBuf[8]; OSMesgQueue waitMQ; void *waitBuf[8]; u8 p154[4]; u8 schedulerThread[0x1B0]; u8 audioThread[0x1B0]; u8 graphicsThread[0x1B0]; ClientS_ *clientList; NUScTask *curGraphicsTask; NUScTask *curAudioTask; NUScTask *graphicsTaskSuspended; u32 frameRate; u8 retraceCount; u8 frameBufferNum; } SchedS_;
extern SchedS_ nusched;
extern s32 osTvType;
extern OSViMode osViModeTable[];
extern u8 D_80129BC0[];
extern u8 D_8012BBC0[];
extern void *D_8012DBC0[];
extern s32 D_80140E40;
extern void (*D_800FB180)(void);
void nuScEventHandler(void);
void nuScExecuteAudio(void);
void nuScExecuteGraphics(void);
void nuScEventBroadcast(s16 *msg);
void nuScWaitTaskReady(FrameR_ *f);
extern u8 D_801345B0;

void nuScCreateScheduler(u8 mode, u8 numFields) {
    nusched.curGraphicsTask = 0;
    nusched.curAudioTask = 0;
    nusched.graphicsTaskSuspended = 0;
    nusched.clientList = 0;
    nusched.retraceMsg = 1;
    nusched.prenmiMsg = 2;
    nusched.frameRate = numFields;
    nusched.frameBufferNum = 2;
    if (osTvType == 0) {
        nusched.retraceCount = 50;
    } else {
        nusched.retraceCount = 60;
    }
    D_801345B0 = 0;
    osCreateMesgQueue(&nusched.retraceMQ, nusched.retraceBuf, 8);
    osCreateMesgQueue(&nusched.rspMQ, nusched.rspBuf, 8);
    osCreateMesgQueue(&nusched.rdpMQ, nusched.rdpBuf, 8);
    osCreateMesgQueue(&nusched.gfxMQ, nusched.gfxBuf, 8);
    osCreateMesgQueue(&nusched.audioRequestMQ, nusched.audioBuf, 8);
    osCreateMesgQueue(&nusched.waitMQ, nusched.waitBuf, 8);
    osCreateViManager(0xFE);
    osViSetMode(&osViModeTable[mode]);
    osViBlack(1);
    osViSetEvent(&nusched.retraceMQ, (void *)0x29A, numFields);
    osSetEventMesg(4, &nusched.rspMQ, (void *)0x29B);
    osSetEventMesg(9, &nusched.rdpMQ, (void *)0x29C);
    osSetEventMesg(0xE, &nusched.retraceMQ, (void *)0x29D);
    osCreateThread(nusched.schedulerThread, 19, nuScEventHandler, (s32)&nusched, D_80129BC0, 120);
    osStartThread(nusched.schedulerThread);
    osCreateThread(nusched.audioThread, 18, nuScExecuteAudio, (s32)&nusched, D_8012BBC0, 110);
    osStartThread(nusched.audioThread);
    osCreateThread(nusched.graphicsThread, 17, nuScExecuteGraphics, (s32)&nusched, D_8012DBC0, 100);
    osStartThread(nusched.graphicsThread);
}

s32 *nuScGetAudioMQ(void) {
    return &D_8014D304;
}

s32 *nuScGetGfxMQ(void) {
    return &D_8014D33C;
}

void nuScEventHandler(void) {
    void *msg;
    s32 beforeResetFrame;

    D_80140E40 = 0;
    while (1) {
        osRecvMesg(&nusched.retraceMQ, (void **)&msg, 1);
        switch ((s32)msg) {
        case 0x29A:
            D_80140E40++;
            nuScEventBroadcast(&nusched.retraceMsg);
            if (D_801345B0) {
                if (beforeResetFrame) {
                    beforeResetFrame--;
                } else {
                    D_801345B0 |= 2;
                    osAfterPreNMI();
                    osViSetYScale(1.0f);
                    osViBlack(1);
                }
            }
            break;
        case 0x29D:
            D_801345B0 = 1;
            nuScEventBroadcast(&nusched.prenmiMsg);
            if (D_800FB180) {
                D_800FB180();
            }
            beforeResetFrame = (nusched.retraceCount >> 1) / nusched.frameRate - 3;
            break;
        }
    }
}

void nuScAddClient(ClientS_ *c, void *mq, u16 flags) {
    s32 mask;

    mask = osSetIntMask(1);
    c->mq = mq;
    c->next = nusched.clientList;
    c->mask = flags;
    nusched.clientList = c;
    if ((flags & 2) && D_801345B0 != 0) {
        osSendMesg(mq, &nusched.prenmiMsg, 0);
    }
    osSetIntMask(mask);
}

void func_800D59A0(RetraceCfg_ *cfg, s16 count) {
    s32 mask;

    mask = osSetIntMask(1);
    cfg->count = count;
    osSetIntMask(mask);
}

void nuScRemoveClient(ClientS_ *c) {
    s32 mask;
    ClientS_ *p;
    ClientS_ *prev;

    mask = osSetIntMask(1);
    prev = 0;
    p = nusched.clientList;
    while (p != 0) {
        if (p == c) {
            if (prev != 0) {
                prev->next = p->next;
            } else {
                nusched.clientList = p->next;
            }
            break;
        }
        prev = p;
        p = p->next;
    }
    osSetIntMask(mask);
}

/* nuScEventBroadcast: sends msg to every client whose mask includes its type. */
void nuScEventBroadcast(s16 *msg) {
    ClientS_ *p;

    for (p = nusched.clientList; p != 0; p = p->next) {
        if (p->mask & *msg) {
            osSendMesg(p->mq, msg, 0);
        }
    }
}

void nuScExecuteAudio(void) {
    NUScTask *gfxTask;
    NUScTask *audioTask;
    void *msg;
    u32 yieldFlag;

    while (1) {
        osRecvMesg(&nusched.audioRequestMQ, (void **)&audioTask, 1);
        if (!(D_801345B0 & 2)) {
            osWritebackDCacheAll();
            yieldFlag = 0;
            gfxTask = nusched.curGraphicsTask;
            if (gfxTask) {
                osSpTaskYield();
                osRecvMesg(&nusched.rspMQ, (void **)&msg, 1);
                if (osSpTaskYielded(&gfxTask->list)) {
                    yieldFlag = 1;
                } else {
                    yieldFlag = 2;
                }
            }
            nusched.curAudioTask = audioTask;
            osSpTaskLoad(&audioTask->list);
            osSpTaskStartGo(&audioTask->list);
            osRecvMesg(&nusched.rspMQ, (void **)&msg, 1);
            nusched.curAudioTask = 0;
            if (nusched.graphicsTaskSuspended) {
                osSendMesg(&nusched.waitMQ, &msg, 1);
            }
            if (yieldFlag == 1) {
                osSpTaskLoad(&gfxTask->list);
                osSpTaskStartGo(&gfxTask->list);
            } else if (yieldFlag == 2) {
                osSendMesg(&nusched.rspMQ, &msg, 1);
            }
        }
        osSendMesg(audioTask->msgQ, audioTask->msg, 1);
    }
}

void nuScExecuteGraphics(void) {
    s32 mask;
    NUScTask *gfxTask;
    void *msg;

    while (1) {
        osRecvMesg(&nusched.gfxMQ, (void **)&gfxTask, 1);
        if (!(D_801345B0 & 2)) {
            nuScWaitTaskReady((FrameR_ *)gfxTask);
            mask = osSetIntMask(1);
            if (nusched.curAudioTask) {
                nusched.graphicsTaskSuspended = gfxTask;
                osSetIntMask(mask);
                osRecvMesg(&nusched.waitMQ, (void **)&msg, 1);
                mask = osSetIntMask(1);
                nusched.graphicsTaskSuspended = 0;
            }
            osSetIntMask(mask);
            mask = osSetIntMask(1);
            nusched.curGraphicsTask = gfxTask;
            osSetIntMask(mask);
            osSpTaskLoad(&gfxTask->list);
            osSpTaskStartGo(&gfxTask->list);
            osRecvMesg(&nusched.rspMQ, (void **)&msg, 1);
            mask = osSetIntMask(1);
            nusched.curGraphicsTask = 0;
            osSetIntMask(mask);
            if (!(gfxTask->flags & 2)) {
                osRecvMesg(&nusched.rdpMQ, (void **)&msg, 1);
            }
        }
        osSendMesg(gfxTask->msgQ, gfxTask, 1);
    }
}

void nuScWaitTaskReady(FrameR_ *f) {
    u8 *base = &D_8014D97D;
    void *fb = f->fb;
    u8 *mq;
    ClientR_ c;

    if (*base == 1) {
        return;
    }
    mq = base - 0x561;
    while (osViGetCurrentFramebuffer() == fb || osViGetNextFramebuffer() == fb) {
        nuScAddClient((ClientS_ *)&c, mq, 1);
        osRecvMesg(mq, 0, 1);
        nuScRemoveClient((ClientS_ *)&c);
    }
}

void nuScSetFrameBufferNum(s8 arg0) {
    D_8014D97D = arg0;
}

u8 nuScGetFrameRate(void) {
    return D_8014D97C;
}

extern u8 D_80141598[];
extern s32 nuGfxTaskSpool;
extern void (*nuGfxFunc)(s32);
extern void (*nuGfxPreNMIFunc)(void);
void nuGfxThread(void *arg) {
    ClientW_ c;
    s16 *msg;

    osCreateMesgQueue(D_80141598, D_8012DBC0, 8);
    nuScAddClient((ClientS_ *)&c, D_80141598, 3);
    while (1) {
        osRecvMesg(D_80141598, (void **)&msg, 1);
        switch (*msg) {
            case 1:
                if (nuGfxFunc != 0) {
                    nuGfxFunc(nuGfxTaskSpool);
                }
                break;
            case 2:
                if (nuGfxPreNMIFunc != 0) {
                    nuGfxPreNMIFunc();
                }
                break;
        }
    }
}

void nuGfxThreadStart(void) {
    osCreateThread(&D_8012DBE0, 4, &nuGfxThread, 0, &D_8012FD90, 0x32);
    osStartThread(&D_8012DBE0);
}
