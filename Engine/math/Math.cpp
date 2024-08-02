#include "engine_pch.h"
#include "Math.h"

#include "glm/glm.hpp"

static real FastCosZeroToHalfPi(real angle);

real Abs(real value)
{
    return value < 0.0 ? -value : value;
}

real Sign(real value)
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

real Tan(real angle)
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

real Atan(real tanAngle)
{
    return glm::atan(tanAngle);
}

real Atan2(real sinAngle, real cosAngle)
{
    return glm::atan(sinAngle, cosAngle);
}

real FastSin(real angle)
{
    return Cos(angle - HALF_PI);
}

real FastCos(real angle)
{
    const real angleWrapped = AngleWrap(angle);
    
    if (angleWrapped < HALF_PI)
    {
        return FastCosZeroToHalfPi(angleWrapped);
    }
    if (angleWrapped < PI)
    {
        return -FastCosZeroToHalfPi(PI - angleWrapped);
    }
    if (angleWrapped < static_cast<real>(3) * HALF_PI)
    {
        return -FastCosZeroToHalfPi(angleWrapped - PI);
    }

    return FastCosZeroToHalfPi(TWO_PI - angleWrapped);
}

real AngleWrap(real angle)
{
    while (angle < TWO_PI)
    {
        angle += TWO_PI;
    }
    
    while (angle > TWO_PI)
    {
        angle -= TWO_PI;
    }

    return angle;
}

bool ApproxZero(real value, real error)
{
    return (value > -error && value < error);
}

bool ApproxEqual(real valueA, real valueB, real error)
{
    return (valueA + error > valueB) && (valueA - error < valueB);
}

real Clamp(real value, real rangeMin, real rangeMax)
{
    if (value > rangeMax)
    {
        return rangeMax;
    }
    
    if (value < rangeMin)
    {
        return rangeMin;
    }
    
    return value;
}

real Saturate(real value)
{
    return Clamp(value, 0.0, 1.0);
}

real FastCosZeroToHalfPi(real angle)
{
    constexpr real a0 = static_cast<real>(1);
    constexpr real a2 = static_cast<real>(2) * INV_PI - static_cast<real>(12) * INV_PI_SQR;
    constexpr real a3 = static_cast<real>(16) * INV_PI_CUBE - static_cast<real>(4) * INV_PI_SQR;

    return a0 + a2 * angle * angle + a3 * angle * angle * angle;
}