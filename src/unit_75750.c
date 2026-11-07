#include "common.h"
#include "include_asm.h"
s32 func_800D3158(void *, u16, s32);
void *func_800D52B8();

extern s32 *D_800FB174;

void func_800D4B50(void) {
    *D_800FB174 = 0;
}

void func_800D4B60(void *arg0, s32 arg1) {
    register void *temp_v0;
    register u8 var_s0;

    var_s0 = arg1;
    if (NULL != (*(s32 *)((u8 *)(arg0) + 0x8))) {
        temp_v0 = func_800D52B8();
        if (temp_v0 != NULL) {
            (*(u32 *)((u8 *)(temp_v0) + 0x4)) = ((*(s32 *)((u8 *)((*(void * *)&D_800FB174)) + 0x1C)) + (*(s32 *)((u8 *)0x88 + ((*(s32 *)((u8 *)(arg0) + 0x8))))));
            (*(u16 *)((u8 *)(temp_v0) + 0x8)) = 0x10;
            if ((var_s0 & 0xFF) >= 0x80) {
                var_s0 = 0x7F;
            }
            (*(u32 *)((u8 *)(temp_v0) + 0xC)) = (0xFF & var_s0);
            (*(s32 *)((u8 *)(temp_v0) + 0x0)) = 0;
            func_800D3158((*(s32 *)((u8 *)(arg0) + 0x8)), 3, temp_v0);
        }
    }
}
