#include "common.h"
#include "include_asm.h"
typedef struct OSThread_ { struct OSThread_ *next; s32 priority; struct OSThread_ **queue; struct OSThread_ *tlnext; u16 state; } OSThread_;
extern OSThread_ *__osRunningThread;
extern OSThread_ *D_800FB60C;
u32 __osDisableInt(void);
void __osRestoreInt(u32);
void __osDequeueThread(OSThread_ **queue, OSThread_ *t);
void __osDispatchThread(void);



void osDestroyThread(OSThread_ *t) {
    register u32 saveMask;
    register OSThread_ *pred;
    register OSThread_ *succ;

    saveMask = __osDisableInt();
    if (t == 0) {
        t = __osRunningThread;
    } else if (t->state != 1) {
        __osDequeueThread(t->queue, t);
    }
    if (D_800FB60C == t) {
        D_800FB60C = D_800FB60C->tlnext;
    } else {
        pred = D_800FB60C;
        while (pred->priority != -1) {
            succ = pred->tlnext;
            if (succ == t) {
                pred->tlnext = t->tlnext;
                break;
            }
            pred = succ;
        }
    }
    if (t == __osRunningThread) {
        __osDispatchThread();
    }
    __osRestoreInt(saveMask);
}

