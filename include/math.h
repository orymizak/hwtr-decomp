#ifndef MATH_H
#define MATH_H

/* Geometric types shared by the game code. Layouts are fixed by the matching build. */

typedef struct { f32 x, y, z; } Vec3f;          /* 0x0C */
typedef struct { s32 x, y, z; } Vec3i;          /* 0x0C */
typedef struct { f32 x, y, z, w; } Vec4f;       /* 0x10 */
typedef Vec4f Quat;                             /* rotation quaternion (x, y, z, w) */
typedef struct { f32 m[3][3]; } Mtx3f;          /* 3x3 rotation */
typedef struct { f32 m[3][4]; } Mtx34f;         /* 3x3 rotation + translation column */

#endif
