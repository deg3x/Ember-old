#pragma once

#include "Definitions.h"

struct ENGINE_API Vector4
{
    real x;
    real y;
    real z;
    real w;

    constexpr Vector4() : x(0.0), y(0.0), z(0.0), w(0.0) {}
    constexpr Vector4(real x, real y, real z, real w) : x(x), y(y), z(z), w(w) {}

    real Length() const;
    real LengthSqr() const;
    Vector4 Normalize() const;

    bool IsEqual(const Vector4& vector, real error = EPSILON) const;
    bool IsZero(real error = EPSILON) const;
    
    std::string ToString() const;

    static real Dot(const Vector4& lhs, const Vector4& rhs);
    static real Length(const Vector4& vector);
    static real LengthSqr(const Vector4& vector);
    static Vector4 Normalize(const Vector4& vector);

    Vector4 operator-() const;
    
    Vector4& operator+=(const Vector4& rhs);
    Vector4& operator-=(const Vector4& rhs);
    Vector4& operator*=(const Vector4& rhs);

    Vector4& operator*=(real rhs);
    Vector4& operator/=(real rhs);

    real& operator[](int idx);
    const real& operator[](int idx) const;
};

inline Vector4 operator+(const Vector4& lhs, const Vector4& rhs)
{
    Vector4 ret = lhs;

    ret += rhs;
    
    return ret;
}

inline Vector4 operator-(const Vector4& lhs, const Vector4& rhs)
{
    Vector4 ret = lhs;

    ret -= rhs;
    
    return ret;
}

inline Vector4 operator*(const Vector4& lhs, const Vector4& rhs)
{
    Vector4 ret = lhs;

    ret *= rhs;
    
    return ret;
}

inline Vector4 operator*(const Vector4& lhs, real rhs)
{
    Vector4 ret = lhs;

    ret *= rhs;
    
    return ret;
}

inline Vector4 operator*(real lhs, const Vector4& rhs)
{
    Vector4 ret = rhs;

    ret *= lhs;
    
    return ret;
}

inline Vector4 operator/(const Vector4& lhs, real rhs)
{
    Vector4 ret = lhs;

    ret /= rhs;
    
    return ret;
}

inline Vector4 operator/(real lhs, const Vector4& rhs)
{
    Vector4 ret;

    ret.x = lhs / rhs.x;
    ret.y = lhs / rhs.y;
    ret.z = lhs / rhs.z;
    ret.w = lhs / rhs.w;
    
    return ret;
}