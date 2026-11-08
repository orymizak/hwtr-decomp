#include "common.h"
#include "include_asm.h"
typedef struct { u32 w0; u32 w1; } Acmd_;
typedef struct { u8 p0[0x14]; u32 state; u32 lstate; u8 p1C[0x14]; s32 (*dma)(s32, s32, void *); void *dmaState; u8 p38[8]; s32 first; s32 memin; } Load_;



INCLUDE_ASM("asm/nonmatchings/unit_79E60", alAdpcmPull);

INCLUDE_ASM("asm/nonmatchings/unit_79E60", alRaw16Pull);

INCLUDE_ASM("asm/nonmatchings/unit_79E60", alLoadParam);

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
