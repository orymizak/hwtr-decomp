#include "common.h"
#include "include_asm.h"
s32 func_800CE228(s32,s32);
s32 func_800D59E0(void *);
extern s8 D_800FABA0;
extern u8 D_80144450;



void func_800CD930(s16 arg0, s32 arg1) {
    switch (arg0) {
    case 2:
        func_800CE228(2, 0x19);
        func_800CE228(1, 0x19);
        return;
    case 1:
        if (arg1 == 0x1C) {
            D_800FABA0 = 0;
            func_800D59E0(&D_80144450);
        }
        return;
    }
}
