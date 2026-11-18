#ifndef PHYSICS_H
#define PHYSICS_H

#include "math.h"

/* Gravity in inches/s^2 (32.17 ft/s^2): mass = weight / GRAVITY. */
#define GRAVITY 386.0f

/* Rigid body (0x118). Inertia tensors are in body space; iw is the world-space inverse. */
typedef struct {
    /* 0x000 */ Mtx3f inertia;
    /* 0x024 */ Mtx3f invInertia;
    /* 0x048 */ f32 mass;
    /* 0x04C */ f32 im;        /* 1 / mass */
    /* 0x050 */ f32 f50;
    /* 0x054 */ Vec3f g;       /* gravity */
    /* 0x060 */ Vec3f f60;
    /* 0x06C */ Vec3f pos;
    /* 0x078 */ Vec3f mom;     /* linear momentum */
    /* 0x084 */ Vec3f vel;
    /* 0x090 */ f32 speed;
    /* 0x094 */ Mtx3f r;       /* orientation */
    /* 0x0B8 */ Vec3f am;      /* angular momentum */
    /* 0x0C4 */ Mtx3f iw;      /* world inverse inertia */
    /* 0x0E8 */ Vec3f w;       /* angular velocity */
    /* 0x0F4 */ f32 wlen;
    /* 0x0F8 */ Vec3f f;       /* accumulated force */
    /* 0x104 */ Vec3f t;       /* accumulated torque */
    /* 0x110 */ u8 frozen;
    /* 0x111 */ u8 pad111[3];
    /* 0x114 */ s32 x114;
} RigidBody;
/* Collision object (0x88): track objects, cars, debris (registered with Coll_Register). */
typedef struct {
    /* 0x00 */ u8 pad0[4];
    /* 0x04 */ s32 flags;
    /* 0x08 */ u8 state;      /* copy of body->frozen */
    /* 0x09 */ u8 pad9[3];
    /* 0x0C */ Vec3i pos;     /* start of the collision shape */
    /* 0x18 */ u8 pad18[0xC];
    /* 0x24 */ f32 m[9];
    /* 0x48 */ u8 pad48[0x14];
    /* 0x5C */ s32 kind;
    /* 0x60 */ RigidBody *body;
    /* 0x64 */ u8 *owner;     /* the car or object this collider belongs to */
    /* 0x68 */ u8 pad68[0x1C];
    /* 0x84 */ u32 weight;    /* 0 = immovable */
} Collider; /* size = 0x88 */

#endif
