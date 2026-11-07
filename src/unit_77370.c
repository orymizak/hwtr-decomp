#include "common.h"
#include "include_asm.h"
void bzero(void *, s32);
void func_800D7930(u8 *, void *, s32);
void func_800D7BF0(u8 *, void *, void *, s32, s32);
s32 func_800D7208(s32, void *);



void func_800D6770(void *arg0) {
    (*(s32 *)((u8 *)(arg0) + 0x8)) = func_800D7208(0x208, arg0);
}

void func_800D67A0(u8 *a0, u8 *a1, u8 *a2, s32 a3, s32 a4) {
    u8 name[0x20];
    u8 ext[4];
    s32 len;
    u8 *p;

    if (*(s32 *)(a0 + 0xC) == 0) {
        *(s32 *)(a0 + 8) = 1;
        return;
    }
    bzero(name, 0x20);
    bzero(ext, 4);
    len = 0;
    if (*a1 != 0) {
        p = a1;
        do {
            p++;
            len++;
        } while (*p != 0);
    }
    func_800D7930(a1, name, len);
    func_800D7930(a2, ext, 1);
    func_800D7BF0(a0, name, ext, a3, a4);
}
