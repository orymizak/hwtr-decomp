#include "common.h"
#include "include_asm.h"
typedef struct NodeT_ { struct NodeT_ *next; void **entries; u16 id; u8 count; } NodeT_;
extern NodeT_ *D_800FB220;
s32 osSetIntMask(s32);
typedef struct { u16 type; u8 status; u8 errnum; } ContStatusS_;
extern u8 D_8014EB40[];
extern void *D_8012FE70[];
extern u8 D_80132040[];
void osSetEventMesg(s32 e, void *mq, void *msg);
s32 osContInit(void *mq, u8 *bitpattern, ContStatusS_ *data);
void osCreateThread(void *t, s32 id, void (*entry)(void *), void *arg, void *sp, s32 pri);
void func_800D72AC(void *arg);
typedef struct { u8 b[0x18]; } MqI_;
typedef struct { u16 cmd; MqI_ *mq; s32 result; void *args; } ReqI_;
extern u8 D_80144460[];
void osCreateMesgQueue(void *mq, void **msg, s32 count);
s32 osSendMesg(void *mq, void *msg, s32 flag);
s32 osRecvMesg(void *mq, void **msg, s32 flag);
s32 func_800D7208(u16 cmd, void *args);

s32 osStartThread(s32 *);
extern s32 D_8012FE90;

void func_800D7130(void) {
    func_800D7208(0x304, 0);
}

void func_800D7150(void) {
    func_800D7208(0x305, 0);
}

u8 func_800D7170(void) {
    ContStatusS_ status[4];
    u8 pattern;

    osCreateMesgQueue(D_8014EB40, D_8012FE70, 8);
    osSetEventMesg(5, D_8014EB40, 0);
    osContInit(D_8014EB40, &pattern, status);
    osCreateThread(((u8 *)&D_8012FE90), 5, func_800D72AC, 0, D_80132040, 0x73);
    ((void (*)(void *t))osStartThread)(((u8 *)&D_8012FE90));
    return pattern;
}


s32 func_800D7208(u16 cmd, void *args) {
    MqI_ mq;
    ReqI_ req;
    void *msgbuf;

    req.cmd = cmd;
    req.args = args;
    req.mq = &mq;
    osCreateMesgQueue(&mq, &msgbuf, 1);
    osSendMesg(D_80144460, &req, 1);
    osRecvMesg(&mq, 0, 1);
    return req.result;
}

void func_800D726C(void) {
    func_800D7208(0x7F00, 0);
}

void func_800D728C(void) {
    osStartThread(&D_8012FE90);
}

typedef struct { s16 mesg; u8 p2[2]; void *rtnMesgQ; s32 error; } SiMesgT_;
typedef struct { u8 b[0x10]; } SiClientT_;
void func_800D5910(void *c, void *mq, s32 flags);
void func_800D59A0(void *arg0, s16 arg1);
void osStopThread(void *);
void func_800D72AC(void *arg) {
    SiClientT_ client;
    void *buf[8];
    SiMesgT_ *msg;
    NodeT_ **cb;
    s32 ret;
    u16 major;
    u16 minor;

    osCreateMesgQueue(D_80144460, buf, 8);
    func_800D5910(&client, D_80144460, 1);
    while (1) {
        osRecvMesg(D_80144460, (void **)&msg, 1);
        cb = &D_800FB220;
        switch (msg->mesg) {
        case 1:
            while (*cb) {
                if ((*cb)->entries[0]) {
                    ret = ((s32 (*)(SiMesgT_ *))(*cb)->entries[0])(msg);
                }
                if (ret) {
                    break;
                }
                cb = &(*cb)->next;
            }
            break;
        case 0x7F00:
            osSendMesg(msg->rtnMesgQ, 0, 1);
            func_800D59A0(&client, 0);
            osStopThread(0);
            func_800D59A0(&client, 1);
            break;
        default:
            major = msg->mesg & 0xFF00;
            minor = msg->mesg & 0xFF;
            while (*cb) {
                if ((*cb)->id == major) {
                    if (minor < (*cb)->count) {
                        msg->error = ((s32 (*)(SiMesgT_ *))(*cb)->entries[minor])(msg);
                    }
                    if (msg->rtnMesgQ) {
                        osSendMesg(msg->rtnMesgQ, 0, 1);
                    }
                    break;
                }
                cb = &(*cb)->next;
            }
            break;
        }
    }
}

void func_800D7450(NodeT_ *n) {
    NodeT_ **pp = &D_800FB220;
    s32 cnt;
    s32 mask;

    while (*pp != 0) {
        if ((*pp)->id == n->id) {
            return;
        }
        pp = &(*pp)->next;
    }
    for (cnt = 1; n->entries[cnt] != 0; cnt++) {
    }
    mask = osSetIntMask(1);
    *pp = n;
    n->next = 0;
    n->count = cnt;
    osSetIntMask(mask);
}

void func_800D74F0(NodeT_ *n) {
    NodeT_ **pp;
    s32 mask;

    pp = &D_800FB220;
    while (*pp != 0) {
        if ((*pp)->next == n) {
            mask = osSetIntMask(1);
            (*pp)->next = n->next;
            n->next = 0;
            osSetIntMask(mask);
            return;
        }
        pp = &(*pp)->next;
    }
}
