#include "common.h"
#include "include_asm.h"
/* nusys NUContPakFile and the per-channel OSPfs pool (0x68 each). */
typedef struct { u8 p0[8]; s32 channel; u8 pC[0x5C]; } Pfs72_;
typedef struct { Pfs72_ *pfs; s32 file_no; s32 error; s32 type; } PakFile72_;
s32 func_800D7208(s32, void *);
extern u8 D_8014E998;



void func_800D66D0(PakFile72_ *file, s32 ch) {
    Pfs72_ *pfs = &((Pfs72_ *)&D_8014E998)[ch];

    file->pfs = pfs;
    pfs->channel = ch;
    file->error = func_800D7208(0x201, file);
}
