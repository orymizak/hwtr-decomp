#include "common.h"
#include "ultra64.h"
#include "include_asm.h"
typedef struct { u8 b[0x10]; } ClientM_;
void nuScAddClient(ClientM_ *c, OSMesgQueue *mq, s32 mask);
void nuScRemoveClient(ClientM_ *c);



void nuGfxRetraceWait(s32 n) {
    ClientM_ client;
    OSMesgQueue mq;
    void *msgbuf;

    osCreateMesgQueue(&mq, &msgbuf, 1);
    nuScAddClient(&client, &mq, 1);
    while (n != 0) {
        osRecvMesg(&mq, 0, 1);
        n--;
    }
    nuScRemoveClient(&client);
}
