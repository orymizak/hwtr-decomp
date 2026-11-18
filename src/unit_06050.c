#include "common.h"
#include "ultra64.h"
#include "math.h"
#include "include_asm.h"
typedef struct { s32 a; s32 b; s32 c; } V12_;
typedef struct { s32 a; s32 b; s32 c; s32 d; } V16_;
typedef struct { V16_ q; V12_ v; } E1C_;
typedef struct { s32 a; u16 node; u16 h6; s32 b; s32 c; E1C_ *p; } E14_;
typedef struct { s32 idx; s32 a; s32 b; s32 c; } Rec16_;
typedef struct { Vec3f v; f32 s; u16 node; char name[0xE]; } Ent20_;
extern void *D_80138170;
void func_800734F4(void *);
void func_8006AA34(void);
void func_8006DC28(void);
extern char D_800FE9F0[];
char *strcpy(char *, const char *);
char *strcat(char *, const char *);
s32 File_Load(char *, s32);
extern void *D_80101E84;
void Memory_Free(void *);
typedef struct { s32 a; s32 b; s32 c; s32 d; } Vec4L_;
u8 func_80066A2C(u8 a, s32 b, s32 c);
typedef struct { u16 unk0; s16 x; s16 y; s16 z; s32 unk8; s32 unkC; s32 unk10; s32 unk14; } Node_;
typedef struct { Vec3i p; f32 m[9]; s32 d; f32 e; } Cam_;
typedef struct { f32 m[21]; } Node54_;
typedef struct {
    /* 0x00 */ u8 p0[6];
    /* 0x06 */ u16 n6;
    /* 0x08 */ u8 p8[3];
    /* 0x0B */ u8 bB;
    /* 0x0C */ u16 nC;
    /* 0x0E */ u16 nE;
    /* 0x10 */ u16 n10;
    /* 0x12 */ u8 p12[2];
    /* 0x14 */ Vec3f v14;
    /* 0x20 */ Vec3f v20;
    /* 0x2C */ Node54_ *nodes;
    /* 0x30 */ u8 p30[8];
    /* 0x38 */ Node_ *links;
    /* 0x3C */ u8 p3C[4];
    /* 0x40 */ Cam_ *cams;
    /* 0x44 */ E14_ *paths;
    /* 0x48 */ Ent20_ *ents;
    /* 0x4C */ Rec16_ *recs;
} World_;
typedef struct { s32 d[0x114 / 4]; } Big114_;
typedef struct { s32 d[0x40 / 4]; } Big40_;
typedef struct { u8 p0[9]; u8 b9; } MdlMat_;
typedef struct { u8 p0[0x30]; s32 flags; u8 p34[8]; MdlMat_ *mat; u8 p40[0x44]; s32 flags84; } MdlInst_;
typedef struct {
    /* 0x000 */ u8 p0[0x2C];
    /* 0x02C */ MdlInst_ *inst;
    /* 0x030 */ u8 p30[0xCC];
    /* 0x0FC */ u8 nParts;
    /* 0x0FD */ u8 pFD;
    /* 0x0FE */ s8 id;
    /* 0x0FF */ u8 pFF;
    /* 0x100 */ u8 cur;
    /* 0x101 */ u8 tgt;
    /* 0x102 */ u8 r;
    /* 0x103 */ u8 g;
    /* 0x104 */ u8 b;
    /* 0x105 */ u8 b105;
    /* 0x106 */ u8 p106[7];
    /* 0x10D */ u8 b10D;
    /* 0x10E */ u8 b10E;
    /* 0x10F */ u8 p10F[0x2D];
    /* 0x13C */ Vec4L_ v;
    /* 0x14C */ u8 b14C;
    /* 0x14D */ u8 c[3];
    /* 0x150 */ u8 p150[0x10];
    /* 0x160 */ f32 m160[3][4];
} Model_;
typedef struct { s32 cur[6]; s32 tgt[6]; s32 dt; } Fade65_;
typedef struct { f32 f0; s32 i4; f32 f8; f32 fC; u8 p10[0x30]; Vec3f pos; Vec3f vel; f32 f58; f32 f5C; u8 p60[4]; s16 h64; u8 b66; u8 c[3]; u8 b6A; u8 p6B; u16 s6C; s16 s6E; } Pt94_;
typedef struct { u8 p0[0xC]; f32 x; u8 p10[0xC]; f32 y; u8 p20[0xC]; f32 z; u8 p30[8]; f32 r; } O_9A4;
void func_8007E694(Vec3f, Vec3f);
void func_8007E974(Vec3f, Vec3f);
extern u8 D_80138A6C[];
extern s32 D_80138A90[];
extern u8 D_800F20F6;
void func_8007C318(void *, Pt94_ *);
extern Fade65_ D_80138A78;
extern u8 D_80138AAC;
extern u8 D_80148088;
extern u8 D_80134FC0;
extern u8 D_80145488;
void func_8007D14C(void *);
void func_8006DAC4(s32);
void func_800738C0(void *, s32);
void func_80075600(s32 arg0, s8 arg1, s8 arg2, s8 arg3);
void func_800756A0(s32);
void func_80075720(s32);
void func_800674DC(void);
s32 rand(void);
void func_8007A858(u8 *, u8 *, s32 *, s32);
void func_8007DE54(Vec3f, u32, u8 *, Vec3f);
s32 File_GetSize(char *);
extern char D_800FE964[];
s32 func_800682A4(s32 unused, char *name, Big114_ *a, Big40_ *b);
extern u8 D_80138150[];
extern u8 D_80138156[];
extern f32 D_8013815C;
extern f32 D_80138160;
extern f32 D_80138164;
extern f32 D_80138168;
extern s32 D_80101E88[];
extern s32 D_800F0218[];
u8 File_PollAsync(s32);
s32 func_80073AC8(s32);
void func_80067A00(u8, u8, f32, f32, f32);
s32 Memory_Alloc();
extern u8 D_80101E80;

extern World_ *D_8013816C;
extern s32 D_8013818C;
s32 GfxPools_Shutdown();
s32 func_800741D8(s32);
extern Model_ *D_80138174[6];
s32 func_80066D80(s32);
s32 Gfx_SetLookAt(s32 *);
extern s32 D_800F2138;

extern u8 D_80138AAD;
extern u8 D_80138AAE;
extern u8 D_80138AAF;
extern u8 D_80138AB0;
extern u8 D_80138AB1;
extern f32 D_80138AB4;
void func_80066504(u8);
void func_80066C9C(s32 a, s32 b);
void func_80075580(void);
void func_80075858(void);
s32 func_80065450(void) {
    void **p = (void **)&D_8013816C;
    void **q = p + 2;
    s32 i;

    *p = NULL;
    D_80138170 = NULL;
    D_8013818C = 0;
    D_80138AAC = 0;
    for (i = 0; i < 6; i++) {
        q[i] = NULL;
        func_80066504(i);
    }
    D_80138AAD = 0;
    D_80138AAE = 0;
    D_80138AAF = 0;
    D_80138AB0 = 0;
    D_80138AB1 = 0;
    D_80138AB4 = 2.0f;
    func_80066C9C(1, 1);
    func_80075580();
    func_80075858();
    return 1;
}

s32 func_80065514(void) {
    if ((*(s32 *)((u8 *)&D_80101E80 + 0x0)) == 0) {
        (*(s32 *)((u8 *)&D_80101E80 + 0x0)) = Memory_Alloc(0x12C00);
    }
    if ((*(s32 *)((u8 *)&D_80101E80 + 0x4)) == 0) {
        (*(s32 *)((u8 *)&D_80101E80 + 0x4)) = Memory_Alloc(0x12C00);
    }
    return ((*(s32 *)((u8 *)&D_80101E80 + 0x0)) == 0) + ((*(s32 *)((u8 *)&D_80101E80 + 0x4)) == 0);
}

s32 func_80065584(void) {
    void **p = (void **)&D_80101E80;

    Memory_Free(*p);
    Memory_Free(D_80101E84);
    *p = NULL;
    D_80101E84 = NULL;
    return 0;
}

typedef struct { u8 p0[4]; char name[0x13]; u8 variant; } TrackName_;
extern char D_800FE944[];
extern char *D_800F01E0[8];
extern u8 D_800F0200[8][3];
s32 strcmp(const char *a, const char *b);
char *strlwr(char *);
s32 func_8006586C(void);
s32 func_8007319C(char *);
World_ *func_800731DC(s32);
void func_800734AC(World_ *);
void func_8006DD34(World_ *);
void func_8006572C(char *, u8);
void func_8006A540(Vec3f *, Vec3f *, u8, u8, u8);

/* Loads "<name><variant>.WLD" and applies the matching per-track RGB color (white if none). */
s32 func_800655CC(TrackName_ *t) {
    char buf[64];
    World_ **wp = &D_8013816C;
    u8 r;
    u8 g;
    u8 b;
    u32 i;

    sprintf(buf, D_800FE944, t->name, t->variant);
    if (*wp != NULL) {
        func_8006586C();
    }
    *wp = func_800731DC(func_8007319C(buf));
    if (D_80138AB0 == 1) {
        func_800734AC(*wp);
    }
    func_8006DD34(*wp);
    b = 255;
    g = 255;
    r = 255;
    func_8006572C(t->name, t->variant);
    for (i = 0; i < 8; i++) {
        strlwr(buf);
        if (strcmp(buf, D_800F01E0[i]) == 0) {
            r = D_800F0200[i][0];
            g = D_800F0200[i][1];
            b = D_800F0200[i][2];
            break;
        }
    }
    func_8006A540(&D_8013816C->v14, &D_8013816C->v20, r, g, b);
    return 1;
}

INCLUDE_ASM("asm/nonmatchings/unit_06050", func_8006572C);

s32 func_8006586C(void) {
    void **p = (void **)&D_8013816C;

    func_800734F4(*p);
    func_800734F4(D_80138170);
    func_8006AA34();
    func_8006DC28();
    *p = NULL;
    D_80138170 = NULL;
    return 1;
}

u16 func_800658C4(void) {
    return D_8013816C->n10;
}

void func_800658D4(s32 i, Vec3f *pos, s32 *a, s32 *b, s32 *c) {
    Rec16_ *r = &D_8013816C->recs[i];
    f32 *m = D_8013816C->nodes[r->idx].m;

    pos->x = m[3];
    pos->y = m[7];
    pos->z = m[11];
    *a = r->a;
    *b = r->b;
    *c = r->c;
}


void func_80065940(s32 dt) {
    u32 i;
    u32 k;
    Model_ *o;
    f32 c;
    f32 t;

    D_80138A78.dt = dt;
    if (dt != 0) {
        func_8007D14C(&D_80148088);
        func_8007D14C(&D_80134FC0);
        func_8007D14C(&D_80145488);
        func_8006DAC4(dt);
        func_800738C0(D_8013816C, dt);
        for (i = 0; i < 6; i++) {
            o = D_80138174[i];
            if (o == NULL) {
                continue;
            }
            func_800738C0(o, dt);
            if (o->b105 < 5) {
                o->b105 = 0;
            } else {
                o->b105 -= 5;
            }
            for (k = 0; k < 12; k++) {
                if (o->b10D < o->b10E) {
                    o->b10D++;
                } else if (o->b10E < o->b10D) {
                    o->b10D--;
                } else {
                    break;
                }
            }
            if (D_80138A78.cur[i] < D_80138A78.tgt[i]) {
                D_80138A78.cur[i] += 15;
                if (D_80138A78.cur[i] > D_80138A78.tgt[i]) {
                    D_80138A78.cur[i] = D_80138A78.tgt[i];
                }
            }
            if (D_80138A78.cur[i] > D_80138A78.tgt[i]) {
                D_80138A78.cur[i] -= 15;
                if (D_80138A78.cur[i] < D_80138A78.tgt[i]) {
                    D_80138A78.cur[i] = D_80138A78.tgt[i];
                }
            }
            if (o->cur != o->tgt) {
                c = o->cur / 255.0f;
                t = o->tgt / 255.0f;
                if (c < t) {
                    c += 0.05f;
                    if (t < c) {
                        c = t;
                    }
                } else {
                    c -= 0.05f;
                    if (c < t) {
                        c = t;
                    }
                }
                o->cur = c * 255.0f;
                if (c != 0.0f) {
                    func_80075600(o->id, c * o->r, c * o->g, c * o->b);
                    func_800756A0(o->id);
                } else {
                    func_80075720(o->id);
                }
            }
        }
        func_800674DC();
        D_80138AAC = 0;
    }
}

INCLUDE_ASM("asm/nonmatchings/unit_06050", func_80065C5C);

void func_800664A4(s32 arg0) {
    D_8013818C = arg0;
}

void func_800664B0(void) {
    u32 i;

    GfxPools_Shutdown();
    for (i = 0; i < 6; i++) {
        if (D_80138174[i] != NULL) {
            func_800741D8((s32)D_80138174[i]);
        }
    }
}

INCLUDE_ASM("asm/nonmatchings/unit_06050", func_80066504);

INCLUDE_ASM("asm/nonmatchings/unit_06050", func_800665A4);

void func_800665D0(u32 color, void *r, void *g, void *b) {
    u32 c = color & 0xFFFF;
    u8 cr = c >> 11;
    u8 cg = (c >> 6) & 0x1F;
    u8 cb = (c >> 1) & 0x1F;

    *(u8 *)r = cr;
    *(u8 *)g = cg;
    *(u8 *)b = cb;
}

INCLUDE_ASM("asm/nonmatchings/unit_06050", func_800665F8);

void func_8006673C(u8 i) {
    D_80138174[i]->b105 = 0;
}

void func_80066758(u8 i) {
    D_80138174[i]->b105 = 0xFF;
}

void func_80066778(u8 i, s8 val) {
    Model_ *m = D_80138174[i];

    if (m != NULL) {
        m->inst->mat->b9 = val;
    }
}

void func_800667A8(u8 i, u8 on) {
    Model_ *m = D_80138174[i];

    if (m != NULL) {
        if (on) {
            m->inst->flags |= 0x40;
            return;
        }
        m->inst->flags &= ~0x40;
    }
}

u8 func_800667F8(u8 a, s32 b) {
    return func_80066A2C(a, b, 0);
}

extern char D_800FE95C[];
s32 File_LoadAsync(char *name, void *dst);
s32 func_8006695C(u8 i);
s32 func_80066B94(u8 i);

/* Starts loading a model file asynchronously, waiting for any previous load in that slot. */
s32 func_8006681C(u8 id, char *name, u8 mode) {
    char path[64];
    u32 i;
    s32 h;

    if (D_800F0218[id] != 0) {
        while (!(u8)func_8006695C(id)) {
        }
    }
    D_80138150[id] = mode ^ 1;
    D_80138156[id] = mode ^ 1;
    strcpy(path, name);
    strcat(path, D_800FE95C);
    if (D_80138174[id] != NULL) {
        func_80066B94(id);
    }
    GfxPools_Shutdown();
    for (i = 0; i < 6; i++) {
        if (D_80138174[i] != NULL) {
            func_800741D8((s32)D_80138174[i]);
        }
    }
    h = File_LoadAsync(path, ((void **)&D_80101E80)[id]);
    D_80101E88[id] = h;
    if (h != -1) {
        D_800F0218[id] = 1;
        return 1;
    }
    return 0;
}

s32 func_8006695C(u8 i) {
    s32 j;

    if (File_PollAsync(D_80101E88[i]) == 0) {
        return 0;
    }
    D_800F0218[i] = 0;
    D_80138174[i] = (Model_ *)func_80073AC8(((s32 *)&D_80101E80)[i]);
    for (j = 0; j < D_80138174[i]->nParts; j++) {
        func_80067A00(i, j, 0.0f, 0.0f, 0.0f);
    }
    return 1;
}


INCLUDE_ASM("asm/nonmatchings/unit_06050", func_80066A2C);

s32 func_80066B94(u8 i) {
    func_800741D8((s32)D_80138174[i]);
    D_80138174[i] = NULL;
    return 1;
}

INCLUDE_ASM("asm/nonmatchings/unit_06050", func_80066BD4);

void func_80066C64(s32 arg0) {
    Gfx_SetLookAt(&D_800F2138);
    func_80066D80(arg0 & 0xFF);
}

void func_80066C9C(s32 a, s32 b) {
    u8 y = 1;
    u8 x = 24;
    f32 *p;
    s32 i;

    if ((a == 0) & (b == 0)) {
        x = 0;
        y = 0;
    }
    for (i = 0; i < 6; i++) {
        D_80138150[i] = x;
        D_80138156[i] = y;
    }
    p = &D_8013815C;
    *p = 700.0f;
    D_80138160 = 1400.0f;
    D_80138164 = 2800.0f;
    D_80138168 = 4800.0f;
    if (a != 1) {
        *p = 490.0f;
        D_80138160 = 980.0f;
        D_80138164 = 1960.0f;
        D_80138168 = 3360.0f;
    } else {
        D_80138156[0] = 0;
    }
}

INCLUDE_ASM("asm/nonmatchings/unit_06050", func_80066D80);

void func_80066ECC(u8 i, u8 *r, u8 *g, u8 *b) {
    Model_ *m = D_80138174[i];

    *r = m->c[0];
    *g = m->c[1];
    *b = m->c[2];
}

INCLUDE_ASM("asm/nonmatchings/unit_06050", func_80066EFC);

INCLUDE_ASM("asm/nonmatchings/unit_06050", func_80067284);

INCLUDE_ASM("asm/nonmatchings/unit_06050", func_80067310);

INCLUDE_ASM("asm/nonmatchings/unit_06050", func_800674DC);

void func_80067950(u8 i, f32 v) {
    u32 j;

    for (j = 0; j < D_80138174[i]->nParts; j++) {
        func_80067A00(i, j, 0.0f, 0.0f, v);
    }
}

INCLUDE_ASM("asm/nonmatchings/unit_06050", func_80067A00);

void func_80067EFC(u8 i, u8 val, Vec4L_ *src) {
    D_80138174[i]->b14C = val;
    D_80138174[i]->v = *src;
}


void func_80067F40(u8 i, f32 v) {
    D_80138174[i]->b10E = v * 240.0f;
}


inline void func_80067FA4(u8 i, f32 v, u8 flag) {
    s32 n;

    if (v == 0.0f) {
        v = 0.2f;
    }
    D_80138A90[i] = v * 100.0f;
    if (!flag) {
        D_80138A90[i - 6] = v * 100.0f;
    }
}

void func_8006810C(u8, u8, u8);
void func_800665F8(u8, u8);
void func_80068020(u8 id, u32 flags) {
    func_8006810C(id, (flags >> 1) & 1, ((flags >> 3) ^ 1) & 1);
    func_80067FA4(id, !(flags & 1) ? 1.0f : 0.0f, ((flags >> 3) ^ 1) & 1);
    func_800665F8(id, (flags >> 2) & 1);
}

INCLUDE_ASM("asm/nonmatchings/unit_06050", func_8006810C);

s32 func_80068270(s32 arg0, s32 arg1, s32 arg2) {
    return func_800682A4(0, arg0, arg1, arg2) & 0xFF;
}

s32 func_800682A4(s32 unused, char *name, Big114_ *a, Big40_ *b) {
    char path[32];
    u8 *buf;

    strcpy(path, name);
    strcat(path, D_800FE964);
    buf = (u8 *)Memory_Alloc(File_GetSize(path));
    File_Load(path, (s32)buf);
    *a = *(Big114_ *)(buf + 4);
    *b = *(Big40_ *)(buf + 0x118);
    Memory_Free(buf);
    return 1;
}


typedef struct { s32 a; s32 b; } Pair68_;
void func_8006DB84(s32 a, f32 b, u8 c);

/* Unless mode is 2, passes the model matrix moved 96 units along its local Y axis to func_8006DB84. */
void func_80068390(u8 slot, u8 mode, Pair68_ unused) {
    f32 m[3][4];

    D_80138A6C[slot] = 1;
    D_80138174[slot]->inst->flags84 &= ~0x40;
    if (mode != 2) {
        m[0][0] = D_80138174[slot]->m160[0][0];
        m[0][1] = D_80138174[slot]->m160[0][1];
        m[0][2] = D_80138174[slot]->m160[0][2];
        m[0][3] = D_80138174[slot]->m160[0][3];
        m[1][0] = D_80138174[slot]->m160[1][0];
        m[1][1] = D_80138174[slot]->m160[1][1];
        m[1][2] = D_80138174[slot]->m160[1][2];
        m[1][3] = D_80138174[slot]->m160[1][3];
        m[2][0] = D_80138174[slot]->m160[2][0];
        m[2][1] = D_80138174[slot]->m160[2][1];
        m[2][2] = D_80138174[slot]->m160[2][2];
        m[2][3] = D_80138174[slot]->m160[2][3];
        m[0][3] += m[0][0] * 0.0f + m[0][1] * 96.0f + m[0][2] * 0.0f;
        m[1][3] += m[1][0] * 0.0f + m[1][1] * 96.0f + m[1][2] * 0.0f;
        m[2][3] += m[2][0] * 0.0f + m[2][1] * 96.0f + m[2][2] * 0.0f;
        func_8006DB84((s32)m, 1.0f, 1);
    }
    D_80138174[slot]->b105 = 0;
}

INCLUDE_ASM("asm/nonmatchings/unit_06050", func_80068504);

INCLUDE_ASM("asm/nonmatchings/unit_06050", func_800685E0);

u16 func_800687D8(void) {
    return D_8013816C->n6;
}

void func_800687E8(s32 i, f32 *pos, f32 *rot, f32 *a3, s32 *a4, s32 *a5, s32 *a6, s32 *a7) {
    Node_ *n = &D_8013816C->links[i];
    f32 *m = D_8013816C->nodes[n->unk0].m;

    rot[0] = m[0];
    rot[1] = m[1];
    rot[2] = m[2];
    rot[3] = m[4];
    rot[4] = m[5];
    rot[5] = m[6];
    rot[6] = m[8];
    rot[7] = m[9];
    rot[8] = m[10];
    pos[0] = m[3];
    pos[1] = m[7];
    pos[2] = m[11];
    a3[0] = n->x;
    a3[1] = n->y;
    a3[2] = n->z;
    *a4 = n->unkC;
    *a5 = n->unk8;
    *a6 = n->unk10;
    *a7 = n->unk14;
}

void func_800688F4(s32 idx, Vec3f pos, f32 *m) {
    f32 *q = D_8013816C->nodes[D_8013816C->links[idx].unk0].m;

    q[0] = m[0];
    q[1] = m[1];
    q[2] = m[2];
    q[4] = m[3];
    q[5] = m[4];
    q[6] = m[5];
    q[8] = m[6];
    q[9] = m[7];
    q[10] = m[8];
    q[3] = pos.x;
    q[7] = pos.y;
    q[11] = pos.z;
}

void func_800689A4(O_9A4 *o, Vec3f v) {
    Vec3f d;
    Vec3f p;
    u32 s;
    f32 len;
    u32 i, n;

    if (o == NULL) {
        return;
    }
    s = o->r / 2.5f;
    v.x /= 14.0f;
    v.y /= 14.0f;
    v.z /= 14.0f;
    len = sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);
    n = (rand() & 7) + 12;
    for (i = 0; i < n; i++) {
        p.x = o->x;
        p.y = o->y;
        p.z = o->z;
        p.x += (f32)((rand() & 0x7F) - 64) / 128.0f * (f32)s;
        p.y += (f32)((rand() & 0x7F) - 64) / 128.0f * (f32)s;
        p.z += (f32)((rand() & 0x7F) - 64) / 128.0f * (f32)s;
        d.x = v.x + ((f32)(rand() & 0x7F) - 64.0f) / 64.0f * len;
        d.y = v.y + ((f32)(rand() & 0x7F) - 64.0f) / 64.0f * len;
        d.z = v.z + ((f32)(rand() & 0x7F) - 64.0f) / 64.0f * len;
        func_8007E694(p, d);
    }
    n = (rand() & 7) + 12;
    v.x /= 3.0f;
    v.y /= 3.0f;
    v.z /= 3.0f;
    for (i = 0; i < n; i++) {
        p.x = o->x;
        p.y = o->y;
        p.z = o->z;
        p.x += (f32)((rand() & 0x7F) - 64) / 128.0f * (f32)s;
        p.y += (f32)((rand() & 0x7F) - 64) / 128.0f * (f32)s;
        p.z += (f32)((rand() & 0x7F) - 64) / 128.0f * (f32)s;
        d.x = v.x + ((f32)(rand() & 0x7F) - 64.0f) / 127.0f * len;
        d.y = v.y + ((f32)(rand() & 0x7F) - 64.0f) / 127.0f * len;
        d.z = v.z + ((f32)(rand() & 0x7F) - 64.0f) / 127.0f * len;
        func_8007E974(p, d);
    }
}

void func_80068E98(u8 *o, Vec3f vel, u8 *verts) {
    Vec3f v;
    Vec3f p;
    s32 idx[16];
    u8 *m;
    u32 n;
    u32 sz;
    u32 i;
    f32 sp;

    if (o == NULL) {
        return;
    }
    m = *(u8 **)(o + 0x3C);
    if (m != NULL) {
        n = *(u16 *)(m + 2) >> 1;
        if (n > 16) {
            n = 16;
        }
        if (n < 6) {
            n = 6;
        }
    } else {
        n = (rand() & 7) + 7;
    }
    func_8007A858(m, verts, idx, 16);
    sz = *(f32 *)(o + 0x38) / 2.5f;
    vel.x /= 14.0f;
    vel.y /= 14.0f;
    vel.z /= 14.0f;
    sp = sqrtf(vel.x * vel.x + vel.y * vel.y + vel.z * vel.z);
    for (i = 0; i < n; i++) {
        p.x = *(f32 *)(o + 0xC);
        p.y = *(f32 *)(o + 0x1C);
        p.z = *(f32 *)(o + 0x2C);
        p.x += ((rand() & 0x7F) - 64) / 128.0f * sz;
        p.y += ((rand() & 0x7F) - 64) / 128.0f * sz;
        p.z += ((rand() & 0x7F) - 64) / 128.0f * sz;
        v.x = vel.x + ((rand() & 0x7F) - 64.0f) / 127.0f * sp;
        v.y = vel.y + ((rand() & 0x7F) - 64.0f) / 127.0f * sp;
        v.z = vel.z + ((rand() & 0x7F) - 64.0f) / 127.0f * sp;
        func_8007DE54(p, sz, verts + idx[rand() & 0xF] * 8, v);
    }
}

INCLUDE_ASM("asm/nonmatchings/unit_06050", func_80069214);

void func_8006936C(Vec3f pos, Vec3f vel) {
    Pt94_ pt;

    pt.pos = pos;
    pt.vel = vel;
    pt.vel.x /= -15.0f;
    pt.vel.y /= -15.0f;
    pt.vel.z = pt.vel.z / -15.0f + 1.0f;
    pt.b66 = 0x20;
    pt.h64 = 1000;
    pt.b6A = 4;
    pt.s6C = 0x8C00;
    pt.i4 = 0;
    pt.f8 = 0.0f;
    pt.fC = 0.0f;
    pt.s6E = -0x200;
    pt.f58 = -3.0f;
    pt.f0 = 4.0f;
    pt.c[2] = (rand() & 0xF) - 0x65;
    pt.c[1] = (rand() & 0xF) - 0x65;
    pt.c[0] = (rand() & 0xF) - 0x65;
    pt.f5C = 100.0f;
    func_8007C318(&D_80145488, &pt);
}


void func_80069494(u8 **car, f32 speed, Vec3f pos, Vec3f vel) {
    Pt94_ pt;
    Model_ *o;
    f32 r;

    if (car == NULL || *car == NULL || *(s32 *)(*car + 0x5C) != 3) {
        return;
    }
    o = D_80138174[**(u8 **)(*car + 0x64)];
    if (o == NULL) {
        return;
    }
    if (speed < 200.0f) {
        return;
    }
    pt.pos = pos;
    pt.vel = vel;
    pt.vel.x /= 15.0f;
    pt.vel.y /= 15.0f;
    pt.vel.z /= 15.0f;
    pt.vel.x += rand() & 3;
    pt.vel.y += rand() & 3;
    pt.vel.z += (rand() & 3) + 20;
    pt.b66 = 0x41;
    pt.i4 = 0;
    pt.f58 = -2.0f;
    pt.f0 = 20.0f;
    pt.f8 = rand();
    pt.fC = ((rand() & 0xFF) - 128.0f) / 1000.0f;
    pt.h64 = 1000;
    pt.b6A = 0;
    pt.s6C = 0xFF00;
    pt.s6E = 0;
    r = (rand() % 21) / 100.0;
    pt.c[2] = (u8)((0.8 - r) * o->c[2]) >> D_800F20F6;
    pt.c[1] = (u8)((0.8 - r) * o->c[1]) >> D_800F20F6;
    pt.c[0] = (u8)((0.8 - r) * o->c[0]) >> D_800F20F6;
    if (D_80138A6C[**(u8 **)(*car + 0x64)] == 1) {
        pt.c[0] >>= 4;
        pt.c[2] >>= 4;
        pt.c[1] >>= 4;
    }
    if (D_80138A90[**(u8 **)(*car + 0x64)] != 100) {
        pt.c[2] >>= 2;
        pt.c[1] >>= 2;
        pt.c[0] >>= 2;
    }
    pt.f5C = 100.0f;
    func_8007C318(&D_80145488, &pt);
}

typedef struct { u8 p0[0x6C]; Vec3f pos; u8 p78[0xC]; Vec3f vel; f32 speed; f32 r[3][3]; } Bds9_;
typedef struct { u8 p0[0x18]; f32 w; f32 h; f32 l; u8 p24[0x3C]; Bds9_ *body; } Cars9_;
typedef struct { Cars9_ *car; Vec3f pos; Vec3f n; u8 flag; } Hits9_;
extern u32 (*D_8014169C)(s32);

extern inline void Sparks9_(Vec3f pos, Vec3f vel) {
    f32 pad[2];
    Pt94_ pt;

    pt.pos = pos;
    pt.vel = vel;
    pt.vel.x /= -15.0f;
    pt.vel.y /= -15.0f;
    pt.vel.z = pt.vel.z / -15.0f + 1.0f;
    pt.b66 = 0x20;
    pt.h64 = 1000;
    pt.b6A = 4;
    pt.s6C = 0x8C00;
    pt.i4 = 0;
    pt.f8 = 0.0f;
    pt.fC = 0.0f;
    pt.s6E = -0x200;
    pt.f58 = -3.0f;
    pt.f0 = 4.0f;
    pt.c[2] = (rand() & 0xF) - 0x65;
    pt.c[1] = (rand() & 0xF) - 0x65;
    pt.c[0] = (rand() & 0xF) - 0x65;
    pt.f5C = 100.0f;
    func_8007C318(&D_80145488, &pt);
}

void func_800698C0(Hits9_ *h, f32 speed) {
    Vec3f t;
    Vec3f v;
    Vec3f ax;
    Vec3f ay;
    Vec3f az;
    Vec3f q;
    Vec3f p;
    Bds9_ *b;
    f32 dx;
    f32 dy;
    f32 dz;
    f32 d;
    f32 k;

    if (h->flag == 1) {
        return;
    }
    b = h->car->body;
    v = b->vel;
    if (v.x * v.x + v.y * v.y + v.z * v.z > 100.0f) {
        d = v.x * h->n.x + v.y * h->n.y + v.z * h->n.z;
        t.x = h->n.x * d;
        t.y = h->n.y * d;
        t.z = h->n.z * d;
        v.x = v.x - t.x;
        v.y = v.y - t.y;
        v.z = v.z - t.z;
        t.x = h->n.x * 10.0f;
        t.y = h->n.y * 10.0f;
        t.z = h->n.z * 60.0f;
        v.x = v.x + t.x;
        v.y = v.y + t.y;
        v.z = v.z + t.z;
        ax.x = b->r[0][0];
        ax.y = b->r[1][0];
        ax.z = b->r[2][0];
        ay.x = b->r[0][1];
        ay.y = b->r[1][1];
        ay.z = b->r[2][1];
        az.x = b->r[0][2];
        az.y = b->r[1][2];
        az.z = b->r[2][2];
        dx = ax.x * h->n.x + ax.y * h->n.y + ax.z * h->n.z;
        dy = ay.x * h->n.x + ay.y * h->n.y + ay.z * h->n.z;
        dz = az.x * h->n.x + az.y * h->n.y + az.z * h->n.z;
        if ((dx < 0.0f ? -dx : dx) > 0.9f) {
            if (dx > 0.0f) {
                t.x = -h->car->w;
                t.y = 0.0f;
                t.z = 0.0f;
            } else {
                t.x = h->car->w;
                t.y = 0.0f;
                t.z = 0.0f;
            }
            k = D_8014169C(2000) * 0.001f + -1.0f;
            t.y += k * h->car->h;
            k = D_8014169C(2000) * 0.001f + -1.0f;
            t.z += k * h->car->l;
            p.x = b->r[0][0] * t.x + b->r[0][1] * t.y + b->r[0][2] * t.z;
            p.y = b->r[1][0] * t.x + b->r[1][1] * t.y + b->r[1][2] * t.z;
            p.z = b->r[2][0] * t.x + b->r[2][1] * t.y + b->r[2][2] * t.z;
            q = p;
            q.x += b->pos.x;
            q.y += b->pos.y;
            q.z += b->pos.z;
        } else if ((dy < 0.0f ? -dy : dy) > 0.9f) {
            if (dy > 0.0f) {
                t.x = 0.0f;
                t.y = -h->car->h;
                t.z = 0.0f;
            } else {
                t.x = 0.0f;
                t.y = h->car->h;
                t.z = 0.0f;
            }
            k = D_8014169C(2000) * 0.001f + -1.0f;
            t.x += k * h->car->w;
            k = D_8014169C(2000) * 0.001f + -1.0f;
            t.z += k * h->car->l;
            p.x = b->r[0][0] * t.x + b->r[0][1] * t.y + b->r[0][2] * t.z;
            p.y = b->r[1][0] * t.x + b->r[1][1] * t.y + b->r[1][2] * t.z;
            p.z = b->r[2][0] * t.x + b->r[2][1] * t.y + b->r[2][2] * t.z;
            q = p;
            q.x += b->pos.x;
            q.y += b->pos.y;
            q.z += b->pos.z;
        } else if ((dz < 0.0f ? -dz : dz) > 0.9f) {
            if (dz > 0.0f) {
                t.x = 0.0f;
                t.y = 0.0f;
                t.z = -h->car->l;
            } else {
                t.x = 0.0f;
                t.y = 0.0f;
                t.z = h->car->l;
            }
            k = D_8014169C(2000) * 0.001f + -1.0f;
            t.x += k * h->car->w;
            k = D_8014169C(2000) * 0.001f + -1.0f;
            t.y += k * h->car->h;
            p.x = b->r[0][0] * t.x + b->r[0][1] * t.y + b->r[0][2] * t.z;
            p.y = b->r[1][0] * t.x + b->r[1][1] * t.y + b->r[1][2] * t.z;
            p.z = b->r[2][0] * t.x + b->r[2][1] * t.y + b->r[2][2] * t.z;
            q = p;
            q.x += b->pos.x;
            q.y += b->pos.y;
            q.z += b->pos.z;
        } else {
            q = h->pos;
        }
        Sparks9_(q, v);
        func_80069494((u8 **)h, speed, q, v);
    }
}

u16 func_8006A1E0(void) {
    return D_8013816C->nC;
}

void func_8006A1F0(s32 i, s32 *a, s32 *h, s32 *b, s32 *c) {
    E14_ *e = &D_8013816C->paths[i];

    *a = e->a;
    *h = e->h6;
    *b = e->b;
    *c = e->c;
}

void func_8006A234(s32 a, s32 b, V12_ *v, V16_ *q) {
    *v = D_8013816C->paths[a].p[b].v;
    *q = D_8013816C->paths[a].p[b].q;
}

void func_8006A2B8(s32 i, Vec3f *t, f32 *m) {
    f32 *d = D_8013816C->nodes[D_8013816C->paths[i].node].m;

    d[0] = m[0];
    d[1] = m[1];
    d[2] = m[2];
    d[3] = t->x;
    d[4] = m[3];
    d[5] = m[4];
    d[6] = m[5];
    d[7] = t->y;
    d[8] = m[6];
    d[9] = m[7];
    d[10] = m[8];
    d[11] = t->z;
}

u8 func_8006A358(void) {
    return D_8013816C->bB;
}

void func_8006A368(u8 i, Vec3i *a1, f32 *a2, s32 *a3, f32 *a4) {
    Cam_ *c = &D_8013816C->cams[i];

    *a1 = c->p;
    *a3 = c->d;
    *a4 = c->e;
    a2[0] = c->m[0];
    a2[1] = c->m[1];
    a2[2] = c->m[2];
    a2[3] = c->m[3];
    a2[4] = c->m[4];
    a2[5] = c->m[5];
    a2[6] = c->m[6];
    a2[7] = c->m[7];
    a2[8] = c->m[8];
}

s32 func_8006A400(char *name, s32 dst) {
    char buf[64];

    strcpy(buf, name);
    strcat(buf, D_800FE9F0);
    File_Load(buf, dst);
    return 1;
}

u16 func_8006A450(void) {
    return D_8013816C->nE;
}

void func_8006A460(s32 i, Vec3f *v, f32 *s, char *name) {
    *v = D_8013816C->ents[i].v;
    *s = D_8013816C->ents[i].s;
    strcpy(name, D_8013816C->ents[i].name);
}

void func_8006A4D4(s32 i, s8 on) {
    s32 node = (s32)&D_8013816C->nodes[D_8013816C->ents[i].node];

    if (!(on & 0xFF)) {
        func_80072D48(node);
        return;
    }
    func_80072D5C(node);
}
