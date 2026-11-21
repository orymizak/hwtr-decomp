#include "common.h"
#include "ultra64.h"
#include "pak.h"
#include "include_asm.h"
s32 strcmp(const char *, const char *);
typedef struct { u8 d[0x2C]; } E2C5_;
typedef struct { u8 n; u8 pad[3]; E2C5_ *arr; } T2C5_;
typedef struct { u8 pad[0x14]; u32 lo; u32 hi; } Bits5_;
extern s8 gUnlockBitMap[];
extern u8 D_80123D20[];
void MenuFont_LoadAll(void);
void Menu_LayoutAll(void);
void Menu_ResetAllPos(void);
extern s32 gMenuAnimTime[];
extern u8 gNameListCount;
extern void *gNameList[];
extern u8 D_80100BB0;
void NameList_Clear(void);
typedef struct { u8 d[0x140]; } E140_;
typedef struct { u8 a; u8 pad; u16 x; u16 y; u16 x2; u16 y2; } E0A2_;
typedef struct { u8 pad[2]; u8 n; u8 pad2; E0A2_ *arr; u8 pad3[0xB]; u8 mode; } T0A2_;
typedef struct { s16 x; s16 y; } P16b_;
typedef struct { u8 a; u8 pad; u16 x; u16 y; u16 x2; u16 y2; } E0A5_;
typedef struct { s32 pad0; s16 a; s16 b; s16 c; u8 kind; u8 padB; u16 d; u16 padE; } Src10_;
typedef struct { f32 p[3]; f32 q[3]; f32 v[3]; f32 w[3]; u16 e; u16 f; } Dst34_;
typedef struct { s32 d[0x88C / 4]; } Big88C_;
typedef struct { s32 d[0x140 / 4]; } Blk140_;
extern u8 *gMenuObjs[];
typedef struct { u8 a; u8 pad; s16 x; s16 y; s16 x2; s16 y2; } E0A7_;
u8 Pos_IsNear(P16b_, P16b_);
f32 MenuPos_ToWorldY(s32 ms);
void *Memory_Alloc(s32);
void Memory_Free(void *);
extern u8 D_80123D28[];
extern u8 D_80123D24[];
extern s32 gSaveIds[];
extern s32 (*sysPakOpen[])(s32);
extern s32 (*sysTimeGetMillis)(void);
void MenuText_Animate(u8 *, f32);
void MenuSprite_Animate(u8 *, f32);
void MenuButton_Animate(u8 *, f32);
void MenuModel_Animate(void *, f32);
s32 MenuModels_StartEnter(s32 arg0);
void MenuText_Layout(u8 *t);
s32 MenuFont_Kerning(s32, u8, u8);
s32 MenuFont_CharWidth(s32, u8);
void MenuText_ResetPos(u8 *t);
extern u32 gPakStatus[];
extern u32 (*sysPakGetState)(u32);
s8 NameList_Add(char *s);
extern E140_ gSaveSlots[][4];
extern void (*sysWorldLoad)(u8, s32);
void Menu_LoadModel(s32 x);
extern s32 gSaveSlotCount[];
extern u8 gSaves;

extern s32 (*sysRandRange)(s32);
s32 Password_Encode();
s32 Password_Decode();
extern s32 (*sysWorldFree)(s32);
void Menu_PrepareEnter();
void Menu_PrepareLeave();
s32 MenuModels_StartLeave(s32);
s32 MenuFont_FreeAll();

s32 Save_Probe(void) {
    u32 i;
    u32 j;
    s32 s;
    s32 ok;

    for (i = 0; i < 4; i++) {
        memset(&gSaves + i * 0x88C, 0, 0x88C);
        D_80123D28[i] = 0;
    }
    memset(D_80123D24, 0, 4);
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            memset(&gSaveSlots[i][j], 0, 0x140);
        }
    }
    for (i = 0; i < 4; i++) {
        gSaveIds[i] = 0;
        gSaveSlotCount[i] = 0;
    }
    for (i = 0; i < 4; i++) {
        sysPakOpen[0](i);
        s = sysPakOpen[1](i);
        gPakStatus[i] = s;
        if (s != 4) {
            D_80123D24[i] = 1;
        } else {
            D_80123D24[i] = 0;
        }
        if (gPakStatus[i] != 1) {
            D_80123D20[i] = 1;
        } else {
            D_80123D20[i] = 0;
        }
    }
    if (D_80123D20[0] || D_80123D20[1] || D_80123D20[2] || D_80123D20[3]) {
        ok = 0;
        if (!(D_80123D20[0] && gPakStatus[0]) && !(D_80123D20[1] && gPakStatus[1]) &&
            !(D_80123D20[2] && gPakStatus[2]) && !(D_80123D20[3] && gPakStatus[3])) {
            ok = 1;
        }
        return ok;
    }
    return 0;
}

s32 Save_IsSupported(void) {
    return 1;
}

s32 Pak_GetStatus(u32 i) {
    u32 s;

    if (i >= 4) {
        return PAK_STATUS_DAMAGED;
    }
    if (gPakStatus[i] & 0x40) {
        return PAK_STATUS_UNK8;
    }
    s = sysPakGetState(i);
    if (s == PAK_STATE_OK) {
        return PAK_STATUS_OK;
    } else if (s & PAK_STATE_CHANGED) {
        return PAK_STATUS_CHANGED;
    } else if (s == PAK_STATE_NO_FILE) {
        return PAK_STATUS_NO_FILE;
    } else if (s & PAK_STATE_NONE) {
        return PAK_STATUS_NONE;
    } else if (s & PAK_STATE_DAMAGED) {
        return PAK_STATUS_DAMAGED;
    } else if (s & PAK_STATE_FULL) {
        return PAK_STATUS_FULL;
    } else if (s & PAK_STATE_UNK10) {
        return PAK_STATUS_UNK6;
    } else if (s & PAK_STATE_UNK40) {
        return PAK_STATUS_UNK8;
    } else if (s & PAK_STATE_RUMBLE) {
        return PAK_STATUS_RUMBLE;
    }
    return PAK_STATUS_DAMAGED;
}


void Save_NewId(void);

s32 Save_CreateOnPak(u32 i) {
    u8 *buf;
    u32 sum;
    u16 j;
    u8 ok;
    u8 *p;
    u32 n;

    if (i >= 4) {
        return 0;
    }
    buf = Memory_Alloc(0xDA0);
    memset(buf, 0, 0xDA0);
    *(s32 *)buf = ((s32 (*)(void))Save_NewId)();
    sum = 0;
    p = buf;
    for (j = 0, n = 0xD90; j < n; j++) {
        sum += *p++;
    }
    *(u32 *)(buf + 0xD90) = sum;
    ok = ((u8 (*)(u32, u8 *))sysPakOpen[3])(i, buf);
    Memory_Free(buf);
    if (ok) {
        gPakStatus[i] &= ~4;
    }
    return ok;
}

INCLUDE_ASM("asm/nonmatchings/unit_50510", Save_ReadAll);

void Save_NewId(void);
s32 Save_Commit(void) {
    u8 *buf;
    u32 i;
    u32 j;
    u16 k;
    s32 sum;
    s32 done;
    s32 *id;
    u8 *c;
    u16 n;

    done = 0;
    buf = Memory_Alloc(0xDA0);
    for (i = 0; i < 4; i++) {
        if (D_80123D20[i] != 0 && (D_80123D24[i] != 0 || (done ^ 1))) {
            *(Big88C_ *)buf = *(Big88C_ *)(&gSaves + i * 0x88C);
            id = &gSaveIds[i];
            if (*id == 0) {
                *id = ((s32 (*)(void))Save_NewId)();
            }
            *(s32 *)buf = *id;
            buf[0x88C] = gSaveSlotCount[i];
            for (j = 0; j < (u32)gSaveSlotCount[i]; j++) {
                *(Blk140_ *)(buf + 0x890 + j * 0x140) = *(Blk140_ *)&gSaveSlots[i][j];
            }
            sum = 0;
            c = buf;
            n = 0xD90;
            for (k = 0; k < n; k++) {
                sum += *c++;
            }
            *(s32 *)(buf + 0xD90) = sum;
            if (((s32 (*)(s32, u8 *))sysPakOpen[3])(i, buf) == 0) {
                gPakStatus[i] = ((u32 (*)(u32))sysPakOpen[1])(i);
                Memory_Free(buf);
                return 0;
            }
            if (D_80123D24[i] == 0) {
                done = 1;
            }
        }
    }
    Memory_Free(buf);
    return 1;
}

u8 *Save_GetActiveProfile(void) {
    u8 i;

    for (i = 0; i < 4; i++) {
        if (D_80123D20[i] != 0) {
            return i * 0x88C + &gSaves;
        }
    }
    return &gSaves;
}


void *Save_GetProfile(s32 arg0) {
    return (arg0 * 0x88C) + &gSaves;
}

E140_ *SaveSlot_Get(u32 i, u32 j) {
    if (i >= 4 || j >= (u32)gSaveSlotCount[i]) {
        return NULL;
    }
    return &gSaveSlots[i][j];
}

extern s32 D_80122904[][4][0x50];
extern Big88C_ D_80120F54[];

E140_ *SaveSlot_FindPlayer(u32 i) {
    u32 k;
    u32 m;

    if (i >= 2) {
        return NULL;
    }
    for (k = 0; k < 4; k++) {
        for (m = 0; m < 4; m++) {
            if (D_80123D28[k] && m < (u32)gSaveSlotCount[k] && D_80122904[k][m][0] == D_80120F54[k].d[i]) {
                return &gSaveSlots[k][m];
            }
        }
    }
    return NULL;
}

s32 SaveSlot_GetCount(u32 i) {
    s32 r;

    if (i < 4) {
        r = gSaveSlotCount[i];
    } else {
        r = 0;
    }
    return r;
}

INCLUDE_ASM("asm/nonmatchings/unit_50510", SaveSlot_Add);

s32 SaveSlot_Delete(u32 i, u32 j) {
    u32 k;

    if (i >= 4 || j >= (u32)gSaveSlotCount[i]) {
        return 0;
    }
    for (k = j; k < 3; k++) {
        *(Blk140_ *)&gSaveSlots[i][k] = *(Blk140_ *)&gSaveSlots[i][k + 1];
    }
    gSaveSlotCount[i]--;
    return 1;
}

void Stats_PickupCollectedNop(void) {

}

void func_800B061C(void) {

}

void Save_NewId(void) {
    do {

    } while (sysRandRange(0x17D78400) == 0);
}

void Unlocks_GetMask(Bits5_ *b, u32 *mask) {
    u8 i;
    s32 bit;

    i = 0;
    *mask = 0;
    for (; i < 24; i++) {
        bit = gUnlockBitMap[i];
        if (bit == -1) {
            return;
        }
        if (bit >= 32) {
            if ((b->hi >> (bit - 32)) & 1) {
                *mask |= 1 << i;
            }
        } else if ((b->lo >> bit) & 1) {
            *mask |= 1 << i;
        }
    }
}

void Unlocks_SetMask(Bits5_ *b, u32 mask) {
    u8 i;

    for (i = 0; i < 24; i++, mask >>= 1) {
        if (mask == 0) {
            return;
        }
        if (mask & 1) {
            s8 bit = gUnlockBitMap[i];

            if (bit >= 32) {
                b->hi |= 1 << (bit - 32);
            } else {
                b->lo |= 1 << bit;
            }
        }
    }
}

INCLUDE_ASM("asm/nonmatchings/unit_50510", Password_Encode);

INCLUDE_ASM("asm/nonmatchings/unit_50510", Password_Decode);

s32 Password_Get(void) {
    Password_Encode();
    return 1;
}

void Password_Apply(void) {
    Password_Decode();
}

s8 NameList_Add(char *s) {
    u8 i;
    u8 n;

    for (i = 0; i < gNameListCount; i++) {
        if (strcmp(s, gNameList[i]) == 0) {
            return -1;
        }
    }
    n = gNameListCount;
    gNameList[n] = s;
    gNameListCount = n + 1;
    return n;
}


s8 NameList_Find(char *s) {
    u8 i;

    for (i = 0; i < gNameListCount; i++) {
        if (strcmp(s, gNameList[i]) == 0) {
            return i;
        }
    }
    return -1;
}

void NameList_Remove(char *s) {
    u8 i;
    u8 j;

    for (i = 0; i < gNameListCount; i++) {
        if (strcmp(s, gNameList[i]) == 0) {
            for (j = i; j < gNameListCount; j++) {
                gNameList[j] = gNameList[j + 1];
            }
            gNameList[j] = &D_80100BB0;
            gNameListCount--;
            return;
        }
    }
}

void NameList_Clear(void) {
    u8 i;

    for (i = 0; i < gNameListCount; i++) {
        gNameList[i] = &D_80100BB0;
    }
    gNameListCount = 0;
}


void Menu_LoadModel(s32 x) {
    s8 r = NameList_Add(x);

    if (r >= 0) {
        sysWorldLoad(r, x);
    }
}

f32 MenuPos_ToWorldX(s32 arg0) {
    return (((f32) arg0 / 1000.0f) * 1000.0f) + -500.0f;
}

f32 MenuPos_ToWorldY(s32 ms) {
    f32 t;
    t = ms / 1000.0f;
    t = -t * 750.0f;
    return t + 375.0f;
}


f32 MenuPos_ToWorldZ(s32 arg0) {
    return (((f32) (arg0 + 0x1F4) / 1000.0f) * 1000.0f) + -500.0f;
}

void MenuText_Layout(u8 *t) {
    s32 i;
    s32 w;
    s16 x;
    s32 y;
    s32 v;

    w = 0;
    if (t[0x18] != 0) {
        for (i = 0; i < t[0x18] - 1; i++) {
            if (t[0x28] == 2) {
                w += 0x23;
            } else {
                w += MenuFont_Kerning(t[0x28], (*(E0A5_ **)(t + 0x1C))[i].a, (*(E0A5_ **)(t + 0x1C))[i + 1].a);
            }
        }
        if (t[0x28] == 2) {
            w += 0x23;
        } else {
            w += MenuFont_CharWidth(t[0x28], (*(E0A5_ **)(t + 0x1C))[i].a);
        }
        w /= 2;
        if (t[0x27] == 0) {
            v = *(s16 *)(t + 0x20);
        } else {
            v = *(s16 *)(t + 0x20) - w;
        }
        y = *(s16 *)(t + 0x22);
        x = v;
        (*(E0A5_ **)(t + 0x1C))[0].x2 = x;
        (*(E0A5_ **)(t + 0x1C))[0].y2 = y;
        *(P16b_ *)&(*(E0A5_ **)(t + 0x1C))[0].x = *(P16b_ *)&(*(E0A5_ **)(t + 0x1C))[0].x2;
        for (i = 1; i < t[0x18]; i++) {
            if (t[0x28] == 2) {
                x += 0x23;
            } else {
                x += MenuFont_Kerning(t[0x28], (*(E0A5_ **)(t + 0x1C))[i - 1].a, (*(E0A5_ **)(t + 0x1C))[i].a);
            }
            (*(E0A5_ **)(t + 0x1C))[i].x2 = x;
            (*(E0A5_ **)(t + 0x1C))[i].y2 = y;
            *(P16b_ *)&(*(E0A5_ **)(t + 0x1C))[i].x = *(P16b_ *)&(*(E0A5_ **)(t + 0x1C))[i].x2;
        }
    }
}

void MenuSprite_Layout(u8 *t) {
    u8 i;
    s32 w;
    s32 x;
    s32 y;

    i = 0;
    w = 0;
    x = (*(s16 *)(t + 8) + *(s16 *)(t + 0xA)) / 2;
    y = *(s16 *)(t + 0xC);
    for (; i < t[2] - 1; i++) {
        w += MenuFont_Kerning(0, (*(E0A5_ **)(t + 4))[i].a, (*(E0A5_ **)(t + 4))[i + 1].a);
    }
    w += MenuFont_CharWidth(0, (*(E0A5_ **)(t + 4))[i].a);
    w /= 2;
    x -= w;
    (*(E0A5_ **)(t + 4))[0].x2 = x;
    (*(E0A5_ **)(t + 4))[0].y2 = y;
    *(P16b_ *)&(*(E0A5_ **)(t + 4))[0].x = *(P16b_ *)&(*(E0A5_ **)(t + 4))[0].x2;
    for (i = 1; i < t[2]; i++) {
        x += MenuFont_Kerning(0, (*(E0A5_ **)(t + 4))[i - 1].a, (*(E0A5_ **)(t + 4))[i].a);
        (*(E0A5_ **)(t + 4))[i].x2 = x;
        (*(E0A5_ **)(t + 4))[i].y2 = y;
        *(P16b_ *)&(*(E0A5_ **)(t + 4))[i].x = *(P16b_ *)&(*(E0A5_ **)(t + 4))[i].x2;
    }
}

/* Lays out a button label: centred between x0 and x1, or left-aligned at x0 when t[0x13] is 1. */
void MenuButton_Layout(u8 *t) {
    s32 i;
    s32 w;
    s32 x;
    s32 y;
    s32 mid;

    mid = (*(s16 *)(t + 8) + *(s16 *)(t + 0xA)) / 2;
    y = *(s16 *)(t + 0xC);
    if (t[0x13] != 1) {
        w = 0;
        for (i = 0; i < t[2] - 1; i++) {
            w += MenuFont_Kerning(0, (*(E0A5_ **)(t + 4))[i].a, (*(E0A5_ **)(t + 4))[i + 1].a);
        }
        w += MenuFont_CharWidth(0, (*(E0A5_ **)(t + 4))[i].a);
        w /= 2;
        x = mid - w;
    } else {
        x = *(s16 *)(t + 8);
    }
    (*(E0A5_ **)(t + 4))[0].x2 = x;
    (*(E0A5_ **)(t + 4))[0].y2 = y;
    *(P16b_ *)&(*(E0A5_ **)(t + 4))[0].x = *(P16b_ *)&(*(E0A5_ **)(t + 4))[0].x2;
    for (i = 1; i < t[2]; i++) {
        x += MenuFont_Kerning(0, (*(E0A5_ **)(t + 4))[i - 1].a, (*(E0A5_ **)(t + 4))[i].a);
        (*(E0A5_ **)(t + 4))[i].x2 = x;
        (*(E0A5_ **)(t + 4))[i].y2 = y;
        *(P16b_ *)&(*(E0A5_ **)(t + 4))[i].x = *(P16b_ *)&(*(E0A5_ **)(t + 4))[i].x2;
    }
}

void Menu_LayoutAll(void) {
    u32 k;
    u32 i;

    for (k = 0; k < 33; k++) {
        for (i = 0; i < gMenuObjs[k][0]; i++) {
            MenuText_Layout(*(u8 **)(gMenuObjs[k] + 4) + i * 0x2C);
        }
        for (i = 0; i < gMenuObjs[k][8]; i++) {
            MenuSprite_Layout(*(u8 **)(gMenuObjs[k] + 0xC) + i * 0x18);
        }
        for (i = 0; i < gMenuObjs[k][0x10]; i++) {
            MenuButton_Layout(*(u8 **)(gMenuObjs[k] + 0x14) + i * 0x18);
        }
    }
}

void MenuText_ResetPos(u8 *t) {
    u32 i;

    if (t[0x26] == 2) {
        for (i = 0; i < t[0x18]; i++) {
            (*(E0A5_ **)(t + 0x1C))[i].x = (*(E0A5_ **)(t + 0x1C))[i].x2 + 640;
            (*(E0A5_ **)(t + 0x1C))[i].y = (*(E0A5_ **)(t + 0x1C))[i].y2;
        }
    } else if (t[0x26] == 1) {
        for (i = 0; i < t[0x18]; i++) {
            (*(E0A5_ **)(t + 0x1C))[i].x = (*(E0A5_ **)(t + 0x1C))[i].x2 - 640;
            (*(E0A5_ **)(t + 0x1C))[i].y = (*(E0A5_ **)(t + 0x1C))[i].y2;
        }
    } else if (t[0x26] == 0) {
        for (i = 0; i < t[0x18]; i++) {
            *(P16b_ *)&(*(E0A5_ **)(t + 0x1C))[i].x = *(P16b_ *)&(*(E0A5_ **)(t + 0x1C))[i].x2;
        }
    }
}


void MenuText_SetLeaveTarget(u8 *t) {
    u32 i;

    if (t[0x26] == 0) {
        for (i = 0; i < t[0x18]; i++) {
            (*(E0A5_ **)(t + 0x1C))[i].x2 = (*(E0A5_ **)(t + 0x1C))[i].x + 640;
            (*(E0A5_ **)(t + 0x1C))[i].y2 = (*(E0A5_ **)(t + 0x1C))[i].y;
            (*(E0A5_ **)(t + 0x1C))[i].x = (*(E0A5_ **)(t + 0x1C))[i].x2;
        }
    } else if (t[0x26] == 2) {
        for (i = 0; i < t[0x18]; i++) {
            (*(E0A5_ **)(t + 0x1C))[i].x2 = (*(E0A5_ **)(t + 0x1C))[i].x + 640;
            (*(E0A5_ **)(t + 0x1C))[i].y2 = (*(E0A5_ **)(t + 0x1C))[i].y;
        }
    } else if (t[0x26] == 1) {
        for (i = 0; i < t[0x18]; i++) {
            (*(E0A5_ **)(t + 0x1C))[i].x2 = (*(E0A5_ **)(t + 0x1C))[i].x - 640;
            (*(E0A5_ **)(t + 0x1C))[i].y2 = (*(E0A5_ **)(t + 0x1C))[i].y;
        }
    }
}

void MenuSprite_ResetPos(u8 *t) {
    u32 i;

    if (t[0x13] == 2) {
        for (i = 0; i < t[2]; i++) {
            (*(E0A5_ **)(t + 4))[i].x = (*(E0A5_ **)(t + 4))[i].x2 + 640;
            (*(E0A5_ **)(t + 4))[i].y = (*(E0A5_ **)(t + 4))[i].y2;
        }
    } else if (t[0x13] == 1) {
        for (i = 0; i < t[2]; i++) {
            (*(E0A5_ **)(t + 4))[i].x = (*(E0A5_ **)(t + 4))[i].x2 - 640;
            (*(E0A5_ **)(t + 4))[i].y = (*(E0A5_ **)(t + 4))[i].y2;
        }
    } else if (t[0x13] == 0) {
        for (i = 0; i < t[2]; i++) {
            *(P16b_ *)&(*(E0A5_ **)(t + 4))[i].x = *(P16b_ *)&(*(E0A5_ **)(t + 4))[i].x2;
        }
    }
}


void MenuSprite_SetLeaveTarget(T0A2_ *t) {
    u32 i;

    if (t->mode == 2) {
        for (i = 0; i < t->n; i++) {
            t->arr[i].x2 = t->arr[i].x + 640;
            t->arr[i].y2 = t->arr[i].y;
        }
    } else if (t->mode == 1) {
        for (i = 0; i < t->n; i++) {
            t->arr[i].x2 = t->arr[i].x - 640;
            t->arr[i].y2 = t->arr[i].y;
        }
    }
}


void MenuButton_ResetPos(u8 *t) {
    u32 i;

    if (t[0x13] == 2) {
        for (i = 0; i < t[2]; i++) {
            (*(E0A5_ **)(t + 4))[i].x = (*(E0A5_ **)(t + 4))[i].x2 + 640;
            (*(E0A5_ **)(t + 4))[i].y = (*(E0A5_ **)(t + 4))[i].y2;
        }
    } else if (t[0x13] == 1) {
        for (i = 0; i < t[2]; i++) {
            (*(E0A5_ **)(t + 4))[i].x = (*(E0A5_ **)(t + 4))[i].x2 - 640;
            (*(E0A5_ **)(t + 4))[i].y = (*(E0A5_ **)(t + 4))[i].y2;
        }
    } else if (t[0x13] == 0) {
        for (i = 0; i < t[2]; i++) {
            *(P16b_ *)&(*(E0A5_ **)(t + 4))[i].x = *(P16b_ *)&(*(E0A5_ **)(t + 4))[i].x2;
        }
    }
}


void MenuButton_SetLeaveTarget(T0A2_ *t) {
    u32 i;

    if (t->mode == 2) {
        for (i = 0; i < t->n; i++) {
            t->arr[i].x2 = t->arr[i].x + 640;
            t->arr[i].y2 = t->arr[i].y;
        }
    } else if (t->mode == 1) {
        for (i = 0; i < t->n; i++) {
            t->arr[i].x2 = t->arr[i].x - 640;
            t->arr[i].y2 = t->arr[i].y;
        }
    }
}


void MenuButton_Layout(u8 *t);

void Menu_PrepareEnter(t)
    u8 *t;
{
    u32 i;

    for (i = 0; i < t[0]; i++) {
        MenuText_Layout(*(u8 **)(t + 4) + i * 0x2C);
        MenuText_ResetPos(*(u8 **)(t + 4) + i * 0x2C);
    }
    for (i = 0; i < t[8]; i++) {
        MenuSprite_Layout(*(u8 **)(t + 0xC) + i * 0x18);
        MenuSprite_ResetPos(*(u8 **)(t + 0xC) + i * 0x18);
    }
    for (i = 0; i < t[0x10]; i++) {
        MenuButton_Layout(*(u8 **)(t + 0x14) + i * 0x18);
        MenuButton_ResetPos(*(u8 **)(t + 0x14) + i * 0x18);
    }
}

typedef struct { u8 d[0x2C]; } E2Cc_;

void Menu_PrepareLeave(t)
    u8 *t;
{
    u32 i;

    for (i = 0; i < t[0]; i++) {
        MenuText_SetLeaveTarget((u8 *)&(*(E2Cc_ **)(t + 4))[i]);
    }
    for (i = 0; i < t[8]; i++) {
        MenuSprite_SetLeaveTarget((T0A2_ *)&(*(OSMesgQueue **)(t + 0xC))[i]);
    }
    for (i = 0; i < t[0x10]; i++) {
        MenuButton_SetLeaveTarget((T0A2_ *)&(*(OSMesgQueue **)(t + 0x14))[i]);
    }
}

void Menu_ResetAllPos(void) {
    u32 k;
    u32 i;

    for (k = 0; k < 33; k++) {
        for (i = 0; i < gMenuObjs[k][8]; i++) {
            MenuSprite_ResetPos(*(u8 **)(gMenuObjs[k] + 0xC) + i * 0x18);
        }
        for (i = 0; i < gMenuObjs[k][0x10]; i++) {
            MenuButton_ResetPos(*(u8 **)(gMenuObjs[k] + 0x14) + i * 0x18);
        }
    }
}

s32 MenuModels_StartEnter(s32 arg0) {
    u8 *t;
    s32 i;
    f32 off;
    Src10_ *s;
    Dst34_ *d;

    t = (u8 *) arg0;
    for (i = 0; i < t[0x20]; i++) {
        off = 500.0f - MenuPos_ToWorldX((*(Src10_ **) (t + 0x24))[i].a);
        (*(Dst34_ **) (t + 0x28))[i].q[0] = MenuPos_ToWorldX((*(Src10_ **) (t + 0x24))[i].a);
        (*(Dst34_ **) (t + 0x28))[i].q[1] = MenuPos_ToWorldY((*(Src10_ **) (t + 0x24))[i].b);
        (*(Dst34_ **) (t + 0x28))[i].q[2] = MenuPos_ToWorldZ((*(Src10_ **) (t + 0x24))[i].c);
        switch ((*(Src10_ **) (t + 0x24))[i].kind) {
            case 0:
                (*(Dst34_ **) (t + 0x28))[i].p[0] = (*(Dst34_ **) (t + 0x28))[i].q[0];
                goto common;
            case 2:
                (*(Dst34_ **) (t + 0x28))[i].p[0] = (*(Dst34_ **) (t + 0x28))[i].q[0] + off;
            common:
                (*(Dst34_ **) (t + 0x28))[i].p[1] = (*(Dst34_ **) (t + 0x28))[i].q[1];
                (*(Dst34_ **) (t + 0x28))[i].p[2] = (*(Dst34_ **) (t + 0x28))[i].q[2];
                (*(Dst34_ **) (t + 0x28))[i].v[0] = 0.0f;
                (*(Dst34_ **) (t + 0x28))[i].v[1] = 0.0f;
                (*(Dst34_ **) (t + 0x28))[i].v[2] = 0.0f;
                break;
        }
        (*(Dst34_ **) (t + 0x28))[i].w[0] = 0.0f;
        (*(Dst34_ **) (t + 0x28))[i].w[1] = 0.0f;
        (*(Dst34_ **) (t + 0x28))[i].w[2] = 0.0f;
        (*(Dst34_ **) (t + 0x28))[i].e = (*(Src10_ **) (t + 0x24))[i].d;
        (*(Dst34_ **) (t + 0x28))[i].f = (*(Src10_ **) (t + 0x24))[i].d;
    }
}

s32 MenuModels_StartLeave(s32 arg0) {
    u8 *t;
    s32 i;

    t = (u8 *) arg0;
    for (i = 0; i < t[0x20]; i++) {
        switch ((*(Src10_ **) (t + 0x24))[i].kind) {
            case 0:
                break;
            case 2:
                (*(Dst34_ **) (t + 0x28))[i].q[0] = (*(Dst34_ **) (t + 0x28))[i].p[0] + 1000.0f;
                (*(Dst34_ **) (t + 0x28))[i].q[1] = (*(Dst34_ **) (t + 0x28))[i].p[1];
                (*(Dst34_ **) (t + 0x28))[i].q[2] = (*(Dst34_ **) (t + 0x28))[i].p[2];
                (*(Dst34_ **) (t + 0x28))[i].w[0] = 0.0f;
                (*(Dst34_ **) (t + 0x28))[i].w[1] = 0.0f;
                (*(Dst34_ **) (t + 0x28))[i].w[2] = 0.0f;
                break;
        }
        (*(Dst34_ **) (t + 0x28))[i].v[0] = 0.0f;
        (*(Dst34_ **) (t + 0x28))[i].v[1] = 0.0f;
        (*(Dst34_ **) (t + 0x28))[i].v[2] = 0.0f;
        (*(Dst34_ **) (t + 0x28))[i].e = (*(Src10_ **) (t + 0x24))[i].d;
        (*(Dst34_ **) (t + 0x28))[i].f = (*(Src10_ **) (t + 0x24))[i].d;
    }
}

void MenuText_Animate(u8 *t, f32 s) {
    u8 i;
    s32 dx;
    s32 dy;

    for (i = 0; i < t[0x18]; i++) {
        dx = (*(E0A7_ **)(t + 0x1C))[i].x2 - (*(E0A7_ **)(t + 0x1C))[i].x;
        dx *= s;
        dy = (*(E0A7_ **)(t + 0x1C))[i].y2 - (*(E0A7_ **)(t + 0x1C))[i].y;
        dy *= s;
        if (dx > 0) {
            (*(E0A7_ **)(t + 0x1C))[i].x += dx;
        } else {
            (*(E0A7_ **)(t + 0x1C))[i].x = (*(E0A7_ **)(t + 0x1C))[i].x2;
        }
        if (dy > 0) {
            (*(E0A7_ **)(t + 0x1C))[i].y += dy;
        } else {
            (*(E0A7_ **)(t + 0x1C))[i].y = (*(E0A7_ **)(t + 0x1C))[i].y2;
        }
    }
}

void MenuSprite_Animate(u8 *t, f32 s) {
    u8 i;
    s32 dx;
    s32 dy;

    for (i = 0; i < t[2]; i++) {
        dx = (*(E0A7_ **)(t + 4))[i].x2 - (*(E0A7_ **)(t + 4))[i].x;
        dx *= s;
        dy = (*(E0A7_ **)(t + 4))[i].y2 - (*(E0A7_ **)(t + 4))[i].y;
        dy *= s;
        if (dx > 0) {
            (*(E0A7_ **)(t + 4))[i].x += dx;
        } else {
            (*(E0A7_ **)(t + 4))[i].x = (*(E0A7_ **)(t + 4))[i].x2;
        }
        if (dy > 0) {
            (*(E0A7_ **)(t + 4))[i].y += dy;
        } else {
            (*(E0A7_ **)(t + 4))[i].y = (*(E0A7_ **)(t + 4))[i].y2;
        }
    }
}

void MenuButton_Animate(u8 *t, f32 s) {
    s32 i;
    s32 dx;
    s32 dy;

    for (i = 0; i < t[2]; i++) {
        dx = (*(E0A7_ **)(t + 4))[i].x2 - (*(E0A7_ **)(t + 4))[i].x;
        dx *= s;
        dy = (*(E0A7_ **)(t + 4))[i].y2 - (*(E0A7_ **)(t + 4))[i].y;
        dy *= s;
        if (dx > 0) {
            (*(E0A7_ **)(t + 4))[i].x += dx;
        } else {
            (*(E0A7_ **)(t + 4))[i].x = (*(E0A7_ **)(t + 4))[i].x2;
        }
        if (dy > 0) {
            (*(E0A7_ **)(t + 4))[i].y += dy;
        } else {
            (*(E0A7_ **)(t + 4))[i].y = (*(E0A7_ **)(t + 4))[i].y2;
        }
    }
}

typedef struct { f32 x, y, z; } V3mm_;
typedef struct { V3mm_ pos; V3mm_ targetPos; V3mm_ rot; V3mm_ targetRot; s16 target; s16 value; } MenuModel_;

void MenuModel_Animate(void *p, f32 k) {
    MenuModel_ *m = p;
    V3mm_ d;
    V3mm_ r;
    s32 diff;

    d.x = m->targetPos.x - m->pos.x;
    d.y = m->targetPos.y - m->pos.y;
    d.z = m->targetPos.z - m->pos.z;
    d.x *= k;
    d.y *= k;
    d.z *= k;
    m->pos.x += d.x;
    m->pos.y += d.y;
    m->pos.z += d.z;
    r.x = m->targetRot.x - m->rot.x;
    r.y = m->targetRot.y - m->rot.y;
    r.z = m->targetRot.z - m->rot.z;
    r.x *= k;
    r.y *= k;
    r.z *= k;
    m->rot.x += r.x;
    m->rot.y += r.y;
    m->rot.z += r.z;
    diff = m->target - m->value;
    diff = diff * k;
    m->value += diff;
}

void Menu_Animate(u8 *t, u8 i) {
    f32 dt;
    s32 now;
    u32 d;
    s32 k;

    if (gMenuAnimTime == NULL) {
        dt = 0.15f;
        gMenuAnimTime[i] = sysTimeGetMillis();
    } else {
        now = sysTimeGetMillis();
        d = now - gMenuAnimTime[i];
        if (d > 100) {
            dt = 0.1f;
        } else {
            dt = d / 100.0f;
        }
        gMenuAnimTime[i] = now;
    }
    for (k = 0; k < t[0]; k++) {
        MenuText_Animate(*(s32 *) (t + 4) + k * 0x2C, dt);
    }
    for (k = 0; k < t[8]; k++) {
        MenuSprite_Animate(*(s32 *) (t + 0xC) + k * 0x18, dt);
    }
    for (k = 0; k < t[0x10]; k++) {
        MenuButton_Animate(*(s32 *) (t + 0x14) + k * 0x18, dt);
    }
    for (k = 0; k < t[0x20]; k++) {
        MenuModel_Animate(*(s32 *) (t + 0x28) + k * 0x34, dt);
    }
}

void Menu_LoadModels(u8 *menu) {
    u8 i;

    for (i = 0; i < menu[0x20]; i++) {
        Menu_LoadModel(*(s32 *)(*(u8 **)(menu + 0x24) + i * 0x10));
    }
}

void Menu_FreeModels(void) {
    u8 i;

    for (i = 0; i < 32; i++) {
        sysWorldFree(i);
    }
    NameList_Clear();
}

void Menu_StartEnter(s32 arg0) {
    Menu_PrepareEnter();
    MenuModels_StartEnter(arg0);
}

void Menu_StartLeave(s32 arg0) {
    Menu_PrepareLeave();
    MenuModels_StartLeave(arg0);
}

void Menu_ResetTexts(T2C5_ *t) {
    u8 i;

    for (i = 0; i < t->n; i++) {
        MenuText_Layout((s32)&t->arr[i]);
        MenuText_ResetPos(&t->arr[i]);
    }
}


void Menu_LayoutTexts(u8 *t) {
    u8 i;

    for (i = 0; i < t[0]; i++) {
        MenuText_Layout(*(u8 **)(t + 4) + i * 0x2C);
    }
}

void Menu_ResetState(void) {
    u8 i;

    MenuFont_LoadAll();
    Menu_LayoutAll();
    Menu_ResetAllPos();
    for (i = 0; i < 33; i++) {
        gMenuAnimTime[i] = 0;
    }
}

void Menu_FreeFonts(void) {
    MenuFont_FreeAll();
}

/* Returns 1 when the two points are at most 10 apart on both axes. */
u8 Pos_IsNear(P16b_ a, P16b_ b) {
    if ((u32)(a.x - b.x + 10) >= 21 || (u32)(a.y - b.y + 10) >= 21) {
        return 0;
    }
    return 1;
}

s32 MenuText_IsSettled(u8 *t) {
    u8 i;

    for (i = 0; i < t[0x18]; i++) {
        if (!Pos_IsNear(*(P16b_ *)&(*(E0A5_ **)(t + 0x1C))[i].x, *(P16b_ *)&(*(E0A5_ **)(t + 0x1C))[i].x2)) {
            return 0;
        }
    }
    return 1;
}

s32 MenuSprite_IsSettled(u8 *t) {
    u8 i;

    for (i = 0; i < t[2]; i++) {
        if (!Pos_IsNear(*(P16b_ *)&(*(E0A5_ **)(t + 4))[i].x, *(P16b_ *)&(*(E0A5_ **)(t + 4))[i].x2)) {
            return 0;
        }
    }
    return 1;
}

s32 MenuButton_IsSettled(u8 *t) {
    u8 i;

    for (i = 0; i < t[2]; i++) {
        if (!Pos_IsNear(*(P16b_ *)&(*(E0A5_ **)(t + 4))[i].x, *(P16b_ *)&(*(E0A5_ **)(t + 4))[i].x2)) {
            return 0;
        }
    }
    return 1;
}

void Menu_Nop(s32 a) {
    s32 x[4];
}
