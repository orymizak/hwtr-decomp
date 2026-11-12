#include "common.h"
#include "include_asm.h"
typedef struct { u32 w0; u32 w1; } Acmd_;
typedef struct { u8 p0[0xC]; u32 state; u32 lstate; u8 p14[0x14]; s32 (*dma)(s32, s32, void *); void *dmaState; u8 p30[8]; s32 first; s32 memin; } Load_;
typedef struct { u32 start; u32 end; u32 count; u8 state[0x20]; } LoopL_;
typedef struct { s32 order; s32 npredictors; } BookL_;
typedef struct { u8 *base; s32 len; u8 type; u8 flags; u8 p[2]; LoopL_ *loop; BookL_ *book; } WaveL_;
typedef struct { u8 p0[0x10]; void *lstate; u32 start; u32 end; u32 count; WaveL_ *table; s32 bookSize; u8 p28[8]; s32 sample; s32 lastsam; s32 first; s32 memin; } LoadL_;
void alCopy(void *, void *, s32);



INCLUDE_ASM("asm/nonmatchings/unit_741B0", func_800D35B0);

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
