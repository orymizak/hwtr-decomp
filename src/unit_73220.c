#include "common.h"
#include "include_asm.h"
typedef struct PNode_ { struct PNode_ *next; } PNode_;
typedef struct { u8 p0[0x48]; s32 unk48; s32 unk4C; s32 unk50; u8 p54[6]; s16 unk5A; u8 p5C[0x18]; s32 unk74; s32 unk78; PNode_ *unk7C; PNode_ *unk80; s32 unk84; } ObjF_;
void func_800D39F8(ObjF_ *e, s32 id, void *param);
typedef struct { u32 w0; u32 w1; } AcmdF_;
typedef struct { u8 p0[0x14]; s32 sourceCount; s32 maxSources; void **sources; } BusF_;
typedef struct { u8 p0[0x34]; BusF_ *unk34; } DrvG_;
typedef struct { u32 w0; u32 w1; } AcmdE_;
typedef struct { u8 p0[0x54]; void *state; s16 pan; s16 volume; s16 cvolL; s16 cvolR; s16 f60; s16 f62; u16 lratl; s16 lratm; s16 ltgt; u16 rratl; s16 rratm; s16 rtgt; s32 delta; s32 segEnd; s32 first; u8 p7C[8]; s32 motion; } EnvE_;
typedef struct { s16 fc; s16 fgain; s32 pad4; s16 fccoef[16]; void *fstate; s32 first; } LPF_;
typedef struct { u8 filter[0x14]; void *state; f32 ratio; s32 upitch; f32 delta; s32 first; void *ctrlList; void *ctrlTail; s32 motion; } RSF_;
typedef struct { u32 input; u32 output; s16 ffcoef; s16 fbcoef; s16 gain; f32 rsinc; f32 rsval; s32 rsdelta; f32 rsgain; LPF_ *lp; RSF_ *rs; } DLF_;
typedef struct { u8 filter[0x14]; s16 *base; s16 *input; u32 length; DLF_ *delay; u8 section_count; s32 pad28; } FXF_;
typedef struct { s32 maxVVoices; s32 maxPVoices; s32 maxUpdates; s32 maxFXbusses; void *dmaproc; void *heap; s32 outputRate; u8 fxType; s32 *params; } SCF_;
typedef struct { u32 w0; u32 w1; } AcmdM_;
typedef struct CPM_ { struct CPM_ *next; s32 delta; s16 type; s16 unity; union { f32 f; s32 i; void *p; } data; union { f32 f; s32 i; } moredata; } CPM_;
typedef struct { struct CPM_ *next; s32 delta; s16 type; s16 unity; f32 pitch; s16 volume; u8 pan; u8 fxMix; s32 samples; void *wave; } SPAM_;
typedef struct { struct CPM_ *next; s32 delta; s16 type; s16 unity; void *wave; } SPM_;
typedef struct { u8 p0[0x88]; s32 offset; } PVM_;
typedef struct { struct CPM_ *next; s32 delta; s16 type; s16 pad; PVM_ *pvoice; } FPM_;
typedef struct { u8 p0[0x44]; f32 ratio; s32 upitch; u8 p4C[8]; void *state; s16 pan; s16 volume; s16 cvolL; s16 cvolR; s16 dryamt; s16 wetamt; u16 lratl; s16 lratm; s16 ltgt; u16 rratl; s16 rratm; s16 rtgt; s32 delta; s32 segEnd; s32 first; CPM_ *ctrlList; CPM_ *ctrlTail; s32 motion; } EnvM_;
void func_800D535C(PVM_ *pv);
void func_800D52E4(CPM_ *p);
AcmdM_ *func_800D2BE0(void *filter, s32 sampleOffset, AcmdM_ *p);
void _init_lpfilter(void *);
extern s32 D_800FAED0[];
extern s32 D_800FAF38[];
extern s32 D_800FAFC0[];
extern s32 D_800FAFE8[];
extern s32 D_800FB010[];
extern s32 D_800FB038[];
extern s16 D_800FB060[];
s16 func_800D341C(f64, f64, s32, u16 *);
AcmdE_ *func_800D3CD0(void *, s16 *, AcmdE_ *);
u32 osVirtualToPhysical(void *);
extern DrvG_ *D_800FB174;
s32 alHeapDBAlloc(s32, s32, void *, s32, s32);



AcmdF_ *func_800D2620(s32 a, AcmdF_ *p) {
    BusF_ *m = D_800FB174->unk34;
    void **sources = m->sources;
    s32 i;

    { AcmdF_ *_a = p++; _a->w0 = 0x020007C0; _a->w1 = 0x2E0; }
    for (i = 0; i < m->sourceCount; i++) {
        p = func_800D2BE0(sources[i], a, p);
    }
    return p;
}


void func_800D26C0(FXF_ **rp, SCF_ *c, void *hp) {
    u16 i, j, k;
    s32 *param = 0;
    DLF_ *d;
    FXF_ *r;

    r = (FXF_ *)alHeapDBAlloc(0, 0, hp, 1, sizeof(FXF_));
    *rp = r;
    switch (c->fxType) {
        case 1: param = D_800FAED0; break;
        case 2: param = D_800FAF38; break;
        case 5: param = D_800FAFC0; break;
        case 3: param = D_800FAFE8; break;
        case 4: param = D_800FB010; break;
        case 6: param = c->params; break;
        default: param = D_800FB038; break;
    }
    j = 0;
    r->section_count = param[j++];
    r->length = param[j++];
    r->delay = (DLF_ *)alHeapDBAlloc(0, 0, hp, r->section_count, sizeof(DLF_));
    r->base = (s16 *)alHeapDBAlloc(0, 0, hp, r->length, sizeof(s16));
    r->input = r->base;
    for (k = 0; k < r->length; k++) {
        r->base[k] = 0;
    }
    for (i = 0; i < r->section_count; i++) {
        d = &r->delay[i];
        d->input = param[j++];
        d->output = param[j++];
        d->fbcoef = param[j++];
        d->ffcoef = param[j++];
        d->gain = param[j++];
        if (param[j]) {
            d->rsinc = ((((f32)param[j++]) / 1000) * 2.0) / c->outputRate;
            d->rsgain = (((f32)param[j++]) / 173123.404906676) * (d->output - d->input);
            d->rsval = 1.0;
            d->rsdelta = 0.0;
            d->rs = (RSF_ *)alHeapDBAlloc(0, 0, hp, 1, sizeof(RSF_));
            d->rs->state = (void *)alHeapDBAlloc(0, 0, hp, 1, 0x20);
            d->rs->delta = 0.0;
            d->rs->first = 1;
        } else {
            d->rs = 0;
            j++;
            j++;
        }
        if (param[j]) {
            d->lp = (LPF_ *)alHeapDBAlloc(0, 0, hp, 1, sizeof(LPF_));
            d->lp->fstate = (void *)alHeapDBAlloc(0, 0, hp, 1, 8);
            d->lp->fc = param[j++];
            _init_lpfilter(d->lp);
        } else {
            d->lp = 0;
            j++;
        }
    }
}

void func_800D2AAC(s32 arg0, void *arg1, s32 arg2) {
    (*(s32 *)((u8 *)(arg0) + 0xC)) = alHeapDBAlloc(0, 0, (void *) arg2, 1, 0x20);
    (*(s32 *)((u8 *)(arg0) + 0x10)) = alHeapDBAlloc(0, 0, (void *) arg2, 1, 0x20);
    (*(s32 *)((u8 *)(arg0) + 0x28)) = ((s32 (*)(s32)) arg1)(arg0 + 0x2C);
    (*(s32 *)((u8 *)(arg0) + 0x34)) = 0;
    (*(s32 *)((u8 *)(arg0) + 0x38)) = 1;
    (*(s32 *)((u8 *)(arg0) + 0x3C)) = 0;
    (*(s32 *)((u8 *)(arg0) + 0x40)) = alHeapDBAlloc(0, 0, (void *) arg2, 1, 0x20);
    (*(s32 *)((u8 *)(arg0) + 0x4C)) = 0;
    (*(s32 *)((u8 *)(arg0) + 0x50)) = 1;
    (*(s32 *)((u8 *)(arg0) + 0x48)) = 0;
    (*(f32 *)((u8 *)(arg0) + 0x44)) = 1.0f;
    (*(s32 *)((u8 *)(arg0) + 0x54)) = alHeapDBAlloc(0, 0, (void *) arg2, 1, 0x50);
    (*(s32 *)((u8 *)(arg0) + 0x78)) = 1;
    (*(s32 *)((u8 *)(arg0) + 0x84)) = 0;
    (*(s16 *)((u8 *)(arg0) + 0x5A)) = 1;
    (*(s16 *)((u8 *)(arg0) + 0x68)) = 1;
    (*(s16 *)((u8 *)(arg0) + 0x6E)) = 1;
    (*(s16 *)((u8 *)(arg0) + 0x5C)) = 1;
    (*(s16 *)((u8 *)(arg0) + 0x5E)) = 1;
    (*(s16 *)((u8 *)(arg0) + 0x60)) = 0;
    (*(s16 *)((u8 *)(arg0) + 0x62)) = 0;
    (*(s16 *)((u8 *)(arg0) + 0x66)) = 1;
    (*(s16 *)((u8 *)(arg0) + 0x64)) = 0;
    (*(s16 *)((u8 *)(arg0) + 0x66)) = 1;
    (*(s16 *)((u8 *)(arg0) + 0x64)) = 0;
    (*(s32 *)((u8 *)(arg0) + 0x70)) = 0;
    (*(s32 *)((u8 *)(arg0) + 0x74)) = 0;
    (*(s16 *)((u8 *)(arg0) + 0x58)) = 0;
    (*(s32 *)((u8 *)(arg0) + 0x7C)) = 0;
    (*(s32 *)((u8 *)(arg0) + 0x80)) = 0;
}

s32 func_800D3158(ObjF_ *e, s32 id, void *param);
AcmdE_ *func_800D31FC(void *filter, s16 *inp, s16 *outp, s32 outCount, AcmdE_ *p);
s32 func_800D355C(s32 arg0, s32 arg1, s16 arg2, s32 arg3);
AcmdM_ *func_800D2BE0(void *filter, s32 sampleOffset, AcmdM_ *p) {
    AcmdM_ *ptr = p;
    EnvM_ *e = (EnvM_ *)filter;
    s16 inp;
    s32 lastOffset;
    s32 thisOffset = sampleOffset;
    s32 samples;
    s16 loutp = 0;
    s32 fVol;
    s32 outCount = 184;
    CPM_ *thisParam;

    inp = 0;

    while (e->ctrlList != 0) {
        lastOffset = thisOffset;
        thisOffset = e->ctrlList->delta;
        samples = ((thisOffset - lastOffset + 92) / 184) * 184;
        if (samples == 0)
            thisOffset = lastOffset;
        if (samples > outCount)
            break;

        switch (e->ctrlList->type) {
        case 13: {
            SPAM_ *param = (SPAM_ *)e->ctrlList;
            s32 tmp;

            if (param->unity) {
                e->upitch = 1;
            }
            ((void (*)(void *e, s32 id, void *param))func_800D39F8)(e, 5, param->wave);
            e->motion = 1;
            e->first = 1;
            e->delta = 0;
            e->segEnd = ((param->samples + 92) / 184) * 184;
            tmp = ((s32)param->volume * (s32)param->volume) >> 15;
            e->volume = (s16)tmp;
            e->pan = param->pan;
            e->dryamt = D_800FB060[param->fxMix];
            e->wetamt = D_800FB060[128 - param->fxMix - 1];
            if (param->samples) {
                e->cvolL = 1;
                e->cvolR = 1;
            } else {
                e->cvolL = (e->volume * D_800FB060[e->pan]) >> 15;
                e->cvolR = (e->volume * D_800FB060[128 - e->pan - 1]) >> 15;
            }
            e->ratio = param->pitch;
        } break;

        case 16:
        case 12:
        case 11:
            ptr = ((AcmdM_ * (*)(void *filter, s16 *inp, s16 *outp, s32 outCount, AcmdM_ *p))func_800D31FC)(e, &inp, &loutp, samples, ptr);
            if (e->delta >= e->segEnd) {
                e->ltgt = (e->volume * D_800FB060[e->pan]) >> 15;
                e->rtgt = (e->volume * D_800FB060[128 - e->pan - 1]) >> 15;
                e->delta = e->segEnd;
                e->cvolL = e->ltgt;
                e->cvolR = e->rtgt;
            } else {
                e->cvolL = ((s32 (*)(s32 arg0, s32 arg1, s16 arg2, s32 arg3))func_800D355C)(e->cvolL, e->delta, e->lratm, e->lratl);
                e->cvolR = ((s32 (*)(s32 arg0, s32 arg1, s16 arg2, s32 arg3))func_800D355C)(e->cvolR, e->delta, e->rratm, e->rratl);
            }
            if (e->cvolL == 0) e->cvolL = 1;
            if (e->cvolR == 0) e->cvolR = 1;
            if (e->ctrlList->type == 12)
                e->pan = (s16)e->ctrlList->data.i;
            if (e->ctrlList->type == 11) {
                e->delta = 0;
                fVol = (e->ctrlList->data.i);
                fVol = (fVol * fVol) >> 15;
                e->volume = (s16)fVol;
                e->segEnd = ((e->ctrlList->moredata.i + 92) / 184) * 184;
            }
            if (e->ctrlList->type == 16) {
                e->dryamt = D_800FB060[e->ctrlList->data.i];
                e->wetamt = D_800FB060[128 - e->ctrlList->data.i - 1];
            }
            e->first = 1;
            break;

        case 14: {
            SPM_ *sp = (SPM_ *)e->ctrlList;
            if (sp->unity) {
                e->upitch = 1;
            }
            ((void (*)(void *e, s32 id, void *param))func_800D39F8)(e, 5, sp->wave);
            e->motion = 1;
        } break;

        case 15:
            ptr = ((AcmdM_ * (*)(void *filter, s16 *inp, s16 *outp, s32 outCount, AcmdM_ *p))func_800D31FC)(e, &inp, &loutp, samples, ptr);
            ((s32 (*)(void *e, s32 id, void *param))func_800D3158)(e, 4, 0);
            break;

        case 0: {
            FPM_ *param = (FPM_ *)e->ctrlList;
            param->pvoice->offset = 0;
            func_800D535C(param->pvoice);
        } break;

        case 7:
            ptr = ((AcmdM_ * (*)(void *filter, s16 *inp, s16 *outp, s32 outCount, AcmdM_ *p))func_800D31FC)(e, &inp, &loutp, samples, ptr);
            e->ratio = e->ctrlList->data.f;
            break;

        case 8:
            ptr = ((AcmdM_ * (*)(void *filter, s16 *inp, s16 *outp, s32 outCount, AcmdM_ *p))func_800D31FC)(e, &inp, &loutp, samples, ptr);
            e->upitch = 1;
            break;

        case 5:
            ptr = ((AcmdM_ * (*)(void *filter, s16 *inp, s16 *outp, s32 outCount, AcmdM_ *p))func_800D31FC)(e, &inp, &loutp, samples, ptr);
            ((void (*)(void *e, s32 id, void *param))func_800D39F8)(e, 5, e->ctrlList->data.p);
            break;

        default:
            ptr = ((AcmdM_ * (*)(void *filter, s16 *inp, s16 *outp, s32 outCount, AcmdM_ *p))func_800D31FC)(e, &inp, &loutp, samples, ptr);
            ((s32 (*)(void *e, s32 id, void *param))func_800D3158)(e, e->ctrlList->type, e->ctrlList->data.p);
            break;
        }
        loutp += (samples << 1);
        outCount -= samples;

        thisParam = e->ctrlList;
        e->ctrlList = e->ctrlList->next;
        if (e->ctrlList == 0)
            e->ctrlTail = 0;
        func_800D52E4(thisParam);
    }

    ptr = ((AcmdM_ * (*)(void *filter, s16 *inp, s16 *outp, s32 outCount, AcmdM_ *p))func_800D31FC)(e, &inp, &loutp, outCount, ptr);
    if (e->delta > e->segEnd)
        e->delta = e->segEnd;
    return ptr;
}

s32 func_800D3158(ObjF_ *e, s32 id, void *param) {
    switch (id) {
        case 3:
            if (e->unk80 != 0) {
                e->unk80->next = param;
            } else {
                e->unk7C = param;
            }
            e->unk80 = param;
            break;
        case 4:
            e->unk78 = 1;
            e->unk84 = 0;
            e->unk5A = 1;
            e->unk74 = 0;
            e->unk4C = 0;
            e->unk50 = 1;
            e->unk48 = 0;
            func_800D39F8(e, 4, param);
            break;
        case 9:
            e->unk84 = 1;
            break;
        default:
            func_800D39F8(e, id, param);
            break;
    }
    return 0;
}


AcmdE_ *func_800D31FC(void *filter, s16 *inp, s16 *outp, s32 outCount, AcmdE_ *p) {
    AcmdE_ *ptr = p;
    EnvE_ *e = (EnvE_ *)filter;

    if (e->motion != 1 || !outCount) {
        return ptr;
    }
    ptr = func_800D3CD0(e, inp, p);
    if (e->first) {
        e->first = 0;
        e->ltgt = (e->volume * D_800FB060[e->pan]) >> 15;
        e->lratm = func_800D341C((f64)e->cvolL, (f64)e->ltgt, e->segEnd, &e->lratl);
        e->rtgt = (e->volume * D_800FB060[127 - e->pan]) >> 15;
        e->rratm = func_800D341C((f64)e->cvolR, (f64)e->rtgt, e->segEnd, &e->rratl);
        {
            AcmdE_ *_a = ptr++;
            _a->w0 = (9 << 24) | (0 << 16) | (u16)e->ltgt;
            _a->w1 = ((u16)e->lratm << 16) | e->lratl;
        }
        {
            AcmdE_ *_a = ptr++;
            _a->w0 = (9 << 24) | (6 << 16) | (u16)e->cvolL;
            _a->w1 = ((u16)e->f60 << 16) | (u16)e->f62;
        }
        {
            AcmdE_ *_a = ptr++;
            _a->w0 = (9 << 24) | (4 << 16) | (u16)e->rtgt;
            _a->w1 = ((u16)e->rratm << 16) | e->rratl;
        }
        {
            AcmdE_ *_a = ptr++;
            _a->w0 = (3 << 24) | (1 << 16) | (u16)e->cvolR;
            _a->w1 = osVirtualToPhysical(e->state);
        }
    } else {
        AcmdE_ *_a = ptr++;
        _a->w0 = (3 << 24);
        _a->w1 = osVirtualToPhysical(e->state);
    }
    *inp += 0x170;
    e->delta += 184;
    return ptr;
}

INCLUDE_ASM("asm/nonmatchings/unit_73220", func_800D341C);

s32 func_800D355C(s32 arg0, s32 arg1, s16 arg2, s32 arg3) {
    s32 temp_a1;
    s32 var_v0;

    temp_a1 = arg1 >> 3;
    if (temp_a1 != 0) {
        var_v0 = (arg0 + (((s32) ((arg3 & 0xFFFF) * temp_a1) >> 0x10) + (arg2 * temp_a1))) << 0x10;
    } else {
        var_v0 = arg0 << 0x10;
    }
    return var_v0 >> 0x10;
}
