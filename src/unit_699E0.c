#include "common.h"
#include "include_asm.h"
extern void (*D_80141004)(s32);
extern void (*D_80141000)(s32, void *);
extern u8 D_80101070;
extern u8 D_80101078;
void func_800C8DE0(void);



extern u8 D_800F64F0[];
extern u8 (*D_80141008)(u8, u8);
s8 func_80086F80(u8 font, u8 a, u8 b);
extern inline u8 Chr_Index(u8 c) {
    u8 i;

    for (i = 0; i < 75; i++) {
        if (D_800F64F0[i] == c) {
            return i;
        }
    }
    return 0xFF;
}
extern inline u8 Chr_At(u8 i) {
    if (i < 75) {
        return D_800F64F0[i];
    }
    return 0;
}
extern inline u8 Chr_Width(u8 font, u8 c) {
    if (c == 0xFF) {
        return 0;
    }
    if (c == ' ') {
        return 5;
    }
    return D_80141008(font, c);
}
s32 func_8008711C(u8 font, u8 a, u8 pct);

/* Passes every character of the table with its width to func_8008711C. */
void func_800C8DE0(void) {
    u8 i;

    for (i = 0; i < 75; i++) {
        func_8008711C(0, D_800F64F0[i], Chr_Width(0, Chr_At(Chr_Index(D_800F64F0[i]))));
    }
}

s32 func_800C8EE4(u8 font, u8 a, u8 b) {
    u8 ia;
    u8 ib;
    s8 k;

    ia = Chr_Index(a);
    ib = Chr_Index(b);
    if (ia == 0xFF || ib == 0xFF) {
        return 0;
    }
    k = func_80086F80(0, a, b);
    return k + Chr_Width(font, Chr_At(ia));
}

u8 func_800C902C(u8 font, u8 c) {
    return Chr_Width(font, Chr_At(Chr_Index(c)));
}

void func_800C90DC(void) {
    void (**fp)(s32, void *) = &D_80141000;

    (*fp)(0, &D_80101070);
    (*fp)(1, &D_80101078);
    func_800C8DE0();
}

void func_800C9130(void) {
    void (**fp)(s32) = &D_80141004;

    (*fp)(0);
    (*fp)(1);
}


/* Returns the character after c in the character table, wrapping around. */
u8 func_800C916C(u8 c) {
    u8 i = Chr_Index(c);

    i++;
    i %= 75;
    return Chr_At(i);
}

/* Returns the character before c in the character table, wrapping around. */
u8 func_800C9200(u8 c) {
    u8 i = Chr_Index(c);

    if (i == 0) {
        i = 74;
    } else {
        i--;
    }
    return Chr_At(i);
}
