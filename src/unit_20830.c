#include "common.h"
#include "include_asm.h"
typedef struct { u8 p0[8]; s32 n; u8 pC[4]; s32 a[600]; s32 b[600]; f32 c[600]; } Queue7F_;
typedef struct { u8 pad[4]; s32 a; s32 b; void *c; } Buf20_;
void SortQueue_Sort(void *);
void SortQueue_Draw(void *);
extern s32 gSortQueueIndex;

typedef struct { u32 w0; s32 w4; s32 w8; void *wC; } Hp2C0_;
typedef struct { Hp2C0_ *a; Hp2C0_ *b; } Hp2_;
typedef struct { u16 n; u8 p2[6]; s16 *v; u8 pC[4]; u8 *p10; } MeshFC_;
extern Hp2_ gSortQueues;
extern u8 D_8010C420[];

void SortQueue_AddMeshTo(void *arg0, MeshFC_ *m, s32 v, f32 *mtx) {
    s32 j;
    s32 i;
    s16 *s;

    if ((u32)(m->n + *(s32 *)((u8 *)arg0 + 8)) < 0x257U) {
        j = *(s32 *)((u8 *)arg0 + 8);
        for (i = 0; i < m->n; j++, i++) {
            *(u8 **)((u8 *)arg0 + j * 4 + 0x10) = m->p10 + i * 12;
            *(s32 *)((u8 *)arg0 + j * 4 + 0x970) = v;
            *(f32 *)((u8 *)arg0 + j * 4 + 0x12D0) = mtx[8] * m->v[i * 3] + mtx[9] * m->v[i * 3 + 1] + mtx[10] * m->v[i * 3 + 2] + mtx[11];
        }
        *(s32 *)((u8 *)arg0 + 8) = j;
    }
}

void SortQueue_AddTo(void *arg0, s32 a, s32 b, f32 c) {
    Queue7F_ *q = arg0;
    s32 n = q->n;

    if ((u32)(n + 1) < 599) {
        q->a[n] = a;
        q->b[n] = b;
        q->c[n] = c;
        q->n++;
    }
}

INCLUDE_ASM("asm/nonmatchings/unit_20830", SortQueue_Sort);

typedef struct { s32 f0; s32 f4; u8 p8[0x208]; s32 f210; s32 f214; u8 p218[0xFC]; s32 f314; s32 f318; u8 p31C[0x4]; } S_D_8014C740;
typedef struct { u32 w0; u32 w1; } G9_;
typedef struct { s32 id; s32 a; s32 b; } Dl80_;
typedef struct { u8 pad0[4]; u32 start; u32 end; G9_ *buf; Dl80_ *dl[600]; s32 mtx[1]; } Q80_;
extern S_D_8014C740 gGfxPools[];
extern s32 gGfxPoolIndex;
extern s32 gDisplayListHead;
void GfxPool_Grow(void *, u32);
void SortQueue_Draw(void *arg) {
    Q80_ *q = arg;
    G9_ *g;
    s32 mtx;
    s32 dl;
    u32 i;

    dl = -1;
    g = q->buf;
    mtx = 0;
    if ((u32) (gDisplayListHead + 8) >= (u32) gGfxPools[gGfxPoolIndex].f4) {
        gGfxPools[gGfxPoolIndex].f0 = gDisplayListHead;
        GfxPool_Grow(&gGfxPools[gGfxPoolIndex], 1U);
        gDisplayListHead = gGfxPools[gGfxPoolIndex].f0;
    }
    ((G9_ *) gDisplayListHead)->w0 = 0xDE000000;
    ((G9_ *) gDisplayListHead)->w1 = (u32) g - 0x80000000;
    {
        G9_ *_g = g++;
        _g->w0 = 0xE7000000;
        _g->w1 = 0;
    }
    {
        G9_ *_g = g++;
        _g->w0 = 0xE2001D00;
        _g->w1 = 0;
    }
    {
        G9_ *_g = g++;
        _g->w0 = 0xE3000C00;
        _g->w1 = 0x80000;
    }
    {
        G9_ *_g = g++;
        _g->w0 = 0xE3000A01;
        _g->w1 = 0x100000;
    }
    {
        G9_ *_g = g++;
        _g->w0 = 0xD9000000;
        _g->w1 = 0;
    }
    {
        G9_ *_g = g++;
        _g->w0 = 0xD9FFFFFF;
        _g->w1 = 0x210005;
    }
    gDisplayListHead += 8;
    for (i = q->start; i < q->end; i++) {
        if (q->mtx[i] != mtx) {
            mtx = q->mtx[i];
            {
                G9_ *_g = g++;
                _g->w0 = 0xDA380003;
                _g->w1 = mtx - 0x80000000;
            }
        }
        if (q->dl[i]->id != dl) {
            dl = q->dl[i]->id;
            {
                G9_ *_g = g++;
                _g->w0 = 0xDE000000;
                _g->w1 = q->dl[i]->a - 0x80000000;
            }
        } else {
            G9_ *_g = g++;
            _g->w0 = 0xDE000000;
            _g->w1 = q->dl[i]->b - 0x80000000;
        }
    }
    {
        G9_ *_g = g++;
        _g->w0 = 0xDF000000;
        _g->w1 = 0;
    }
    q->buf = g;
    q->start = q->end;
}

void SortQueue_Init(void) {
    gSortQueues.a = (Hp2C0_ *)D_8010C420;
    gSortQueues.b = (Hp2C0_ *)(D_8010C420 + 0x41F0);
    gSortQueues.a->w8 = 0;
    gSortQueues.b->w8 = 0;
    gSortQueues.a->w4 = 0;
    gSortQueues.b->w4 = 0;
    gSortQueues.a->wC = (u8 *)gSortQueues.a + 0x1C30;
    gSortQueues.b->wC = (u8 *)gSortQueues.b + 0x1C30;
    gSortQueues.a->w0 = 0xA87BC3E8;
    gSortQueues.b->w0 = 0xA87BC3E8;
    gSortQueueIndex = 0;
}

void SortQueue_Shutdown(void) {
    gSortQueues.a = 0;
    gSortQueues.b = 0;
}

/* Queues the mesh vertices with their view depth in the current queue. */
void SortQueue_AddMesh(MeshFC_ *m, s32 v, f32 *mtx) {
    Queue7F_ *q = ((Queue7F_ **)&gSortQueues)[gSortQueueIndex];
    s32 j;
    s32 i;

    if ((u32)(m->n + q->n) < 0x257U) {
        j = q->n;
        for (i = 0; i < m->n; j++, i++) {
            q->a[j] = (s32)(m->p10 + i * 12);
            q->b[j] = v;
            q->c[j] = mtx[8] * m->v[i * 3] + mtx[9] * m->v[i * 3 + 1] + mtx[10] * m->v[i * 3 + 2] + mtx[11];
        }
        q->n = j;
    }
}

void SortQueue_Add(s32 a, s32 b, f32 c) {
    Queue7F_ *q = ((Queue7F_ **)&gSortQueues)[gSortQueueIndex];
    s32 n = q->n;

    if ((u32)(n + 1) < 599) {
        q->a[n] = a;
        q->b[n] = b;
        q->c[n] = c;
        q->n++;
    }
}

void SortQueue_Flush(void) {
    SortQueue_Sort(*(s32 *)(&(*(u8 *)&gSortQueues) + (gSortQueueIndex * 4)));
    SortQueue_Draw(*(s32 *)(&(*(u8 *)&gSortQueues) + (gSortQueueIndex * 4)));
}

void SortQueue_Swap(void) {
    s32 i = gSortQueueIndex ^ 1;

    ((Buf20_ * *)&gSortQueues)[i]->a = 0;
    ((Buf20_ * *)&gSortQueues)[i]->b = 0;
    gSortQueueIndex = i;
    ((Buf20_ * *)&gSortQueues)[i]->c = (u8 *)((Buf20_ * *)&gSortQueues)[i] + 0x1C30;
}

