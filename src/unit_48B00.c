#include "common.h"
#include "include_asm.h"
typedef struct Nd8_ { s32 a; struct Nd8_ *prev; struct Nd8_ *next; u16 cnt; } Nd8_;
typedef struct { s16 a; s16 b; s32 c; s32 d; } Obj0C_;
typedef struct { u16 size; u16 cap; u16 count; u16 pad; u8 *data; } VecA7_;
void *memcpy(void *, const void *, u32);
void *Memory_Alloc(s32 size);
void *memset(void *dst, s32 value, u32 size);
s32 Memory_Free(void *);



Obj0C_ *func_800A7F00(s16 a, s16 b) {
    Obj0C_ *o = Memory_Alloc(0xC);

    memset(o, 0, 0xC);
    o->a = a;
    o->b = b;
    return o;
}

void func_800A7F5C(void *arg0) {
    void *temp_a0;

    temp_a0 = (*(s32 *)((u8 *)(arg0) + 0x8));
    if (temp_a0 != NULL) {
        Memory_Free(temp_a0);
    }
    Memory_Free(arg0);
}

void *func_800A7F98(VecA7_ *v) {
    u8 *n;

    if (v->count % v->cap == 0) {
        n = Memory_Alloc(v->size * (v->count + v->cap));
        memcpy(n, v->data, v->size * v->count);
        Memory_Free(v->data);
        v->data = n;
    }
    return v->data + v->size * v->count++;
}


INCLUDE_ASM("asm/nonmatchings/unit_48B00", func_800A8050);

s32 func_800A81B4(void *arg0, s32 arg1) {
    return *(s32 *)((u8 *)arg0 + 0x8) + (*(u16 *)((u8 *)arg0 + 0x0) * (arg1 & 0xFFFF));
}

u16 func_800A81D4(void *arg0) {
    return *(u16 *)((u8 *)arg0 + 0x4);
}

void *func_800A81DC(void) {
    void *p = Memory_Alloc(4);

    memset(p, 0, 4);
    return p;
}

void func_800A8218(s32 *arg0) {
    s32 *var_a0;
    s32 *var_s0;

    var_s0 = *arg0;
    var_a0 = var_s0;
    if (var_s0 != NULL) {
        do {
            var_s0 = (*(s32 *)((u8 *)(var_s0) + 0x8));
            ((s32 (*)(s32 *))Memory_Free)(var_a0);
            var_a0 = var_s0;
        } while (var_s0 != NULL);
    }
    ((s32 (*)(s32 *))Memory_Free)(arg0);
}

void func_800A8268(Nd8_ **list, s32 key) {
    Nd8_ *n;

    for (n = *list; n != NULL; n = n->next) {
        if (n->a == key) {
            break;
        }
    }
    if (n == NULL) {
        n = Memory_Alloc(0x10);
        n->a = key;
        n->prev = NULL;
        n->next = *list;
        n->cnt = 0;
        *list = n;
        if (n->next != NULL) {
            n->next->prev = n;
        }
    }
    n->cnt++;
}


void func_800A82F8(void *arg0, s32 arg1) {
    u16 temp_v0;
    void *temp_v1;
    void *temp_v1_2;
    void *var_a0;

    var_a0 = (*(s32 *)arg0);
    if (var_a0 != NULL) {
loop_1:
        if ((*(s32 *)((u8 *)(var_a0) + 0x0)) != arg1) {
            var_a0 = (*(s32 *)((u8 *)(var_a0) + 0x8));
            if (var_a0 != NULL) {
                goto loop_1;
            }
        }
        if (var_a0 != NULL) {
            temp_v0 = (*(s16 *)((u8 *)(var_a0) + 0xC)) - 1;
            (*(s16 *)((u8 *)(var_a0) + 0xC)) = temp_v0;
            if (!(temp_v0 & 0xFFFF)) {
                temp_v1 = (*(s32 *)((u8 *)(var_a0) + 0x8));
                if (temp_v1 != NULL) {
                    (*(s32 *)((u8 *)(temp_v1) + 0x4)) = (void *) (*(s32 *)((u8 *)(var_a0) + 0x4));
                }
                temp_v1_2 = (*(s32 *)((u8 *)(var_a0) + 0x4));
                if (temp_v1_2 != NULL) {
                    (*(s32 *)((u8 *)(temp_v1_2) + 0x8)) = (void *) (*(s32 *)((u8 *)(var_a0) + 0x8));
                }
                if (var_a0 == (*(s32 *)arg0)) {
                    (*(s32 *)arg0) = (void *) (*(s32 *)((u8 *)(var_a0) + 0x8));
                }
                Memory_Free((u32) var_a0);
            }
        }
    }
}
