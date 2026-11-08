#include "common.h"
#include "include_asm.h"
typedef struct { u32 ctrl; u32 width; } ViComRegs_;
typedef struct { u8 type; u8 pad[3]; ViComRegs_ comRegs; } OSViMode_;
typedef struct { u16 state; u16 retraceCount; void *framep; OSViMode_ *modep; u32 control; void *msgq; void *msg; u8 xy[0x18]; } ViContext_;
extern ViContext_ D_800FC890[2];
extern ViContext_ *__osViCurr;
extern ViContext_ *__osViNext;
extern s32 D_80000300;
extern OSViMode_ D_800FC900;
extern OSViMode_ D_800FC950;
extern OSViMode_ D_800FC9A0;
void bzero(void *p, s32 n);
void __osViSwapContext(void);



void __osViInit(void) {
    bzero(D_800FC890, sizeof(D_800FC890));
    __osViCurr = &D_800FC890[0];
    __osViNext = &D_800FC890[1];
    __osViNext->retraceCount = 1;
    __osViCurr->retraceCount = 1;
    __osViNext->framep = (void *)0x80000000;
    __osViCurr->framep = (void *)0x80000000;
    if (D_80000300 == 0) {
        __osViNext->modep = &D_800FC950;
    } else if (D_80000300 == 2) {
        __osViNext->modep = &D_800FC9A0;
    } else {
        __osViNext->modep = &D_800FC900;
    }
    __osViNext->state = 0x20;
    __osViNext->control = __osViNext->modep->comRegs.ctrl;
    while (*(volatile u32 *)0xA4400010 > 10) {
    }
    *(volatile u32 *)0xA4400000 = 0;
    __osViSwapContext();
}
