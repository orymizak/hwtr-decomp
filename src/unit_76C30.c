#include "common.h"
#include "ultra64.h"
#include "include_asm.h"
typedef struct { u8 b[0x10]; } ClientM_;
void func_800D5910(ClientM_ *c, OSMesgQueue *mq, s32 mask);
void func_800D59E0(ClientM_ *c);



void func_800D6030(s32 n) {
    ClientM_ client;
    OSMesgQueue mq;
    void *msgbuf;

    osCreateMesgQueue(&mq, &msgbuf, 1);
    func_800D5910(&client, &mq, 1);
    while (n != 0) {
        osRecvMesg(&mq, 0, 1);
        n--;
    }
    func_800D59E0(&client);
}
