#include "common.h"
#include "include_asm.h"
s32 func_8009610C(s32);
extern void (*D_8014C0A8)(s32, s32, f32, f32);
void func_800ABEC8(u8, u32, u8);
void *func_800960D8(u32);
extern s32 D_800F3908;
extern s32 D_800F390C;
extern s32 D_800F3910;
extern u8 D_800F3918;
extern u8 D_800F3919;
extern u8 D_800F391A;

extern s32 D_800F3900;
extern s32 D_800F3904;

void func_800A76C0(s32 arg0) {
    D_800F3900 += arg0;
}

s32 func_800A76D8(void) {
    return D_800F3900;
}

s32 func_800A76E4(void) {
    return D_800F3904;
}

void func_800A76F0(void) {
    D_800F3900 = 0;
}

INCLUDE_ASM("asm/nonmatchings/unit_482C0", func_800A76FC);

s32 func_800A780C(u8 ch, u8 cp) {
    u8 *c;
    u8 ok;
    u8 k;
    u8 i;
    u8 best;
    u32 t;

    c = func_800960D8(ch);
    best = 0;
    if (c[0x47B] != 0) {
        return 1;
    }
    k = cp - 1;
    ok = 1;
    for (i = 0; i < k; i++) {
        if ((c + i)[0x444] == 0) {
            ok = 0;
            break;
        }
    }
    if ((D_800F391A == 0) & (ok == 0)) {
        if ((u8)func_8009610C(ch) != 0) {
            if (c[0x479] != 0 || k != D_800F3910 - 1) {
                if (k != c[0x479] - 1) {
                    D_8014C0A8(11, 0, 1.0f, 1.0f);
                }
            }
        }
    }
    if (!ok || (c + k)[0x444] != 0) {
        return 0;
    }
    if (k < (u32)(D_800F3910 - 1)) {
        if (D_800F391A == 0) {
            if ((u8)func_8009610C(ch) != 0) {
                D_8014C0A8(10, 0, 1.0f, 1.0f);
            }
        }
        (c + k)[0x444] = 1;
        c[0x479]++;
        goto done;
    }
    if (k != D_800F3910 - 1) {
        return 1;
    }
    if (D_800F391A == 0) {
        if ((u8)func_8009610C(ch) != 0) {
            D_8014C0A8(12, 0, 1.0f, 1.0f);
        }
        t = D_800F3900;
        if (D_800F3918 == 1) {
            if (*(u32 *)(c + 0x470) == 0 || t - *(u32 *)(c + 0x450) < *(u32 *)(c + 0x470)) {
                *(u32 *)(c + 0x470) = t - *(u32 *)(c + 0x450);
                best = 1;
            }
            if ((u8)func_8009610C(ch) != 0) {
                func_800ABEC8(c[0xC], t - *(u32 *)(c + 0x450), best);
            }
            *(u32 *)(c + 0x450) = t;
            c[0x478] = 1;
        } else {
            if (*(u32 *)(c + 0x470) == 0 || t - *(u32 *)(c + (c[0x478] << 2) + 0x44C) < *(u32 *)(c + 0x470)) {
                *(u32 *)(c + 0x470) = t - *(u32 *)(c + (c[0x478] << 2) + 0x44C);
                best = 1;
            }
            if ((u8)func_8009610C(ch) != 0) {
                func_800ABEC8(c[0xC], t - *(u32 *)(c + (c[0x478] << 2) + 0x44C), best);
            }
            *(u32 *)(c + c[0x478] * 4 + 0x450) = t;
            c[0x478]++;
            if (D_800F3919 == 0 && c[0x478] == D_800F390C) {
                c[0x47B] = 1;
            }
        }
    }
    c[0x479] = 0;
    for (i = 0; i < 10; i++) {
        (c + i)[0x444] = 0;
    }
done:
    return 1;
}

s32 func_800A7B88(void *arg0) {
  s32 new_var;
  D_800F3904 = 0;
  D_800F3908 = (s32) (*((u8 *) (((u8 *) arg0) + 0x17)));
  D_800F390C = (s32) (*((u8 *) (((u8 *) arg0) + 0x19)));
  new_var = *((s32 *) (((u8 *) arg0) + 0x0));
  D_800F3910 = (s32) (*((u8 *) (((u8 *) arg0) + 0x1A)));
  if (new_var & 2) {
    D_800F391A = 1;
  }
  else
  {
    D_800F391A = 0;
  }
  if ((*((s32 *) (((u8 *) arg0) + 0x0))) & 4) {
    D_800F3919 = 1;
  }
  else
  {
    D_800F3919 = 0;
  }
  if ((*((s32 *) (((u8 *) arg0) + 0x0))) & 8) {
    D_800F3918 = 1;
  }
  else
  {
    D_800F3918 = 0;
  }
  return 1;
}

s32 func_800A7C2C(void) {
    return 1;
}

s32 func_800A7C34(u8 arg0) {
  void *temp_v0;
  temp_v0 = func_800960D8(arg0 & 0xFF);
  if ((*((u8 *) (((u8 *) temp_v0) + 0x47B))) == 0) {
    if (temp_v0 || arg0) {
      return 0;
    }
    else
    {
      return 0;
    }
  }
  *((s32 *) (((u8 *) temp_v0) + 0x14)) = 0;
  *((s32 *) (((u8 *) temp_v0) + 0x18)) = 0;
  *((u8 *) (((u8 *) temp_v0) + 0x24)) = 1;
  *((u8 *) (((u8 *) temp_v0) + 0x25)) = 0;
  *((u8 *) (((u8 *) temp_v0) + 0x27)) = 0;
  *((f32 *) (((u8 *) temp_v0) + 0x10)) = 1.0f;
  return 1;
}

INCLUDE_ASM("asm/nonmatchings/unit_482C0", func_800A7C8C);
