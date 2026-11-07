#include "common.h"
#include "include_asm.h"
typedef struct { void *next; s32 priority; void *queue; void *tlnext; u16 state; u16 flags; s32 id; s32 fp; u8 p1C[0x1C]; s64 a0; u8 p40[0xB0]; s64 sp; u8 pF8[8]; s64 ra; u8 p108[0x10]; u32 sr; void *pc; u8 p120[8]; u32 rcp; u32 fpcsr; } Thread_;
s32 __osDisableInt(void);
void __osRestoreInt(s32);
extern u8 D_800E3AA8[];
extern Thread_ *D_800FB60C;

extern void *__osRunningThread;

void osCreateThread(Thread_ *t, s32 id, void *entry, void *arg, void *sp, s32 p) {
    s32 saveMask;

    t->id = id;
    t->priority = p;
    t->next = NULL;
    t->queue = NULL;
    t->pc = entry;
    t->a0 = (s64)(s32)arg;
    t->sp = (s64)(s32)sp - 16;
    t->ra = (s64)(s32)D_800E3AA8;
    t->sr = 0xFF03;
    t->rcp = 0x3F;
    t->fpcsr = 0x01000800;
    t->fp = 0;
    t->state = 1;
    t->flags = 0;
    saveMask = __osDisableInt();
    t->tlnext = D_800FB60C;
    D_800FB60C = t;
    __osRestoreInt(saveMask);
}

s32 osGetThreadPri(void *arg0) {
    void *var_a0;

    var_a0 = arg0;
    if (var_a0 == NULL) {
        var_a0 = __osRunningThread;
    }
    return *(s32 *)((u8 *)var_a0 + 0x4);
}
