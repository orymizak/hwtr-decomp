#include "common.h"
#include "include_asm.h"
#include "PRinternal/piint.h"
#include "PR/ultraerror.h"

// TODO: this comes from a header
#ident "$Revision: 1.17 $"


s32 __osEPiRawWriteIo(OSPiHandle* pihandle, u32 devAddr, u32 data) {
    register u32 stat;
    register u32 domain;


    EPI_SYNC(pihandle, stat, domain);
    IO_WRITE(pihandle->baseAddress | devAddr, data);

    return 0;
}
