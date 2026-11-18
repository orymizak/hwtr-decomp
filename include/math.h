#ifndef MATH_H
#define MATH_H

/* Geometric types shared by the game code. Layouts are fixed by the matching build. */

typedef struct { f32 x, y, z; } Vec3f;          /* 0x0C */
typedef struct { s32 x, y, z; } Vec3i;          /* 0x0C */
typedef struct { f32 x, y, z, w; } Vec4f;       /* 0x10 */
typedef Vec4f Quat;                             /* rotation quaternion (x, y, z, w) */
typedef struct { f32 m[3][3]; } Mtx3f;          /* 3x3 rotation */
typedef struct { f32 m[3][4]; } Mtx34f;         /* 3x3 rotation + translation column */

/* Camera constants. The game converts degrees with pi = 3.14159, hence the last digits. */
#define CAM_FOV_60DEG 1.0471967f  /* 60 degrees */
#define CAM_FOV_130DEG 2.2689264f /* 130 degrees */
#define CAM_ASPECT_4_3 1.3333334f /* 4:3 */
#define CAM_NEAR 48.0f
#define CAM_FAR 5000.0f

#ifndef ABS
#define ABS(x) ((x) < 0.0f ? -(x) : (x))
#endif
#ifndef MIN
#define MIN(a, b) ((a) < (b) ? (a) : (b))
#endif
#ifndef MAX
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#endif

/* Vector helpers. Statement macros expand to a { } block; the operand order of each
   expression is the one the matching code needs. */
#define VEC3_DOT(a, b) ((a).x * (b).x + (a).y * (b).y + (a).z * (b).z)
#define VEC3_ABS_SUM(v) (ABS((v).x) + ABS((v).y) + ABS((v).z))
#define VEC3_ADD(d, a, b) { (d).x = (a).x + (b).x; (d).y = (a).y + (b).y; (d).z = (a).z + (b).z; }
#define VEC3_SUB(d, a, b) { (d).x = (a).x - (b).x; (d).y = (a).y - (b).y; (d).z = (a).z - (b).z; }
#define VEC3_SCALE(d, a, k) { (d).x = (a).x * (k); (d).y = (a).y * (k); (d).z = (a).z * (k); }
#define VEC3_CROSS(d, a, b) { (d).x = (a).y * (b).z - (b).y * (a).z; (d).y = (b).x * (a).z - (a).x * (b).z; \
                              (d).z = (a).x * (b).y - (b).x * (a).y; }
#define VEC3_NORMALIZE(v) { f32 _l = sqrtf(VEC3_DOT(v, v)); f32 _i = 1.0f / _l; \
                            (v).x = (v).x * _i; (v).y = (v).y * _i; (v).z = (v).z * _i; }
/* d = transpose(s) for 3x3 arrays (row by row of d). */
#define MTX3_TRANSPOSE(d, s) { (d)[0][0] = (s)[0][0]; (d)[0][1] = (s)[1][0]; (d)[0][2] = (s)[2][0]; \
                               (d)[1][0] = (s)[0][1]; (d)[1][1] = (s)[1][1]; (d)[1][2] = (s)[2][1]; \
                               (d)[2][0] = (s)[0][2]; (d)[2][1] = (s)[1][2]; (d)[2][2] = (s)[2][2]; }
/* Store three vectors as the columns of a 3x3 matrix. */
#define MTX3_SET_COLUMNS(m, a, b, c) { (m)[0][0] = (a).x; (m)[1][0] = (a).y; (m)[2][0] = (a).z; \
                                       (m)[0][1] = (b).x; (m)[1][1] = (b).y; (m)[2][1] = (b).z; \
                                       (m)[0][2] = (c).x; (m)[1][2] = (c).y; (m)[2][2] = (c).z; }

#endif
