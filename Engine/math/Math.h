#pragma once

#include "Definitions.h"

#ifdef EMBER_DOUBLE_PRECISION
    constexpr real PI       = 3.1415926535897932384626433832795;
    constexpr real TWO_PI   = 2.0 * PI;
    constexpr real HALF_PI  = 0.5 * PI;
    constexpr real RAD2DEG  = 180.0 / PI;
    constexpr real DEG2RAD  = PI / 180.0;
    constexpr real REAL_MIN = 2.2250738585072014e-308;
    constexpr real REAL_MAX = 1.7976931348623158e+308;
#else
    constexpr real PI       = 3.1415926535897932f;
    constexpr real TWO_PI   = 2.0f * PI;
    constexpr real HALF_PI  = 0.5f * PI;
    constexpr real RAD2DEG  = 180.0f / PI;
    constexpr real DEG2RAD  = PI / 180.0f;
    constexpr real REAL_MIN = 1.175494351e-38F;
    constexpr real REAL_MAX = 3.402823466e+38F;
#endif

real ENGINE_API Sqrt(real value);
real ENGINE_API FastInvSqrtApproxOne(real value);

real ENGINE_API Sin(real value);
real ENGINE_API Cos(real value);
real ENGINE_API Tan(real value);
real ENGINE_API Asin(real value);
real ENGINE_API Acos(real value);
real ENGINE_API Atan(real value);
real ENGINE_API Atan2(real sinAngle, real cosAngle);



bool ENGINE_API ApproxZero(real value);
