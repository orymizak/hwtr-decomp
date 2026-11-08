#include "common.h"
#include "include_asm.h"
typedef struct { u32 start; u32 end; u32 count; s16 state[16]; } AdpcmLoop_;
typedef struct { u32 start; u32 end; u32 count; } RawLoop_;
typedef struct { s32 order; s32 npredictors; s16 book[1]; } AdpcmBook_;
typedef struct { u8 *base; s32 len; u8 type; u8 flags; union { struct { AdpcmLoop_ *loop; AdpcmBook_ *book; } adpcmWave; struct { RawLoop_ *loop; } rawWave; } waveInfo; } WaveTable_;
typedef struct { void *source; void *handler; void *setParam; s32 inp; s32 outp; } Filter4_;
typedef struct { Filter4_ filter; void *state; void *lstate; RawLoop_ loop; WaveTable_ *table; s32 bookSize; void *dma; void *dmaState; s32 sample; s32 lastsam; s32 first; s32 memin; } LoadFilter4_;
void *alAdpcmPull();
void *alRaw16Pull();
void alCopy(void *src, void *dest, s32 len);
typedef struct { u32 w0; u32 w1; } Acmd_;
typedef struct { u8 p0[0x14]; u32 state; u32 lstate; u8 p1C[0x14]; s32 (*dma)(s32, s32, void *); void *dmaState; u8 p38[8]; s32 first; s32 memin; } Load_;



INCLUDE_ASM("asm/nonmatchings/unit_79E60", alAdpcmPull);

INCLUDE_ASM("asm/nonmatchings/unit_79E60", alRaw16Pull);

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
