#include "common.h"
#include "ultra64.h"
#include "include_asm.h"
typedef struct { u8 p0[0x20]; void *ucode; u8 p24[4]; void *ucode_data; u8 p2C[0x14]; void *data_ptr; u32 data_size; } ScTaskI_;
extern ScTaskI_ D_80138AB8;
extern u8 D_8014D304[];
extern u8 D_800FABA1;
extern void (*D_800FABA4)(s32, u8);
s32 func_800CDB5C(s32);
s32 func_800CE95C(void *);
extern u8 D_800E7AA0;
extern u8 D_800E7B70;
extern s8 D_800FABA0;
extern u8 D_800FABA8;
extern s32 D_80138AC8;
extern void *D_80138AD0;
extern s32 D_80138AD4;
extern s32 D_80138AE4;
extern s32 D_80138AE8;
extern s32 D_80138AEC;
extern s32 D_80138AF0;
extern s32 D_80138AF4;
extern s32 D_80138B00;
extern s32 D_80138B04;
extern void *D_80138B08;
extern s32 D_80138B0C;
s32 func_800D5910(void *, void *,s32);
extern u8 D_80127900;
extern u8 D_80127910;
extern u8 D_80134780;
extern u8 D_80144450;
extern u8 D_80145528;

    /* TODO (Clean version): Unify ScTaskI_ and individual extern symbols D_80138AC8..D_80138B0C
    into standard libultra OSScTask (audioScTask):
        extern OSScTask D_80138AB8;
    func_800CD600 initializes audioScTask for the audio microcode:
        audioScTask.list.t.type = M_AUDTASK; // 2
        audioScTask.list.t.ucode_boot = (u64 *)rspbootTextStart;
        audioScTask.list.t.ucode_boot_size = (u8 *)rspbootTextEnd - (u8 *)rspbootTextStart;
        audioScTask.list.t.dram_stack_size = SP_DRAM_STACK_SIZE8; // 0x800
        audioScTask.msgQ = &audioReplyQ;
        audioScTask.msg = NULL;
    */
void func_800CD600(s32 arg0) {
    u8 *start = &D_800E7AA0;

    D_800FABA0 = 1;
    D_80138B08 = &D_80134780;
    D_80138AC8 = 2;
    D_80138AD4 = &D_800E7B70 - start;
    D_80138B0C = 0;
    D_80138AD0 = start;
    D_80138AE4 = 0x800;
    D_80138AE8 = 0;
    D_80138AEC = 0;
    D_80138AF0 = 0;
    D_80138AF4 = 0;
    D_80138B00 = 0;
    D_80138B04 = 0;
    func_800CE95C(&D_800FABA8);
    func_800CDB5C(arg0);
}

    /* TODO (Clean version): Replace raw byte arrays with typed OSMesgQueue and message buffers:
    extern OSMesgQueue audioEventQ;    // D_80145528
    extern OSMesg audioEventBuf[4];    // D_80127900
    extern OSMesgQueue audioReplyQ;    // D_80134780
    extern OSMesg audioReplyBuf[1];    // D_80127910
    extern OSSched sc;                 // D_80144450
    osCreateMesgQueue(&audioEventQ, audioEventBuf, 4);
    osCreateMesgQueue(&audioReplyQ, audioReplyBuf, 1);
    osScAddClient(&sc, &audioClient, &audioEventQ, OS_SC_RETRACE_MSG | OS_SC_DONE_MSG);
    */
void func_800CD6C4(void) {
    osCreateMesgQueue(&D_80145528, &D_80127900, 4);
    osCreateMesgQueue(&D_80134780, &D_80127910, 1);
    func_800D5910(&D_80144450, &D_80145528, 3);
}

    /* TODO (Clean version): Process scheduler messages using libultra OSScMsg types:
    for (;;) {
        osRecvMesg(&audioEventQ, (OSMesg *)&msg, OS_MESG_BLOCK);
        if (*msg == OS_SC_RETRACE_MSG) { // 1
            return;
        }
        if (*msg == OS_SC_DONE_MSG) {    // 2
            if (D_800FABA4 != NULL) {
                D_800FABA4(2, D_800FABA1);
            }
            D_800FABA1++;
        }
    }
    */
void func_800CD728(void) {
    s16 *msg;

    for (;;) {
        osRecvMesg(&D_80145528, (void **)&msg, 1);
        if (*msg == 1) {
            return;
        }
        if (*msg == 2) {
            if (D_800FABA4 != NULL) {
                D_800FABA4(2, D_800FABA1);
            }
            D_800FABA1++;
        }
    }
}

    /* TODO (Clean version): Unpack microcode task arguments into audioScTask directly:
    void func_800CD7B8(u32 *a) {
        if (audioEnabled) { // D_800FABA0
            audioScTask.list.t.data_ptr    = (u64 *)a[0];
            audioScTask.list.t.data_size   = a[1];
            audioScTask.list.t.ucode       = (u64 *)a[2];
            audioScTask.list.t.ucode_data  = (u64 *)a[3];
            osSendMesg(sc->audioRequestMQ, &audioScTask, OS_MESG_BLOCK);
            osRecvMesg(&audioReplyQ, NULL, OS_MESG_BLOCK);
        }
        if (D_800FABA4 != NULL && D_800FABA1 != 0) {
            D_800FABA4(1, D_800FABA1);
            D_800FABA1++;
        }
    }
    */
void func_800CD7B8(u32 *a) {
    if ((*(u8 *)&D_800FABA0) == 1) {
        D_80138AB8.data_ptr = (void *)a[0];
        D_80138AB8.data_size = a[1];
        D_80138AB8.ucode = (void *)a[2];
        D_80138AB8.ucode_data = (void *)a[3];
        osSendMesg(D_8014D304, &D_80138AB8, 1);
        osRecvMesg(((u8 *)&D_80134780), 0, 1);
    }
    if (D_800FABA4 != 0 && D_800FABA1 != 0) {
        D_800FABA4(1, D_800FABA1);
        D_800FABA1++;
    }
}
