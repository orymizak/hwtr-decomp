#include "common.h"
#include "include_asm.h"



extern const f64 D_80101B08;
extern const f64 D_80101B10;
extern const f64 D_80101B18;
extern const f64 D_80101B20;
extern const f64 D_80101B28;
extern const f64 D_80101B30;
extern const f64 D_80101B38;
extern const f32 D_80101B40;
extern const f32 D_80101BE0;
f32 sinf(f32 x) {
    f64 dx, xsq, poly;
    f64 dn;
    s32 n;
    f64 result;
    s32 ix, xpt;

    ix = *(s32 *)&x;
    xpt = (ix >> 22);
    xpt &= 0x1ff;
    if (xpt < 0xff) {
        dx = x;
        if (xpt >= 0xe6) {
            xsq = dx * dx;
            poly = ((D_80101B20 * xsq + D_80101B18) * xsq + D_80101B10) * xsq + D_80101B08;
            result = dx + (dx * xsq) * poly;
            return ((f32)result);
        }
        return (x);
    }
    if (xpt < 0x136) {
        dx = x;
        dn = dx * D_80101B28;
        n = (s32)((dn >= 0.0) ? (dn + 0.5) : (dn - 0.5));
        dn = n;
        dx = dx - dn * D_80101B30;
        dx = dx - dn * D_80101B38;
        xsq = dx * dx;
        poly = ((D_80101B20 * xsq + D_80101B18) * xsq + D_80101B10) * xsq + D_80101B08;
        result = dx + (dx * xsq) * poly;
        if ((n & 1) == 0)
            return ((f32)result);
        return (-(f32)result);
    }
    if (x != x) {
        return (D_80101BE0);
    }
    return (D_80101B40);
}

INCLUDE_ASM("asm/nonmatchings/unit_7E470", bcopy);
