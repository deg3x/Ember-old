#pragma once

#include "Definitions.h"

struct ENGINE_API Quaternion
{
    real x;
    real y;
    real z;
    real w;

    Quaternion& Normalize();

    static real Dot(const Quaternion& lhs, const Quaternion& rhs);
    static real Length(const Quaternion& quat);
    static real LengthSqr(const Quaternion& quat);
    static Quaternion Inverse(const Quaternion& quat);
    static Quaternion Normalize(const Quaternion& quat);
    static Quaternion Conjugate(const Quaternion& quat);
    
    Quaternion& operator+=(const Quaternion& rhs);
    Quaternion& operator-=(const Quaternion& rhs);
    Quaternion& operator*=(const Quaternion& rhs);
    Quaternion& operator/=(const Quaternion& rhs);
    
    Quaternion& operator+=(real rhs);
    Quaternion& operator-=(real rhs);
    Quaternion& operator*=(real rhs);
    Quaternion& operator/=(real rhs);
};

inline Quaternion operator+(const Quaternion& lhs, const Quaternion& rhs)
{
    Quaternion ret = lhs;

    ret += rhs;
    
    return ret;
}

inline Quaternion operator-(const Quaternion& lhs, const Quaternion& rhs)
{
    Quaternion ret = lhs;

    ret -= rhs;
    
    return ret;
}

inline Quaternion operator*(const Quaternion& lhs, const Quaternion& rhs)
{
    Quaternion ret = lhs;

    ret *= rhs;
    
    return ret;
}

inline Quaternion operator/(const Quaternion& lhs, const Quaternion& rhs)
{
    Quaternion ret = lhs;

    ret /= rhs;
    
    return ret;
}

inline Quaternion operator*(const Quaternion& lhs, real rhs)
{
    Quaternion ret = lhs;

    ret *= rhs;
    
    return ret;
}

inline Quaternion operator*(real lhs, const Quaternion& rhs)
{
    Quaternion ret = rhs;

    ret *= lhs;
    
    return ret;
}

inline Quaternion operator/(const Quaternion& lhs, real rhs)
{
    Quaternion ret = lhs;

    ret /= rhs;
    
    return ret;
}

inline Quaternion operator/(real lhs, const Quaternion& rhs)
{
    Quaternion ret;

    ret.w = lhs / rhs.w;
    ret.x = lhs / rhs.x;
    ret.y = lhs / rhs.y;
    ret.z = lhs / rhs.z;
    
    return ret;
}