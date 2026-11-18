#include "common.h"
#include "ultra64.h"
#include "PR/sched.h"
#include "include_asm.h"
/* Scheduler client used by the audio thread: client node plus two message queues. */
typedef struct { u8 client[8]; u8 q[0x18]; void *qbuf[4]; u8 q2[0x18]; void *q2buf[4]; } AudClient_;
typedef struct { void *data; u32 size; void *ucode; void *ucode_data; } AudInfoT_;
s32 func_800D23FC();
extern s32 D_801279D4;

extern s32 D_801279D0;

void func_800D1F40(s32 arg0) {
    D_801279D0 = arg0;
}

void func_800D1F4C(void) {
    AudClient_ *c;

    c = (AudClient_ *)func_800D23FC(sizeof(AudClient_));
    D_801279D4 = (s32)c;
    osCreateMesgQueue((s32)c->q, (s32)c->qbuf, 4);
    osCreateMesgQueue((s32)((AudClient_ *)D_801279D4)->q2, (s32)((AudClient_ *)D_801279D4)->q2buf, 4);
    osScAddClient(D_801279D0, D_801279D4, (s32)((AudClient_ *)D_801279D4)->q);
}

void func_800D1FB0(void) {
    s16 *sp10;

    do {
        osRecvMesg(D_801279D4 + 8, &sp10, 1);
        osRecvMesg(D_801279D4 + 8, NULL, 0);
    } while (*sp10 != 1);
}

extern u8 D_800E7AA0[];
extern u8 D_800E7B70[];
void func_800D2010(AudInfoT_ *a) {
    OSScTask t;
    u8 msg[0x20];

    t.next = 0;
    t.msgQ = D_801279D4 + 0x30;
    t.msg = msg;
    t.flags = OS_SC_NEEDS_RSP;
    t.list.t.data_ptr = a->data;
    t.list.t.data_size = a->size;
    t.list.t.type = M_AUDTASK;
    t.list.t.ucode_boot = D_800E7AA0;
    t.list.t.ucode_boot_size = D_800E7B70 - D_800E7AA0;
    t.list.t.flags = 0;
    t.list.t.ucode = a->ucode;
    t.list.t.ucode_data = a->ucode_data;
    t.list.t.ucode_size = 0x1000;
    t.list.t.ucode_data_size = 0x800;
    t.list.t.dram_stack = NULL;
    t.list.t.dram_stack_size = 0;
    t.list.t.output_buff = NULL;
    t.list.t.output_buff_size = NULL;
    t.list.t.yield_data_ptr = NULL;
    t.list.t.yield_data_size = 0;
    osSendMesg(osScGetCmdQ(D_801279D0), &t, 1);
    osRecvMesg(D_801279D4 + 0x30, 0, 1);
}
