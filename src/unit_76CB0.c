#include "common.h"
#include "ultra64.h"
#include "include_asm.h"



extern u32 *D_8014C6C8;
extern u32 D_800F1CB0;
extern u32 D_80138D30;
extern u32 D_8014C648;
void nuGfxRetraceWait(s32);
void nuScSetFrameBufferNum(u8);
void nuGfxSetCfb(u32 *fb, u32 num) {
    u32 i;

    D_8014C6C8 = fb;
    D_800F1CB0 = num;
    D_80138D30 = 0;
    D_8014C648 = fb[0];
    nuGfxRetraceWait(1);
    nuScSetFrameBufferNum(num);
    if (num > 1) {
        for (i = 0; i < num - 1; i++) {
            if (D_8014C6C8[i] != D_8014C6C8[i + 1]) {
                break;
            }
        }
        if (i == num - 1) {
            nuScSetFrameBufferNum(1);
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
