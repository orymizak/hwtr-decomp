#include "common.h"
#include "n_audio.h"
#include "include_asm.h"



typedef struct { u8 p0[4]; s32 unk4; u8 p8[4]; s32 unkC; u8 p10[0x18]; s32 unk28; s32 unk2C; s32 unk30; s32 unk34; s32 unk38; s32 unk3C; } AmCfg_;
typedef struct { s32 a; s32 b; } AmBuf_;
extern u8 D_8014C158;
extern OSThread D_801279E0; /* audio manager thread */
extern s32 D_80127B98;
extern AmBuf_ *D_80127B94;
extern s32 D_80127B90;
ALDMANew AudDma_Init(s32, s32);
ALHeap *SndHeap_Get(void);
void SndSyn_Init(void *, ALSynConfig *);
s32 Snd_InitFrameSize(s32, s32, s32, s32);
s32 SndHeap_Alloc(s32);
void Snd_ManagerThread(void *);

void Snd_CreateManager(AmCfg_ *c, s32 a1, u8 fx) {
    ALSynConfig sc;
    u32 i;
    s32 n;

    sc.maxVVoices = sc.maxPVoices = c->unk4;
    sc.maxUpdates = c->unk28;
    sc.dmaproc = AudDma_Init(c->unk38, c->unk3C);
    sc.fxType = fx;
    sc.outputRate = osAiSetFrequency(c->unk2C);
    sc.heap = SndHeap_Get();
    SndSyn_Init(&D_8014C158, &sc);
    n = Snd_InitFrameSize(c->unk34, sc.outputRate, a1, 15);
    D_80127B98 = SndHeap_Alloc(c->unk30 << 3);
    D_80127B94 = (AmBuf_ *)SndHeap_Alloc(0x18);
    for (i = 0; i < 3; i++) {
        D_80127B94[i].a = SndHeap_Alloc(n << 2);
    }
    D_80127B90 = SndHeap_Alloc(0x2000);
    osCreateThread(&D_801279E0, 3, Snd_ManagerThread, NULL, (void *)(D_80127B90 + 0x2000), c->unkC);
    osStartThread(&D_801279E0);
}


typedef struct { void *data; u32 size; void *ucode; void *ucode_data; } AudInfoT_;
typedef struct {
    void (*init)(void);
    void (*waitRetrace)(void);
    void (*runTask)(AudInfoT_ *);
} SndTaskHooks;
extern SndTaskHooks *gSndTaskHooks;
extern u8 D_800E7B70[];
extern u8 D_800FCD20[];
extern AmBuf_ *D_800FAEB0;
void AudDma_Update(void);
s32 Snd_GetFrameSamples(u32);

void Snd_ManagerThread(void *arg) {
    AudInfoT_ task;
    s32 cmdCount;
    u32 bufIdx;
    s32 len;
    u32 status;
    AmBuf_ *buf;
    Acmd *cmdListEnd;

    task.data = (void *)D_80127B98;
    task.ucode = D_800E7B70;
    task.ucode_data = D_800FCD20;

    bufIdx = 0;
    gSndTaskHooks->init();

    while (1) {
        do {
            gSndTaskHooks->waitRetrace();
            status = osAiGetStatus();
            len = osAiGetLength() >> 2;
        } while (status & 0x80000000);

        AudDma_Update();

        if (D_800FAEB0 != NULL && cmdCount != 0) {
            osAiSetNextBuffer((void *)D_800FAEB0->a, D_800FAEB0->b << 2);
        }

        buf = &D_80127B94[bufIdx];
        buf->b = Snd_GetFrameSamples(len);
        cmdListEnd = n_alAudioFrame((Acmd *)D_80127B98, &cmdCount, (s16 *)osVirtualToPhysical((void *)buf->a), buf->b);

        if (cmdCount != 0) {
            task.size = (cmdListEnd - (Acmd *)D_80127B98) * sizeof(Acmd);
            gSndTaskHooks->runTask(&task);
            D_800FAEB0 = buf;
        }

        bufIdx = (bufIdx + 1) % 3;
    }
}
