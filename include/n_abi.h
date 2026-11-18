#ifndef N_ABI_H
#define N_ABI_H

/* NuSystem n_audio microcode commands that differ from PR/abi.h: buffer sizes and DMEM
 * addresses travel in the command word instead of a preceding aSetBuffer. Encodings are the
 * ones the game's driver writes. */

#include "PR/abi.h"

#define n_aLoadBuffer(pkt, c, d, s)                                                         \
    {                                                                                       \
        Acmd *_a = (Acmd *)pkt;                                                             \
        _a->words.w0 = _SHIFTL(A_LOADBUFF, 24, 8) | _SHIFTL(c, 12, 12) | _SHIFTL(d, 0, 12); \
        _a->words.w1 = (unsigned int)(s);                                                   \
    }

#define n_aSaveBuffer(pkt, c, d, s)                                                         \
    {                                                                                       \
        Acmd *_a = (Acmd *)pkt;                                                             \
        _a->words.w0 = _SHIFTL(A_SAVEBUFF, 24, 8) | _SHIFTL(c, 12, 12) | _SHIFTL(d, 0, 12); \
        _a->words.w1 = (unsigned int)(s);                                                   \
    }

/* Interleaves the fixed left/right main-bus buffers; only the opcode word is written. */
#define n_aInterleave(pkt)                             \
    {                                                  \
        Acmd *_a = (Acmd *)pkt;                        \
        _a->words.w0 = _SHIFTL(A_INTERLEAVE, 24, 8);   \
    }

#define n_aEnvMixer(pkt, f, t, s)                                                          \
    {                                                                                      \
        Acmd *_a = (Acmd *)pkt;                                                            \
        _a->words.w0 = _SHIFTL(A_ENVMIXER, 24, 8) | _SHIFTL(f, 16, 8) | _SHIFTL(t, 0, 16); \
        _a->words.w1 = (unsigned int)(s);                                                  \
    }

#define n_aResample(pkt, s, f, p, i, o)                                                     \
    {                                                                                       \
        Acmd *_a = (Acmd *)pkt;                                                             \
        _a->words.w0 = _SHIFTL(A_RESAMPLE, 24, 8) | _SHIFTL(s, 0, 24);                      \
        _a->words.w1 = _SHIFTL(f, 30, 2) | _SHIFTL(p, 14, 16) | _SHIFTL(i, 2, 12) | _SHIFTL(o, 0, 2); \
    }

#define n_aPoleFilter(pkt, f, g, t, s)                                                    \
    {                                                                                     \
        Acmd *_a = (Acmd *)pkt;                                                           \
        _a->words.w0 = _SHIFTL(A_POLEF, 24, 8) | _SHIFTL(f, 16, 8) | _SHIFTL(g, 0, 16);   \
        _a->words.w1 = _SHIFTL(t, 24, 8) | _SHIFTL(s, 0, 24);                             \
    }

#define n_aADPCMdec(pkt, s, f, c, a, d)                                                   \
    {                                                                                     \
        Acmd *_a = (Acmd *)pkt;                                                           \
        _a->words.w0 = _SHIFTL(A_ADPCM, 24, 8) | _SHIFTL(s, 0, 24);                       \
        _a->words.w1 = _SHIFTL(f, 28, 4) | _SHIFTL(c, 16, 12) | _SHIFTL(a, 12, 4) | _SHIFTL(d, 0, 12); \
    }

#endif
