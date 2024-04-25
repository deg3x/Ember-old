#pragma once

#include "Definitions.h"

constexpr real PI      = 3.1415926535897932384626433832795;
constexpr real TWO_PI  = 2.0 * PI;
constexpr real HALF_PI = 0.5 * PI;
constexpr real RAD2DEG = 180.0 / PI;
constexpr real DEG2RAD = PI / 180.0;

real ENGINE_API Sqrt(real value);
real ENGINE_API FastInvSqrtApproxOne(real value);

real ENGINE_API Sin(real value);
real ENGINE_API Cos(real value);
real ENGINE_API Asin(real value);
real ENGINE_API Acos(real value);