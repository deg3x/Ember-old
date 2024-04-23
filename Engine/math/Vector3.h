#pragma once

#include "Definitions.h"

struct ENGINE_API Vector3
{
    float x;
    float y;
    float z;

    static float Dot(const Vector3& lhs, const Vector3& rhs);
    static float Length(const Vector3& vector);
    static float LengthSqr(const Vector3& vector);
    static Vector3 Cross(const Vector3& lhs, const Vector3& rhs);
    static Vector3 Normalize(const Vector3& vector);
    
    Vector3& operator+=(const Vector3& rhs);
    Vector3& operator-=(const Vector3& rhs);

    Vector3& operator*=(float rhs);
    Vector3& operator/=(float rhs);
};

inline Vector3 operator+(const Vector3& lhs, const Vector3& rhs)
{
    Vector3 ret = lhs;

    ret += rhs;
    
    return ret;
}

inline Vector3 operator-(const Vector3& lhs, const Vector3& rhs)
{
    Vector3 ret = lhs;

    ret -= rhs;
    
    return ret;
}

inline Vector3 operator/(const Vector3& lhs, float rhs)
{
    Vector3 ret = lhs;

    ret /= rhs;
    
    return ret;
}

inline Vector3 operator/(float lhs, const Vector3& rhs)
{
    Vector3 ret;

    ret.x = lhs / rhs.x;
    ret.y = lhs / rhs.y;
    ret.z = lhs / rhs.z;
    
    return ret;
}