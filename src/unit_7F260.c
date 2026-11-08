#include "common.h"
#include "include_asm.h"
u32 osVirtualToPhysical(void *);
void osWritebackDCache(void *, s32);
void osInvalDCache(void *, s32);
u32 __scSchedule(s32, void *, void *, u32);

s32 osSetIntMask(s32);
s32 osViGetCurrentFramebuffer();
s32 osViGetNextFramebuffer();
s32 osSpTaskYield(void *);

INCLUDE_ASM("asm/nonmatchings/unit_7F260", osCreateScheduler);

void osScAddClient(void *arg0, void *arg1, s32 arg2) {
    s32 temp_v0;

    temp_v0 = osSetIntMask(1);
    *(s32 *)((u8 *)arg1 + 0x4) = arg2;
    *(s32 *)((u8 *)arg1 + 0x0) = (void *) *(s32 *)((u8 *)arg0 + 0x260);
    *(s32 *)((u8 *)arg0 + 0x260) = arg1;
    osSetIntMask(temp_v0);
}

INCLUDE_ASM("asm/nonmatchings/unit_7F260", osScRemoveClient);

s32 osScGetCmdQ(s32 arg0) {
    return arg0 + 0x78;
}

INCLUDE_ASM("asm/nonmatchings/unit_7F260", __scMain);

INCLUDE_ASM("asm/nonmatchings/unit_7F260", __scHandleRetrace);

INCLUDE_ASM("asm/nonmatchings/unit_7F260", __scHandleRSP);

void __scHandleRDP(s32 arg0) {
    s32 sp10;
    s32 sp14;
    u32 var_s0;
    void *temp_a1;

    temp_a1 = (*(s32 *)((u8 *)(arg0) + 0x278));
    sp10 = 0;
    sp14 = 0;
    (*(s32 *)((u8 *)(arg0) + 0x278)) = NULL;
    (*(s32 *)((u8 *)(temp_a1) + 0x4)) = (s32) ((*(s32 *)((u8 *)(temp_a1) + 0x4)) & ~1);
    __scTaskComplete(arg0, (s32) temp_a1);
    var_s0 = ((*(s32 *)((u8 *)(arg0) + 0x274)) == 0) * 2;
    if ((*(s32 *)((u8 *)(arg0) + 0x278)) == NULL) {
        var_s0 |= 1;
    }
    if (__scSchedule(arg0, &sp10, &sp14, var_s0) != var_s0) {
        __scExec(arg0, sp10, sp14);
    }
}

s32 __scTaskReady(s32 arg0) {
    s32 temp_s0;

    if (arg0 != 0) {
        temp_s0 = osViGetCurrentFramebuffer();
        return arg0 & -(temp_s0 == osViGetNextFramebuffer());
    }
    return 0;
}

INCLUDE_ASM("asm/nonmatchings/unit_7F260", __scTaskComplete);

INCLUDE_ASM("asm/nonmatchings/unit_7F260", __scAppendList);

INCLUDE_ASM("asm/nonmatchings/unit_7F260", __scExec);

void __scYield(void *arg0) {
    void *temp_a0;

    temp_a0 = *(s32 *)((u8 *)arg0 + 0x274);
    if (*(s32 *)((u8 *)temp_a0 + 0x10) == 1) {
        *(s32 *)((u8 *)temp_a0 + 0x4) = (s32) (*(s32 *)((u8 *)temp_a0 + 0x4) | 0x10);
        osSpTaskYield(temp_a0);
    }
}

INCLUDE_ASM("asm/nonmatchings/unit_7F260", __scSchedule);

s32 __osSiRawStartDma(s32 direction, void *dramAddr) {
    if (*(volatile u32 *)0xA4800018 & 3) {
        return -1;
    }
    if (direction == 1) {
        osWritebackDCache(dramAddr, 64);
    }
    *(volatile u32 *)0xA4800000 = osVirtualToPhysical(dramAddr);
    if (direction == 0) {
        *(volatile u32 *)0xA4800004 = 0x1FC007C0;
    } else {
        *(volatile u32 *)0xA4800010 = 0x1FC007C0;
    }
    if (direction == 0) {
        osInvalDCache(dramAddr, 64);
    }
    return 0;
}
