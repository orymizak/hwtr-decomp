#include "common.h"
#include "include_asm.h"



extern u32 *D_8014C6C8;
extern u32 D_800F1CB0;
extern u32 D_80138D30;
extern u32 D_8014C648;
void func_800D6030(s32);
void func_800D5DE8(u8);
u32 osViGetCurrentFramebuffer(void);
void func_800D60B0(u32 *fb, u32 num) {
    u32 i;

    D_8014C6C8 = fb;
    D_800F1CB0 = num;
    D_80138D30 = 0;
    D_8014C648 = fb[0];
    func_800D6030(1);
    func_800D5DE8(num);
    if (num > 1) {
        for (i = 0; i < num - 1; i++) {
            if (D_8014C6C8[i] != D_8014C6C8[i + 1]) {
                break;
            }
        }
        if (i == num - 1) {
            func_800D5DE8(1);
        } else {
            for (i = 0; i < num; i++) {
                if (osViGetCurrentFramebuffer() != D_8014C6C8[i]) {
                    D_80138D30 = i;
                    D_8014C648 = D_8014C6C8[i];
                    break;
                }
            }
        }
    }
}
