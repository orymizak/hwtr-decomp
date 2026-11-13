#include "common.h"
#include "include_asm.h"

s32 Memory_Free(s32);
extern s32 D_800F2630;
extern s32 D_801345B4;
extern s32 D_80138B10;
extern s32 D_8014C6FC;

INCLUDE_ASM("asm/nonmatchings/unit_1CC80", func_8007C080);

void func_8007C224(void) {
    if (D_8014C6FC != 0) {
        Memory_Free(D_8014C6FC);
    }
    if (D_80138B10 != 0) {
        Memory_Free(D_80138B10);
    }
    if (D_801345B4 != 0) {
        Memory_Free(D_801345B4);
    }
    if (D_800F2630 != 0) {
        Memory_Free(D_800F2630);
    }
    D_8014C6FC = 0;
    D_80138B10 = 0;
    D_801345B4 = 0;
    D_800F2630 = 0;
}

void func_8007C2B4(void *arg0, s32 arg1, s32 arg2) {
    *(s32 *)((u8 *)arg0 + 0x4) = 0;
    *(s32 *)((u8 *)arg0 + 0x8) = 0;
    *(s32 *)((u8 *)arg0 + 0xC) = arg1;
    *(s32 *)((u8 *)arg0 + 0x0) = arg2;
}

void func_8007C2C8(void *arg0) {
    u32 temp_v1;
    u32 var_a1;
    void *temp_v1_2;

    if (arg0 != NULL) {
        temp_v1 = *(s32 *)((u8 *)arg0 + 0x0);
        var_a1 = *(s32 *)((u8 *)arg0 + 0x8) + 0xF;
        if (var_a1 >= temp_v1) {
            var_a1 -= temp_v1;
        }
        temp_v1_2 = *(s32 *)((u8 *)arg0 + 0xC) + (var_a1 * 0x70);
        if (*(u8 *)((u8 *)temp_v1_2 + 0x66) & 4) {
            *(s16 *)((u8 *)temp_v1_2 + 0x6E) = -0xE00;
        }
    }
}

typedef struct { u8 kind; u8 frames; } PDef_;
typedef struct {
    u8 pad0[0x60];
    PDef_ *def;
    s16 life; u8 flags; u8 r; u8 g; u8 b; u8 kind; u8 frames; s16 h6C; s16 h6E;
} Prt70_;
typedef struct { u32 cap; s32 tail; s32 head; Prt70_ *buf; } PPool_;
extern PDef_ *D_800F2330[];
static inline void Pool_Fix(PPool_ *pool) {
    u32 i;
    Prt70_ *e;

    if (pool != NULL) {
        i = pool->head + 15;
        if (i >= pool->cap) {
            i -= pool->cap;
        }
        e = &pool->buf[i];
        if (e->flags & 4) {
            e->h6E = -0xE00;
        }
    }
}
void func_8007C318(PPool_ *pool, Prt70_ *p) {
    if (pool != NULL) {
        if (!(p->flags & 0x80)) {
            p->def = D_800F2330[p->kind];
            p->kind = p->def->kind;
            p->frames = p->def->frames;
        } else {
            p->frames = 0;
        }
        pool->buf[pool->head] = *p;
        if (++pool->head == pool->cap) {
            pool->head = 0;
        }
        if (pool->head == pool->tail) {
            if (++pool->tail == pool->cap) {
                pool->tail = 0;
            }
        }
        Pool_Fix(pool);
    }
}

INCLUDE_ASM("asm/nonmatchings/unit_1CC80", func_8007C434);

INCLUDE_ASM("asm/nonmatchings/unit_1CC80", func_8007D14C);

INCLUDE_ASM("asm/nonmatchings/unit_1CC80", func_8007D364);

INCLUDE_ASM("asm/nonmatchings/unit_1CC80", func_8007D808);

INCLUDE_ASM("asm/nonmatchings/unit_1CC80", func_8007DB10);

INCLUDE_ASM("asm/nonmatchings/unit_1CC80", func_8007DE54);

INCLUDE_ASM("asm/nonmatchings/unit_1CC80", func_8007E694);

INCLUDE_ASM("asm/nonmatchings/unit_1CC80", func_8007E974);
