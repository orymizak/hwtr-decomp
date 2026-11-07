#include "common.h"
#include "include_asm.h"
extern u8 D_800FB1B0;
extern u8 D_800FB1B4;



void func_800D6880(void *arg0, void *arg1) {
    bcopy(arg0, &D_800FB1B0, 2);
    bcopy(arg1, &D_800FB1B4, 4);
}
