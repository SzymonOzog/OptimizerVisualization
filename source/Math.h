#pragma once
#include "DataStructures.h"

namespace Math
{
    inline float DotProduct(const Vec3& a, const Vec3& b)
    {
        return a.x * b.x + a.y * b.y + a.z * b.z;
    }
    inline Vec3 CrossProduct(const Vec3& a, const Vec3& b)
    {
        Vec3 result;
        result.x = a.y * b.z - a.z * b.y;
        result.y = a.z * b.x - a.x * b.z;
        result.z = a.x * b.y - a.y * b.x;
        return result;
    }
    inline Vec3 Hadamard(const Vec3& a, const Vec3& b)
    {
        return {a.x*b.x, a.y*b.y,a.z*b.z};
    }
}