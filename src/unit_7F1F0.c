#include "common.h"
#include "include_asm.h"
#include "PR/os_internal.h"
#include "PR/rcp.h"

void osSpTaskYield(void) {
    __osSpSetStatus(SP_SET_YIELD);
}
