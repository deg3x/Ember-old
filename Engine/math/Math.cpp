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
    constexpr real a =  1.5f;
    constexpr real b = -0.5f;

    return a + b * value;
}