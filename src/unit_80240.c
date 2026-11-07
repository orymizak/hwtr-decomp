#include "common.h"
#include "include_asm.h"
extern u64 D_801415B0;
extern u32 D_801345C4;
u32 __osDisableInt(void);
void __osRestoreInt(u32);
u32 osGetCount(void);



u64 osGetTime(void) {
    u32 tmptime;
    u32 elapseCount;
    u64 currentCount;
    register u32 saveMask;

    saveMask = __osDisableInt();
    tmptime = osGetCount();
    elapseCount = tmptime - D_801345C4;
    currentCount = D_801415B0;
    __osRestoreInt(saveMask);
    return currentCount + elapseCount;
}
