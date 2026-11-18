#ifndef COMMON_H
#define COMMON_H

#include <PR/ultratypes.h>

#undef NULL /* ultratypes.h defines it as 0 */
#define NULL ((void *)0)

#define ARRAY_COUNT(arr) (s32)(sizeof(arr) / sizeof((arr)[0]))

void *memset(void *dst, int value, size_t size);
void *memcpy(void *dst, const void *src, size_t size);

#endif
