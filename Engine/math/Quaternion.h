#pragma once

#include "Definitions.h"

struct ENGINE_API Quaternion
{
    float x;
    float y;
    float z;
    float w;

    static float Dot(const Quaternion& lhs, const Quaternion& rhs);
    static float Length(const Quaternion& quat);
    static float LengthSqr(const Quaternion& quat);
    static Quaternion Inverse(const Quaternion& quat);
    static Quaternion Normalize(const Quaternion& quat);
    static Quaternion Conjugate(const Quaternion& quat);
    
    Quaternion& operator+=(const Quaternion& rhs);
    Quaternion& operator-=(const Quaternion& rhs);
    Quaternion& operator*=(const Quaternion& rhs);
    Quaternion& operator/=(const Quaternion& rhs);
    
    Quaternion& operator+=(float rhs);
    Quaternion& operator-=(float rhs);
    Quaternion& operator*=(float rhs);
    Quaternion& operator/=(float rhs);
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

inline Quaternion operator*(const Quaternion& lhs, float rhs)
{
    Quaternion ret = lhs;

    ret *= rhs;
    
    return ret;
}

inline Quaternion operator*(float lhs, const Quaternion& rhs)
{
    Quaternion ret = rhs;

    ret *= lhs;
    
    return ret;
}

inline Quaternion operator/(const Quaternion& lhs, float rhs)
{
    Quaternion ret = lhs;

    ret /= rhs;
    
    return ret;
}

inline Quaternion operator/(float lhs, const Quaternion& rhs)
{
    Quaternion ret;

    ret.w = lhs / rhs.w;
    ret.x = lhs / rhs.x;
    ret.y = lhs / rhs.y;
    ret.z = lhs / rhs.z;
    
    return ret;
}