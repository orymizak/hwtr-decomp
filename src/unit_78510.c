#include "common.h"
#include "include_asm.h"
typedef struct { u8 jis; u8 n64; } JisT_;
extern JisT_ D_800FB260[];
s32 func_800D7208(s32,s32);



void func_800D7910(void) {
    func_800D7208(0x103, 0);
}

void func_800D7930(u8 *src, u8 *dest, u32 len) {
    u32 cnt;
    u32 i;
    u8 c;

    for (cnt = 0; cnt < len; dest++, cnt++, src++) {
        c = *src;
        if (c == 0) {
            break;
        }
        if (c >= 'A' && c <= 'Z') {
            *dest = c - 0x27;
        } else if (c >= 'a' && c <= 'z') {
            *dest = c - 0x47;
        } else if (c >= '0' && c <= '9') {
            *dest = c - 0x20;
        } else if (c >= 0xB1 && c <= 0xDC) {
            if (src[1] == 0xDE) {
                if (c < 0xCA) {
                    *dest = c - 0x3A;
                } else {
                    *dest = c - 0x3F;
                }
                src++;
            } else if (src[1] == 0xDF) {
                *dest = c - 0x3A;
                src++;
            } else {
                *dest = c - 0x61;
            }
        } else {
            for (i = 0; D_800FB260[i].n64 != 0; i++) {
                if (D_800FB260[i].jis == c) {
                    *dest = D_800FB260[i].n64;
                    break;
                }
            }
            if (D_800FB260[i].n64 == 0) {
                *dest = 0xF;
            }
        }
    }
}

void func_800D7A7C(u8 *src, u8 *dest, u32 len) {
    u32 cnt;
    u32 i;
    u8 c;

    for (cnt = 0; cnt < len; dest++, cnt++, src++) {
        c = *src;
        if (c == 0) {
            *dest = c;
        } else if (c >= 0x1A && c <= 0x33) {
            *dest = c + 0x27;
        } else if (c >= 0x10 && c <= 0x19) {
            *dest = c + 0x20;
        } else if (c >= 0x50 && c <= 0x7B) {
            *dest = c + 0x61;
        } else if (c >= 0x7C && c <= 0x8A) {
            *dest = c + 0x3A;
            dest[1] = 0xDE;
            dest++;
        } else if (c >= 0x8B && c <= 0x8F) {
            *dest = c + 0x3F;
            dest[1] = 0xDE;
        } else if (c >= 0x90 && c <= 0x94) {
            *dest = c + 0x3A;
            dest[1] = 0xDF;
        } else {
            for (i = 0; D_800FB260[i].n64 != 0; i++) {
                if (D_800FB260[i].n64 == c) {
                    *dest = D_800FB260[i].jis;
                    break;
                }
            }
            if (D_800FB260[i].n64 == 0) {
                *dest = 0xF;
            }
        }
    }
}
