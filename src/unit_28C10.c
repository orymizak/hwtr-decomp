#include "common.h"
#include "include_asm.h"
typedef struct { u8 b; u8 pad[3]; s32 w[6]; } E1C8_;
extern E1C8_ D_800F316C[];
extern u8 (*D_8014169C)(u8);
extern s32 D_801410E0[];
extern char D_8010080C[];
s32 sprintf(char *, const char *, ...);
typedef struct Lnk89_ { s32 *obj; s32 x; struct Lnk89_ *next; } Lnk89_;
extern Lnk89_ **D_80118470[];
void func_800A82F8(Lnk89_ **, s32 *);
void Memory_Free(void *);
s32 func_80095970(void *);
extern s32 (*D_80141768)(u8,s32);



INCLUDE_ASM("asm/nonmatchings/unit_28C10", func_80088010);

void func_80088558(void *arg0, void *arg1) {
    if ((*(s32 *)((u8 *)(arg1) + 0x6D8)) == 2) {
        (*(f32 *)((u8 *)(arg1) + 0x4CC)) = (f32) ((*(f32 *)((u8 *)(arg1) + 0x4CC)) / (*(f32 *)((u8 *)(arg0) + 0xA4)));
        (*(u8 *)((u8 *)(arg1) + 0x4C8)) = (u8) ((*(u8 *)((u8 *)(arg1) + 0x4C8)) - (*(u8 *)((u8 *)(arg0) + 0xA8)));
        (*(u8 *)((u8 *)(arg1) + 0x4C9)) = (u8) ((*(u8 *)((u8 *)(arg1) + 0x4C9)) - (*(u8 *)((u8 *)(arg0) + 0xA9)));
        (*(u8 *)((u8 *)(arg1) + 0x4CA)) = (u8) ((*(u8 *)((u8 *)(arg1) + 0x4CA)) - (*(u8 *)((u8 *)(arg0) + 0xAA)));
        (*(u8 *)((u8 *)(arg1) + 0x4CB)) = (u8) ((*(u8 *)((u8 *)(arg1) + 0x4CB)) - (*(u8 *)((u8 *)(arg0) + 0xAB)));
        (*(f32 *)((u8 *)(arg1) + 0x4D0)) = (f32) ((*(f32 *)((u8 *)(arg1) + 0x4D0)) / (*(f32 *)((u8 *)(arg0) + 0x38)));
        (*(f32 *)((u8 *)(arg1) + 0x4D4)) = (f32) ((*(f32 *)((u8 *)(arg1) + 0x4D4)) / (*(f32 *)((u8 *)(arg0) + 0x3C)));
        (*(f32 *)((u8 *)(arg1) + 0x4D8)) = (f32) ((*(f32 *)((u8 *)(arg1) + 0x4D8)) / (*(f32 *)((u8 *)(arg0) + 0x40)));
        (*(f32 *)((u8 *)(arg1) + 0x4DC)) = (f32) ((*(f32 *)((u8 *)(arg1) + 0x4DC)) / (*(f32 *)((u8 *)(arg0) + 0x44)));
        (*(f32 *)((u8 *)(arg1) + 0x4E0)) = (f32) ((*(f32 *)((u8 *)(arg1) + 0x4E0)) / (*(f32 *)((u8 *)(arg0) + 0x48)));
        (*(f32 *)((u8 *)(arg1) + 0x4E4)) = (f32) ((*(f32 *)((u8 *)(arg1) + 0x4E4)) / (*(f32 *)((u8 *)(arg0) + 0x4C)));
        (*(f32 *)((u8 *)(arg1) + 0x4E8)) = (f32) ((*(f32 *)((u8 *)(arg1) + 0x4E8)) / (*(f32 *)((u8 *)(arg0) + 0x50)));
        (*(f32 *)((u8 *)(arg1) + 0x4EC)) = (f32) ((*(f32 *)((u8 *)(arg1) + 0x4EC)) / (*(f32 *)((u8 *)(arg0) + 0x54)));
        (*(f32 *)((u8 *)(arg1) + 0x4F0)) = (f32) ((*(f32 *)((u8 *)(arg1) + 0x4F0)) / (*(f32 *)((u8 *)(arg0) + 0x58)));
        (*(f32 *)((u8 *)(arg1) + 0x4F4)) = (f32) ((*(f32 *)((u8 *)(arg1) + 0x4F4)) / (*(f32 *)((u8 *)(arg0) + 0x5C)));
        (*(f32 *)((u8 *)(arg1) + 0x4F8)) = (f32) ((*(f32 *)((u8 *)(arg1) + 0x4F8)) / (*(f32 *)((u8 *)(arg0) + 0x6C)));
        (*(f32 *)((u8 *)(arg1) + 0x4FC)) = (f32) ((*(f32 *)((u8 *)(arg1) + 0x4FC)) / (*(f32 *)((u8 *)(arg0) + 0x70)));
        (*(f32 *)((u8 *)(arg1) + 0x504)) = (f32) ((*(f32 *)((u8 *)(arg1) + 0x504)) / (*(f32 *)((u8 *)(arg0) + 0x74)));
        (*(f32 *)((u8 *)(arg1) + 0x508)) = (f32) ((*(f32 *)((u8 *)(arg1) + 0x508)) / (*(f32 *)((u8 *)(arg0) + 0x78)));
        (*(f32 *)((u8 *)(arg1) + 0x510)) = (f32) ((*(f32 *)((u8 *)(arg1) + 0x510)) / (*(f32 *)((u8 *)(arg0) + 0x7C)));
        (*(f32 *)((u8 *)(arg1) + 0x514)) = (f32) ((*(f32 *)((u8 *)(arg1) + 0x514)) / (*(f32 *)((u8 *)(arg0) + 0x80)));
        (*(f32 *)((u8 *)(arg1) + 0x51C)) = (f32) ((*(f32 *)((u8 *)(arg1) + 0x51C)) / (*(f32 *)((u8 *)(arg0) + 0x84)));
        (*(f32 *)((u8 *)(arg1) + 0x520)) = (f32) ((*(f32 *)((u8 *)(arg1) + 0x520)) / (*(f32 *)((u8 *)(arg0) + 0x88)));
        (*(f32 *)((u8 *)(arg1) + 0x528)) = (f32) ((*(f32 *)((u8 *)(arg1) + 0x528)) / (*(f32 *)((u8 *)(arg0) + 0x64)));
        (*(f32 *)((u8 *)(arg1) + 0x530)) = (f32) ((*(f32 *)((u8 *)(arg1) + 0x530)) / (*(f32 *)((u8 *)(arg0) + 0x60)));
        (*(f32 *)((u8 *)(arg1) + 0x52C)) = (f32) ((*(f32 *)((u8 *)(arg1) + 0x52C)) / (*(f32 *)((u8 *)(arg0) + 0x68)));
        (*(f32 *)((u8 *)(arg1) + 0x400)) = (f32) ((*(f32 *)((u8 *)(arg1) + 0x400)) / (*(f32 *)((u8 *)(arg0) + 0x8C)));
        (*(f32 *)((u8 *)(arg1) + 0x404)) = (f32) ((*(f32 *)((u8 *)(arg1) + 0x404)) / (*(f32 *)((u8 *)(arg0) + 0x90)));
        (*(f32 *)((u8 *)(arg1) + 0x42C)) = (f32) ((*(f32 *)((u8 *)(arg1) + 0x42C)) / (*(f32 *)((u8 *)(arg0) + 0x94)));
        if ((*(s32 *)((u8 *)(arg0) + 0x34)) & 8) {
            (*(u8 *)((u8 *)(arg1) + 0x6BD)) = 0;
        }
        if ((*(s32 *)((u8 *)(arg0) + 0x34)) & 0x10) {
            (*(u8 *)((u8 *)(arg1) + 0x6BE)) = 0;
            (*(f32 *)((u8 *)(arg1) + 0x53C)) = (f32) ((*(f32 *)((u8 *)(arg1) + 0x53C)) / 10.0f);
        }
        if ((*(s32 *)((u8 *)(arg0) + 0x34)) & 0x20) {
            (*(u8 *)((u8 *)(arg1) + 0x6BF)) = 0;
        }
        if ((*(s32 *)((u8 *)(arg0) + 0x34)) & 0x40) {
            (*(u8 *)((u8 *)(arg1) + 0x6C0)) = 0;
        }
        if ((*(s32 *)((u8 *)(arg0) + 0x34)) & 1) {
            (*(u8 *)((u8 *)(arg1) + 0x6BC)) = 0;
        }
        if ((*(s32 *)((u8 *)(arg0) + 0x34)) & 2) {
            D_80141768((*(u8 *)((u8 *)(arg1) + 0x0)), 0);
        }
        func_80095970(arg1);
        (*(f32 *)((u8 *)(arg1) + 0x74)) = (f32) (*(f32 *)((u8 *)(arg1) + 0x4D0));
        (*(f32 *)((u8 *)(arg1) + 0x78)) = (f32) (1.0f / (*(f32 *)((u8 *)(arg1) + 0x4D0)));
    }
}

INCLUDE_ASM("asm/nonmatchings/unit_28C10", func_80088810);

INCLUDE_ASM("asm/nonmatchings/unit_28C10", func_800888E0);

INCLUDE_ASM("asm/nonmatchings/unit_28C10", func_80088B50);

INCLUDE_ASM("asm/nonmatchings/unit_28C10", func_8008955C);

INCLUDE_ASM("asm/nonmatchings/unit_28C10", func_80089848);

INCLUDE_ASM("asm/nonmatchings/unit_28C10", func_80089B10);

void func_80089F28(u8 *o) {
    Lnk89_ *n = *D_80118470[*o];
    s32 *x;

    while (n != NULL) {
        x = n->obj;
        n = n->next;
        func_80088558((void *)*x, o);
        func_800A82F8(D_80118470[*o], x);
        Memory_Free(x);
    }
}


INCLUDE_ASM("asm/nonmatchings/unit_28C10", func_80089FC4);

INCLUDE_ASM("asm/nonmatchings/unit_28C10", func_8008AA9C);

INCLUDE_ASM("asm/nonmatchings/unit_28C10", func_8008AD04);

INCLUDE_ASM("asm/nonmatchings/unit_28C10", func_8008AEC0);

void func_8008B9FC(u16 i, char *out) {
    if (i < 49) {
        sprintf(out, D_8010080C, D_801410E0[D_800F316C[i].w[D_8014169C(D_800F316C[i].b)]]);
    }
}

