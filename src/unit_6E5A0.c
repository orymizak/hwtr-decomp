#include "common.h"
#include "include_asm.h"
s32 func_800D242C();
s32 func_800CD9EC(void);
void *func_800D2444();

s32 func_800D23FC();

void func_800CD9A0(void) {
    func_800D23FC();
}

s32 func_800CD9BC(void) {
    void *temp_s0;

    temp_s0 = func_800D2444();
    return (*(s32 *)((u8 *)(temp_s0) + 0x8)) - func_800CD9EC();
}

s32 func_800CD9EC(void) {
    return func_800D242C();
}
