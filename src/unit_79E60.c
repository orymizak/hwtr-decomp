#include "common.h"
#include "include_asm.h"
typedef struct { u32 w0; u32 w1; } Acmd7_;
typedef struct { u32 start; u32 end; u32 count; } RawLoop7_;
typedef struct { u8 *base; s32 len; u8 type; u8 flags; } WaveTable7_;
typedef struct { u8 filter[0x14]; void *state; void *lstate; RawLoop7_ loop; WaveTable7_ *table; s32 bookSize; s32 (*dma)(s32, s32, void *); void *dmaState; s32 sample; s32 lastsam; s32 first; s32 memin; } LoadFilter7_;
typedef struct { u32 start; u32 end; u32 count; s16 state[16]; } AdpcmLoop_;
typedef struct { u32 start; u32 end; u32 count; } RawLoop_;
typedef struct { s32 order; s32 npredictors; s16 book[1]; } AdpcmBook_;
typedef struct { u8 *base; s32 len; u8 type; u8 flags; union { struct { AdpcmLoop_ *loop; AdpcmBook_ *book; } adpcmWave; struct { RawLoop_ *loop; } rawWave; } waveInfo; } WaveTable_;
typedef struct { void *source; void *handler; void *setParam; s32 inp; s32 outp; } Filter4_;
typedef struct { Filter4_ filter; void *state; void *lstate; RawLoop_ loop; WaveTable_ *table; s32 bookSize; void *dma; void *dmaState; s32 sample; s32 lastsam; s32 first; s32 memin; } LoadFilter4_;
Acmd7_ *alRaw16Pull(void *filter, s16 *outp, s32 outCount, s32 sampleOffset, Acmd7_ *p);
void alCopy(void *src, void *dest, s32 len);
typedef struct { u32 w0; u32 w1; } Acmd_;
typedef struct { u8 p0[0x14]; u32 state; u32 lstate; u8 p1C[0x14]; s32 (*dma)(s32, s32, void *); void *dmaState; u8 p38[8]; s32 first; s32 memin; } Load_;



Acmd_ *_decodeChunk(Acmd_ *ptr, Load_ *f, s32 tsam, s32 nbytes, s16 outp, s16 inp, u32 flags);
Acmd_ *alAdpcmPull(void *filter, s16 *outp, s32 outCount, s32 sampleOffset, Acmd_ *p) {
    Acmd_ *ptr = p;
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
    LoadFilter4_ *f = (LoadFilter4_ *)filter;

    if (outCount == 0)
        return ptr;
    inp = 0;
    { Acmd_ *_a = ptr++; _a->w0 = (11 << 24) | (f->bookSize & 0xFFFFFF); _a->w1 = (u32)f->table->waveInfo.adpcmWave.book->book & 0x1FFFFFFF; }
    looped = (outCount + f->sample > f->loop.end) && (f->loop.count != 0);
    if (looped)
        nSam = f->loop.end - f->sample;
    else
        nSam = outCount;
    if (f->lastsam)
        nLeft = 16 - f->lastsam;
    else
        nLeft = 0;
    tsam = nSam - nLeft;
    if (tsam < 0) tsam = 0;
    nframes = (tsam + 16 - 1) >> 4;
    nbytes = nframes * 9;
    if (looped) {
        ptr = _decodeChunk(ptr, (Load_ *)f, tsam, nbytes, *outp, inp, f->first);
        if (f->lastsam)
            *outp += (f->lastsam << 1);
        else
            *outp += (16 << 1);
        f->lastsam = f->loop.start & 0xf;
        f->memin = (s32)f->table->base + 9 * ((s32)(f->loop.start >> 4) + 1);
        f->sample = f->loop.start;
        bEnd = *outp;
        while (outCount > nSam) {
            outCount -= nSam;
            op = (bEnd + ((nframes + 1) << (4 + 1))) & ~0x1f;
            bEnd += (nSam << 1);
            if ((f->loop.count != -1) && (f->loop.count != 0))
                f->loop.count--;
            nSam = (outCount < f->loop.end - f->loop.start) ? outCount : f->loop.end - f->loop.start;
            tsam = nSam - 16 + f->lastsam;
            if (tsam < 0) tsam = 0;
            nframes = (tsam + 16 - 1) >> 4;
            nbytes = nframes * 9;
            ptr = _decodeChunk(ptr, (Load_ *)f, tsam, nbytes, op, inp, f->first | 2);
            { Acmd_ *_a = ptr++; _a->w0 = (10 << 24) | ((op + (f->lastsam << 1)) & 0xFFFFFF); _a->w1 = ((bEnd & 0xFFFF) << 16) | ((nSam << 1) & 0xFFFF); }
        }
        f->lastsam = (outCount + f->lastsam) & 0xf;
        f->sample += outCount;
        f->memin += 9 * nframes;
        return ptr;
    }
    nSam = nframes << 4;
    overFlow = f->memin + nbytes - ((s32)f->table->base + f->table->len);
    if (overFlow < 0)
        overFlow = 0;
    nOver = (overFlow / 9) << 4;
    if (nOver > nSam + nLeft)
        nOver = nSam + nLeft;
    nbytes -= overFlow;
    if ((nOver - (nOver & 0xf)) < outCount) {
        decoded = 1;
        ptr = _decodeChunk(ptr, (Load_ *)f, nSam - nOver, nbytes, *outp, inp, f->first);
        if (f->lastsam)
            *outp += (f->lastsam << 1);
        else
            *outp += (16 << 1);
        f->lastsam = (outCount + f->lastsam) & 0xf;
        f->sample += outCount;
        f->memin += 9 * nframes;
    } else {
        f->lastsam = 0;
        f->memin += 9 * nframes;
    }
    if (nOver) {
        f->lastsam = 0;
        if (decoded)
            startZero = (nLeft + nSam - nOver) << 1;
        else
            startZero = 0;
        { Acmd_ *_a = ptr++; _a->w0 = (2 << 24) | ((startZero + *outp) & 0xFFFFFF); _a->w1 = (u32)(nOver << 1); }
    }
    return ptr;
}

Acmd7_ *alRaw16Pull(void *filter, s16 *outp, s32 outCount, s32 sampleOffset, Acmd7_ *p) {
    Acmd7_ *ptr = p;
    s32 nbytes;
    s32 dramLoc;
    s32 dramAlign;
    s32 dmemAlign;
    s32 overFlow;
    s32 startZero;
    s32 nSam;
    s32 op;
    LoadFilter7_ *f = (LoadFilter7_ *)filter;

    if (outCount == 0)
        return ptr;
    if ((outCount + f->sample > f->loop.end) && (f->loop.count != 0)) {
        nSam = f->loop.end - f->sample;
        nbytes = nSam << 1;
        if (nSam > 0) {
            dramLoc = (f->dma)(f->memin, nbytes, f->dmaState);
            dramAlign = dramLoc & 0x7;
            nbytes += dramAlign;
            { Acmd7_ *_a = ptr++; _a->w0 = (8 << 24) | (*outp & 0xFFFF); _a->w1 = (nbytes + 8 - (nbytes & 0x7)) & 0xFFFF; }
            { Acmd7_ *_a = ptr++; _a->w0 = 4 << 24; _a->w1 = (u32)(dramLoc - dramAlign); }
        } else
            dramAlign = 0;
        *outp += dramAlign;
        f->memin = (s32)f->table->base + (f->loop.start << 1);
        f->sample = f->loop.start;
        op = *outp;
        while (outCount > nSam) {
            op += (nSam << 1);
            outCount -= nSam;
            if ((f->loop.count != -1) && (f->loop.count != 0))
                f->loop.count--;
            nSam = (outCount < f->loop.end - f->loop.start) ? outCount : f->loop.end - f->loop.start;
            nbytes = nSam << 1;
            dramLoc = (f->dma)(f->memin, nbytes, f->dmaState);
            dramAlign = dramLoc & 0x7;
            nbytes += dramAlign;
            if (op & 0x7)
                dmemAlign = 8 - (op & 0x7);
            else
                dmemAlign = 0;
            { Acmd7_ *_a = ptr++; _a->w0 = (8 << 24) | ((op + dmemAlign) & 0xFFFF); _a->w1 = (nbytes + 8 - (nbytes & 0x7)) & 0xFFFF; }
            { Acmd7_ *_a = ptr++; _a->w0 = 4 << 24; _a->w1 = (u32)(dramLoc - dramAlign); }
            if (dramAlign || dmemAlign)
                { Acmd7_ *_a = ptr++; _a->w0 = (10 << 24) | ((op + dramAlign + dmemAlign) & 0xFFFFFF); _a->w1 = ((op & 0xFFFF) << 16) | ((nSam << 1) & 0xFFFF); }
        }
        f->sample += outCount;
        f->memin += (outCount << 1);
        return ptr;
    }
    nbytes = outCount << 1;
    overFlow = f->memin + nbytes - ((s32)f->table->base + f->table->len);
    if (overFlow < 0)
        overFlow = 0;
    if (overFlow > nbytes)
        overFlow = nbytes;
    if (overFlow < nbytes) {
        if (outCount > 0) {
            nbytes -= overFlow;
            dramLoc = (f->dma)(f->memin, nbytes, f->dmaState);
            dramAlign = dramLoc & 0x7;
            nbytes += dramAlign;
            { Acmd7_ *_a = ptr++; _a->w0 = (8 << 24) | (*outp & 0xFFFF); _a->w1 = (nbytes + 8 - (nbytes & 0x7)) & 0xFFFF; }
            { Acmd7_ *_a = ptr++; _a->w0 = 4 << 24; _a->w1 = (u32)(dramLoc - dramAlign); }
        } else
            dramAlign = 0;
        *outp += dramAlign;
        f->sample += outCount;
        f->memin += outCount << 1;
    } else {
        f->memin += outCount << 1;
    }
    if (overFlow) {
        startZero = (outCount << 1) - overFlow;
        if (startZero < 0)
            startZero = 0;
        { Acmd7_ *_a = ptr++; _a->w0 = (2 << 24) | ((startZero + *outp) & 0xFFFFFF); _a->w1 = (u32)overFlow; }
    }
    return ptr;
}

s32 alLoadParam(void *filter, s32 paramID, void *param) {
    LoadFilter4_ *a = (LoadFilter4_ *)filter;
    Filter4_ *f = (Filter4_ *)filter;

    switch (paramID) {
        case 5:
            a->table = (WaveTable_ *)param;
            a->memin = (s32)a->table->base;
            a->sample = 0;
            switch (a->table->type) {
                case 0:
                    f->handler = alAdpcmPull;
                    a->table->len = 9 * ((s32)(a->table->len / 9));
                    a->bookSize = 2 * a->table->waveInfo.adpcmWave.book->order * a->table->waveInfo.adpcmWave.book->npredictors * 8;
                    if (a->table->waveInfo.adpcmWave.loop) {
                        a->loop.start = a->table->waveInfo.adpcmWave.loop->start;
                        a->loop.end = a->table->waveInfo.adpcmWave.loop->end;
                        a->loop.count = a->table->waveInfo.adpcmWave.loop->count;
                        alCopy(a->table->waveInfo.adpcmWave.loop->state, a->lstate, 32);
                    } else {
                        a->loop.start = a->loop.end = a->loop.count = 0;
                    }
                    break;
                case 1:
                    f->handler = alRaw16Pull;
                    if (a->table->waveInfo.rawWave.loop) {
                        a->loop.start = a->table->waveInfo.rawWave.loop->start;
                        a->loop.end = a->table->waveInfo.rawWave.loop->end;
                        a->loop.count = a->table->waveInfo.rawWave.loop->count;
                    } else {
                        a->loop.start = a->loop.end = a->loop.count = 0;
                    }
                    break;
                default:
                    break;
            }
            break;
        case 4:
            a->lastsam = 0;
            a->first = 1;
            a->sample = 0;
            if (a->table) {
                a->memin = (s32)a->table->base;
                if (a->table->type == 0) {
                    if (a->table->waveInfo.adpcmWave.loop)
                        a->loop.count = a->table->waveInfo.adpcmWave.loop->count;
                } else if (a->table->type == 1) {
                    if (a->table->waveInfo.rawWave.loop)
                        a->loop.count = a->table->waveInfo.rawWave.loop->count;
                }
            }
            break;
        default:
            break;
    }
}

Acmd_ *_decodeChunk(Acmd_ *ptr, Load_ *f, s32 tsam, s32 nbytes, s16 outp, s16 inp, u32 flags) {
    s32 dramAlign, dramLoc;

    if (nbytes > 0) {
        dramLoc = f->dma(f->memin, nbytes, f->dmaState);
        dramAlign = dramLoc & 7;
        nbytes += dramAlign;
        { Acmd_ *a = ptr++; a->w0 = (8 << 24) | (inp & 0xFFFF); a->w1 = (nbytes + 8 - (nbytes & 7)) & 0xFFFF; }
        { Acmd_ *a = ptr++; a->w0 = 4 << 24; a->w1 = dramLoc - dramAlign; }
    } else {
        dramAlign = 0;
    }
    if (flags & 2) {
        Acmd_ *a = ptr++;
        a->w0 = 0x0F000000;
        a->w1 = f->lstate & 0x1FFFFFFF;
    }
    { Acmd_ *a = ptr++; a->w0 = (8 << 24) | ((inp + dramAlign) & 0xFFFF); a->w1 = (outp << 16) | ((tsam << 1) & 0xFFFF); }
    { Acmd_ *a = ptr++; a->w0 = (1 << 24) | ((flags & 0xFF) << 16); a->w1 = f->state & 0x1FFFFFFF; }
    f->first = 0;
    return ptr;
}
