#include "common.h"
#include "include_asm.h"
s32 osCartRomInit();
s32 osCreatePiManager(s32, void *, void *,s32);
extern u8 D_8012FD90;
extern u8 D_8012FDA8;
extern s32 D_80148040;

extern s32 D_80140E44;
s32 func_800D6460();
s32 func_800D69E0();
s32 func_800D7170();
s32 func_800D7570();

void func_800D61F0(void) {
    D_80140E44 = 0x80;
}

void func_800D6200(void) {
    osCreatePiManager(0x96, &D_8012FD90, &D_8012FDA8, 0x32);
    D_80148040 = osCartRomInit();
}

INCLUDE_ASM("asm/nonmatchings/unit_76DF0", func_800D6240);

s32 func_800D6300(void) {
    s32 temp_s0;

    temp_s0 = func_800D7170();
    func_800D7570();
    func_800D6460();
    func_800D69E0();
    return temp_s0 & 0xFF;
}
