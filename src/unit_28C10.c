#include "common.h"
#include "ultra64.h"
#include "include_asm.h"
#include "car.h"
typedef struct { u8 b; u8 pad[3]; s32 w[6]; } E1C8_;
extern E1C8_ D_800F316C[];
extern u8 (*svcRandRange)(u8);
extern s32 gLangText[];
extern char D_8010080C[];
typedef struct Lnk89_ { s32 *obj; s32 x; struct Lnk89_ *next; } Lnk89_;
typedef struct { u8 *def; s32 t; u8 pad8; u8 hold; } Tmr89_;
typedef struct { s32 pad0; u8 id; u8 pad5[3]; s32 on; s32 t; u8 fired; u8 pad11[7]; } Evt18_;
typedef struct { u8 p0[0x34]; s32 flags; f32 f[0x18]; Vec3f v; f32 fA4; u8 col[4]; } P_8010;
extern u16 D_800F302A;
extern u16 D_800F302C;
extern u16 D_800F302E;
extern u16 D_800F3030;
extern u16 D_800F3032;
extern u16 D_800F3034;
void Hud_ShowTurbosGained(u8, s32);
void Stats_PickupCollectedNop(u8, s32);
void Car_ApplyPickup(u8 *arg0, u8 *arg1);
extern void (*audSfxPlay)(s32, s32, f32, f32);
void RefList_Add(Lnk89_ **, s32 *);
void *Memory_Alloc(u32 size);
u8 *Pickup_PickDef(u8 *);
s32 Pickup_GetHudIcon(u8 *);
extern Evt18_ *gPickupSpawns;
extern u32 gPickupSpawnCount;
extern void (*svcTrackSetEntityVisible)(u8, s32, f32);
s32 RaceClock_Get(void);
extern Lnk89_ **gCarPickups[];
void RefList_Release(Lnk89_ **, s32 *);
void Memory_Free(void *);
s32 Car_ComputeMass(void *);
extern s32 (*D_80141768)(u8,s32);



void Car_ApplyPickup(u8 *arg0, u8 *arg1) {
    P_8010 *p = (P_8010 *)arg0;
    Car *c = (Car *)arg1;
    Vec3f v;
    Vec3f d;
    Vec3f t;

    if (c->state != 2) {
        return;
    }
    v.x = p->v.x * c->scale;
    v.y = p->v.y * c->scale;
    v.z = p->v.z * c->scale;
    t.x = c->rot.m[0][0] * v.x + c->rot.m[0][1] * v.y + c->rot.m[0][2] * v.z;
    t.y = c->rot.m[1][0] * v.x + c->rot.m[1][1] * v.y + c->rot.m[1][2] * v.z;
    t.z = c->rot.m[2][0] * v.x + c->rot.m[2][1] * v.y + c->rot.m[2][2] * v.z;
    d = t;
    c->impulse.x = d.x + c->impulse.x;
    c->impulse.y = d.y + c->impulse.y;
    c->impulse.z = d.z + c->impulse.z;
    c->k4CC *= p->fA4;
    c->col[0] += p->col[0];
    c->col[1] += p->col[1];
    c->col[2] += p->col[2];
    c->col[3] += p->col[3];
    c->k[0] *= p->f[0];
    c->k[1] *= p->f[1];
    c->k[2] *= p->f[2];
    c->k[3] *= p->f[3];
    c->k[4] *= p->f[4];
    c->k[5] *= p->f[5];
    c->k[6] *= p->f[6];
    c->k[7] *= p->f[7];
    c->k[8] *= p->f[8];
    c->k[9] *= p->f[9];
    c->k[10] *= p->f[13];
    c->k[11] *= p->f[14];
    c->k[13] *= p->f[15];
    c->k[14] *= p->f[16];
    c->k[16] *= p->f[17];
    c->k[17] *= p->f[18];
    c->k[19] *= p->f[19];
    c->k[20] *= p->f[20];
    c->k[22] *= p->f[11];
    c->k530 *= p->f[10];
    c->k[23] *= p->f[12];
    c->r3EC.hi *= p->f[21];
    c->r3EC.lo *= p->f[22];
    c->k42C *= p->f[23];
    if (p->flags & 8) {
        c->pickupFx[1] = 1;
    }
    if (p->flags & 0x10) {
        c->pickupFx[2] = 1;
        c->k53C *= 10.0f;
    }
    if (p->flags & 0x20) {
        c->pickupFx[3] = 1;
    }
    if (p->flags & 0x40) {
        c->pickupFx[4] = 1;
    }
    if (p->flags & 1) {
        c->pickupFx[0] = 1;
    }
    if (p->flags & 2) {
        D_80141768(c->id, 1);
    }
    if ((p->flags & 4) && c->turbos < 10) {
        Hud_ShowTurbosGained(c->id, 1);
        c->turbos++;
    }
    if (c->flags & 1) {
        if (p->flags & 0x01000000) {
            if (c->id == 0) {
                D_800F302E = D_800F302A;
            } else if (c->id == 1) {
                D_800F3030 = D_800F302A;
            }
        }
        if (p->flags & 0x02000000) {
            if (c->id == 0) {
                D_800F3032 = D_800F302C;
            } else if (c->id == 1) {
                D_800F3034 = D_800F302C;
            }
        }
        if (p->flags & 0x04000000) {
            Stats_PickupCollectedNop(c->id, 3);
        }
        if (p->flags & 0x08000000) {
            Stats_PickupCollectedNop(c->id, 4);
        }
        if (p->flags & 0x10000000) {
            Stats_PickupCollectedNop(c->id, 5);
        }
        if (p->flags & 0x20000000) {
            Stats_PickupCollectedNop(c->id, 6);
        }
        if (p->flags & 0x40000000) {
            Stats_PickupCollectedNop(c->id, 7);
        }
        if (p->flags & 0x80000000) {
            Stats_PickupCollectedNop(c->id, 8);
        }
    }
    Car_ComputeMass(c);
    c->scale = c->k[0];
    c->invScale = 1.0f / c->k[0];
}

void Car_RemovePickup(P_8010 *p, Car *c) {
    if (c->state == 2) {
        c->k4CC /= p->fA4;
        c->col[0] -= p->col[0];
        c->col[1] -= p->col[1];
        c->col[2] -= p->col[2];
        c->col[3] -= p->col[3];
        c->k[0] /= p->f[0];
        c->k[1] /= p->f[1];
        c->k[2] /= p->f[2];
        c->k[3] /= p->f[3];
        c->k[4] /= p->f[4];
        c->k[5] /= p->f[5];
        c->k[6] /= p->f[6];
        c->k[7] /= p->f[7];
        c->k[8] /= p->f[8];
        c->k[9] /= p->f[9];
        c->k[10] /= p->f[13];
        c->k[11] /= p->f[14];
        c->k[13] /= p->f[15];
        c->k[14] /= p->f[16];
        c->k[16] /= p->f[17];
        c->k[17] /= p->f[18];
        c->k[19] /= p->f[19];
        c->k[20] /= p->f[20];
        c->k[22] /= p->f[11];
        c->k530 /= p->f[10];
        c->k[23] /= p->f[12];
        c->r3EC.hi /= p->f[21];
        c->r3EC.lo /= p->f[22];
        c->k42C /= p->f[23];
        if (p->flags & 8) {
            c->pickupFx[1] = 0;
        }
        if (p->flags & 0x10) {
            c->pickupFx[2] = 0;
            c->k53C /= 10.0f;
        }
        if (p->flags & 0x20) {
            c->pickupFx[3] = 0;
        }
        if (p->flags & 0x40) {
            c->pickupFx[4] = 0;
        }
        if (p->flags & 1) {
            c->pickupFx[0] = 0;
        }
        if (p->flags & 2) {
            D_80141768(c->id, 0);
        }
        Car_ComputeMass(c);
        c->scale = c->k[0];
        c->invScale = 1.0f / c->k[0];
    }
}

extern u8 D_800FCA20[];
s32 strcmp(const char *, const char *);
inline s32 Str_EqualNoCase(u8 *a, u8 *b) {
    char bufA[128];
    char bufB[128];
    s32 i;

    for (i = 0; *a != 0; a++) {
        if (D_800FCA20[*a] & 4) {
            bufA[i++] = *a - 0x20;
        } else {
            bufA[i++] = *a;
        }
    }
    bufA[i] = 0;
    for (i = 0; *b != 0; b++) {
        if (D_800FCA20[*b] & 4) {
            bufB[i++] = *b - 0x20;
        } else {
            bufB[i++] = *b;
        }
    }
    bufB[i] = 0;
    return strcmp(bufA, bufB) == 0;
}

extern u8 D_80100720[];
extern Lnk89_ **D_80118488;
extern u8 (*svcTrackLoadDup)(u8 *, void *);
s32 PickupModel_Load(u8 *s) {
    Lnk89_ *n;
    u8 found;
    void *obj;

    if (Str_EqualNoCase(s, D_80100720)) {
        return 0;
    }
    found = 0;
    n = *D_80118488;
    while (n != NULL && !found) {
        if (Str_EqualNoCase(s, (u8 *)n->obj)) {
            found = 1;
        }
        n = n->next;
    }
    if (found) {
        return 0;
    }
    obj = Memory_Alloc(0xB0);
    RefList_Add(D_80118488, obj);
    if (svcTrackLoadDup(s, obj)) {
        return 1;
    }
    RefList_Release(D_80118488, obj);
    Memory_Free(obj);
    return 0;
}

extern u8 D_80100720[];
extern u8 D_80100728[];
extern u8 D_80100730[];
extern u8 D_80100738[];
extern u8 D_80100744[];
extern u8 D_8010074C[];
extern u8 D_80100754[];
extern u8 D_8010075C[];
extern u8 D_80100764[];
extern u8 D_8010076C[];
extern u8 D_80100774[];
extern u8 D_8010077C[];

s32 Pickup_TypeFromName(u8 *name) {
    if (Str_EqualNoCase(name, D_80100728)) { /* "turbo" */
        return 1;
    }
    if (Str_EqualNoCase(name, D_80100730)) { /* "brakes" */
        return 2;
    }
    if (Str_EqualNoCase(name, D_80100738)) { /* "handling" */
        return 3;
    }
    if (Str_EqualNoCase(name, D_80100744)) { /* "gyro" */
        return 4;
    }
    if (Str_EqualNoCase(name, D_8010074C)) { /* "sticky" */
        return 5;
    }
    if (Str_EqualNoCase(name, D_80100720)) { /* "random" */
        return 6;
    }
    if (Str_EqualNoCase(name, D_80100754)) { /* "stealth" */
        return 7;
    }
    if (Str_EqualNoCase(name, D_8010075C)) { /* "boing" */
        return 8;
    }
    if (Str_EqualNoCase(name, D_80100764)) { /* "steel" */
        return 9;
    }
    if (Str_EqualNoCase(name, D_8010076C)) { /* "rubber" */
        return 10;
    }
    if (Str_EqualNoCase(name, D_80100774)) { /* "points" */
        return 12;
    }
    if (Str_EqualNoCase(name, D_8010077C)) { /* "4x4" */
        return 11;
    }
    return 0;
}

INCLUDE_ASM("asm/nonmatchings/unit_28C10", Pickup_Init);

#include "race.h"
extern u8 D_800F3028;
void Coll_Unregister(void *);
void CollShape_Free(void *);
void RefList_Free(Lnk89_ **);

static inline void SetBit64(u32 *words, s16 bit) {
    if (bit >= 0) {
        if (bit < 32) {
            words[0] |= 1 << bit;
        } else if (bit < 64) {
            words[1] |= 1 << (bit - 32);
        }
    }
}

u8 Pickup_Shutdown(RaceResult *res) {
    u32 i;
    Lnk89_ ***pp;
    Lnk89_ *n;
    void *x;

    if (D_800F3028 != 0) {
        D_800F3028 = 0;
        SetBit64((u32 *)((u8 *)res + 4), (s16)D_800F302E);
        SetBit64((u32 *)((u8 *)res + 4), (s16)D_800F3032);
        SetBit64((u32 *)((u8 *)res + 0xC), (s16)D_800F3030);
        SetBit64((u32 *)((u8 *)res + 0xC), (s16)D_800F3034);

        if (gPickupSpawns != NULL) {
            for (i = 0; i < gPickupSpawnCount; i++) {
                Coll_Unregister(*(void **)((u8 *)&gPickupSpawns[i] + 0x14));
                CollShape_Free((u8 *)*(void **)((u8 *)&gPickupSpawns[i] + 0x14) + 0xC);
                if (gPickupSpawns[i].pad0 != 0) {
                    Memory_Free((void *)gPickupSpawns[i].pad0);
                    Memory_Free(*(void **)((u8 *)&gPickupSpawns[i] + 0x14));
                    gPickupSpawns[i].pad0 = 0;
                    *(void **)((u8 *)&gPickupSpawns[i] + 0x14) = NULL;
                }
            }
            Memory_Free(gPickupSpawns);
            gPickupSpawns = NULL;
        }

        pp = gCarPickups;
        do {
            /* TODO(match-debt): pinning p to a0 keeps the original allocation (49 words differ without it). */
            register Lnk89_ **p asm("$4") = *pp;
            if (p != NULL) {
                for (n = *p; n != NULL; ) {
                    x = n->obj;
                    n = n->next;
                    Memory_Free(x);
                }
                RefList_Free(*pp);
                *pp = NULL;
            }
            pp++;
        } while (pp < gCarPickups + 6);

        for (n = *D_80118488; n != NULL; ) {
            x = n->obj;
            n = n->next;
            Memory_Free(x);
        }
        RefList_Free(D_80118488);
    }
    return 1;
}

extern u8 D_80100780[];
extern u8 D_80100788[];
char *strcpy(char *, const char *);

static inline void Pickup_PickRandomName(u8 *dst) {
    Lnk89_ *m;
    s32 cnt;
    s32 k;
    s32 r;

    cnt = 0;
    for (m = *D_80118488; m != NULL; m = m->next) {
        cnt++;
    }
    r = ((s32 (*)(s32))svcRandRange)(cnt);
    for (k = 0, m = *D_80118488; m != NULL; m = m->next, k++) {
        u8 *name = (u8 *)m->obj;

        if (k == r) {
            strcpy((char *)dst, (char *)name);
            break;
        }
    }
}

u8 *Pickup_PickDef(u8 *pickup) {
    u8 **req = (u8 **)pickup;
    u8 name[0x50];
    Lnk89_ *n;

    if (Str_EqualNoCase(*req, D_80100720)) {
        do {
            Pickup_PickRandomName(name);
        } while (Str_EqualNoCase(name, D_80100780) || Str_EqualNoCase(name, D_80100788));
    } else {
        strcpy((char *)name, (char *)*req);
    }
    for (n = *D_80118488; n != NULL; n = n->next) {
        u8 *def = (u8 *)n->obj;

        if (Str_EqualNoCase(name, def)) {
            return def;
        }
    }
    return NULL;
}

void Pickup_ClearCar(u8 *o) {
    Lnk89_ *n = *gCarPickups[*o];
    s32 *x;

    while (n != NULL) {
        x = n->obj;
        n = n->next;
        Car_RemovePickup((void *)*x, o);
        RefList_Release(gCarPickups[*o], x);
        Memory_Free(x);
    }
}


s32 Pickup_GetHudIcon(u8 *name) {
    /* "turbo" is tested twice; the second string was probably meant to be another name. */
    if (Str_EqualNoCase(name, D_80100728) || Str_EqualNoCase(name, D_80100728)) { /* "turbo" */
        return 1;
    }
    if (Str_EqualNoCase(name, D_80100730)) { /* "brakes" */
        return 2;
    }
    if (Str_EqualNoCase(name, D_80100738)) { /* "handling" */
        return 3;
    }
    if (Str_EqualNoCase(name, D_80100744)) { /* "gyro" */
        return 4;
    }
    if (Str_EqualNoCase(name, D_8010074C)) { /* "sticky" */
        return 5;
    }
    if (Str_EqualNoCase(name, D_80100720)) { /* "random" */
        return 6;
    }
    if (Str_EqualNoCase(name, D_80100754)) { /* "stealth" */
        return 7;
    }
    if (Str_EqualNoCase(name, D_8010075C)) { /* "boing" */
        return 8;
    }
    if (Str_EqualNoCase(name, D_80100764)) { /* "steel" */
        return 9;
    }
    if (Str_EqualNoCase(name, D_8010076C)) { /* "rubber" */
        return 10;
    }
    if (Str_EqualNoCase(name, D_80100774)) { /* "points" */
        return 12;
    }
    if (Str_EqualNoCase(name, D_8010077C)) { /* "4x4" */
        return 11;
    }
    return 0;
}

s32 Pickup_Collect(u8 *o, u8 *q) {
    u8 *d;
    Tmr89_ *t;
    Lnk89_ *l;
    Lnk89_ *m;
    s32 n;
    Tmr89_ *nt;
    s32 r;

    if (q[0x10] == 0) {
        return 0;
    }
    d = Pickup_PickDef(q);
    if (d == NULL) {
        return 0;
    }

    l = *gCarPickups[*o];
    while (l != NULL) {
        t = (Tmr89_ *) l->obj;
        l = l->next;
        Car_RemovePickup(t->def, o);
        RefList_Release(gCarPickups[*o], (s32 *) t);
        Memory_Free(t);
    }

    m = *gCarPickups[*o];
    n = 0;
    while (m != NULL) {
        n += (((Tmr89_ *) m->obj)->hold != 0);
        m = m->next;
    }

    if (n == 0) {
        nt = Memory_Alloc(0xC);
        nt->t = RaceClock_Get();
        nt->def = d;
        nt->hold = d[0xAC];
        nt->pad8 = *(s32 *) (d + 0x30);
        if (d[0xAC] != 0) {
            m = *gCarPickups[*o];
            while (m != NULL) {
                t = (Tmr89_ *) m->obj;
                m = m->next;
                if (t->hold != 0) {
                    Car_RemovePickup(t->def, o);
                    RefList_Release(gCarPickups[*o], (s32 *) t);
                    Memory_Free(t);
                    break;
                }
            }
        }
        RefList_Add(gCarPickups[*o], (s32 *) nt);
        if (nt->hold == 0) {
            Car_ApplyPickup(d, Car_Get(*o));
        }
        svcTrackSetEntityVisible(q[4], 0, 0.0f);
        r = Pickup_GetHudIcon(d);
        *(s32 *) (o + 0x6DC) = r;
        if (r == 1) {
            *(s32 *) (o + 0x6DC) = 0;
        }
        if (*(s32 *) (o + 4) & 3) {
            audSfxPlay(0x18, 0, 1.0f, 1.0f);
        }
    }
    *(s32 *) (q + 0xC) = RaceClock_Get();
    q[0x10] = 0;
    return 1;
}

void Pickup_Update(void) {
    s32 now;
    u32 i;
    s32 n;
    Lnk89_ *l;
    Lnk89_ *next;
    Tmr89_ *t;
    Car *p;

    now = RaceClock_Get();
    for (i = 0; i < 6; i++) {
        p = Car_Get(i);
        l = *gCarPickups[i];
        n = 0;
        while (l != NULL) {
            t = (Tmr89_ *) l->obj;
            next = l->next;
            n++;
            if (t->hold == 1) {
                t->t = RaceClock_Get();
            } else if ((u32) (now - t->t) > (u32) (*(s32 *) (t->def + 0x2C) * 1000)) {
                Car_RemovePickup(t->def, p);
                RefList_Release(gCarPickups[i], (s32 *) t);
                Memory_Free(t);
            }
            l = next;
        }
        if (n == 0) {
            p->unk6DC = 0;
        }
    }
    for (i = 0; i < gPickupSpawnCount; i++) {
        if (gPickupSpawns[i].fired == 0 && (u32) (now - gPickupSpawns[i].t) > 10000 && gPickupSpawns[i].on != 0) {
            gPickupSpawns[i].fired = 1;
            svcTrackSetEntityVisible(gPickupSpawns[i].id, 1, 1.0f);
        }
    }
}

INCLUDE_ASM("asm/nonmatchings/unit_28C10", Trick_Evaluate);

void Trick_GetName(u16 i, char *out) {
    if (i < 49) {
        sprintf(out, D_8010080C, gLangText[D_800F316C[i].w[svcRandRange(D_800F316C[i].b)]]);
    }
}

