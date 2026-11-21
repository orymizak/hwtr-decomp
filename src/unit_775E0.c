#include "common.h"
#include "ultra64.h"
#include "include_asm.h"
typedef struct { u16 a; u16 b; u16 id; u8 state; u8 flags; u8 x; u8 y; } PakSlotQ_;
typedef struct { u16 a; u16 b; u8 p4[2]; u8 state; u8 pad[9]; } PakInfoQ_;
typedef struct { u8 ch; u8 p1[7]; PakInfoQ_ *info; } ArgsQ_;
typedef struct { u8 p0[0xC]; ArgsQ_ *args; } ReqQ_;
typedef struct { u16 id; u8 state; u8 flags; u8 pad[6]; } PakSlotJ_;
extern PakSlotJ_ D_80140F6C[4];
void nuSiCallBackAdd(void *a);
typedef struct { u8 p0[0xC]; u8 *args; } ReqG_;
extern u8 D_8014EB40[];
extern OSPfs nuContPfs[];
typedef struct { u8 *unk0; } ReqArgA_;
typedef struct { u8 p0[0xC]; u8 *args; } ReqA_;
extern u8 D_80140F6A[];

s32 nuSiCallBackRemove(s32 *);
extern s32 D_800FB210;
extern u8 D_80140F6F;

void nuContRmbMgrInit(void) {
    u32 i;

    for (i = 0; i < 4; i++) {
        D_80140F6C[i].state = 2;
        D_80140F6C[i].flags = 0;
        D_80140F6C[i].id = i;
    }
    nuSiCallBackAdd(((u8 *)&D_800FB210));
}


void nuContRmbMgrRemove(void) {
    nuSiCallBackRemove(&D_800FB210);
}

extern PakSlotQ_ D_80140F68[4];
extern u32 D_800FB1F0;
s32 contRmbControl(PakSlotQ_ *r, u32 ch) {
    s32 ret;
    u32 n;

    ret = 0;
    switch (r->state) {
    case 2:
        break;
    case 1:
        if (r->id != 0) {
            ret = __osMotorAccess(&nuContPfs[ch], 0);
        } else {
            r->state = 2;
        }
        r->id--;
        break;
    case 3:
        if (r->b != 0) {
            r->id += r->a;
            n = r->id >> 8;
            r->id &= 0xFF;
            if (n != 0) {
                ret = __osMotorAccess(&nuContPfs[ch], 1);
            } else {
                ret = __osMotorAccess(&nuContPfs[ch], 0);
            }
        } else {
            ret = __osMotorAccess(&nuContPfs[ch], 0);
            r->state = 1;
            r->id = 2;
        }
        r->b--;
        break;
    case 4:
        ret = osMotorInit(D_8014EB40, &nuContPfs[ch], ch);
        if (ret == 0) {
            __osMotorAccess(&nuContPfs[ch], 0);
        }
        r->state = 1;
        r->id = 2;
        break;
    }
    return ret;
}

s32 contRmbRetrace(void *mesg) {
    u32 i;
    PakSlotQ_ *r;

    for (i = 0; i < 4; i++) {
        r = &D_80140F68[i];
        switch (r->flags) {
        case 0:
            break;
        case 1:
            if (contRmbControl(r, i)) {
                r->flags = 0;
            }
            break;
        case 2:
            if (!r->x) {
                if (!(r->id % D_800FB1F0)) {
                    if (!osMotorInit(D_8014EB40, &nuContPfs[i], i)) {
                        r->x = 1;
                        r->y = 2;
                    }
                }
                r->id++;
            } else {
                if (contRmbControl(r, i)) {
                    r->id = i;
                    r->x = 0;
                    r->y = 0;
                }
            }
            break;
        case 0x81:
        case 0x82:
            if (r->y == 2) {
                contRmbControl(r, i);
            }
            break;
        }
    }
    return 0;
}

s32 contRmbCheckMesg(ReqG_ *r) {
    u8 ch = r->args[0];
    return osMotorInit(D_8014EB40, &nuContPfs[ch], ch);
}


s32 contRmbForceStopMesg(void) {
    u32 i;

    for (i = 0; i < 4; i++) {
        D_80140F6C[i].state = 4;
        D_80140F6C[i].flags |= 0x80;
    }
    return 0;
}

s32 contRmbForceStopEndMesg(void) {
    u32 i;

    for (i = 0; i < 4; i++) {
        D_80140F6C[i].flags &= 0x7F;
    }
    return 0;
}

s32 contRmbStartMesg(ReqQ_ *r) {
    ArgsQ_ *a = r->args;
    PakInfoQ_ *info = a->info;

    D_80140F68[a->ch].state = info->state;
    D_80140F68[a->ch].b = info->b;
    D_80140F68[a->ch].a = info->a;
    D_80140F68[a->ch].id = 0;
    return 0;
}

s32 contRmbStopMesg(ReqA_ *r) {
    *(s16 *)(D_80140F6A + r->args[0] * 10) = 0;
    return 0;
}


s32 nuSiSendMesg(u16 cmd, void *args);
void nuContRmbStart(u32 contNo, u16 freq, u16 frame) {
    PakInfoQ_ info;
    ArgsQ_ args;

    if (!((&D_80140F6F)[contNo * 10] & 0x80)) {
        info.state = 3;
        info.a = freq;
        info.b = frame;
        args.ch = contNo;
        args.info = &info;
        nuSiSendMesg(0x302, &args);
    }
}
