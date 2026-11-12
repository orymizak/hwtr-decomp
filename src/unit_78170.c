#include "common.h"
#include "include_asm.h"
extern u8 D_8014EB40[];
s32 osContStartReadData(void *mq);
void osContGetReadData(void *data);
s32 func_800D76B4(void *data, s32 mask);
extern s32 (*D_800FB230)(s16);
extern u8 D_801347A0;
extern s32 D_801380AC;
extern u8 D_80141090;
s32 osRecvMesg(void *,s32,s32);
s32 osSendMesg(void *,s32,s32);
extern u8 D_8014ECB0;

s32 func_800D74F0(s32 *);
extern s32 D_800FB248;

typedef struct { u16 type; u8 status; u8 errnum; } ContStatusM_;
extern ContStatusM_ D_801380B8[4];
extern s32 D_80132040;
extern s32 D_80132044;
extern s32 D_80143840;
void func_800D78E0(void);
void func_800D7910(void);
void osCreateMesgQueue(void *, void *, s32);
s32 func_800D7450(void *);
u8 func_800D7570(void) {
    s32 i;
    s32 pattern;
    s32 bit;

    func_800D78E0();
    osCreateMesgQueue(&D_801347A0, &D_80132040, 1);
    osCreateMesgQueue(&D_8014ECB0, &D_80132044, 1);
    func_800D7450(&D_800FB248);
    func_800D7910();
    bit = 1;
    pattern = 0;
    D_80143840 = 0;
    for (i = 0; i < 4; i++) {
        if (D_801380B8[i].errnum != 0) {
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

void func_800D7644(void) {
    func_800D74F0(&D_800FB248);
}

void func_800D7664(void) {
    osSendMesg(&D_8014ECB0, 0, 1);
}

void func_800D768C(void) {
    osRecvMesg(&D_8014ECB0, 0, 1);
}

s32 func_800D76B4(void *data, s32 mask) {
    s32 r = osContStartReadData(D_8014EB40);

    if (r != 0) {
        return r;
    }
    ((s32 (*)(void *mq, void **msg, s32 flag))osRecvMesg)(D_8014EB40, 0, 1);
    if (mask & D_801380AC) {
        return 0;
    }
    func_800D7664();
    osContGetReadData(data);
    func_800D768C();
    return 0;
}


INCLUDE_ASM("asm/nonmatchings/unit_78170", func_800D773C);

s32 func_800D778C(s16 *arg0) {
    if (D_801380AC == 0) {
        osRecvMesg(&D_801347A0, 0, 0);
        func_800D76B4(&D_80141090, 1);
        if (D_800FB230 != NULL) {
            D_800FB230(*arg0);
        }
        osSendMesg(&D_801347A0, 0, 0);
    }
    return 0;
}

void func_800D7818(void *arg0) {
    ((s32 (*)(s32,s32))func_800D76B4)((*(s32 *)((u8 *)(arg0) + 0xC)), 0);
}

s32 func_800D7838(s16 *p) {
    s32 r;

    osRecvMesg(&D_801347A0, 0, 0);
    r = func_800D76B4(&D_80141090, 0);
    if (r != 0) {
        return r;
    }
    if (D_800FB230 != 0) {
        D_800FB230(*p);
    }
    return 0;
}
