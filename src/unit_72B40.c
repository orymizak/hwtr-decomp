#include "common.h"
#include "include_asm.h"
typedef struct { u32 type; u32 flags; void *ucode_boot; u32 ucode_boot_size; void *ucode; u32 ucode_size; void *ucode_data; u32 ucode_data_size; void *dram_stack; u32 dram_stack_size; void *output_buff; void *output_buff_size; void *data_ptr; u32 data_size; void *yield_data_ptr; u32 yield_data_size; } OSTaskT_;
typedef struct OSScTaskT_ { struct OSScTaskT_ *next; u32 state; u32 flags; void *framebuffer; OSTaskT_ list; void *msgQ; void *msg; u64 startTime; u64 totalTime; } OSScTaskT_;
typedef struct { void *data; u32 size; void *ucode; void *ucode_data; } AudInfoT_;
s32 osRecvMesg(s32, s16 **,s32);
s32 func_800D23FC();
s32 osScAddClient(s32, s32, s32);
s32 osCreateMesgQueue(s32, s32,s32);
extern s32 D_801279D4;

extern s32 D_801279D0;

void func_800D1F40(s32 arg0) {
    D_801279D0 = arg0;
}

void func_800D1F4C(void) {
    s32 temp_v0;

    temp_v0 = func_800D23FC(0x58);
    D_801279D4 = temp_v0;
    osCreateMesgQueue(temp_v0 + 8, temp_v0 + 0x20, 4);
    osCreateMesgQueue(D_801279D4 + 0x30, D_801279D4 + 0x48, 4);
    osScAddClient(D_801279D0, D_801279D4, D_801279D4 + 8);
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
void *osScGetCmdQ(void *sc);
s32 osSendMesg(void *mq, void *msg, s32 flag);
void func_800D2010(AudInfoT_ *a) {
    OSScTaskT_ t;
    u8 msg[0x20];

    t.next = 0;
    t.msgQ = D_801279D4 + 0x30;
    t.msg = msg;
    t.flags = 2;
    t.list.data_ptr = a->data;
    t.list.data_size = a->size;
    t.list.type = 2;
    t.list.ucode_boot = D_800E7AA0;
    t.list.ucode_boot_size = D_800E7B70 - D_800E7AA0;
    t.list.flags = 0;
    t.list.ucode = a->ucode;
    t.list.ucode_data = a->ucode_data;
    t.list.ucode_size = 0x1000;
    t.list.ucode_data_size = 0x800;
    t.list.dram_stack = 0;
    t.list.dram_stack_size = 0;
    t.list.output_buff = 0;
    t.list.output_buff_size = 0;
    t.list.yield_data_ptr = 0;
    t.list.yield_data_size = 0;
    osSendMesg(osScGetCmdQ(D_801279D0), &t, 1);
    osRecvMesg(D_801279D4 + 0x30, 0, 1);
}
