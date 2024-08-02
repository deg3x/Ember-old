#pragma once

#include "Definitions.h"

#ifdef EMBER_DOUBLE_PRECISION
    constexpr real PI       = 3.1415926535897932384626433832795;
    constexpr real TWO_PI   = 6.28318530717958647692;
    constexpr real HALF_PI  = 1.57079632679489661923;
    constexpr real REAL_MIN = 2.2250738585072014e-308;
    constexpr real REAL_MAX = 1.7976931348623158e+308;
    constexpr real EPSILON  = 2.2204460492503131e-016;
#else
    constexpr real PI       = 3.1415926535897932f;
    constexpr real TWO_PI   = 6.28318530717f;
    constexpr real HALF_PI  = 1.57079632679f;
    constexpr real REAL_MIN = 1.175494351e-38f;
    constexpr real REAL_MAX = 3.402823466e+38f;
    constexpr real EPSILON  = 1.192092896e-07f;
#endif

constexpr real PI_SQR      = PI * PI;
constexpr real PI_CUBE     = PI * PI * PI;
constexpr real INV_PI_SQR  = static_cast<real>(1) / PI_SQR;
constexpr real INV_PI_CUBE = static_cast<real>(1) / PI_CUBE;
constexpr real INV_PI      = static_cast<real>(1) / PI;
constexpr real RAD2DEG     = static_cast<real>(180) / PI;
constexpr real DEG2RAD     = PI / static_cast<real>(180);

real ENGINE_API Abs(real value);
real ENGINE_API Sign(real value);
real ENGINE_API Sqrt(real value);
real ENGINE_API FastInvSqrtApproxOne(real value);

real ENGINE_API Sin(real angle);
real ENGINE_API Cos(real angle);
real ENGINE_API Tan(real angle);
real ENGINE_API Asin(real sinAngle);
real ENGINE_API Acos(real cosAngle);
real ENGINE_API Atan(real tanAngle);
real ENGINE_API Atan2(real sinAngle, real cosAngle);
real ENGINE_API FastSin(real angle);
real ENGINE_API FastCos(real angle);

real ENGINE_API AngleWrap(real angle);

bool ENGINE_API ApproxZero(real value, real error = EPSILON);
bool ENGINE_API ApproxEqual(real valueA, real valueB, real error = EPSILON);

real ENGINE_API Clamp(real value, real rangeMin, real rangeMax);
real ENGINE_API Saturate(real value);
