#include "common.h"
#include "include_asm.h"
typedef struct ALFilter_ { struct ALFilter_ *source; void *handler; s32 (*setParam)(void *, s32, void *); } ALFilter_;
typedef struct { u8 pad0[0x18]; void *ctrlList; s32 unk1C; s32 unk20; s32 unk24; u8 pad28[8]; s32 unk30; } ALMix_;



INCLUDE_ASM("asm/nonmatchings/unit_7B750", func_800DAB50);

INCLUDE_ASM("asm/nonmatchings/unit_7B750", func_800DAC90);

INCLUDE_ASM("asm/nonmatchings/unit_7B750", func_800DACC0);

s32 func_800DAEAC(void *filter, s32 param, void *value) {
    ALFilter_ *f = filter;
    ALMix_ *e = filter;

    switch (param) {
    case 1:
        f->source = value;
        break;
    case 4:
        e->unk20 = 0;
        e->unk24 = 1;
        e->unk30 = 0;
        e->unk1C = 0;
        if (f->source != NULL) {
            f->source->setParam(f->source, 4, NULL);
        }
        break;
    case 9:
        e->unk30 = 1;
        if (f->source != NULL) {
            f->source->setParam(f->source, 9, NULL);
        }
        break;
    case 7:
        e->ctrlList = value;
        break;
    case 8:
        e->unk1C = 1;
        break;
    default:
        if (f->source != NULL) {
            f->source->setParam(f->source, param, value);
        }
        break;
    }
    return 0;
}
