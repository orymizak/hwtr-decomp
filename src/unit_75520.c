#include "common.h"
#include "include_asm.h"
typedef struct Player4_ { struct Player4_ *next; void *clientData; void *handler; s32 callTime; s32 samplesLeft; } Player4_;
typedef struct { Player4_ *head; u8 p4[0x1C]; s32 curSamples; } Drv4_;
extern Drv4_ *D_800FB174;
s32 osSetIntMask(s32);



void func_800D4920(Player4_ *client) {
    s32 mask = osSetIntMask(1);
    Drv4_ *drvr = D_800FB174;

    client->samplesLeft = drvr->curSamples;
    client->next = drvr->head;
    drvr->head = client;
    osSetIntMask(mask);
}

