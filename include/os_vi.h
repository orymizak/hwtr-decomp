#ifndef OS_VI_H
#define OS_VI_H

/* Minimal libultra VI manager types (layout from ultralib's viint.h). */

#define VI_STATE_YSCALE_UPDATED 0x04
#define VI_STATE_BUFFER_UPDATED 0x10

typedef struct {
    /* 0x0 */ f32 factor;
    /* 0x4 */ u16 offset;
    /* 0x8 */ u32 scale;
} __OSViScale;

typedef struct {
    /* 0x00 */ u16 state;
    /* 0x02 */ u16 retraceCount;
    /* 0x04 */ void *framep;
    /* 0x08 */ void *modep;
    /* 0x0C */ u32 control;
    /* 0x10 */ void *msgq;
    /* 0x14 */ void *msg;
    /* 0x18 */ __OSViScale x;
    /* 0x24 */ __OSViScale y;
} __OSViContext;

extern __OSViContext *__osViNext;

u32 __osDisableInt(void);
void __osRestoreInt(u32);

#endif
