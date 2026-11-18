#include "common.h"
#include "include_asm.h"
extern s32 D_801206B0;
s32 func_800A76D8(void);
s32 func_80098300(u8 *, s32);
s32 func_800AF8EC(u8 *, s32);
s32 func_80087FF4(u8 *, s32);
s32 func_800A6BA4(u8 *, s32);
s32 func_80090D80(u8 *, s32);
void func_80098584(u8 *, s32);
void func_800AF8FC(u8 *, s32);
void func_80088004(u8 *, s32);
void func_800A6D98(u8 *, s32);
void func_80090FA8(u8 *, s32);
extern s32 D_8011F6A4;
extern u8 D_8011F6B0[];

extern s32 D_8011F6A0;

s32 func_800AEF80(void) {
    D_8011F6A0 = 0;
    D_8011F6A4 = 0;
    memset(D_8011F6B0, 0, 0x1000);
    return 1;
}


    /* TODO (Clean version): func_800AEFBC is identical to func_800AEF80. Both clear D_8011F6A0, D_8011F6A4 and zero 0x1000 bytes in D_8011F6B0. */
s32 func_800AEFBC(void) {
    D_8011F6A0 = 0;
    D_8011F6A4 = 0;
    memset(D_8011F6B0, 0, 0x1000);
    return 1;
}


/* Appends a snapshot (header of five block sizes + total, then the blocks) at most once per second. */
void func_800AEFF8(void) {
    u8 *h;
    u8 *q;
    u32 room;
    u32 i;
    /* TODO(match-debt): the header size only goes through a variable here; a literal folds into q - (h + 0x18). */
    /* TODO (Clean version): Replace raw s32 array indexing with a structured snapshot type:
       typedef struct {
           s32 module_sizes[5];
           s32 payload_size;
           u8 payload[];
       } SnapshotEntry;

       SnapshotEntry *entry = (SnapshotEntry *)h;
       entry->module_sizes[0] = func_80098300(q, room);
       ...
       entry->payload_size = (q - entry->payload);
    */
    s32 hdr = 0x18;

    if (D_8011F6A0 != 0 && (u32)(func_800A76D8() - D_801206B0) < 1000) {
        return;
    }
    room = 0x1000 - D_8011F6A4;
    if (room < 0x18) {
        return;
    }
    h = D_8011F6B0;
    for (i = 0; i < D_8011F6A0; i++) {
        h += ((s32 *)h)[5] + 0x18;
    }
    q = h + 0x18;
    room -= 0x18;
    if ((((s32 *)h)[0] = func_80098300(q, room)) == 0) {
        return;
    }
    room -= ((s32 *)h)[0];
    q += ((s32 *)h)[0];
    if ((((s32 *)h)[1] = func_800AF8EC(q, room)) == 0) {
        return;
    }
    room -= ((s32 *)h)[1];
    q += ((s32 *)h)[1];
    if ((((s32 *)h)[2] = func_80087FF4(q, room)) == 0) {
        return;
    }
    room -= ((s32 *)h)[2];
    q += ((s32 *)h)[2];
    if ((((s32 *)h)[3] = func_800A6BA4(q, room)) == 0) {
        return;
    }
    room -= ((s32 *)h)[3];
    q += ((s32 *)h)[3];
    if ((((s32 *)h)[4] = func_80090D80(q, room)) == 0) {
        return;
    }
    q += ((s32 *)h)[4];
    ((s32 *)h)[5] = (q - hdr) - h;
    D_8011F6A0++;
    D_8011F6A4 += 0x18 + ((s32 *)h)[5];
    D_801206B0 = func_800A76D8();
}

s32 func_800AF174(void) {
    return D_8011F6A0;
}

void func_800AF180(u32 n) {
    u8 *h = D_8011F6B0;
    u8 *q;
    u32 i;

    /* TODO (Bug check): Missing bounds check on `n`. If `n >= D_8011F6A0`, pointer traversal iterates past valid recorded snapshots:
       if (n >= D_8011F6A0) {
           return;
       }
    */
    for (i = 0; i < n; i++) {
        h += ((s32 *)h)[5] + 0x18;
    }
    q = h + 0x18;
    func_80098584(q, ((s32 *)h)[0]);
    q += ((s32 *)h)[0];
    func_800AF8FC(q, ((s32 *)h)[1]);
    q += ((s32 *)h)[1];
    func_80088004(q, ((s32 *)h)[2]);
    q += ((s32 *)h)[2];
    func_800A6D98(q, ((s32 *)h)[3]);
    func_80090FA8(q + ((s32 *)h)[3], ((s32 *)h)[4]);
}

