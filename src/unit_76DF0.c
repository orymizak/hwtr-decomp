#include "common.h"
#include "ultra64.h"
#include "include_asm.h"
extern u8 D_8012FD90;
extern u8 D_8012FDA8;
extern s32 D_80148040;

extern s32 nuGfxDisplay;
s32 nuContPakMgrInit();
s32 nuContRmbMgrInit();
s32 nuSiMgrInit();
s32 nuContMgrInit();

void nuGfxDisplayOn(void) {
    nuGfxDisplay = 0x80;
}

void nuPiInit(void) {
    osCreatePiManager(0x96, &D_8012FD90, &D_8012FDA8, 0x32);
    D_80148040 = osCartRomInit();
}

void nuPiReadRom(u32 rom, void *buf, u32 size) {
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

s32 nuContInit(void) {
    s32 ok;

    ok = nuSiMgrInit();
    nuContMgrInit();
    nuContPakMgrInit();
    nuContRmbMgrInit();
    return ok & 0xFF;
}
