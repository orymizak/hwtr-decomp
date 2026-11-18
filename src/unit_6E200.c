#include "common.h"
#include "ultra64.h"
#include "PR/sched.h"
#include "include_asm.h"
extern OSScTask D_80138AB8; /* audio RSP task */
extern u8 D_8014D304[];
extern u8 D_800FABA1;
extern void (*D_800FABA4)(s32, u8);
s32 Snd_Init(s32);
s32 func_800CE95C(void *);
extern u8 D_800E7AA0;
extern u8 D_800E7B70;
extern s8 D_800FABA0;
extern u8 D_800FABA8;
extern void *D_80138AD0;
extern void *D_80138B08;
s32 nuScAddClient(void *, void *,s32);
extern OSMesg D_80127900[4];
extern OSMesg D_80127910[1];
extern OSMesgQueue D_80134780; /* audio task reply queue */
extern u8 D_80144450;
extern OSMesgQueue D_80145528; /* scheduler events for audio */

void func_800CD600(s32 arg0) {
    u8 *start = &D_800E7AA0;

    D_800FABA0 = 1;
    D_80138AB8.msgQ = &D_80134780;
    D_80138AB8.list.t.type = M_AUDTASK;
    D_80138AB8.list.t.ucode_boot_size = &D_800E7B70 - start;
    D_80138AB8.msg = NULL;
    D_80138AB8.list.t.ucode_boot = (u64 *)start;
    D_80138AB8.list.t.ucode_data_size = SP_UCODE_DATA_SIZE;
    D_80138AB8.list.t.dram_stack = NULL;
    D_80138AB8.list.t.dram_stack_size = 0;
    D_80138AB8.list.t.output_buff = NULL;
    D_80138AB8.list.t.output_buff_size = 0;
    D_80138AB8.list.t.yield_data_ptr = NULL;
    D_80138AB8.list.t.yield_data_size = 0;
    func_800CE95C(&D_800FABA8);
    Snd_Init(arg0);
}

void func_800CD6C4(void) {
    osCreateMesgQueue(&D_80145528, D_80127900, 4);
    osCreateMesgQueue(&D_80134780, D_80127910, 1);
    nuScAddClient(&D_80144450, &D_80145528, 3);
}

void func_800CD728(void) {
    s16 *msg;

    for (;;) {
        osRecvMesg(&D_80145528, (OSMesg *)&msg, OS_MESG_BLOCK);
        if (*msg == OS_SC_RETRACE_MSG) {
            return;
        }
        if (*msg == OS_SC_DONE_MSG) {
            if (D_800FABA4 != NULL) {
                D_800FABA4(2, D_800FABA1);
            }
            D_800FABA1++;
        }
    }
}

void func_800CD7B8(u32 *a) {
    if ((*(u8 *)&D_800FABA0) == 1) {
        D_80138AB8.list.t.data_ptr = (u64 *)a[0];
        D_80138AB8.list.t.data_size = a[1];
        D_80138AB8.list.t.ucode = (u64 *)a[2];
        D_80138AB8.list.t.ucode_data = (u64 *)a[3];
        osSendMesg((OSMesgQueue *)D_8014D304, &D_80138AB8, OS_MESG_BLOCK);
        osRecvMesg(&D_80134780, NULL, OS_MESG_BLOCK);
    }
    if (D_800FABA4 != 0 && D_800FABA1 != 0) {
        D_800FABA4(1, D_800FABA1);
        D_800FABA1++;
    }
}
