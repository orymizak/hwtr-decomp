#include "common.h"
#include "include_asm.h"



typedef struct { s32 maxVVoices; s32 maxPVoices; s32 maxUpdates; s32 maxFXbusses; s32 dmaproc; s32 heap; s32 outputRate; u8 fxType; u8 p1D[3]; s32 params; } SynCfg_;
typedef struct { u8 p0[4]; s32 unk4; u8 p8[4]; s32 unkC; u8 p10[0x18]; s32 unk28; s32 unk2C; s32 unk30; s32 unk34; s32 unk38; s32 unk3C; } AmCfg_;
typedef struct { s32 a; s32 b; } AmBuf_;
extern u8 D_8014C158;
extern u8 D_801279E0;
extern s32 D_80127B98;
extern AmBuf_ *D_80127B94;
extern s32 D_80127B90;
s32 func_800D1AE0(s32, s32);
s32 osAiSetFrequency(u32);
s32 func_800D2444(void);
void func_800D1290(void *, SynCfg_ *);
s32 func_800D24F0(s32, s32, s32, s32);
s32 func_800D23FC(s32);
void func_800D2228(void *);
void osCreateThread(void *, s32, void (*)(void *), void *, void *, s32);
void osStartThread(void *);

void func_800D20F0(AmCfg_ *c, s32 a1, u8 fx) {
    SynCfg_ sc;
    u32 i;
    s32 n;

    sc.maxVVoices = sc.maxPVoices = c->unk4;
    sc.maxUpdates = c->unk28;
    sc.dmaproc = func_800D1AE0(c->unk38, c->unk3C);
    sc.fxType = fx;
    sc.outputRate = osAiSetFrequency(c->unk2C);
    sc.heap = func_800D2444();
    func_800D1290(&D_8014C158, &sc);
    n = func_800D24F0(c->unk34, sc.outputRate, a1, 15);
    D_80127B98 = func_800D23FC(c->unk30 << 3);
    D_80127B94 = (AmBuf_ *)func_800D23FC(0x18);
    for (i = 0; i < 3; i++) {
        D_80127B94[i].a = func_800D23FC(n << 2);
    }
    D_80127B90 = func_800D23FC(0x2000);
    osCreateThread(&D_801279E0, 3, func_800D2228, 0, (void *)(D_80127B90 + 0x2000), c->unkC);
    osStartThread(&D_801279E0);
}


INCLUDE_ASM("asm/nonmatchings/unit_72CF0", func_800D2228);
