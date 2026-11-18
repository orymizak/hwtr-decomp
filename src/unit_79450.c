#include "common.h"
#include "include_asm.h"
#include "PR/rcp.h"

// TODO: this comes from a header
#ident "$Revision: 1.17 $"
#include "PR/rcp.h"

// TODO: this comes from a header
#ident "$Revision: 1.17 $"
#include "PR/rcp.h"
#include "PR/ultraerror.h"
#include "PRinternal/osint.h"

extern // TODO: not sure if this should be here
extern s32 osViClock;



// TODO: this comes from a header
#ident "$Revision: 1.17 $"


/**
 * Returns the number of bytes remaining in a currently ongoing audio DMA.
 *
 * Note that audio DMA is double-buffered, a DMA can be queued while another is in-progress. This only returns
 * information about the currently in-progress DMA.
 */
u32 osAiGetLength(void) {
    return IO_READ(AI_LEN_REG);
}


u32 osAiGetStatus(void) {
    return IO_READ(AI_STATUS_REG);
}


/**
 * Programs the operating frequency of the Audio DAC.
 *
 * @param frequency Target Playback frequency.
 * @return The actual playback frequency, or -1 if the supplied frequency cannot be used.
 */
s32 osAiSetFrequency(u32 frequency) {
    register unsigned int dacRate;
    register unsigned char bitRate;
    register float f;


    // Calculate the DAC sample period ("dperiod") (dperiod + 1 = vid_clock / frequency)
    f = osViClock / (float)frequency + .5f;
    dacRate = f;

    // Upcoming division by 66. If dacRate is smaller than 2 * 66 = 132, bitrate will be 1 and AI_BITRATE_REG will be
    // programmed with 0, which results in no audio output. Return -1 to indicate an unusable frequency.
    if (dacRate < AI_MIN_DAC_RATE) {
        return -1;
    }

    // Calculate the largest "bitrate" (ABUS clock half period, "aclockhp") supported for this dacrate. These two
    // quantities must satisfy (dperiod + 1) >= 66 * (aclockhp + 1), here this is taken as equality.
    bitRate = dacRate / 66;
    // Clamp to max value
    if (bitRate > AI_MAX_BIT_RATE) {
        bitRate = AI_MAX_BIT_RATE;
    }

    IO_WRITE(AI_DACRATE_REG, dacRate - 1);
    IO_WRITE(AI_BITRATE_REG, bitRate - 1);
    // Return the true playback frequency (frequency = vid_clock / (dperiod + 1)), which may differ from the target
    // frequency.
    return osViClock / (s32)dacRate;
}
