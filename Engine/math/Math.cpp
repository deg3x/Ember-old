#include "engine_pch.h"
#include "Math.h"

real Sqrt(real num)
{
    // Shameless implementation
    // Change in the future
    return glm::sqrt(num);
}

real FastInvSqrtApproxOne(real value)
{
    constexpr real a =  1.875f;
    constexpr real b = -1.250f;
    constexpr real c =  0.375f;

    return a + b * value + c * value * value;
}