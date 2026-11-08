#include "common.h"
#include "include_asm.h"
typedef struct { u32 w0; u32 w1; } AcmdO_;
typedef struct { u8 p0[0x14]; s16 *base; u8 p18[4]; s32 length; } FxO_;
typedef struct { u32 w0; u32 w1; } Acmd_;
typedef struct { u8 p0[0x14]; s16 *base; u8 p18[4]; s32 length; } Fx_;
u32 osVirtualToPhysical(void *);
typedef struct { u16 fc; } LP_;
typedef struct { u32 input; u32 output; s16 ffcoef; s16 fbcoef; s16 gain; s16 pad; f32 rsinc; f32 rsval; s32 rsdelta; f32 rsgain; LP_ *lp; s32 pad24; } ALDelay_;
typedef struct { u8 pad0[0x20]; ALDelay_ *delay; } ALFx_;
extern void _init_lpfilter(LP_ *);
extern u8 *D_800FB174;



INCLUDE_ASM("asm/nonmatchings/unit_74AB0", func_800D3EB0);

s32 func_800D41C0(void *filter, s32 paramID, void *param) {
    ALFx_ *f = filter;
    s32 p = (paramID - 2) % 8;
    s32 s = (paramID - 2) / 8;
    s32 val = *(s32 *)param;

    switch (p) {
    case 0:
        f->delay[s].input = (u32)val & 0xFFFFFFF8;
        break;
    case 1:
        f->delay[s].output = (u32)val & 0xFFFFFFF8;
        break;
    case 3:
        f->delay[s].ffcoef = (s16)val;
        break;
    case 2:
        f->delay[s].fbcoef = (s16)val;
        break;
    case 4:
        f->delay[s].gain = (s16)val;
        break;
    case 5:
        f->delay[s].rsinc = ((((f32)val) / 1000) * 2.0) / *(s32 *)(D_800FB174 + 0x40);
        break;
    case 6:
        f->delay[s].rsgain = (((f32)val) / 173123.404906676) * (f->delay[s].output - f->delay[s].input);
        break;
    case 7:
        if (f->delay[s].lp) {
            f->delay[s].lp->fc = (s16)val;
            _init_lpfilter(f->delay[s].lp);
        }
        break;
    }
    return 0;
}

INCLUDE_ASM("asm/nonmatchings/unit_74AB0", func_800D43B0);

Acmd_ *func_800D45C0(Fx_ *r, s16 *curr_ptr, s32 buff, s32 count, Acmd_ *p) {
    Acmd_ *ptr = p;
    s32 after_end, before_end;
    s16 *updated_ptr, *delay_end;
    Acmd_ *a;

    delay_end = &r->base[r->length];
    if (curr_ptr < r->base) {
        curr_ptr += r->length;
    }
    updated_ptr = curr_ptr + count;
    if (updated_ptr > delay_end) {
        after_end = updated_ptr - delay_end;
        before_end = delay_end - curr_ptr;
        a = ptr++;
        a->w0 = (4 << 24) | (((before_end << 1) & 0xFFF) << 12) | (buff & 0xFFF);
        a->w1 = osVirtualToPhysical(curr_ptr);
        a = ptr++;
        a->w0 = (4 << 24) | (((after_end << 1) & 0xFFF) << 12) | ((buff + (before_end << 1)) & 0xFFF);
        a->w1 = osVirtualToPhysical(r->base);
    } else {
        a = ptr++;
        a->w0 = (4 << 24) | (((count << 1) & 0xFFF) << 12) | (buff & 0xFFF);
        a->w1 = osVirtualToPhysical(curr_ptr);
    }
    return ptr;
}

AcmdO_ *func_800D46E8(FxO_ *r, s16 *curr_ptr, s32 buff, AcmdO_ *p) {
    AcmdO_ *ptr = p;
    s32 after_end, before_end;
    s16 *updated_ptr, *delay_end;

    delay_end = &r->base[r->length];
    if (curr_ptr < r->base) {
        curr_ptr += r->length;
    }
    updated_ptr = curr_ptr + 184;
    if (updated_ptr > delay_end) {
        after_end = updated_ptr - delay_end;
        before_end = delay_end - curr_ptr;
        { AcmdO_ *_a = ptr++; _a->w0 = 0x06000000 | (((before_end << 1) & 0xFFF) << 12) | (buff & 0xFFF); _a->w1 = osVirtualToPhysical(curr_ptr); }
        { AcmdO_ *_a = ptr++; _a->w0 = 0x06000000 | (((after_end << 1) & 0xFFF) << 12) | ((buff + (before_end << 1)) & 0xFFF); _a->w1 = osVirtualToPhysical(r->base); }
    } else {
        { AcmdO_ *_a = ptr++; _a->w0 = 0x06000000 | ((0x170 & 0xFFF) << 12) | (buff & 0xFFF); _a->w1 = osVirtualToPhysical(curr_ptr); }
    }
    return ptr;
}

INCLUDE_ASM("asm/nonmatchings/unit_74AB0", func_800D4800);
