#pragma once

#include "Definitions.h"

struct ENGINE_API Quaternion
{
    float x;
    float y;
    float z;
    float w;
    
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
    Quaternion result = lhs;

    result += rhs;
    
    return result;
}

inline Quaternion operator-(const Quaternion& lhs, const Quaternion& rhs)
{
    Quaternion result = lhs;

    result -= rhs;
    
    return result;
}

inline Quaternion operator*(const Quaternion& lhs, const Quaternion& rhs)
{
    Quaternion result = lhs;

    result *= rhs;
    
    return result;
}

inline Quaternion operator/(const Quaternion& lhs, const Quaternion& rhs)
{
    Quaternion result = lhs;

    result /= rhs;
    
    return result;
}

inline Quaternion operator/(const Quaternion& lhs, float rhs)
{
    Quaternion result = lhs;

    result /= rhs;
    
    return result;
}

inline Quaternion operator/(float lhs, const Quaternion& rhs)
{
    Quaternion result;

    result.w = lhs / rhs.w;
    result.x = lhs / rhs.x;
    result.y = lhs / rhs.y;
    result.z = lhs / rhs.z;
    
    return result;
}