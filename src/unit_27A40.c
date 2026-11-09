#include "common.h"
#include "include_asm.h"
typedef struct { u8 ch; s8 k; } Kern2_;
typedef struct { Kern2_ *list; u8 n; u8 ch; u8 has; u8 pad; } Kern1_;
typedef struct { Kern1_ *tbl; u16 n; u8 pad[2]; } KernFont_;
extern KernFont_ D_800F2800[];
extern s32 D_800F2808;
extern u8 D_80140F90;
extern u8 D_80145568;
extern u8 D_8014C0A8;
extern u8 D_80141698;
u8 func_8007EBF8(void);
u8 func_8006B4F8(void);
u8 func_800610DC(void);
u8 func_80060458(void);
s32 func_80060840();
s32 func_80061334();
s32 func_8006B630();
s32 func_8007EF30();



INCLUDE_ASM("asm/nonmatchings/unit_27A40", func_80086E40);

void func_80086F78(void) {

}

s8 func_80086F80(u8 font, u8 a, u8 b) {
    s16 lo;
    s16 hi;
    s16 mid;
    s16 f1;
    s16 f2;
    Kern1_ *t;
    s8 r;

    lo = 0;
    f1 = 0xFF;
    f2 = 0xFF;
    r = 0;
    if (D_800F2808 < font) {
        return -0x80;
    }
    hi = D_800F2800[font].n;
    t = D_800F2800[font].tbl;
    while (lo <= hi) {
        mid = (lo + hi) / 2;
        if (a == t[mid].ch) {
            f1 = mid;
            break;
        }
        if (t[mid].ch < a) {
            lo = mid + 1;
        } else {
            hi = mid - 1;
        }
    }
    if (f1 != 0xFF) {
        if (t[f1].has == 0) {
            return r;
        }
        lo = 0;
        hi = t[f1].n;
        while (lo <= hi) {
            mid = (lo + hi) / 2;
            if (b == t[f1].list[mid].ch) {
                f2 = mid;
                break;
            }
            if (t[f1].list[mid].ch < b) {
                lo = mid + 1;
            } else {
                hi = mid - 1;
            }
        }
        if (f2 != 0xFF) {
            return t[f1].list[f2].k;
        }
    }
    return r;
}

INCLUDE_ASM("asm/nonmatchings/unit_27A40", func_8008711C);

INCLUDE_ASM("asm/nonmatchings/unit_27A40", func_80087260);

void func_8008744C(void *arg0) {
    (*(f32 *)((u8 *)(arg0) + 0x18)) = (f32) (*(s32 *)((u8 *)(arg0) + 0x14));
}

INCLUDE_ASM("asm/nonmatchings/unit_27A40", func_8008745C);

void func_800876D4(void **tbl, u32 size);
s32 func_800875A0(void) {
    func_800876D4((void **)&D_80140F90, 0xFC);
    if (func_8007EBF8() == 0) {
        return 0;
    }
    func_800876D4((void **)&D_80145568, 0x4C);
    if (func_8006B4F8() != 0) {
        func_800876D4((void **)&D_8014C0A8, 0xAC);
        if (func_800610DC() != 0) {
            func_800876D4((void **)&D_80141698, 0x124);
            if (func_80060458() != 0) {
                return 1;
            }
            func_80061334();
        }
        func_8006B630();
    }
    func_8007EF30();
    return 0;
}


s32 func_80087660(void) {
    s8 var_s0;
    register s32 temp_s1;

    temp_s1 = (func_80060840() & 0xFF) == 0;
    var_s0 = temp_s1;
    if (!(func_80061334() & 0xFF)) {
        var_s0 = temp_s1 + 1;
    }
    if (!(func_8006B630() & 0xFF)) {
        var_s0 += 1;
    }
    if (!(func_8007EF30() & 0xFF)) {
        var_s0 += 1;
    }
    return (var_s0 & 0xFF) == 0;
}

s32 func_8008770C(void);
void func_800876D4(void **tbl, u32 size) {
    s32 i;
    s32 n = size >> 2;

    for (i = 0; i < n; i++) {
        *tbl++ = func_8008770C;
    }
}


s32 func_8008770C(void) {
    return 0;
}
