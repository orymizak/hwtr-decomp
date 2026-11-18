#ifndef N_AUDIO_H
#define N_AUDIO_H

/* NuSystem n_audio: libaudio's synthesizer with the decoder, resampler and envelope mixer of a
 * voice merged into one N_PVoice. The ALParam/ALFilter/ALResampler/ALLowPass/ALDelay/ALFx/...
 * private types are libaudio's own (ultralib src/audio/synthInternals.h); the N_ types below are
 * reconstructed from how the game's driver uses them. Fields named unkXX are not identified yet. */

#include "ultra64.h"
#include "PR/libaudio.h"
#include "audio/synthInternals.h"
#include "n_abi.h"

struct N_ALVoice_s;

/* Physical voice (0x8C). */
typedef struct N_PVoice_s {
    /* 0x00 */ ALLink node;
    /* 0x08 */ struct N_ALVoice_s *vvoice;
    /* decoder */
    /* 0x0C */ ADPCM_STATE *dc_state;
    /* 0x10 */ ADPCM_STATE *dc_lstate;
    /* 0x14 */ ALRawLoop dc_loop;
    /* 0x20 */ struct ALWaveTable_s *dc_table;
    /* 0x24 */ s32 dc_bookSize;
    /* 0x28 */ ALDMAproc dc_dma;
    /* 0x2C */ void *dc_dmaState;
    /* 0x30 */ s32 dc_sample;
    /* 0x34 */ s32 dc_lastsam;
    /* 0x38 */ s32 dc_first;
    /* 0x3C */ s32 dc_memin;
    /* resampler */
    /* 0x40 */ RESAMPLE_STATE *rs_state;
    /* 0x44 */ f32 rs_ratio;
    /* 0x48 */ s32 rs_upitch;
    /* 0x4C */ f32 rs_delta;
    /* 0x50 */ s32 rs_first;
    /* envelope mixer */
    /* 0x54 */ ENVMIX_STATE *em_state;
    /* 0x58 */ s16 em_pan;
    /* 0x5A */ s16 em_volume;
    /* 0x5C */ s16 em_cvolL;
    /* 0x5E */ s16 em_cvolR;
    /* 0x60 */ s16 em_dryamt;
    /* 0x62 */ s16 em_wetamt;
    /* 0x64 */ u16 em_lratl;
    /* 0x66 */ s16 em_lratm;
    /* 0x68 */ s16 em_ltgt;
    /* 0x6A */ u16 em_rratl;
    /* 0x6C */ s16 em_rratm;
    /* 0x6E */ s16 em_rtgt;
    /* 0x70 */ s32 em_delta;
    /* 0x74 */ s32 em_segEnd;
    /* 0x78 */ s32 em_first;
    /* 0x7C */ ALParam *em_ctrlList;
    /* 0x80 */ ALParam *em_ctrlTail;
    /* 0x84 */ s32 em_motion;
    /* 0x88 */ s32 offset;
} N_PVoice; /* size = 0x8C */

/* Virtual voice (libaudio's ALVoice, pointing at an N_PVoice). */
typedef struct N_ALVoice_s {
    /* 0x00 */ ALLink node;
    /* 0x08 */ N_PVoice *pvoice;
    /* 0x0C */ struct ALWaveTable_s *table;
    /* 0x10 */ void *clientPrivate;
    /* 0x14 */ s16 state;
    /* 0x16 */ s16 priority;
    /* 0x18 */ s16 fxBus;
    /* 0x1A */ s16 unityPitch;
} N_ALVoice;

/* ALFreeParam with an N_PVoice. */
typedef struct {
    /* 0x00 */ ALParam *next;
    /* 0x04 */ s32 delta;
    /* 0x08 */ s16 type;
    /* 0x0C */ N_PVoice *pvoice;
} N_ALFreeParam;

typedef struct {
    /* 0x00 */ ALFilter filter;
    /* 0x14 */ s32 sourceCount;
    /* 0x18 */ s32 maxSources;
    /* 0x1C */ N_PVoice **sources;
    /* 0x20 */ ALFx *fx;
    /* 0x24 */ s32 unk24[8];
} N_ALAuxBus; /* size = 0x44 */

/* Main bus: only its pull routine is used, called as pull(sampleOffset, cmdList). */
typedef struct {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ Acmd *(*pull)(s32 sampleOffset, Acmd *cmdList);
} N_ALMainBus;

typedef struct {
    /* 0x00 */ ALPlayer *head;
    /* 0x04 */ ALLink pFreeList;
    /* 0x0C */ ALLink pAllocList;
    /* 0x14 */ ALLink pLameList;
    /* 0x1C */ s32 paramSamples;
    /* 0x20 */ s32 curSamples;
    /* 0x24 */ ALDMANew dma;
    /* 0x28 */ ALHeap *heap;
    /* 0x2C */ ALParam *paramList;
    /* 0x30 */ N_ALMainBus *mainBus;
    /* 0x34 */ N_ALAuxBus *auxBus;
    /* 0x38 */ s32 numPVoices;
    /* 0x3C */ s32 unk3C;
    /* 0x40 */ s32 outputRate;
    /* 0x44 */ s32 maxOutSamples;
    /* 0x48 */ s16 *outBuf;
    /* 0x4C */ s32 maxAuxBusses;
} N_ALSynth; /* size = 0x50 */

extern N_ALSynth *D_800FB174; /* the synthesizer (n_syn) */

ALParam *func_800D52B8(void);                                   /* takes an ALParam from the free list */
s32 func_800D3158(N_PVoice *pvoice, s32 paramID, void *param);  /* voice filter chain setParam */
s32 func_800D53E4(s32 micros);                                  /* microseconds to output samples */

#endif
