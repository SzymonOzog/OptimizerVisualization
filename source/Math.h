#pragma once
#include "DataStructures.h"

namespace Math
{
    inline float dotProduct(const Vec3& a, const Vec3& b)
    {
        return a.x * b.x + a.y * b.y + a.z * b.z;
    }
    
    inline Vec3 crossProduct(const Vec3& a, const Vec3& b)
    {
        Vec3 result;
        result.x = a.y * b.z - a.z * b.y;
        result.y = a.z * b.x - a.x * b.z;
        result.z = a.x * b.y - a.y * b.x;
        return result;
    }
    
    inline Vec3 hadamard(const Vec3& a, const Vec3& b)
    {
        return {a.x*b.x, a.y*b.y,a.z*b.z};
    }

    inline float distance(const Vec3& a, const Vec3& b, bool TwoD = false)
    {
        return Vec3({a.x-b.x, a.y-b.y, 0.f ? TwoD : (a.z-b.z)}).length();
    }

    template <typename T,
            typename CheckTplusT = decltype(std::declval<T>() + std::declval<T>()),
            typename CheckTtimesFloat = decltype(std::declval<T>() * std::declval<float>())>
    inline T lerp(const T& a, const T& b, float t)
    {
        return a * (1-t) + b * t;
    }

    static constexpr float PI = 3.14159265358979323846f;
}