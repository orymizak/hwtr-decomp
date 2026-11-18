#include "common.h"
#include "include_asm.h"
extern void (*sysFontFree)(s32);
extern void (*sysFontLoad)(s32, void *);
extern u8 D_80101070;
extern u8 D_80101078;
void KeyboardChars_SetKerning(void);



extern u8 D_800F64F0[];
extern u8 (*sysFontGetCharWidth)(u8, u8);
s8 Font_GetKerning(u8 font, u8 a, u8 b);
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
    return sysFontGetCharWidth(font, c);
}
s32 Font_ScaleKerning(u8 font, u8 a, u8 pct);

/* Passes every character of the table with its width to Font_ScaleKerning. */
void KeyboardChars_SetKerning(void) {
    u8 i;

    for (i = 0; i < 75; i++) {
        Font_ScaleKerning(0, D_800F64F0[i], Chr_Width(0, Chr_At(Chr_Index(D_800F64F0[i]))));
    }
}

s32 MenuFont_Kerning(u8 font, u8 a, u8 b) {
    u8 ia;
    u8 ib;
    s8 k;

    ia = Chr_Index(a);
    ib = Chr_Index(b);
    if (ia == 0xFF || ib == 0xFF) {
        return 0;
    }
    k = Font_GetKerning(0, a, b);
    return k + Chr_Width(font, Chr_At(ia));
}

u8 MenuFont_CharWidth(u8 font, u8 c) {
    return Chr_Width(font, Chr_At(Chr_Index(c)));
}

void MenuFont_LoadAll(void) {
    void (**fp)(s32, void *) = &sysFontLoad;

    (*fp)(0, &D_80101070);
    (*fp)(1, &D_80101078);
    KeyboardChars_SetKerning();
}

void MenuFont_FreeAll(void) {
    void (**fp)(s32) = &sysFontFree;

    (*fp)(0);
    (*fp)(1);
}


/* Returns the character after c in the character table, wrapping around. */
u8 KeyboardChars_Next(u8 c) {
    u8 i = Chr_Index(c);

    i++;
    i %= 75;
    return Chr_At(i);
}

/* Returns the character before c in the character table, wrapping around. */
u8 KeyboardChars_Prev(u8 c) {
    u8 i = Chr_Index(c);

    if (i == 0) {
        i = 74;
    } else {
        i--;
    }
    return Chr_At(i);
}
