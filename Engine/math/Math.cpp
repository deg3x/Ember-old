#include "engine_pch.h"
#include "Math.h"

real ENGINE_API Abs(real value)
{
    return value < 0.0 ? -value : value;
}

real ENGINE_API Sign(real value)
{
    if (value < 0.0)
    {
        return -1.0;
    }

    if (value > 0.0)
    {
        return 1.0;
    }

    return 0.0;
}

real Sqrt(real value)
{
    // Shameless implementation
    // Change in the future
    return glm::sqrt(value);
}

real FastInvSqrtApproxOne(real value)
{
    constexpr real a =  1.875f;
    constexpr real b = -1.250f;
    constexpr real c =  0.375f;

    return a + b * value + c * value * value;
}

real Sin(real angle)
{
    return glm::sin(angle);
}

real Cos(real angle)
{
    return glm::cos(angle);
}


inline float Tan(float angle)
{
    return glm::tan(angle);
}

real Asin(real sinAngle)
{
    return glm::asin(sinAngle);
}

real Acos(real cosAngle)
{
    return glm::acos(cosAngle);
}

inline float Atan(float tanAngle)
{
    return glm::atan(tanAngle);
}

inline float Atan2(float sinAngle, float cosAngle)
{
    return glm::atan(sinAngle, cosAngle);
}

bool ApproxZero(real value)
{
    return (value > -EPSILON && value < EPSILON);
}

bool ENGINE_API ApproxEqual(real valueA, real valueB, real error)
{
    return (valueA + error > valueB) && (valueA - error < valueB);
}
