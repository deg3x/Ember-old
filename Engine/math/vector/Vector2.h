#pragma once

#include "Definitions.h"

struct ENGINE_API Vector2
{
    real x;
    real y;

    Vector2& Normalize();

    static real Dot(const Vector2& lhs, const Vector2& rhs);
    static real Length(const Vector2& vector);
    static real LengthSqr(const Vector2& vector);
    static Vector2 Normalize(const Vector2& vector);
    
    Vector2& operator+=(const Vector2& rhs);
    Vector2& operator-=(const Vector2& rhs);

    Vector2& operator*=(real rhs);
    Vector2& operator/=(real rhs);
};

inline Vector2 operator+(const Vector2& lhs, const Vector2& rhs)
{
    Vector2 ret = lhs;

    ret += rhs;
    
    return ret;
}

inline Vector2 operator-(const Vector2& lhs, const Vector2& rhs)
{
    Vector2 ret = lhs;

    ret -= rhs;
    
    return ret;
}

inline Vector2 operator/(const Vector2& lhs, real rhs)
{
    Vector2 ret = lhs;

    ret /= rhs;
    
    return ret;
}

inline Vector2 operator/(real lhs, const Vector2& rhs)
{
    Vector2 ret;

    ret.x = lhs / rhs.x;
    ret.y = lhs / rhs.y;
    
    return ret;
}