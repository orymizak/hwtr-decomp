#include "common.h"
#include "include_asm.h"
#include "car.h"
typedef struct { s32 a; s32 b; s32 c; } V12_;
typedef struct { f32 x; f32 y; f32 z; } V3f_;
extern V12_ D_8011855C;
extern V12_ D_80118568;
extern f32 D_80118578[3][3];
typedef struct { s32 w; u8 pad[0x60]; } Ent64w_;
extern Ent64w_ D_801185B8[];
typedef struct { u8 on; u8 pad[0x63]; } Ent64_;
extern Ent64_ D_801185B4[];
extern u8 D_80118620;
typedef struct { s32 x; s32 y; s32 z; } Pos_;
typedef Vec3f Vec3f_;
    /* TODO (Clean version): Unify CamE_, Cam90_, Ent64_, and Ent64w_ into a single Camera struct (sizeof 0x64):
    typedef struct {
        u32 unk0;
        Vec3f_ pos;
        Vec3f_ vel;
        f32 m[3][3];
        f32 fov;
        s32 mode;
        u8 pt;
        u8 car;
        u8 b4A;
        u8 b4B;
        u8 b4C;
        u8 pad4D[3];
        s32 timer;
        u32 shake;
        f32 f58;
        u8 b5C;
        u8 pad5D[3];
        u32 b60;
    } Camera;
    extern Camera gCameras[];
    */
typedef struct { u8 p0[0x44]; s32 mode; u8 pt; u8 car; u8 p4A[2]; u8 b4C; u8 p4D[3]; s32 timer; } Cam90_;
typedef struct { u8 p0[4]; Vec3f_ pos; Vec3f_ vel; f32 m[3][3]; f32 fov; s32 mode; u8 pt; u8 car; u8 b4A; u8 b4B; u8 b4C; u8 p4D[7]; u32 shake; f32 f58; u8 b5C; u8 p5D[3]; u32 b60; } CamE_;
typedef struct { s32 mode; Vec3f_ ofs; f32 dist; } CamT_;
f32 sinf(f32);
extern u8 D_80118550;
extern u8 D_80138D43;
s32 func_8008D3AC(void);
void func_8009099C(CamE_ *);
void func_8009013C(Vec3f_, Vec3f_, Vec3f_, Vec3f_ *, f32, f32, f32, f32);
void func_800A4198(u8, Vec3f_ *);
u8 func_800A4EE8(u8, Vec3f_ *, Vec3f_ *);
Vec3f_ func_800A9068(f32 t, Vec3f_ a, Vec3f_ b);
s32 func_800A58A4(s32 arg0);
extern u8 (*D_80141718)(void);
extern void (*D_8014171C)(u8, Vec3f_ *, void *, s32 *, s32 *);
extern u8 (*D_8014169C)(s32);
extern u8 D_80118621;
extern s32 *D_80118624;
s32 func_80096100(void);
Car *func_800960D8(u8);
f32 sqrtf(f32);
extern u8 D_80118558[];
s32 func_800A5894(void);
extern u8 D_801185AC;
extern u8 D_801185A4;
extern u8 D_801185A2;



INCLUDE_ASM("asm/nonmatchings/unit_2F4F0", func_8008E8F0);

s32 func_8008EA64(void) {
    return 1;
}

void func_80090578(Cam90_ *cam, u32 dt);
void func_800908A0(void);
s32 func_800908A8(void);
void func_8008EA6C(u32 dt) {
    Vec3f_ pos;
    Vec3f_ vel;
    Vec3f_ t;
    Vec3f_ d;
    Vec3f_ f;
    Vec3f_ r;
    Vec3f_ u;
    Vec3f_ ofs;
    Vec3f_ eye;
    Vec3f_ c;
    f32 m[3][3];
    Vec3f_ tmp;
    s32 flags;
    u32 i;
    CamE_ *cam;
    f32 k;
    Car *car;
    u8 vis;
    u8 b3E7;
    f32 f4C4;
    f32 fBC;
    u8 b4C0;
    f32 s;
    f32 far;
    f32 near;
    f32 damp;
    f32 dist;
    f32 olen;
    f32 dp;
    f32 sum;
    f32 *sp;
    s32 (**rnd)(s32);
    s32 idx;

    k = 1000.0f / (f32)dt;
    cam = (CamE_ *)D_80118558;
    for (i = 0; i < D_80118620; cam++, i++) {
        if ((*cam).b5C != 0 && func_800908A8() != 0) {
            func_8009099C(&(*cam));
            continue;
        }
        vis = 1;
        car = func_800960D8((*cam).car);
        pos.x = car->pos.x + car->v8C.x;
        pos.y = car->pos.y + car->v8C.y;
        pos.z = car->pos.z + car->v8C.z;
        m[0][0] = car->rot.m[0][0];
        m[0][1] = car->rot.m[0][1];
        m[0][2] = car->rot.m[0][2];
        m[1][0] = car->rot.m[1][0];
        m[1][1] = car->rot.m[1][1];
        m[1][2] = car->rot.m[1][2];
        m[2][0] = car->rot.m[2][0];
        m[2][1] = car->rot.m[2][1];
        m[2][2] = car->rot.m[2][2];
        vel = car->vel;
        flags = car->flags;
        if (func_8008D3AC() != 0) {
            vis = car->b13C;
        }
        b3E7 = car->cnt3E7;
        b4C0 = car->b4C0;
        f4C4 = car->f4C4;
        fBC = car->fBC;
        switch ((*cam).mode) {
        case 0:
            /* TODO (Clean version): Replace repetitive inlined vector and matrix math with helper macros:
             *   #define VEC3_DOT(a, b) ((a).x * (b).x + (a).y * (b).y + (a).z * (b).z)
             *   #define VEC3_CROSS(dst, a, b) do { \
             *       (dst).x = (a).y * (b).z - (a).z * (b).y; \
             *       (dst).y = (a).z * (b).x - (a).x * (b).z; \
             *       (dst).z = (a).x * (b).y - (a).y * (b).x; \
             *   } while (0)
             *   #define VEC3_NORMALIZE(v) do { \
             *       f32 _l = sqrtf(VEC3_DOT(v, v)); \
             *       f32 _i = 1.0f / _l; \
             *       (v).x *= _i; (v).y *= _i; (v).z *= _i; \
             *   } while (0)
             *   Mtx33_MultVec3f(m, &((CamT_ *)D_80118624)[cam->pt].ofs, &t);
             *   VEC3_ADD(cam->pos, pos, t);
             */
            tmp.x = m[0][0] * (((CamT_ *)D_80118624)[(*cam).pt]).ofs.x + m[0][1] * (((CamT_ *)D_80118624)[(*cam).pt]).ofs.y + m[0][2] * (((CamT_ *)D_80118624)[(*cam).pt]).ofs.z;
            tmp.y = m[1][0] * (((CamT_ *)D_80118624)[(*cam).pt]).ofs.x + m[1][1] * (((CamT_ *)D_80118624)[(*cam).pt]).ofs.y + m[1][2] * (((CamT_ *)D_80118624)[(*cam).pt]).ofs.z;
            tmp.z = m[2][0] * (((CamT_ *)D_80118624)[(*cam).pt]).ofs.x + m[2][1] * (((CamT_ *)D_80118624)[(*cam).pt]).ofs.y + m[2][2] * (((CamT_ *)D_80118624)[(*cam).pt]).ofs.z;
            t = tmp;
            { ((*cam).pos).x = ( pos).x + ( t).x; ((*cam).pos).y = ( pos).y + ( t).y; ((*cam).pos).z = ( pos).z + ( t).z; };
            (*cam).vel = vel;
            (*cam).m[0][0] = m[0][0];
            (*cam).m[0][1] = m[0][1];
            (*cam).m[0][2] = m[0][2];
            (*cam).m[1][0] = m[1][0];
            (*cam).m[1][1] = m[1][1];
            (*cam).m[1][2] = m[1][2];
            (*cam).m[2][0] = m[2][0];
            (*cam).m[2][1] = m[2][1];
            (*cam).m[2][2] = m[2][2];
            (*cam).fov = 1.570795f;
            func_800A4198((*cam).car, &(*cam).pos);
            break;
        case 1:
            if (flags & 0x10) {
                if (!func_800A4EE8((*cam).car, &t, &u)) {
                    flags &= ~0x10;
                }
            }
            if (flags & 0x10) {
                far = 30000.0f;
                near = 10000.0f;
                damp = 0.1f;
                s = ((vel).x * ( t).x + (vel).y * ( t).y + (vel).z * ( t).z);
                { (vel).x = ( t).x * ( s); (vel).y = ( t).y * ( s); (vel).z = ( t).z * ( s); };
                sum = ((((vel).x) < 0.0f ? -((vel).x) : ((vel).x)) + (((vel).y) < 0.0f ? -((vel).y) : ((vel).y)) + (((vel).z) < 0.0f ? -((vel).z) : ((vel).z)));
                if (sum < 1.0f) {
                    vel = t;
                }
                f = vel;
                { f32 _l = sqrtf(((f).x * (f).x + (f).y * (f).y + (f).z * (f).z)); f32 _i = 1.0f / _l; (f).x = (f).x * _i; (f).y = (f).y * _i; (f).z = (f).z * _i; };
                if (car->b744 == 0) {
                    u.x = 0.0f;
                    u.y = 0.0f;
                    u.z = 1.0f;
                }
                { (r).x = ( f).y * ( u).z - ( u).y * ( f).z; (r).y = ( u).x * ( f).z - ( f).x * ( u).z; (r).z = ( f).x * ( u).y - ( u).x * ( f).y; };
                { f32 _l = sqrtf(((r).x * (r).x + (r).y * (r).y + (r).z * (r).z)); f32 _i = 1.0f / _l; (r).x = (r).x * _i; (r).y = (r).y * _i; (r).z = (r).z * _i; };
                { (u).x = ( r).y * ( f).z - ( f).y * ( r).z; (u).y = ( f).x * ( r).z - ( r).x * ( f).z; (u).z = ( r).x * ( f).y - ( f).x * ( r).y; };
            } else {
                far = 30000.0f;
                near = 10000.0f;
                damp = 0.1f;
                if (!(flags & 4)) {
                    vel.z = 0.0f;
                }
                if (sqrtf(((vel).x * ( vel).x + (vel).y * ( vel).y + (vel).z * ( vel).z)) < D_80138D43 * 17.6f) {
                    if (b3E7 != 0 || (*cam).b60 != 0 || (*cam).b4C != 0) {
                        t.x = m[0][1];
                        t.y = m[1][1];
                        t.z = m[2][1];
                    } else {
                        { (t).x = ( pos).x - ( (*cam).pos).x; (t).y = ( pos).y - ( (*cam).pos).y; (t).z = ( pos).z - ( (*cam).pos).z; };
                    }
                    t.z = 0.0f;
                    if (((t.x) < 0.0f ? -(t.x) : (t.x)) + ((t.y) < 0.0f ? -(t.y) : (t.y)) > 0.0f) {
                        f = t;
                    } else {
                        f.x = m[0][2];
                        f.y = m[1][2];
                        f.z = m[2][2];
                    }
                } else {
                    f = vel;
                }
                { f32 _l = sqrtf(((f).x * (f).x + (f).y * (f).y + (f).z * (f).z)); f32 _i = 1.0f / _l; (f).x = (f).x * _i; (f).y = (f).y * _i; (f).z = (f).z * _i; };
                if (flags & 4) {
                    u.x = m[0][2];
                    u.y = m[1][2];
                    u.z = m[2][2];
                    { (r).x = ( f).y * ( u).z - ( u).y * ( f).z; (r).y = ( u).x * ( f).z - ( f).x * ( u).z; (r).z = ( f).x * ( u).y - ( u).x * ( f).y; };
                    if (!(((((r).x) < 0.0f ? -((r).x) : ((r).x)) + (((r).y) < 0.0f ? -((r).y) : ((r).y)) + (((r).z) < 0.0f ? -((r).z) : ((r).z))) > 0.0f)) {
                        r.x = 1.0f;
                        r.y = 0.0f;
                        r.z = 0.0f;
                    }
                    { f32 _l = sqrtf(((r).x * (r).x + (r).y * (r).y + (r).z * (r).z)); f32 _i = 1.0f / _l; (r).x = (r).x * _i; (r).y = (r).y * _i; (r).z = (r).z * _i; };
                    { (u).x = ( r).y * ( f).z - ( f).y * ( r).z; (u).y = ( f).x * ( r).z - ( r).x * ( f).z; (u).z = ( r).x * ( f).y - ( f).x * ( r).y; };
                } else {
                    r.x = f.y;
                    r.y = -f.x;
                    r.z = 0.0f;
                    u.x = 0.0f;
                    u.y = 0.0f;
                    u.z = 1.0f;
                }
            }
            { (t).x = ( u).x * ( (((CamT_ *)D_80118624)[(*cam).pt]).dist); (t).y = ( u).y * ( (((CamT_ *)D_80118624)[(*cam).pt]).dist); (t).z = ( u).z * ( (((CamT_ *)D_80118624)[(*cam).pt]).dist); };
            { (pos).x = ( pos).x + ( t).x; (pos).y = ( pos).y + ( t).y; (pos).z = ( pos).z + ( t).z; };
            ofs = (((CamT_ *)D_80118624)[(*cam).pt]).ofs;
            if (b4C0 != 0) {
                s = ((fBC - f4C4) + 528.0f) / 528.0f;
            } else {
                s = 0.0f;
            }
            (*cam).f58 += (s - (*cam).f58) / k;
            s = (*cam).f58 * 0.5f + 1.0f;
            ofs.x *= s;
            ofs.y *= s;
            eye = pos;
            { (t).x = ( r).x * ( ofs.x); (t).y = ( r).y * ( ofs.x); (t).z = ( r).z * ( ofs.x); };
            { (eye).x = ( eye).x + ( t).x; (eye).y = ( eye).y + ( t).y; (eye).z = ( eye).z + ( t).z; };
            { (t).x = ( f).x * ( ofs.y); (t).y = ( f).y * ( ofs.y); (t).z = ( f).z * ( ofs.y); };
            { (eye).x = ( eye).x + ( t).x; (eye).y = ( eye).y + ( t).y; (eye).z = ( eye).z + ( t).z; };
            { (t).x = ( u).x * ( ofs.z); (t).y = ( u).y * ( ofs.z); (t).z = ( u).z * ( ofs.z); };
            { (eye).x = ( eye).x + ( t).x; (eye).y = ( eye).y + ( t).y; (eye).z = ( eye).z + ( t).z; };
            func_800A4198((*cam).car, &eye);
            { (t).x = ( eye).x - ( pos).x; (t).y = ( eye).y - ( pos).y; (t).z = ( eye).z - ( pos).z; };
            dist = sqrtf(((t).x * ( t).x + (t).y * ( t).y + (t).z * ( t).z));
            olen = sqrtf(((ofs).x * ( ofs).x + (ofs).y * ( ofs).y + (ofs).z * ( ofs).z));
            if (vis == 0 && (*cam).b4C == 0 && (*cam).b60 == 0) {
                { (t).x = ( (*cam).pos).x - ( pos).x; (t).y = ( (*cam).pos).y - ( pos).y; (t).z = ( (*cam).pos).z - ( pos).z; };
                { (d).x = ( eye).x - ( pos).x; (d).y = ( eye).y - ( pos).y; (d).z = ( eye).z - ( pos).z; };
                if (((t).x * ( d).x + (t).y * ( d).y + (t).z * ( d).z) < 0.0f) {
                    eye.z += 120.0f;
                }
                func_8009013C((*cam).pos, (*cam).vel, eye, &eye, k, far, near, damp);
            }
            if (dist < 1.0f) {
                f.y = 1.0f;
            }
            { (f).x = ( pos).x - ( eye).x; (f).y = ( pos).y - ( eye).y; (f).z = ( pos).z - ( eye).z; };
            { f32 _l = sqrtf(((f).x * (f).x + (f).y * (f).y + (f).z * (f).z)); f32 _i = 1.0f / _l; (f).x = (f).x * _i; (f).y = (f).y * _i; (f).z = (f).z * _i; };
            if (flags & 0x10) {
                u.x = 0.0f;
                u.y = 0.0f;
                u.z = 1.0f;
            }
            { (r).x = ( f).y * ( u).z - ( u).y * ( f).z; (r).y = ( u).x * ( f).z - ( f).x * ( u).z; (r).z = ( f).x * ( u).y - ( u).x * ( f).y; };
            { f32 _l = sqrtf(((r).x * (r).x + (r).y * (r).y + (r).z * (r).z)); f32 _i = 1.0f / _l; (r).x = (r).x * _i; (r).y = (r).y * _i; (r).z = (r).z * _i; };
            { (u).x = ( r).y * ( f).z - ( f).y * ( r).z; (u).y = ( f).x * ( r).z - ( r).x * ( f).z; (u).z = ( r).x * ( f).y - ( f).x * ( r).y; };
            { (t).x = ( f).x * ( ((olen) < ( dist) ? (olen) : ( dist))); (t).y = ( f).y * ( ((olen) < ( dist) ? (olen) : ( dist))); (t).z = ( f).z * ( ((olen) < ( dist) ? (olen) : ( dist))); };
            { (eye).x = ( pos).x - ( t).x; (eye).y = ( pos).y - ( t).y; (eye).z = ( pos).z - ( t).z; };
            if ((*cam).b60 != 0) {
                sp = (f32 *)func_800A58A4(func_800A5894() - 1);
                s = (f32)(5000 - (*cam).b60) / 5000.0f;
                s = (sinf((s - 0.5f) * 3.14159f) + 1.0f) * 0.5f;
                tmp = func_800A9068(s, *(Vec3f_ *)&sp[0], eye);
                (*cam).pos = tmp;
                c = func_800A9068(s, *(Vec3f_ *)&sp[3], pos);
                { (f).x = ( c).x - ( (*cam).pos).x; (f).y = ( c).y - ( (*cam).pos).y; (f).z = ( c).z - ( (*cam).pos).z; };
                { f32 _l = sqrtf(((f).x * (f).x + (f).y * (f).y + (f).z * (f).z)); f32 _i = 1.0f / _l; (f).x = (f).x * _i; (f).y = (f).y * _i; (f).z = (f).z * _i; };
                r.x = f.y;
                r.y = -f.x;
                r.z = 0.0f;
                { f32 _l = sqrtf(((r).x * (r).x + (r).y * (r).y + (r).z * (r).z)); f32 _i = 1.0f / _l; (r).x = (r).x * _i; (r).y = (r).y * _i; (r).z = (r).z * _i; };
                { (u).x = ( r).y * ( f).z - ( f).y * ( r).z; (u).y = ( f).x * ( r).z - ( r).x * ( f).z; (u).z = ( r).x * ( f).y - ( f).x * ( r).y; };
                { (*cam).m[0][0] = (r).x; (*cam).m[1][0] = (r).y; (*cam).m[2][0] = (r).z; (*cam).m[0][1] = ( f).x; (*cam).m[1][1] = ( f).y; (*cam).m[2][1] = ( f).z; (*cam).m[0][2] = ( u).x; (*cam).m[1][2] = ( u).y; (*cam).m[2][2] = ( u).z; };
                (*cam).b60 -= dt;
            } else {
                if ((*cam).b4C == 0) {
                    { ((*cam).vel).x = ( eye).x - ( (*cam).pos).x; ((*cam).vel).y = ( eye).y - ( (*cam).pos).y; ((*cam).vel).z = ( eye).z - ( (*cam).pos).z; };
                    { ((*cam).vel).x = ( (*cam).vel).x * ( k); ((*cam).vel).y = ( (*cam).vel).y * ( k); ((*cam).vel).z = ( (*cam).vel).z * ( k); };
                } else {
                    (*cam).vel = vel;
                }
                (*cam).pos = eye;
                { (*cam).m[0][0] = (r).x; (*cam).m[1][0] = (r).y; (*cam).m[2][0] = (r).z; (*cam).m[0][1] = ( f).x; (*cam).m[1][1] = ( f).y; (*cam).m[2][1] = ( f).z; (*cam).m[0][2] = ( u).x; (*cam).m[1][2] = ( u).y; (*cam).m[2][2] = ( u).z; };
                (*cam).fov = 1.570795f;
            }
            break;
        case 2:
            D_8014171C((*cam).pt, &(*cam).pos, (*cam).m, &flags, (s32 *)&(*cam).fov);
            (*cam).vel.x = 0.0f;
            (*cam).vel.y = 0.0f;
            (*cam).vel.z = 0.0f;
            break;
        case 3:
            D_8014171C((*cam).pt, &(*cam).pos, m, &flags, (s32 *)&(*cam).fov);
            (*cam).vel.x = 0.0f;
            (*cam).vel.y = 0.0f;
            (*cam).vel.z = 0.0f;
            { (f).x = ( pos).x - ( (*cam).pos).x; (f).y = ( pos).y - ( (*cam).pos).y; (f).z = ( pos).z - ( (*cam).pos).z; };
            { f32 _l = sqrtf(((f).x * (f).x + (f).y * (f).y + (f).z * (f).z)); f32 _i = 1.0f / _l; (f).x = (f).x * _i; (f).y = (f).y * _i; (f).z = (f).z * _i; };
            r.x = f.y;
            r.y = -f.x;
            r.z = 0.0f;
            { f32 _l = sqrtf(((r).x * (r).x + (r).y * (r).y + (r).z * (r).z)); f32 _i = 1.0f / _l; (r).x = (r).x * _i; (r).y = (r).y * _i; (r).z = (r).z * _i; };
            { (u).x = ( r).y * ( f).z - ( f).y * ( r).z; (u).y = ( f).x * ( r).z - ( r).x * ( f).z; (u).z = ( r).x * ( f).y - ( f).x * ( r).y; };
            { f32 _l = sqrtf(((u).x * (u).x + (u).y * (u).y + (u).z * (u).z)); f32 _i = 1.0f / _l; (u).x = (u).x * _i; (u).y = (u).y * _i; (u).z = (u).z * _i; };
            { (*cam).m[0][0] = (r).x; (*cam).m[1][0] = (r).y; (*cam).m[2][0] = (r).z; (*cam).m[0][1] = ( f).x; (*cam).m[1][1] = ( f).y; (*cam).m[2][1] = ( f).z; (*cam).m[0][2] = ( u).x; (*cam).m[1][2] = ( u).y; (*cam).m[2][2] = ( u).z; };
            break;
        case 4:
            /* TODO (Clean version / Bug check): func_800908A0 is declared as void(void) but invoked
             * as void (*)(CamE_ *, f32). If mode 4 is implemented, align the function prototype:
             *   void func_800908A0(CamE_ *cam, f32 k);
             */
            ((void (*)(CamE_ *, f32))func_800908A0)(&(*cam), k);
            break;
        }
        if ((*cam).shake > dt) {
            rnd = (s32 (**)(s32))&D_8014169C;
            s = (*rnd)((*cam).shake) - 125;
            (*cam).pos.x += s / 10.0f;
            s = (*rnd)((*cam).shake) - 125;
            (*cam).pos.y += s / 10.0f;
            s = (*rnd)((*cam).shake) - 125;
            (*cam).pos.z += s / 10.0f;
            (*cam).shake -= dt;
        } else {
            (*cam).shake = 0;
        }
        (*cam).b4C = 0;
        if (D_80118550 != 0) {
            func_80090578(&(*cam), dt);
        } else if ((*cam).b4A != (*cam).b4B) {
            if ((*cam).b4A != 0) {
                idx = ((*cam).pt + 1) % D_80118621;
                (*cam).pt = idx;
                (*cam).mode = (((CamT_ *)D_80118624)[idx]).mode;
                (*cam).b4C = 1;
                if ((*cam).b60 != 0) {
                    car->cnt3E7 = car->nPoints;
                    (*cam).b60 = 0;
                }
            }
            (*cam).b4B = (*cam).b4A;
        }
    }
}


INCLUDE_ASM("asm/nonmatchings/unit_2F4F0", func_8009013C);

void func_80090410(u8 i, Pos_ *a1, f32 *a2, f32 *a3, f32 *a4, f32 *a5, s8 *a6, u8 *a7) {
    /* TODO (Clean version): Replace raw byte indexing with Camera struct member accesses:
     *   CamE_ *cam = &((CamE_ *)D_80118558)[i];
     *   *a1 = *(Pos_ *)&cam->pos;
     *   a2[0] = cam->m[0][0]; a2[1] = cam->m[1][0]; a2[2] = cam->m[2][0];
     *   a3[0] = cam->m[0][1]; a3[1] = cam->m[1][1]; a3[2] = cam->m[2][1];
     *   a4[0] = cam->m[0][2]; a4[1] = cam->m[1][2]; a4[2] = cam->m[2][2];
     *   *a5 = cam->fov;
     *   *a6 = (cam->mode == 0);
     *   *a7 = cam->car;
     */
    u8 *p = D_80118558 + i * 0x64;

    *a1 = *(Pos_ *)(p + 4);
    a2[0] = *(f32 *)(p + 0x1C);
    a2[1] = *(f32 *)(p + 0x28);
    a2[2] = *(f32 *)(p + 0x34);
    a3[0] = *(f32 *)(p + 0x20);
    a3[1] = *(f32 *)(p + 0x2C);
    a3[2] = *(f32 *)(p + 0x38);
    a4[0] = *(f32 *)(p + 0x24);
    a4[1] = *(f32 *)(p + 0x30);
    a4[2] = *(f32 *)(p + 0x3C);
    *a5 = *(f32 *)(p + 0x40);
    *a6 = *(s32 *)(p + 0x44) == 0;
    *a7 = p[0x49];
}

void func_800904C4(V12_ *a, V12_ *b, V3f_ *c, V3f_ *d) {
    /* TODO (Clean version): D_8011855C, D_80118568, and D_80118578 are pos, vel, and rot columns
     * of the first camera in D_80118558:
     *   CamE_ *cam0 = (CamE_ *)D_80118558;
     *   *a = *(V12_ *)&cam0->pos;
     *   *b = *(V12_ *)&cam0->vel;
     *   c->x = cam0->m[0][1]; c->y = cam0->m[1][1]; c->z = cam0->m[2][1];
     *   d->x = cam0->m[0][2]; d->y = cam0->m[1][2]; d->z = cam0->m[2][2];
     */
    *a = D_8011855C;
    *b = D_80118568;
    c->x = D_80118578[0][0];
    c->y = D_80118578[1][0];
    c->z = D_80118578[2][0];
    d->x = D_80118578[0][1];
    d->y = D_80118578[1][1];
    d->z = D_80118578[2][1];
}


void func_80090550(s32 arg0, s8 arg1) {
    /* TODO (Clean version): &D_801185A2 is &((CamE_ *)D_80118558)[0].b4A (offset 0x4A). Refactor as:
     *   ((CamE_ *)D_80118558)[arg0 & 0xFF].b4A = arg1;
     */
    *(u8 *)(&D_801185A2 + ((arg0 & 0xFF) * 0x64)) = arg1;
}

void func_80090578(Cam90_ *cam, u32 dt) {
    Vec3f_ pos;
    Vec3f_ p;
    Vec3f_ d;
    u8 b[0x28];
    s32 flags;
    s32 x;
    f32 radius;
    f32 mind;
    f32 dist;
    u32 i;
    u32 j;
    u32 count;
    u32 best;
    u32 bestCount;
    u32 bestPt;
    u32 bestCar;
    Car *car;
    s32 v;
    s32 w;

    best = 0;
    radius = 0.0f;
    bestCar = 0;
    if (cam->timer < dt) {
        bestCount = 0;
        bestPt = 0;
        for (i = 0; i < D_80141718(); i++) {
            D_8014171C(i, &pos, b, &flags, &x);
            /* TODO (Clean version / Bug check): 'flags |= 1;' forces the 'if (flags & 1)' branch,
             * making the 'else' branch (radius = 0.0f) dead code. Verify if flags was intended
             * to be tested without unconditional mutation:
             *   radius = (flags & 1) ? 5400.0f : 0.0f;
             */
            flags |= 1;
            if (flags & 2) {
                continue;
            }
            count = 0;
            if (flags & 1) {
                p = pos;
                radius = 5400.0f;
            } else {
                radius = 0.0f;
            }
            mind = 0.0f;
            for (j = 0; j < func_80096100(); j++) {
                car = func_800960D8(j);
                d.x = p.x - car->pos.x;
                d.y = p.y - car->pos.y;
                d.z = p.z - car->pos.z;
                dist = sqrtf(d.x * d.x + d.y * d.y + d.z * d.z);
                if (dist < radius && 0.0f < d.x * car->vel.x + d.y * car->vel.y + d.z * car->vel.z) {
                    count++;
                    if (mind < dist) {
                        mind = dist;
                        best = j;
                    }
                }
            }
            if (bestCount < count) {
                bestCount = count;
                bestPt = i;
                bestCar = best;
            }
        }
        if (bestCount != 0) {
            v = flags;
            cam->car = bestCar;
            cam->pt = bestPt;
            /* TODO (Clean version / Bug check): '(-(v & 1) & 3) | 2' simplifies to (v & 1) ? 3 : 2.
             * Because 'flags |= 1' is unconditionally executed in the loop above, v & 1 is always 1 (mode 3).
             * Clean equivalent:
             *   cam->mode = (v & 1) ? 3 : 2;
             */
            cam->mode = (-(v & 1) & 3) | 2;
        } else {
            do {
                cam->pt = D_8014169C(D_80118621);
            } while ((w = D_80118624[cam->pt * 5]) == 0);
            cam->mode = w;
            for (j = 0; j < 10; j++) {
                cam->car = D_8014169C(func_80096100());
                if (func_800960D8(cam->car)->b4B8 == 0) {
                    break;
                }
            }
        }
        cam->b4C = 1;
        cam->timer = 4000;
    }
    cam->timer -= dt;
}

void func_800908A0(void) {

}

s32 func_800908A8(void) {
    s32 n = func_800A5894();

    if (n == 0) {
        return 0;
    }
    return (n - 1) * 200;
}

    /* TODO (Clean version): D_801185B4[i].on and D_801185B8[i].w correspond to fields b5C (0x5C)
    * and b60 (0x60) of CamE_:
    *   for (i = 0; i < D_80118620; i++) {
    *       if (gCameras[i].b5C != 0) {
    *           gCameras[i].b60 = 5000;
    *           gCameras[i].b5C = 0;
    *       }
    *   }
    */
void func_800908E8(void) {
    u32 i;

    for (i = 0; i < D_80118620; i++) {
        D_801185B4[i].on = 1;
    }
}


void func_80090934(void) {
    u32 i;

    for (i = 0; i < D_80118620; i++) {
        if (D_801185B4[i].on != 0) {
            D_801185B8[i].w = 5000;
            D_801185B4[i].on = 0;
        }
    }
}


INCLUDE_ASM("asm/nonmatchings/unit_2F4F0", func_8009099C);

INCLUDE_ASM("asm/nonmatchings/unit_2F4F0", func_80090D80);

INCLUDE_ASM("asm/nonmatchings/unit_2F4F0", func_80090FA8);

    /* TODO (Clean version): Symbols &D_801185A4 and &D_801185AC represent fields b4C (0x4C)
    * and shake (0x54) of ((CamE_ *)D_80118558)[arg0 & 0xFF]:
    *   ((CamE_ *)D_80118558)[arg0 & 0xFF].b4C = 1;
    *   ((CamE_ *)D_80118558)[arg0 & 0xFF].shake = 0xFA;
    */
void func_800911B4(s32 arg0) {
    *(u8 *)(&D_801185A4 + ((arg0 & 0xFF) * 0x64)) = 1;
}

void func_800911E0(s32 arg0) {
    *(s32 *)(&D_801185AC + ((arg0 & 0xFF) * 0x64)) = 0xFA;
}
