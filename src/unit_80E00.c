#include "common.h"
#include "include_asm.h"
typedef struct { u32 ctrl; u32 width; u32 burst; u32 vSync; u32 hSync; u32 leap; u32 hStart; u32 xScale; u32 vCurrent; } ViCommonRegs_;
typedef struct { u32 origin; u32 yScale; u32 vStart; u32 vBurst; u32 vIntr; } ViFieldRegs_;
typedef struct { u8 type; ViCommonRegs_ comRegs; ViFieldRegs_ fldRegs[2]; } ViMode2_;
typedef struct { f32 factor; u16 offset; u32 scale; } ViScale_;
typedef struct { u16 state; u16 retraceCount; void *framep; ViMode2_ *modep; u32 control; void *msgq; void *msg; ViScale_ x; ViScale_ y; } ViCtx2_;
extern ViCtx2_ *__osViCurr;
extern ViCtx2_ *__osViNext;
extern u32 D_800FB64C;
u32 osVirtualToPhysical(void *);



void __osViSwapContext(void) {
    register ViMode2_ *vm;
    register ViCtx2_ *vc;
    u32 origin;
    u32 hStart;
    u32 vStart;
    u32 nomValue;
    u32 field;

    field = 0;
    vc = __osViNext;
    vm = vc->modep;
    field = *(volatile u32 *)0xA4400010 & 1;
    origin = osVirtualToPhysical(vc->framep) + (vm->fldRegs[field].origin);
    if (vc->state & 2) {
        vc->x.scale |= (vm->comRegs.xScale & ~0xFFF);
    } else {
        vc->x.scale = vm->comRegs.xScale;
    }
    if (vc->state & 4) {
        nomValue = vm->fldRegs[field].yScale & 0xFFF;
        vc->y.scale = vc->y.factor * nomValue;
        vc->y.scale |= vm->fldRegs[field].yScale & ~0xFFF;
    } else {
        vc->y.scale = vm->fldRegs[field].yScale;
    }
    vStart = (vm->fldRegs[field].vStart - (D_800FB64C << 0x10)) + D_800FB64C;
    hStart = vm->comRegs.hStart;
    if (vc->state & 0x20) {
        hStart = 0;
    }
    if (vc->state & 0x40) {
        vc->y.scale = 0;
        origin = osVirtualToPhysical(vc->framep);
    }
    if (vc->state & 0x80) {
        vc->y.scale = (vc->y.offset << 0x10) & (0x3FF << 0x10);
        origin = osVirtualToPhysical(vc->framep);
    }
    *(volatile u32 *)0xA4400004 = origin;
    *(volatile u32 *)0xA4400008 = vm->comRegs.width;
    *(volatile u32 *)0xA4400014 = vm->comRegs.burst;
    *(volatile u32 *)0xA4400018 = vm->comRegs.vSync;
    *(volatile u32 *)0xA440001C = vm->comRegs.hSync;
    *(volatile u32 *)0xA4400020 = vm->comRegs.leap;
    *(volatile u32 *)0xA4400024 = hStart;
    *(volatile u32 *)0xA4400028 = vStart;
    *(volatile u32 *)0xA440002C = vm->fldRegs[field].vBurst;
    *(volatile u32 *)0xA440000C = vm->fldRegs[field].vIntr;
    *(volatile u32 *)0xA4400030 = vc->x.scale;
    *(volatile u32 *)0xA4400034 = vc->y.scale;
    *(volatile u32 *)0xA4400000 = vc->control;
    __osViNext = __osViCurr;
    __osViCurr = vc;
    *__osViNext = *__osViCurr;
}
