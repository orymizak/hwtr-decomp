#include "common.h"
#include "ultra64.h"
#include "include_asm.h"
extern u8 D_8014EB40[];
s32 contReadData(void *data, s32 mask);
extern s32 (*D_800FB230)(s16);
extern u8 nuContWaitMesgQ;
extern s32 nuContDataLockKey;
extern u8 nuContData;
extern u8 nuContDataMutexQ;

s32 nuSiCallBackRemove(s32 *);
extern s32 nuContCallBack;

extern OSContStatus D_801380B8[4];
extern s32 D_80132040;
extern s32 D_80132044;
extern s32 D_80143840;
void nuContDataOpen(void);
void nuContQueryRead(void);
s32 nuSiCallBackAdd(void *);
u8 nuContMgrInit(void) {
    s32 i;
    s32 pattern;
    s32 bit;

    nuContDataOpen();
    osCreateMesgQueue(&nuContWaitMesgQ, &D_80132040, 1);
    osCreateMesgQueue(&nuContDataMutexQ, &D_80132044, 1);
    nuSiCallBackAdd(&nuContCallBack);
    nuContQueryRead();
    bit = 1;
    pattern = 0;
    D_80143840 = 0;
    for (i = 0; i < 4; i++) {
        if (D_801380B8[i].errno != 0) {
            continue;
        }
        if ((D_801380B8[i].type & 0x1F07) == 5) {
            D_80143840++;
            pattern |= bit;
        }
        bit <<= 1;
    }
    return pattern;
}

void nuContMgrRemove(void) {
    nuSiCallBackRemove(&nuContCallBack);
}

void nuContDataLock(void) {
    osSendMesg(&nuContDataMutexQ, 0, 1);
}

void nuContDataUnLock(void) {
    osRecvMesg(&nuContDataMutexQ, 0, 1);
}

s32 contReadData(void *data, s32 mask) {
    s32 r = osContStartReadData(D_8014EB40);

    if (r != 0) {
        return r;
    }
    ((s32 (*)(void *mq, void **msg, s32 flag))osRecvMesg)(D_8014EB40, 0, 1);
    if (mask & nuContDataLockKey) {
        return 0;
    }
    nuContDataLock();
    osContGetReadData(data);
    nuContDataUnLock();
    return 0;
}


INCLUDE_ASM("asm/nonmatchings/unit_78170", func_800D773C);

s32 contRetrace(s16 *arg0) {
    if (nuContDataLockKey == 0) {
        osRecvMesg(&nuContWaitMesgQ, 0, 0);
        contReadData(&nuContData, 1);
        if (D_800FB230 != NULL) {
            D_800FB230(*arg0);
        }
        osSendMesg(&nuContWaitMesgQ, 0, 0);
    }
    return 0;
}

void contRead(u8 *mesg) {
    contReadData(*(void **)(mesg + 0xC), 0);
}

s32 contReadNW(s16 *p) {
    s32 r;

    osRecvMesg(&nuContWaitMesgQ, 0, 0);
    r = contReadData(&nuContData, 0);
    if (r != 0) {
        return r;
    }
    if (D_800FB230 != 0) {
        D_800FB230(*p);
    }
    return 0;
}
