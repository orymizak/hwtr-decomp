#include "common.h"
#include "include_asm.h"
typedef struct PNode_ { struct PNode_ *next; } PNode_;
typedef struct { u8 p0[0x48]; s32 unk48; s32 unk4C; s32 unk50; u8 p54[6]; s16 unk5A; u8 p5C[0x18]; s32 unk74; s32 unk78; PNode_ *unk7C; PNode_ *unk80; s32 unk84; } ObjF_;
void func_800D39F8(ObjF_ *e, s32 id, void *param);
typedef struct { u32 w0; u32 w1; } AcmdF_;
typedef struct { u8 p0[0x14]; s32 sourceCount; s32 maxSources; void **sources; } BusF_;
typedef struct { u8 p0[0x34]; BusF_ *unk34; } DrvG_;
extern DrvG_ *D_800FB174;
AcmdF_ *func_800D2BE0(void *src, s32 a, AcmdF_ *p);
s32 alHeapDBAlloc(s32, s32, void *, s32, s32);



AcmdF_ *func_800D2620(s32 a, AcmdF_ *p) {
    BusF_ *m = D_800FB174->unk34;
    void **sources = m->sources;
    s32 i;

    { AcmdF_ *_a = p++; _a->w0 = 0x020007C0; _a->w1 = 0x2E0; }
    for (i = 0; i < m->sourceCount; i++) {
        p = func_800D2BE0(sources[i], a, p);
    }
    return p;
}


INCLUDE_ASM("asm/nonmatchings/unit_73220", func_800D26C0);

void func_800D2AAC(s32 arg0, void *arg1, s32 arg2) {
    (*(s32 *)((u8 *)(arg0) + 0xC)) = alHeapDBAlloc(0, 0, (void *) arg2, 1, 0x20);
    (*(s32 *)((u8 *)(arg0) + 0x10)) = alHeapDBAlloc(0, 0, (void *) arg2, 1, 0x20);
    (*(s32 *)((u8 *)(arg0) + 0x28)) = ((s32 (*)(s32)) arg1)(arg0 + 0x2C);
    (*(s32 *)((u8 *)(arg0) + 0x34)) = 0;
    (*(s32 *)((u8 *)(arg0) + 0x38)) = 1;
    (*(s32 *)((u8 *)(arg0) + 0x3C)) = 0;
    (*(s32 *)((u8 *)(arg0) + 0x40)) = alHeapDBAlloc(0, 0, (void *) arg2, 1, 0x20);
    (*(s32 *)((u8 *)(arg0) + 0x4C)) = 0;
    (*(s32 *)((u8 *)(arg0) + 0x50)) = 1;
    (*(s32 *)((u8 *)(arg0) + 0x48)) = 0;
    (*(f32 *)((u8 *)(arg0) + 0x44)) = 1.0f;
    (*(s32 *)((u8 *)(arg0) + 0x54)) = alHeapDBAlloc(0, 0, (void *) arg2, 1, 0x50);
    (*(s32 *)((u8 *)(arg0) + 0x78)) = 1;
    (*(s32 *)((u8 *)(arg0) + 0x84)) = 0;
    (*(s16 *)((u8 *)(arg0) + 0x5A)) = 1;
    (*(s16 *)((u8 *)(arg0) + 0x68)) = 1;
    (*(s16 *)((u8 *)(arg0) + 0x6E)) = 1;
    (*(s16 *)((u8 *)(arg0) + 0x5C)) = 1;
    (*(s16 *)((u8 *)(arg0) + 0x5E)) = 1;
    (*(s16 *)((u8 *)(arg0) + 0x60)) = 0;
    (*(s16 *)((u8 *)(arg0) + 0x62)) = 0;
    (*(s16 *)((u8 *)(arg0) + 0x66)) = 1;
    (*(s16 *)((u8 *)(arg0) + 0x64)) = 0;
    (*(s16 *)((u8 *)(arg0) + 0x66)) = 1;
    (*(s16 *)((u8 *)(arg0) + 0x64)) = 0;
    (*(s32 *)((u8 *)(arg0) + 0x70)) = 0;
    (*(s32 *)((u8 *)(arg0) + 0x74)) = 0;
    (*(s16 *)((u8 *)(arg0) + 0x58)) = 0;
    (*(s32 *)((u8 *)(arg0) + 0x7C)) = 0;
    (*(s32 *)((u8 *)(arg0) + 0x80)) = 0;
}

INCLUDE_ASM("asm/nonmatchings/unit_73220", func_800D2BE0);

s32 func_800D3158(ObjF_ *e, s32 id, void *param) {
    switch (id) {
        case 3:
            if (e->unk80 != 0) {
                e->unk80->next = param;
            } else {
                e->unk7C = param;
            }
            e->unk80 = param;
            break;
        case 4:
            e->unk78 = 1;
            e->unk84 = 0;
            e->unk5A = 1;
            e->unk74 = 0;
            e->unk4C = 0;
            e->unk50 = 1;
            e->unk48 = 0;
            func_800D39F8(e, 4, param);
            break;
        case 9:
            e->unk84 = 1;
            break;
        default:
            func_800D39F8(e, id, param);
            break;
    }
    return 0;
}


INCLUDE_ASM("asm/nonmatchings/unit_73220", func_800D31FC);

INCLUDE_ASM("asm/nonmatchings/unit_73220", func_800D341C);

s32 func_800D355C(s32 arg0, s32 arg1, s16 arg2, s32 arg3) {
    s32 temp_a1;
    s32 var_v0;

    temp_a1 = arg1 >> 3;
    if (temp_a1 != 0) {
        var_v0 = (arg0 + (((s32) ((arg3 & 0xFFFF) * temp_a1) >> 0x10) + (arg2 * temp_a1))) << 0x10;
    } else {
        var_v0 = arg0 << 0x10;
    }
    return var_v0 >> 0x10;
}
