#include "common.h"
#include "include_asm.h"
typedef struct { u32 type; u32 flags; u64 *ucode_boot; u32 ucode_boot_size; u64 *ucode; u32 ucode_size; u64 *ucode_data; u32 ucode_data_size; u64 *dram_stack; u32 dram_stack_size; u64 *output_buff; u64 *output_buff_size; u64 *data_ptr; u32 data_size; u64 *yield_data_ptr; u32 yield_data_size; } SpTask_;
extern SpTask_ D_80133230;
void bcopy(void *, void *, s32);
u32 osVirtualToPhysical(void *);
void osWritebackDCache(void *, s32);
s32 __osSpSetPc(u32);
s32 __osSpRawStartDma(s32, u32, void *, u32);

s32 __osSpDeviceBusy();
s32 __osSpSetStatus(s32);

static SpTask_ *_VirtualToPhysicalTask(SpTask_ *intp) {
    SpTask_ *tp;

    tp = &D_80133230;
    bcopy(intp, tp, sizeof(SpTask_));
    if (tp->ucode != NULL) {
        tp->ucode = (u64 *)osVirtualToPhysical(tp->ucode);
    }
    if (tp->ucode_data != NULL) {
        tp->ucode_data = (u64 *)osVirtualToPhysical(tp->ucode_data);
    }
    if (tp->dram_stack != NULL) {
        tp->dram_stack = (u64 *)osVirtualToPhysical(tp->dram_stack);
    }
    if (tp->output_buff != NULL) {
        tp->output_buff = (u64 *)osVirtualToPhysical(tp->output_buff);
    }
    if (tp->output_buff_size != NULL) {
        tp->output_buff_size = (u64 *)osVirtualToPhysical(tp->output_buff_size);
    }
    if (tp->data_ptr != NULL) {
        tp->data_ptr = (u64 *)osVirtualToPhysical(tp->data_ptr);
    }
    if (tp->yield_data_ptr != NULL) {
        tp->yield_data_ptr = (u64 *)osVirtualToPhysical(tp->yield_data_ptr);
    }
    return tp;
}

void osSpTaskLoad(SpTask_ *intp) {
    SpTask_ *tp;

    tp = _VirtualToPhysicalTask(intp);
    if (tp->flags & 1) {
        tp->ucode_data = tp->yield_data_ptr;
        tp->ucode_data_size = tp->yield_data_size;
        intp->flags &= ~1;
        if (tp->flags & 4) {
            tp->ucode = (u64 *)*(volatile u32 *)(((u32)intp->yield_data_ptr + 0xC00 - 4) | 0xA0000000);
        }
    }
    osWritebackDCache(tp, sizeof(SpTask_));
    __osSpSetStatus(0x2B00);
    while (__osSpSetPc(0x04001000) == -1) {
    }
    while (__osSpRawStartDma(1, 0x04001000 - sizeof(*tp), tp, sizeof(SpTask_)) == -1) {
    }
    while (__osSpDeviceBusy()) {
    }
    while (__osSpRawStartDma(1, 0x04001000, tp->ucode_boot, tp->ucode_boot_size) == -1) {
    }
}


void osSpTaskStartGo(void) {
    do {

    } while (__osSpDeviceBusy() != 0);
    __osSpSetStatus(0x125);
}
