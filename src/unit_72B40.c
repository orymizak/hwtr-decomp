#include "common.h"
#include "ultra64.h"
#include "PR/sched.h"
#include "include_asm.h"
/* Scheduler client of the audio thread: retrace/PRENMI messages arrive on msgQ, the end of an
 * audio RSP task on taskMsgQ. */
typedef struct {
    /* 0x00 */ OSScClient client;
    /* 0x08 */ OSMesgQueue msgQ;
    /* 0x20 */ OSMesg msgBuf[4];
    /* 0x30 */ OSMesgQueue taskMsgQ;
    /* 0x48 */ OSMesg taskMsgBuf[4];
} AudClient; /* size = 0x58 */
typedef struct { void *data; u32 size; void *ucode; void *ucode_data; } AudInfoT_;
void *SndHeap_Alloc(s32 size);
extern AudClient *D_801279D4;

extern OSSched *D_801279D0;

void Snd_SetScheduler(OSSched *sc) {
    D_801279D0 = sc;
}

void func_800D1F4C(void) {
    AudClient *c;

    c = SndHeap_Alloc(sizeof(AudClient));
    D_801279D4 = c;
    osCreateMesgQueue(&c->msgQ, c->msgBuf, 4);
    osCreateMesgQueue(&D_801279D4->taskMsgQ, D_801279D4->taskMsgBuf, 4);
    osScAddClient(D_801279D0, &D_801279D4->client, &D_801279D4->msgQ);
}

void func_800D1FB0(void) {
    s16 *msg;

    do {
        osRecvMesg(&D_801279D4->msgQ, (OSMesg *)&msg, OS_MESG_BLOCK);
        osRecvMesg(&D_801279D4->msgQ, NULL, OS_MESG_NOBLOCK);
    } while (*msg != OS_SC_RETRACE_MSG);
}

extern u8 D_800E7AA0[];
extern u8 D_800E7B70[];
void func_800D2010(AudInfoT_ *a) {
    OSScTask t;
    u8 msg[0x20];

    t.next = 0;
    t.msgQ = &D_801279D4->taskMsgQ;
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
    osSendMesg(osScGetCmdQ(D_801279D0), &t, OS_MESG_BLOCK);
    osRecvMesg(&D_801279D4->taskMsgQ, NULL, OS_MESG_BLOCK);
}
