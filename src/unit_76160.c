#include "common.h"
#include "include_asm.h"
typedef struct { u8 b[0x10]; } ClientW_;
typedef struct { u8 b[0x10]; } ClientR_;
typedef struct { u8 p0[0xC]; void *fb; } FrameR_;
s32 osCreateThread(void *,s32, void *,s32, void *, s32);
s32 osStartThread(void *);
extern u8 D_8012DBE0;
extern u8 D_8012FD90;
void func_800D5E00(void *arg);

extern s32 D_8014D304;
extern s32 D_8014D33C;
s32 osSetIntMask(s32);
extern s8 D_8014D97D;
extern u8 D_8014D97C;
typedef struct ClientS_ { struct ClientS_ *next; void *mq; u16 mask; } ClientS_;
typedef struct { u8 b[0x18]; } MqS_;
typedef struct { u8 p0[0x10]; u8 list[0x40]; void *msgQ; void *msg; } ScTaskS_;
typedef struct { s16 retraceMsg; s16 prenmiMsg; MqS_ audioRequestMQ; void *audioBuf[8]; MqS_ gfxMQ; void *gfxBuf[8]; MqS_ retraceMQ; void *retraceBuf[8]; MqS_ rspMQ; void *rspBuf[8]; MqS_ rdpMQ; void *rdpBuf[8]; MqS_ waitMQ; void *waitBuf[8]; u8 p154[4]; u8 schedulerThread[0x1B0]; u8 audioThread[0x1B0]; u8 graphicsThread[0x1B0]; ClientS_ *clientList; ScTaskS_ *curGraphicsTask; ScTaskS_ *curAudioTask; ScTaskS_ *graphicsTaskSuspended; u32 frameRate; u8 retraceCount; u8 frameBufferNum; } SchedS_;
typedef struct { u8 p0[8]; u32 flags; u8 pC[4]; u8 list[0x40]; void *msgQ; } ScTaskG_;
typedef struct { u8 b[0x50]; } ViModeS_;
extern SchedS_ D_8014D300;
extern s32 D_80000300;
extern ViModeS_ D_800FB650[];
extern u8 D_80129BC0[];
extern u8 D_8012BBC0[];
extern void *D_8012DBC0[];
extern s32 D_80140E40;
extern void (*D_800FB180)(void);
s32 osRecvMesg(void *mq, void **msg, s32 flag);
s32 osSendMesg(void *, void *, s32);
void osCreateMesgQueue(void *mq, void **msg, s32 count);
void osCreateViManager(s32);
void osViSetMode(void *);
void osViBlack(u8);
void osViSetEvent(void *, void *, u32);
void osSetEventMesg(s32, void *, void *);
void osWritebackDCacheAll(void);
void osSpTaskYield(void);
s32 osSpTaskYielded(void *);
void osSpTaskLoad(void *);
void osSpTaskStartGo(void *);
void osAfterPreNMI(void);
void osViSetYScale(f32);
void func_800D57C4(void);
void func_800D5ABC(void);
void func_800D5C18(void);
void func_800D5A5C(u16 *msg);
void func_800D5D5C(FrameR_ *f);
extern u8 D_801345B0;

void func_800D5560(u8 mode, u8 numFields) {
    D_8014D300.curGraphicsTask = 0;
    D_8014D300.curAudioTask = 0;
    D_8014D300.graphicsTaskSuspended = 0;
    D_8014D300.clientList = 0;
    D_8014D300.retraceMsg = 1;
    D_8014D300.prenmiMsg = 2;
    D_8014D300.frameRate = numFields;
    D_8014D300.frameBufferNum = 2;
    if (D_80000300 == 0) {
        D_8014D300.retraceCount = 50;
    } else {
        D_8014D300.retraceCount = 60;
    }
    D_801345B0 = 0;
    osCreateMesgQueue(&D_8014D300.retraceMQ, D_8014D300.retraceBuf, 8);
    osCreateMesgQueue(&D_8014D300.rspMQ, D_8014D300.rspBuf, 8);
    osCreateMesgQueue(&D_8014D300.rdpMQ, D_8014D300.rdpBuf, 8);
    osCreateMesgQueue(&D_8014D300.gfxMQ, D_8014D300.gfxBuf, 8);
    osCreateMesgQueue(&D_8014D300.audioRequestMQ, D_8014D300.audioBuf, 8);
    osCreateMesgQueue(&D_8014D300.waitMQ, D_8014D300.waitBuf, 8);
    osCreateViManager(0xFE);
    osViSetMode(&D_800FB650[mode]);
    osViBlack(1);
    osViSetEvent(&D_8014D300.retraceMQ, (void *)0x29A, numFields);
    osSetEventMesg(4, &D_8014D300.rspMQ, (void *)0x29B);
    osSetEventMesg(9, &D_8014D300.rdpMQ, (void *)0x29C);
    osSetEventMesg(0xE, &D_8014D300.retraceMQ, (void *)0x29D);
    osCreateThread(D_8014D300.schedulerThread, 19, func_800D57C4, (s32)&D_8014D300, D_80129BC0, 120);
    osStartThread(D_8014D300.schedulerThread);
    osCreateThread(D_8014D300.audioThread, 18, func_800D5ABC, (s32)&D_8014D300, D_8012BBC0, 110);
    osStartThread(D_8014D300.audioThread);
    osCreateThread(D_8014D300.graphicsThread, 17, func_800D5C18, (s32)&D_8014D300, D_8012DBC0, 100);
    osStartThread(D_8014D300.graphicsThread);
}

s32 *func_800D57AC(void) {
    return &D_8014D304;
}

s32 *func_800D57B8(void) {
    return &D_8014D33C;
}

void func_800D57C4(void) {
    void *msg;
    s32 beforeResetFrame;

    D_80140E40 = 0;
    while (1) {
        osRecvMesg(&D_8014D300.retraceMQ, (void **)&msg, 1);
        switch ((s32)msg) {
        case 0x29A:
            D_80140E40++;
            func_800D5A5C((u16 *)&D_8014D300.retraceMsg);
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
            func_800D5A5C((u16 *)&D_8014D300.prenmiMsg);
            if (D_800FB180) {
                D_800FB180();
            }
            beforeResetFrame = (D_8014D300.retraceCount >> 1) / D_8014D300.frameRate - 3;
            break;
        }
    }
}

void func_800D5910(ClientS_ *c, void *mq, u16 flags) {
    s32 mask;

    mask = osSetIntMask(1);
    c->mq = mq;
    c->next = D_8014D300.clientList;
    c->mask = flags;
    D_8014D300.clientList = c;
    if ((flags & 2) && D_801345B0 != 0) {
        osSendMesg(mq, &D_8014D300.prenmiMsg, 0);
    }
    osSetIntMask(mask);
}

void func_800D59A0(void *arg0, s16 arg1) {
    s32 temp_a0;

    temp_a0 = osSetIntMask(1);
    *(s16 *)((u8 *)arg0 + 0x8) = arg1;
    osSetIntMask(temp_a0);
}

void func_800D59E0(ClientS_ *c) {
    s32 mask;
    ClientS_ *p;
    ClientS_ *prev;

    mask = osSetIntMask(1);
    prev = 0;
    p = D_8014D300.clientList;
    while (p != 0) {
        if (p == c) {
            if (prev != 0) {
                prev->next = p->next;
            } else {
                D_8014D300.clientList = p->next;
            }
            break;
        }
        prev = p;
        p = p->next;
    }
    osSetIntMask(mask);
}

INCLUDE_ASM("asm/nonmatchings/unit_76160", func_800D5A5C);

void func_800D5ABC(void) {
    ScTaskS_ *gfxTask;
    ScTaskS_ *audioTask;
    void *msg;
    u32 yieldFlag;

    while (1) {
        osRecvMesg(&D_8014D300.audioRequestMQ, (void **)&audioTask, 1);
        if (!(D_801345B0 & 2)) {
            osWritebackDCacheAll();
            yieldFlag = 0;
            gfxTask = D_8014D300.curGraphicsTask;
            if (gfxTask) {
                osSpTaskYield();
                osRecvMesg(&D_8014D300.rspMQ, (void **)&msg, 1);
                if (osSpTaskYielded(gfxTask->list)) {
                    yieldFlag = 1;
                } else {
                    yieldFlag = 2;
                }
            }
            D_8014D300.curAudioTask = audioTask;
            osSpTaskLoad(audioTask->list);
            osSpTaskStartGo(audioTask->list);
            osRecvMesg(&D_8014D300.rspMQ, (void **)&msg, 1);
            D_8014D300.curAudioTask = 0;
            if (D_8014D300.graphicsTaskSuspended) {
                osSendMesg(&D_8014D300.waitMQ, &msg, 1);
            }
            if (yieldFlag == 1) {
                osSpTaskLoad(gfxTask->list);
                osSpTaskStartGo(gfxTask->list);
            } else if (yieldFlag == 2) {
                osSendMesg(&D_8014D300.rspMQ, &msg, 1);
            }
        }
        osSendMesg(audioTask->msgQ, audioTask->msg, 1);
    }
}

void func_800D5C18(void) {
    s32 mask;
    ScTaskG_ *gfxTask;
    void *msg;

    while (1) {
        osRecvMesg(&D_8014D300.gfxMQ, (void **)&gfxTask, 1);
        if (!(D_801345B0 & 2)) {
            func_800D5D5C((FrameR_ *)gfxTask);
            mask = osSetIntMask(1);
            if (D_8014D300.curAudioTask) {
                D_8014D300.graphicsTaskSuspended = (ScTaskS_ *)gfxTask;
                osSetIntMask(mask);
                osRecvMesg(&D_8014D300.waitMQ, (void **)&msg, 1);
                mask = osSetIntMask(1);
                D_8014D300.graphicsTaskSuspended = 0;
            }
            osSetIntMask(mask);
            mask = osSetIntMask(1);
            D_8014D300.curGraphicsTask = (ScTaskS_ *)gfxTask;
            osSetIntMask(mask);
            osSpTaskLoad(gfxTask->list);
            osSpTaskStartGo(gfxTask->list);
            osRecvMesg(&D_8014D300.rspMQ, (void **)&msg, 1);
            mask = osSetIntMask(1);
            D_8014D300.curGraphicsTask = 0;
            osSetIntMask(mask);
            if (!(gfxTask->flags & 2)) {
                osRecvMesg(&D_8014D300.rdpMQ, (void **)&msg, 1);
            }
        }
        osSendMesg(gfxTask->msgQ, gfxTask, 1);
    }
}

void *osViGetCurrentFramebuffer(void);
void *osViGetNextFramebuffer(void);
void func_800D5D5C(FrameR_ *f) {
    u8 *base = &D_8014D97D;
    void *fb = f->fb;
    u8 *mq;
    ClientR_ c;

    if (*base == 1) {
        return;
    }
    mq = base - 0x561;
    while (osViGetCurrentFramebuffer() == fb || osViGetNextFramebuffer() == fb) {
        func_800D5910((ClientS_ *)&c, mq, 1);
        osRecvMesg(mq, 0, 1);
        func_800D59E0((ClientS_ *)&c);
    }
}

void func_800D5DE8(s8 arg0) {
    D_8014D97D = arg0;
}

u8 func_800D5DF4(void) {
    return D_8014D97C;
}

extern u8 D_80141598[];
extern s32 D_80145484;
extern void (*D_800FB190)(s32);
extern void (*D_800FB194)(void);
void func_800D5E00(void *arg) {
    ClientW_ c;
    s16 *msg;

    osCreateMesgQueue(D_80141598, D_8012DBC0, 8);
    func_800D5910((ClientS_ *)&c, D_80141598, 3);
    while (1) {
        osRecvMesg(D_80141598, (void **)&msg, 1);
        switch (*msg) {
            case 1:
                if (D_800FB190 != 0) {
                    D_800FB190(D_80145484);
                }
                break;
            case 2:
                if (D_800FB194 != 0) {
                    D_800FB194();
                }
                break;
        }
    }
}

void func_800D5ED0(void) {
    osCreateThread(&D_8012DBE0, 4, &func_800D5E00, 0, &D_8012FD90, 0x32);
    osStartThread(&D_8012DBE0);
}
