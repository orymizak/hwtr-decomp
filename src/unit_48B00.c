#include "common.h"
#include "include_asm.h"
typedef struct Nd8_ { s32 a; struct Nd8_ *prev; struct Nd8_ *next; u16 cnt; } Nd8_;
typedef struct { s16 a; s16 b; s32 c; s32 d; } Obj0C_;
typedef struct { u16 size; u16 cap; u16 count; u16 pad; u8 *data; } VecA7_;
void *memcpy(void *, const void *, size_t);
void *Memory_Alloc(s32 size);
s32 Memory_Free(void *);



Obj0C_ *func_800A7F00(s16 a, s16 b) {
    Obj0C_ *o = Memory_Alloc(0xC);

    memset(o, 0, 0xC);
    o->a = a;
    o->b = b;
    return o;
}

void func_800A7F5C(VecA7_ *v) {
    if (v->data != NULL) {
        Memory_Free(v->data);
    }
    Memory_Free(v);
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


/* Removes element i. Note: when shrinking, the tail goes to n + i rather than n + size * i (original behavior). */
void func_800A8050(VecA7_ *v, u16 i) {
    u8 *n;
    u16 j;

    if ((v->count - 1) % v->cap == 0) {
        n = Memory_Alloc(v->size * (v->count - 1));
        memcpy(n, v->data, v->size * i);
        memcpy(n + i, v->data + v->size * (i + 1), v->size * v->count - i - 1);
        Memory_Free(v->data);
        v->data = n;
    } else {
        for (j = i + 1; j < v->count; j++) {
            memcpy(v->data + v->size * (j - 1), v->data + v->size * j, v->size);
        }
    }
    v->count--;
}

u8 *func_800A81B4(VecA7_ *v, u16 i) {
    return v->data + v->size * i;
}

u16 func_800A81D4(VecA7_ *v) {
    return v->count;
}

void *func_800A81DC(void) {
    void *p = Memory_Alloc(4);

    memset(p, 0, 4);
    return p;
}

void func_800A8218(Nd8_ **list) {
    Nd8_ *n;
    Nd8_ *next;

    next = *list;
    n = next;
    while (next != NULL) {
        next = next->next;
        Memory_Free(n);
        n = next;
    }
    Memory_Free(list);
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


void func_800A82F8(Nd8_ **list, s32 key) {
    Nd8_ *n;

    for (n = *list; n != NULL; n = n->next) {
        if (n->a == key) {
            break;
        }
    }
    if (n != NULL) {
        if (--n->cnt == 0) {
            if (n->next != NULL) {
                n->next->prev = n->prev;
            }
            if (n->prev != NULL) {
                n->prev->next = n->next;
            }
            if (n == *list) {
                *list = n->next;
            }
            Memory_Free(n);
        }
    }
}
