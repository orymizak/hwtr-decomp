#include "common.h"
#include "include_asm.h"
#include "audio/synthInternals.h"


void alSynDelete(ALSynth *drvr)
{
    drvr->head = 0;
}
