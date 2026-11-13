#include "common.h"
#include "include_asm.h"
typedef struct { u32 w0; u32 w1; } Acmd_;
typedef struct { u8 p0[0xC]; u32 state; u32 lstate; u8 p14[0x14]; s32 (*dma)(s32, s32, void *); void *dmaState; u8 p30[8]; s32 first; s32 memin; } Load_;
typedef struct { u32 start; u32 end; u32 count; u8 state[0x20]; } LoopL_;
typedef struct { s32 order; s32 npredictors; } BookL_;
typedef struct { u8 *base; s32 len; u8 type; u8 flags; u8 p[2]; LoopL_ *loop; BookL_ *book; } WaveL_;
typedef struct { u8 p0[0x10]; void *lstate; u32 start; u32 end; u32 count; WaveL_ *table; s32 bookSize; u8 p28[8]; s32 sample; s32 lastsam; s32 first; s32 memin; } LoadL_;
typedef struct { u32 w0; u32 w1; } AcmdA_;
typedef struct { s32 order; s32 npredictors; s16 book[1]; } BookA_;
typedef struct { s32 base; s32 len; u8 type; u8 flags; u8 p[2]; void *loop; BookA_ *book; } WaveA_;
typedef struct { u8 p0[0x10]; void *lstate; u32 start; u32 end; u32 count; WaveA_ *table; s32 bookSize; u8 p28[8]; s32 sample; s32 lastsam; s32 first; s32 memin; } LoadA_;
void alCopy(void *, void *, s32);



Acmd_ *func_800D3B84(Acmd_ *ptr, Load_ *f, s32 tsam, s32 nbytes, s16 outp, s16 inp, u32 flags);
AcmdA_ *func_800D35B0(void *filter, s16 *outp, s32 outCount, AcmdA_ *p) {
    AcmdA_ *ptr = p;
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
    LoadA_ *f = (LoadA_ *)filter;

    if (outCount == 0)
        return ptr;

    inp = 0;
    {
        AcmdA_ *_a = ptr++;
        _a->w0 = 0x0B000000 | (f->bookSize & 0xFFFFFF);
        _a->w1 = (u32)f->table->book->book & 0x1FFFFFFF;
    }

    looped = (outCount + f->sample > f->end) && (f->count != 0);
    if (looped)
        nSam = f->end - f->sample;
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
        ptr = ((AcmdA_ * (*)(AcmdA_ *ptr, LoadA_ *f, s32 tsam, s32 nbytes, s16 outp, s16 inp, u32 flags))func_800D3B84)(ptr, f, tsam, nbytes, *outp, inp, f->first);

        if (f->lastsam)
            *outp += (f->lastsam << 1);
        else
            *outp += (16 << 1);

        f->lastsam = f->start & 0xf;
        f->memin = (s32)f->table->base + 9 * ((s32)(f->start >> 4) + 1);
        f->sample = f->start;

        bEnd = *outp;
        while (outCount > nSam) {
            outCount -= nSam;
            op = (bEnd + ((nframes + 1) << 5) + 16) & ~0x1f;
            bEnd += (nSam << 1);
            if ((f->count != -1) && (f->count != 0))
                f->count--;
            nSam = ((u32)outCount < f->end - f->start) ? outCount : f->end - f->start;
            tsam = nSam - 16 + f->lastsam;
            if (tsam < 0) tsam = 0;
            nframes = (tsam + 16 - 1) >> 4;
            nbytes = nframes * 9;
            ptr = ((AcmdA_ * (*)(AcmdA_ *ptr, LoadA_ *f, s32 tsam, s32 nbytes, s16 outp, s16 inp, u32 flags))func_800D3B84)(ptr, f, tsam, nbytes, op, inp, f->first | 2);
            {
                AcmdA_ *_a = ptr++;
                _a->w0 = 0x0A000000 | ((op + (f->lastsam << 1)) & 0xFFFFFF);
                _a->w1 = (bEnd << 16) | ((nSam << 1) & 0xFFFF);
            }
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
        ptr = ((AcmdA_ * (*)(AcmdA_ *ptr, LoadA_ *f, s32 tsam, s32 nbytes, s16 outp, s16 inp, u32 flags))func_800D3B84)(ptr, f, nSam - nOver, nbytes, *outp, inp, f->first);

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
        {
            AcmdA_ *_a = ptr++;
            _a->w0 = 0x02000000 | ((startZero + *outp) & 0xFFFFFF);
            _a->w1 = nOver << 1;
        }
    }
    return ptr;
}

void func_800D39F8(void *filter, s32 paramID, void *param) {
    LoadL_ *a = (LoadL_ *)filter;

    switch (paramID) {
    case 5:
        a->table = (WaveL_ *)param;
        a->memin = (s32)a->table->base;
        a->sample = 0;
        switch (a->table->type) {
        case 0:
            a->table->len = 9 * ((s32)(a->table->len / 9));
            a->bookSize = 2 * a->table->book->order * a->table->book->npredictors * 8;
            if (a->table->loop) {
                a->start = a->table->loop->start;
                a->end = a->table->loop->end;
                a->count = a->table->loop->count;
                alCopy(a->table->loop->state, a->lstate, 0x20);
            } else {
                a->start = a->end = a->count = 0;
            }
            break;
        case 1:
            if (a->table->loop) {
                a->start = a->table->loop->start;
                a->end = a->table->loop->end;
                a->count = a->table->loop->count;
            } else {
                a->start = a->end = a->count = 0;
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
                if (a->table->loop) {
                    a->count = a->table->loop->count;
                }
            } else if (a->table->type == 1) {
                if (a->table->loop) {
                    a->count = a->table->loop->count;
                }
            }
        }
        break;
    }
}

Acmd_ *func_800D3B84(Acmd_ *ptr, Load_ *f, s32 tsam, s32 nbytes, s16 outp, s16 inp, u32 flags) {
    s32 dramAlign, dramLoc;

    if (nbytes > 0) {
        dramLoc = f->dma(f->memin, nbytes, f->dmaState);
        dramAlign = dramLoc & 7;
        nbytes += dramAlign;
        {
            Acmd_ *a = ptr++;
            a->w0 = (4 << 24) | (((nbytes + 8 - (nbytes & 7)) & 0xFFF) << 12) | (inp & 0xFFF);
            a->w1 = dramLoc - dramAlign;
        }
    } else {
        dramAlign = 0;
    }
    if (flags & 2) {
        {
            Acmd_ *a = ptr++;
            a->w0 = 0x0F000000;
            a->w1 = f->lstate & 0x1FFFFFFF;
        }
    }
    {
        Acmd_ *a = ptr++;
        a->w0 = (1 << 24) | (f->state & 0xFFFFFF);
        a->w1 = (flags << 28) | (((tsam << 1) & 0xFFF) << 16) | (dramAlign << 12) | (outp & 0xFFF);
    }
    f->first = 0;
    return ptr;
}
