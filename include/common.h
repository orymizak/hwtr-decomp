#ifndef COMMON_H
#define COMMON_H

#include <PR/ultratypes.h>

#undef NULL /* ultratypes.h defines it as 0 */
#define NULL ((void *)0)

void *memset(void *dst, int value, size_t size);

#endif
