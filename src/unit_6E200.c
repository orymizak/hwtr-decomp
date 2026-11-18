#include "common.h"
#include "ultra64.h"
#include "PR/sched.h"
#include "include_asm.h"
extern OSScTask gAudioTask; /* audio RSP task */
extern u8 D_8014D304[];
extern u8 gAudioTaskFrame;
extern void (*gAudioTaskCallback)(s32, u8);
s32 Snd_Init(s32);
s32 Snd_SetTaskHooks(void *);
extern u8 D_800E7AA0;
extern u8 D_800E7B70;
extern s8 gAudioTaskEnabled;
extern u8 gAudioTaskHooks;
extern void *D_80138AD0;
extern void *D_80138B08;
s32 nuScAddClient(void *, void *,s32);
extern OSMesg D_80127900[4];
extern OSMesg D_80127910[1];
extern OSMesgQueue D_80134780; /* audio task reply queue */
extern u8 D_80144450;
extern OSMesgQueue D_80145528; /* scheduler events for audio */

void AudioTask_Init(s32 arg0) {
    u8 *start = &D_800E7AA0;

    gAudioTaskEnabled = 1;
    gAudioTask.msgQ = &D_80134780;
    gAudioTask.list.t.type = M_AUDTASK;
    gAudioTask.list.t.ucode_boot_size = &D_800E7B70 - start;
    gAudioTask.msg = NULL;
    gAudioTask.list.t.ucode_boot = (u64 *)start;
    gAudioTask.list.t.ucode_data_size = SP_UCODE_DATA_SIZE;
    gAudioTask.list.t.dram_stack = NULL;
    gAudioTask.list.t.dram_stack_size = 0;
    gAudioTask.list.t.output_buff = NULL;
    gAudioTask.list.t.output_buff_size = 0;
    gAudioTask.list.t.yield_data_ptr = NULL;
    gAudioTask.list.t.yield_data_size = 0;
    Snd_SetTaskHooks(&gAudioTaskHooks);
    Snd_Init(arg0);
}

void AudioTask_CreateQueues(void) {
    osCreateMesgQueue(&D_80145528, D_80127900, 4);
    osCreateMesgQueue(&D_80134780, D_80127910, 1);
    nuScAddClient(&D_80144450, &D_80145528, 3);
}

void AudioTask_WaitRetrace(void) {
    s16 *msg;

    for (;;) {
        osRecvMesg(&D_80145528, (OSMesg *)&msg, OS_MESG_BLOCK);
        if (*msg == OS_SC_RETRACE_MSG) {
            return;
        }
        if (*msg == OS_SC_DONE_MSG) {
            if (gAudioTaskCallback != NULL) {
                gAudioTaskCallback(2, gAudioTaskFrame);
            }
            gAudioTaskFrame++;
        }
    }
}

void AudioTask_Run(u32 *a) {
    if ((*(u8 *)&gAudioTaskEnabled) == 1) {
        gAudioTask.list.t.data_ptr = (u64 *)a[0];
        gAudioTask.list.t.data_size = a[1];
        gAudioTask.list.t.ucode = (u64 *)a[2];
        gAudioTask.list.t.ucode_data = (u64 *)a[3];
        osSendMesg((OSMesgQueue *)D_8014D304, &gAudioTask, OS_MESG_BLOCK);
        osRecvMesg(&D_80134780, NULL, OS_MESG_BLOCK);
    }
    if (gAudioTaskCallback != 0 && gAudioTaskFrame != 0) {
        gAudioTaskCallback(1, gAudioTaskFrame);
        gAudioTaskFrame++;
    }
}
