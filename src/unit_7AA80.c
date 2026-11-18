/* TODO (Clean version):
   These structs are libultra's private audio-driver types.
   Replace with the standard SDK typedefs:
     ALParam8_        → ALParam          (alPlayer.h)
     StartParamAlt8_  → ALStartParamAlt  (fields: unity, pitch, volume, pan, fxMix, samples, wave)
     StartParam8_     → ALStartParam
     PVoice8_         → ALVoice          (pad[0xD8] is sizeof(ALVoice) minus the trailing s32 offset)
     FreeParam8_      → ALFreeParam
     Acmd8_           → Acmd             (the RSP audio command word pair from <libaudio.h>)
     ALFilt8_/ALFilt3_→ ALFilter         (the standard filter vtable: source, handler, setParam, inp, outp, type)
     EnvMix8_         → ALEnvMixer       (extends ALFilter with envelope state: pan, volume, cvolL/R, dry/wetamt, etc.)
     ALEnvMix3_       → ALEnvMixer       (3-field variant used in alEnvmixerParam; padC[8] likely hides 'state' pointer + unused)

   The setParam IDs used throughout are AL_FILTER_SET_* enums:
     1 → AL_FILTER_SET_SOURCE,  3 → AL_FILTER_ADD_UPDATE,  4 → AL_FILTER_RESET
     5 → AL_FILTER_SET_WAVETABLE,  7 → AL_FILTER_SET_PITCH,  8 → AL_FILTER_SET_UNITY_PITCH
     9 → AL_FILTER_START

   The ctrlList->type values are AL_FILTER_*_EVT enums:
     0  → AL_FILTER_FREE_EVT,   11 → AL_FILTER_SET_FXAMT_EVT (actually volume + segEnd)
     12 → AL_FILTER_SET_PAN,    13 → AL_FILTER_START_VOICE_ALT
     14 → AL_FILTER_START_VOICE,15 → AL_FILTER_STOP_VOICE
     16 → AL_FILTER_SET_FXMIX
*/
#include "common.h"
#include "include_asm.h"
typedef struct ALParam8_ { struct ALParam8_ *next; s32 delta; s16 type; union { f32 f; s32 i; } data; union { f32 f; s32 i; } moredata; } ALParam8_;
typedef struct { void *next; s32 delta; s16 type; s16 unity; f32 pitch; s16 volume; u8 pan; u8 fxMix; s32 samples; void *wave; } StartParamAlt8_;
typedef struct { void *next; s32 delta; s16 type; s16 unity; void *wave; } StartParam8_;
typedef struct { u8 pad[0xD8]; s32 offset; } PVoice8_;
typedef struct { void *next; s32 delta; s16 type; PVoice8_ *pvoice; } FreeParam8_;
typedef struct { u32 w0; u32 w1; } Acmd8_;
typedef struct ALFilt8_ { struct ALFilt8_ *source; Acmd8_ *(*handler)(void *, s16 *, s32, s32, Acmd8_ *); s32 (*setParam)(void *, s32, void *); s16 inp; s16 outp; s32 type; } ALFilt8_;
typedef struct { ALFilt8_ filter; void *state; s16 pan; s16 volume; s16 cvolL; s16 cvolR; s16 dryamt; s16 wetamt; u16 lratl; s16 lratm; s16 ltgt; u16 rratl; s16 rratm; s16 rtgt; s32 delta; s32 segEnd; s32 first; struct ALParam8_ *ctrlList; struct ALParam8_ *ctrlTail; void **sources; s32 motion; } EnvMix8_;
typedef struct ALFilt3_ { struct ALFilt3_ *source; void *handler; s32 (*setParam)(void *, s32, void *); } ALFilt3_;
typedef struct ALParam3_ { struct ALParam3_ *next; } ALParam3_;
typedef struct { ALFilt3_ filter; u8 padC[8]; s32 state; s16 pan; s16 volume; u8 pad1C[0x1C]; s32 first; ALParam3_ *ctrlList; ALParam3_ *ctrlTail; void **sources; s32 motion; } ALEnvMix3_;



extern s16 D_800FB490[];
extern void *D_800FB5B0;
Acmd8_ *_pullSubFrame(void *filter, s16 *inp, s16 *outp, s32 outCount, s32 sampleOffset, Acmd8_ *p);
f32 _getVol(f32 ivol, s32 samples, s16 ratem, u16 ratel);
void _freePVoice(s32 arg0, s32 arg1);
void __freeParam(void *param);
/* TODO (Clean version):
   This is libultra's alEnvmixerPull (envmixer.c).
   The switch on ctrlList->type uses AL_FILTER event enums:
     case 13 (AL_FILTER_START_VOICE_ALT):
       Volume squaring: tmp = (volume * volume) >> 15 is the standard
       16-bit Q15 fixed-point squaring for perceptual volume curve.
       D_800FB490[] is the standard libultra pan/volume lookup table
       (alGlobals.panTable, 129 entries of sin/cos quarter-wave).
       Replace D_800FB490[pan] / D_800FB490[128 - pan - 1] with:
         alGlobals.panTable[pan]  / alGlobals.panTable[AL_PAN_RIGHT - pan]
       The union hack to pass f32 pitch as s32 through setParam is:
         (*f->source->setParam)(f->source, AL_FILTER_SET_PITCH, *(s32*)&pitch)
       i.e. a type-punned float→int to fit the void* parameter.

     case 16/12/11 (AL_FILTER_SET_FXMIX / SET_PAN / SET_VOL):
       After _pullSubFrame, the envelope ramp is recomputed.
       The cvolL/R == 0 → 1 clamp avoids a zero-volume dead channel (the RSP
       ENVMIXER microcode treats 0 as silence with no ramp recovery).

     case 14 (AL_FILTER_START_VOICE):
       Minimal start: sets unity pitch flag + wavetable, no volume envelope.

     case 15 (AL_FILTER_STOP_VOICE):
       Pulls remaining samples then resets the filter via AL_FILTER_RESET (4).

     case 0 (AL_FILTER_FREE_EVT):
       D_800FB5B0 is alGlobals.drvr (the ALSynth* driver pointer).
       _freePVoice releases the physical voice back to the driver's voice pool.

   Replace the magic setParam IDs with the standard enums:
     #define AL_FILTER_SET_SOURCE     1
     #define AL_FILTER_ADD_UPDATE     3
     #define AL_FILTER_RESET          4
     #define AL_FILTER_SET_WAVETABLE  5
     #define AL_FILTER_SET_PITCH      7
     #define AL_FILTER_SET_UNITY_PITCH 8
     #define AL_FILTER_START          9
*/
Acmd8_ *alEnvmixerPull(void *filter, s16 *outp, s32 outCount, s32 sampleOffset, Acmd8_ *p) {
    Acmd8_ *ptr = p;
    EnvMix8_ *e = (EnvMix8_ *)filter;
    s16 inp;
    s32 lastOffset;
    s32 thisOffset = sampleOffset;
    s32 samples;
    s16 loutp = 0;
    s32 fVol;
    ALParam8_ *thisParam;

    inp = 0;
    while (e->ctrlList != 0) {
        lastOffset = thisOffset;
        thisOffset = e->ctrlList->delta;
        samples = thisOffset - lastOffset;
        if (samples > outCount)
            break;
        switch (e->ctrlList->type) {
            case 13:
                {
                    StartParamAlt8_ *param = (StartParamAlt8_ *)e->ctrlList;
                    ALFilt8_ *f = (ALFilt8_ *)e;
                    s32 tmp;

                    if (param->unity) {
                        (*e->filter.setParam)(&e->filter, 8, 0);
                    }
                    (*e->filter.setParam)(&e->filter, 5, param->wave);
                    (*e->filter.setParam)(&e->filter, 9, 0);
                    e->first = 1;
                    e->delta = 0;
                    e->segEnd = param->samples;
                    tmp = ((s32)param->volume * (s32)param->volume) >> 15;
                    e->volume = (s16)tmp;
                    e->pan = param->pan;
                    e->dryamt = D_800FB490[param->fxMix];
                    e->wetamt = D_800FB490[128 - param->fxMix - 1];
                    if (param->samples) {
                        e->cvolL = 1;
                        e->cvolR = 1;
                    } else {
                        e->cvolL = (e->volume * D_800FB490[e->pan]) >> 15;
                        e->cvolR = (e->volume * D_800FB490[128 - e->pan - 1]) >> 15;
                    }
                    if (f->source) {
                        union {
                            f32 f;
                            s32 i;
                        } data;
                        data.f = param->pitch;
                        (*f->source->setParam)(f->source, 7, (void *)data.i);
                    }
                }
                break;
            case 16:
            case 12:
            case 11:
                ptr = _pullSubFrame(e, &inp, &loutp, samples, sampleOffset, ptr);
                if (e->delta >= e->segEnd) {
                    e->ltgt = (e->volume * D_800FB490[e->pan]) >> 15;
                    e->rtgt = (e->volume * D_800FB490[128 - e->pan - 1]) >> 15;
                    e->delta = e->segEnd;
                    e->cvolL = e->ltgt;
                    e->cvolR = e->rtgt;
                } else {
                    e->cvolL = _getVol(e->cvolL, e->delta, e->lratm, e->lratl);
                    e->cvolR = _getVol(e->cvolR, e->delta, e->rratm, e->rratl);
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
                    e->segEnd = e->ctrlList->moredata.i;
                }
                if (e->ctrlList->type == 16) {
                    e->dryamt = D_800FB490[e->ctrlList->data.i];
                    e->wetamt = D_800FB490[128 - e->ctrlList->data.i - 1];
                }
                e->first = 1;
                break;
            case 14:
                {
                    StartParam8_ *p = (StartParam8_ *)e->ctrlList;

                    if (p->unity) {
                        (*e->filter.setParam)(&e->filter, 8, 0);
                    }
                    (*e->filter.setParam)(&e->filter, 5, p->wave);
                    (*e->filter.setParam)(&e->filter, 9, 0);
                }
                break;
            case 15:
                {
                    ptr = _pullSubFrame(e, &inp, &loutp, samples, sampleOffset, ptr);
                    (*e->filter.setParam)(&e->filter, 4, 0);
                }
                break;
            case 0:
                {
                    void *drvr = D_800FB5B0;
                    FreeParam8_ *param = (FreeParam8_ *)e->ctrlList;
                    param->pvoice->offset = 0;
                    _freePVoice((s32)drvr, (s32)param->pvoice);
                }
                break;
            default:
                ptr = _pullSubFrame(e, &inp, &loutp, samples, sampleOffset, ptr);
                (*e->filter.setParam)(&e->filter, e->ctrlList->type, (void *)e->ctrlList->data.i);
                break;
        }
        loutp += (samples << 1);
        outCount -= samples;
        thisParam = e->ctrlList;
        e->ctrlList = e->ctrlList->next;
        if (e->ctrlList == 0)
            e->ctrlTail = 0;
        __freeParam(thisParam);
    }
    ptr = _pullSubFrame(e, &inp, &loutp, outCount, sampleOffset, ptr);
    if (e->delta > e->segEnd)
        e->delta = e->segEnd;
    return ptr;
}

/* TODO (Clean version):
   This is alEnvmixerParam (envmixer.c).
   Replace magic paramID constants with:
     case 3 → AL_FILTER_ADD_UPDATE:  appends an ALParam node to the control list
     case 4 → AL_FILTER_RESET:       resets envelope state (first=1, motion=AL_STOPPED, volume=1)
                                      and propagates reset down the filter chain
     case 9 → AL_FILTER_START:       sets motion=AL_PLAYING and propagates start
     case 1 → AL_FILTER_SET_SOURCE:  connects the upstream filter (resampler)
   The two struct variants ALFilt3_/ALEnvMix3_ should collapse to the standard
   ALFilter/ALEnvMixer from <libaudio.h>.
*/
s32 alEnvmixerParam(void *filter, s32 paramID, void *param) {
    ALFilt3_ *f = (ALFilt3_ *)filter;
    ALEnvMix3_ *e = (ALEnvMix3_ *)filter;

    switch (paramID) {
        case 3:
            if (e->ctrlTail) {
                e->ctrlTail->next = (ALParam3_ *)param;
            } else {
                e->ctrlList = (ALParam3_ *)param;
            }
            e->ctrlTail = (ALParam3_ *)param;
            break;
        case 4:
            e->first = 1;
            e->motion = 0;
            e->volume = 1;
            if (f->source)
                (*f->source->setParam)(f->source, 4, param);
            break;
        case 9:
            e->motion = 1;
            if (f->source)
                (*f->source->setParam)(f->source, 9, param);
            break;
        case 1:
            f->source = (ALFilt3_ *)param;
            break;
        default:
            if (f->source)
                (*f->source->setParam)(f->source, paramID, param);
    }
    return 0;
}


extern s16 D_800FB490[];
s16 _getRate(f64 vol, f64 tgt, s32 count, u16 *ratel);
u32 osVirtualToPhysical(void *);
/* TODO (Clean version):
   This is _pullSubFrame from envmixer.c — the core RSP audio command emitter.
   The raw Acmd_ writes should be replaced with libultra audio microcode macros:

   Line 200 (opcode 8 = A_SETBUFF):
     aSetBuffer(ptr++, A_MAIN, *inp, AL_MAIN_L_OUT + *outp, outCount << 1);
     where AL_MAIN_L_OUT = 0x440 (1088).

   Line 201 (opcode 8 = A_SETBUFF, flags=8 → A_AUX):
     aSetBuffer(ptr++, A_AUX, AL_AUX_L_OUT + *outp, AL_AUX_R_OUT + *outp, AL_MAIN_R_OUT + *outp);
     where AL_AUX_L_OUT=0x580 (1408), AL_AUX_R_OUT=0x6C0 (1728), AL_MAIN_R_OUT=0x800 (2048).

   Lines 208-209 (opcode 9 = A_SETVOL, flags 6=A_VOL|A_LEFT, 4=A_VOL|A_RIGHT):
     aSetVolume(ptr++, A_VOL | A_LEFT,  cvolL, 0, 0);
     aSetVolume(ptr++, A_VOL | A_RIGHT, cvolR, 0, 0);

   Lines 210-211 (opcode 9 = A_SETVOL, flags 2=A_RATE|A_LEFT, 0=A_RATE|A_RIGHT):
     aSetVolume(ptr++, A_RATE | A_LEFT,  ltgt, lratm, lratl);
     aSetVolume(ptr++, A_RATE | A_RIGHT, rtgt, rratm, rratl);

   Line 212 (opcode 9 = A_SETVOL, flags 8 → A_AUX):
     aSetVolume(ptr++, A_AUX, dryamt, 0, wetamt);

   Line 213 (opcode 3 = A_ENVMIXER, flags 1|8 = A_INIT|A_AUX):
     aEnvMixer(ptr++, A_INIT | A_AUX, osVirtualToPhysical(e->state));
     First call: initializes the RSP ENVMIXER state from the volumes set above.

   Line 215 (opcode 3 = A_ENVMIXER, flags 0|8 = A_AUX, no A_INIT):
     aEnvMixer(ptr++, A_AUX, osVirtualToPhysical(e->state));
     Subsequent calls: continues envelope from saved RSP state.

   The flag constants:
     A_INIT=1, A_AUX=8, A_VOL=4, A_LEFT=2, A_RATE=0, A_RIGHT=0
     A_MAIN=0, A_SETBUFF=8, A_SETVOL=9, A_ENVMIXER=3
*/
Acmd8_ *_pullSubFrame(void *filter, s16 *inp, s16 *outp, s32 outCount, s32 sampleOffset, Acmd8_ *p) {
    Acmd8_ *ptr = p;
    EnvMix8_ *e = (EnvMix8_ *)filter;
    ALFilt8_ *source = e->filter.source;

    if (e->motion != 1 || !outCount)
        return ptr;
    ptr = (*source->handler)(source, inp, outCount, sampleOffset, p);
    { Acmd8_ *_a = ptr++; _a->w0 = (8 << 24) | (0 << 16) | (*inp & 0xFFFF); _a->w1 = (((1088 + *outp) & 0xFFFF) << 16) | ((outCount << 1) & 0xFFFF); }
    { Acmd8_ *_a = ptr++; _a->w0 = (8 << 24) | (8 << 16) | ((1408 + *outp) & 0xFFFF); _a->w1 = (((1728 + *outp) & 0xFFFF) << 16) | ((2048 + *outp) & 0xFFFF); }
    if (e->first) {
        e->first = 0;
        e->ltgt = (e->volume * D_800FB490[e->pan]) >> 15;
        e->lratm = _getRate((f64)e->cvolL, (f64)e->ltgt, e->segEnd, &(e->lratl));
        e->rtgt = (e->volume * D_800FB490[128 - e->pan - 1]) >> 15;
        e->rratm = _getRate((f64)e->cvolR, (f64)e->rtgt, e->segEnd, &(e->rratl));
        { Acmd8_ *_a = ptr++; _a->w0 = (9 << 24) | ((2 | 4) << 16) | (e->cvolL & 0xFFFF); _a->w1 = 0; }
        { Acmd8_ *_a = ptr++; _a->w0 = (9 << 24) | ((0 | 4) << 16) | (e->cvolR & 0xFFFF); _a->w1 = 0; }
        { Acmd8_ *_a = ptr++; _a->w0 = (9 << 24) | ((2 | 0) << 16) | (e->ltgt & 0xFFFF); _a->w1 = ((e->lratm & 0xFFFF) << 16) | (e->lratl & 0xFFFF); }
        { Acmd8_ *_a = ptr++; _a->w0 = (9 << 24) | ((0 | 0) << 16) | (e->rtgt & 0xFFFF); _a->w1 = ((e->rratm & 0xFFFF) << 16) | (e->rratl & 0xFFFF); }
        { Acmd8_ *_a = ptr++; _a->w0 = (9 << 24) | (8 << 16) | (e->dryamt & 0xFFFF); _a->w1 = (0 << 16) | (e->wetamt & 0xFFFF); }
        { Acmd8_ *_a = ptr++; _a->w0 = (3 << 24) | (((1 | 8) & 0xFF) << 16); _a->w1 = osVirtualToPhysical(e->state); }
    } else
        { Acmd8_ *_a = ptr++; _a->w0 = (3 << 24) | (((0 | 8) & 0xFF) << 16); _a->w1 = osVirtualToPhysical(e->state); }
    *inp += (outCount << 1);
    e->delta += outCount;
    return ptr;
}

/* TODO (Clean version):
   This is a software implementation of frexp() (math.h) used because the
   decompiled code avoids the standard library.
   The iterative loops can be replaced with the standard:
     #include <math.h>
     double frexp(double value, int *eptr);
   or if the project provides its own libultra math stubs, reference that.

   TODO (Bug potential):
   The ascending loop `absvalue >= 1.0; absvalue *= 0.5` is O(exponent).
   For very large values this is slow compared to IEEE-754 bit extraction.
   This matches the original SDK's _frexpf implementation, so it's intentional.
*/
f64 _frexpf(f64 value, s32 *eptr) {
    f64 absvalue;

    *eptr = 0;
    if (value == 0.0)
        return (value);
    absvalue = (value > 0.0) ? value : -value;
    for (; absvalue >= 1.0; absvalue *= 0.5)
        ++*eptr;
    for (; absvalue < 0.5; absvalue += absvalue)
        --*eptr;
    return (value > 0.0 ? absvalue : -absvalue);
}

/* TODO (Bug potential):
   This is a software ldexp() — multiplies `in` by 2^ex.
   The implementation `1 << ex` is UNDEFINED BEHAVIOR for ex >= 31 (s32 overflow)
   and WRONG for negative ex (1 << negative is UB in C).
   The original SDK _ldexpf only gets called with small positive ex values
   (3, 4, 30) in _getRate, so this doesn't trigger in practice.
   Clean version:
     #include <math.h>
     double ldexp(double x, int exp);  // handles all cases correctly
*/
f64 _ldexpf(f64 in, s32 ex) {
    s32 exp;

    if (ex) {
        exp = 1 << ex;
        in *= (f64)exp;
    }
    return in;
}

/* TODO (Clean version):
   This is _getRate from envmixer.c — computes the per-sample volume ramp rate
   as a Q15.16 fixed-point value (ratem:ratel) such that:
     vol * rate^(count) ≈ tgt
   i.e. rate = (tgt/vol)^(1/count), computed via log-table exponentiation.

   The logtab[] array contains pre-computed log2 mantissa corrections for
   8 equally-spaced mantissa bins (indices 8..15 from frexp output mapped to 0..7).
   The constant 0.69314718055994530942 is ln(2).

   The algorithm:
     1. Compute invn = 1/count, scaled to Q2.30 fixed-point (i_invn)
     2. Decompose tgt/vol into mantissa*2^ex via _frexpf
     3. Approximate ln(tgt/vol) using logtab + exponent: eps = (logtab[idx-8] + ex) * ln(2)
     4. Scale eps to Q2.30: eps /= 2^30
     5. Compute (1+eps)^invn via binary exponentiation (squaring loop on i_invn bits)
     6. Apply a^16 = a*=a, a*=a, a*=a, a*=a → rate^8 per the RSP ENVMIXER convention

   The final `a *= (a *= (a *= a))` on line 280 chains four squarings to yield
   rate^(2^4) = rate^16... but the comment in _getVol says "rate values are
   actually rate^8". This discrepancy is because _getRate produces rate^(1/count)
   raised to the 16th power, while _getVol divides samples by 8 to compensate.
   Net effect: rate^(16 * samples/8) = rate^(2*samples), which only matches
   if the caller accounts for the factor of 2 in segEnd. Worth verifying.

   TODO (Bug potential):
   Line 280: `a *= (a *= (a *= a))` — the evaluation order of compound
   assignment with the same lvalue on both sides is unspecified in C89/C99.
   The compiler likely evaluates right-to-left yielding a^16, but this is
   technically undefined behavior. A clean version:
     a = a*a; a = a*a; a = a*a; a = a*a;  // explicit a^16
*/
s16 _getRate(f64 vol, f64 tgt, s32 count, u16 *ratel) {
    s16 s;
    f64 invn = 1.0 / count, eps, a, fs, mant;
    s32 i_invn, ex, indx;

    if (count == 0) {
        if (tgt >= vol) {
            *ratel = 0xffff;
            return 0x7fff;
        } else {
            *ratel = 0;
            return 0;
        }
    }
    if (tgt < 1.0)
        tgt = 1.0;
    if (vol <= 0) vol = 1;
    {
        f64 logtab[] = { -0.912537, -0.752072, -0.607683, -0.476438,
                         -0.356144, -0.245112, -0.142019, -0.045804 };

        i_invn = (s32)_ldexpf(invn, 30);
        mant = _frexpf(tgt / vol, &ex);
        indx = (s32)(_ldexpf(mant, 3 + 1));
        eps = (logtab[indx - 8] + ex) * 0.69314718055994530942;
        eps /= _ldexpf(1, 30);
        fs = (1.0 + eps);
        a = 1.0;
        while (i_invn) {
            if (i_invn & 1)
                a = a * fs;
            fs *= fs;
            i_invn >>= 1;
        }
    }
    a *= (a *= (a *= a));
    s = (s16)a;
    *ratel = (s16)(0xffff * (a - (f32)s));
    return (s16)a;
}

f32 _getVol(f32 ivol, s32 samples, s16 ratem, u16 ratel) {
    f32 r;
    f32 a;
    s32 i;

    /* Rate values are actually rate^8. */
    samples >>= 3;
    if (samples == 0) {
        return ivol;
    }
    r = ((f32)(ratem << 16) + (f32)ratel) / 65536;
    a = 1.0;
    for (i = 0; i < 32; i++) {
        if (samples & 1) {
            a *= r;
        }
        samples >>= 1;
        if (samples == 0) {
            break;
        }
        r *= r;
    }
    ivol *= a;
    return ivol;
}

void alFilterNew(ALFilt8_ *f, void *h, void *s, s32 type) {
    f->source = 0;
    f->handler = h;
    f->setParam = s;
    f->inp = 0;
    f->outp = 0;
    f->type = type;
}
