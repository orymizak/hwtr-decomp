#include "common.h"
#include "include_asm.h"
s32 func_800D242C();
s32 func_800CD9EC(void);
void *func_800D2444();

s32 func_800D23FC();

void func_800CD9A0(void) {
    func_800D23FC();
}

    /* TODO (Clean version):
    Replace raw pointer byte offset 0x8 with a structured type:
    typedef struct {
        u8 pad00[8];
        s32 unk08;
    } ContextHeader; // Adapt type name according to func_800D2444 definition
    ContextHeader *s = func_800D2444();
    return s->unk08 - func_800CD9EC();
    */
s32 func_800CD9BC(void) {
    u8 *s = func_800D2444();

    return *(s32 *)(s + 8) - func_800CD9EC();
}

s32 func_800CD9EC(void) {
    return func_800D242C();
}
