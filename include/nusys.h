#ifndef NUSYS_H
#define NUSYS_H

#include "ultra64.h"

/* NuSystem scheduler task (nusys.h): an OSTask plus the scheduler's bookkeeping. */
typedef struct NUScTask_s {
    /* 0x00 */ struct NUScTask_s *next;
    /* 0x04 */ u32 state;
    /* 0x08 */ u32 flags;
    /* 0x0C */ void *framebuffer;
    /* 0x10 */ OSTask list;
    /* 0x50 */ OSMesgQueue *msgQ;
    /* 0x54 */ OSMesg msg;
} NUScTask; /* size = 0x58 */

#endif
