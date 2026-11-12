#include "common.h"
#include "include_asm.h"
s32 osCartRomInit();
s32 osCreatePiManager(s32, void *, void *,s32);
extern u8 D_8012FD90;
extern u8 D_8012FDA8;
extern s32 D_80148040;

extern s32 D_80140E44;
s32 func_800D6460();
s32 func_800D69E0();
s32 func_800D7170();
s32 func_800D7570();

void func_800D61F0(void) {
    D_80140E44 = 0x80;
}

void func_800D6200(void) {
    osCreatePiManager(0x96, &D_8012FD90, &D_8012FDA8, 0x32);
    D_80148040 = osCartRomInit();
}

typedef struct { u8 b[0x18]; } MqP_;
typedef struct { u16 type; u8 pri; u8 status; MqP_ *retQueue; void *dramAddr; u32 devAddr; u32 size; void *piHandle; } IoMesgP_;
void osCreateMesgQueue(void *, void *, s32);
void osInvalDCache(void *, s32);
s32 osEPiStartDma(s32, void *, s32);
s32 osRecvMesg(void *, void *, s32);
void func_800D6240(u32 rom, void *buf, u32 size) {
    IoMesgP_ io;
    MqP_ mq;
    void *msg;
    u32 n;

    osCreateMesgQueue(&mq, &msg, 1);
    io.pri = 0;
    io.retQueue = &mq;
    osInvalDCache(buf, size);
    while (size) {
        if (size > 0x4000) {
            n = 0x4000;
        } else {
            n = size;
        }
        io.dramAddr = buf;
        io.devAddr = rom;
        io.size = n;
        osEPiStartDma(D_80148040, &io, 0);
        rom += n;
        buf = (u8 *)buf + n;
        size -= n;
        osRecvMesg(&mq, &msg, 1);
    }
}

s32 func_800D6300(void) {
    s32 temp_s0;

    temp_s0 = func_800D7170();
    func_800D7570();
    func_800D6460();
    func_800D69E0();
    return temp_s0 & 0xFF;
}
