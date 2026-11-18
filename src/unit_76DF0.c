#include "common.h"
#include "ultra64.h"
#include "include_asm.h"
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

void func_800D6240(u32 rom, void *buf, u32 size) {
    OSIoMesg io;
    OSMesgQueue mq;
    void *msg;
    u32 n;

    osCreateMesgQueue(&mq, &msg, 1);
    io.hdr.pri = OS_MESG_PRI_NORMAL;
    io.hdr.retQueue = &mq;
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
        osEPiStartDma(D_80148040, &io, OS_READ);
        rom += n;
        buf = (u8 *)buf + n;
        size -= n;
        osRecvMesg(&mq, &msg, OS_MESG_BLOCK);
    }
}

s32 func_800D6300(void) {
    s32 ok;

    ok = func_800D7170();
    func_800D7570();
    func_800D6460();
    func_800D69E0();
    return ok & 0xFF;
}
