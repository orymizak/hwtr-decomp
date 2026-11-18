#include "common.h"
#include "n_audio.h"
#include "include_asm.h"

Acmd *func_800D3B84(Acmd *ptr, N_PVoice *f, s32 tsam, s32 nbytes, s16 outp, s16 inp, u32 flags);

/* ADPCM decoder pull (n_alAdpcmPull): decodes outCount samples into *outp, handling the loop. */
Acmd *func_800D35B0(void *filter, s16 *outp, s32 outCount, Acmd *p) {
    Acmd *ptr = p;
    s16 inp;
    s32 tsam;
    s32 nframes;
    s32 nbytes;
    s32 overFlow;
    s32 startZero;
    s32 nOver;
    s32 nSam;
    s32 op;
    s32 nLeft;
    s32 bEnd;
    s32 decoded = 0;
    s32 looped = 0;
    N_PVoice *f = (N_PVoice *)filter;

    if (outCount == 0)
        return ptr;

    inp = 0;
    aLoadADPCM(ptr++, f->dc_bookSize, K0_TO_PHYS(f->dc_table->waveInfo.adpcmWave.book->book));

    looped = (outCount + f->dc_sample > f->dc_loop.end) && (f->dc_loop.count != 0);
    if (looped)
        nSam = f->dc_loop.end - f->dc_sample;
    else
        nSam = outCount;

    if (f->dc_lastsam)
        nLeft = 16 - f->dc_lastsam;
    else
        nLeft = 0;
    tsam = nSam - nLeft;
    if (tsam < 0) tsam = 0;

    nframes = (tsam + 16 - 1) >> 4;
    nbytes = nframes * 9;

    if (looped) {
        ptr = func_800D3B84(ptr, f, tsam, nbytes, *outp, inp, f->dc_first);

        if (f->dc_lastsam)
            *outp += (f->dc_lastsam << 1);
        else
            *outp += (16 << 1);

        f->dc_lastsam = f->dc_loop.start & 0xf;
        f->dc_memin = (s32)f->dc_table->base + 9 * ((s32)(f->dc_loop.start >> 4) + 1);
        f->dc_sample = f->dc_loop.start;

        bEnd = *outp;
        while (outCount > nSam) {
            outCount -= nSam;
            op = (bEnd + ((nframes + 1) << 5) + 16) & ~0x1f;
            bEnd += (nSam << 1);
            if ((f->dc_loop.count != -1) && (f->dc_loop.count != 0))
                f->dc_loop.count--;
            nSam = ((u32)outCount < f->dc_loop.end - f->dc_loop.start) ? outCount : f->dc_loop.end - f->dc_loop.start;
            tsam = nSam - 16 + f->dc_lastsam;
            if (tsam < 0) tsam = 0;
            nframes = (tsam + 16 - 1) >> 4;
            nbytes = nframes * 9;
            ptr = func_800D3B84(ptr, f, tsam, nbytes, op, inp, f->dc_first | A_LOOP);
            aDMEMMove(ptr++, op + (f->dc_lastsam << 1), bEnd, nSam << 1);
        }

        f->dc_lastsam = (outCount + f->dc_lastsam) & 0xf;
        f->dc_sample += outCount;
        f->dc_memin += 9 * nframes;
        return ptr;
    }

    nSam = nframes << 4;
    overFlow = f->dc_memin + nbytes - ((s32)f->dc_table->base + f->dc_table->len);
    if (overFlow < 0)
        overFlow = 0;
    nOver = (overFlow / 9) << 4;
    if (nOver > nSam + nLeft)
        nOver = nSam + nLeft;

    nbytes -= overFlow;

    if ((nOver - (nOver & 0xf)) < outCount) {
        decoded = 1;
        ptr = func_800D3B84(ptr, f, nSam - nOver, nbytes, *outp, inp, f->dc_first);

        if (f->dc_lastsam)
            *outp += (f->dc_lastsam << 1);
        else
            *outp += (16 << 1);

        f->dc_lastsam = (outCount + f->dc_lastsam) & 0xf;
        f->dc_sample += outCount;
        f->dc_memin += 9 * nframes;
    } else {
        f->dc_lastsam = 0;
        f->dc_memin += 9 * nframes;
    }

    if (nOver) {
        f->dc_lastsam = 0;
        if (decoded)
            startZero = (nLeft + nSam - nOver) << 1;
        else
            startZero = 0;
        aClearBuffer(ptr++, startZero + *outp, nOver << 1);
    }
    return ptr;
}

void func_800D39F8(N_PVoice *a, s32 paramID, void *param) {

    switch (paramID) {
    case AL_FILTER_SET_WAVETABLE:
        a->dc_table = (ALWaveTable *)param;
        a->dc_memin = (s32)a->dc_table->base;
        a->dc_sample = 0;
        switch (a->dc_table->type) {
        case AL_ADPCM_WAVE:
            a->dc_table->len = 9 * ((s32)(a->dc_table->len / 9));
            a->dc_bookSize = 2 * a->dc_table->waveInfo.adpcmWave.book->order * a->dc_table->waveInfo.adpcmWave.book->npredictors * 8;
            if (a->dc_table->waveInfo.adpcmWave.loop) {
                a->dc_loop.start = a->dc_table->waveInfo.adpcmWave.loop->start;
                a->dc_loop.end = a->dc_table->waveInfo.adpcmWave.loop->end;
                a->dc_loop.count = a->dc_table->waveInfo.adpcmWave.loop->count;
                alCopy(a->dc_table->waveInfo.adpcmWave.loop->state, a->dc_lstate, 0x20);
            } else {
                a->dc_loop.start = a->dc_loop.end = a->dc_loop.count = 0;
            }
            break;
        case AL_RAW16_WAVE:
            if (a->dc_table->waveInfo.rawWave.loop) {
                a->dc_loop.start = a->dc_table->waveInfo.rawWave.loop->start;
                a->dc_loop.end = a->dc_table->waveInfo.rawWave.loop->end;
                a->dc_loop.count = a->dc_table->waveInfo.rawWave.loop->count;
            } else {
                a->dc_loop.start = a->dc_loop.end = a->dc_loop.count = 0;
            }
            break;
        default:
            break;
        }
        break;
    case AL_FILTER_RESET:
        a->dc_lastsam = 0;
        a->dc_first = 1;
        a->dc_sample = 0;
        if (a->dc_table) {
            a->dc_memin = (s32)a->dc_table->base;
            if (a->dc_table->type == AL_ADPCM_WAVE) {
                if (a->dc_table->waveInfo.adpcmWave.loop) {
                    a->dc_loop.count = a->dc_table->waveInfo.adpcmWave.loop->count;
                }
            } else if (a->dc_table->type == AL_RAW16_WAVE) {
                if (a->dc_table->waveInfo.rawWave.loop) {
                    a->dc_loop.count = a->dc_table->waveInfo.rawWave.loop->count;
                }
            }
        }
        break;
    }
}

Acmd *func_800D3B84(Acmd *ptr, N_PVoice *f, s32 tsam, s32 nbytes, s16 outp, s16 inp, u32 flags) {
    s32 dramAlign, dramLoc;

    if (nbytes > 0) {
        dramLoc = f->dc_dma(f->dc_memin, nbytes, f->dc_dmaState);
        dramAlign = dramLoc & 7;
        nbytes += dramAlign;
        n_aLoadBuffer(ptr++, nbytes + 8 - (nbytes & 7), inp, dramLoc - dramAlign);
    } else {
        dramAlign = 0;
    }
    if (flags & A_LOOP) {
        aSetLoop(ptr++, K0_TO_PHYS(f->dc_lstate));
    }
    n_aADPCMdec(ptr++, f->dc_state, flags, tsam << 1, dramAlign, outp);
    f->dc_first = 0;
    return ptr;
}
