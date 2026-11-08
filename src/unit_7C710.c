#include "common.h"
#include "include_asm.h"

s32 alSynNew();
extern s32 D_800FB5B0;
s32 alSynDelete();

INCLUDE_ASM("asm/nonmatchings/unit_7C710", alSavePull);

INCLUDE_ASM("asm/nonmatchings/unit_7C710", alSaveParam);

void alInit(s32 arg0) {
    if (D_800FB5B0 == 0) {
        D_800FB5B0 = arg0;
        alSynNew();
    }
}

void alClose(void) {
    if (D_800FB5B0 != 0) {
        alSynDelete();
        D_800FB5B0 = 0;
    }
}

INCLUDE_ASM("asm/nonmatchings/unit_7C710", alLink);

void alUnlink(void *arg0) {
    void **temp_v1_2;
    void *temp_v1;

    temp_v1 = *(s32 *)((u8 *)arg0 + 0x0);
    if (temp_v1 != NULL) {
        *(s32 *)((u8 *)temp_v1 + 0x4) = (void **) *(s32 *)((u8 *)arg0 + 0x4);
    }
    temp_v1_2 = *(s32 *)((u8 *)arg0 + 0x4);
    if (temp_v1_2 != NULL) {
        *temp_v1_2 = *(s32 *)((u8 *)arg0 + 0x0);
    }
}
