#pragma once

#include "Definitions.h"

struct Quaternion;

struct ENGINE_API Vector3
{
    real x;
    real y;
    real z;

    constexpr Vector3() : x(0.0), y(0.0), z(0.0) {}
    constexpr Vector3(real x, real y, real z) : x(x), y(y), z(z) {}
    explicit Vector3(const Quaternion& quat);

    real Length() const;
    real LengthSqr() const;
    Vector3 Normalize() const;

    bool IsEqual(const Vector3& vector, real error = EPSILON) const;
    bool IsZero(real error = EPSILON) const;
    
    Vector3 Rotate(const Vector3& axis, real angle) const;
    
    std::string ToString() const;

    static real Dot(const Vector3& lhs, const Vector3& rhs);
    static real Length(const Vector3& vector);
    static real LengthSqr(const Vector3& vector);
    static Vector3 Cross(const Vector3& lhs, const Vector3& rhs);
    static Vector3 Normalize(const Vector3& vector);

    Vector3 operator-() const;
    
    Vector3& operator+=(const Vector3& rhs);
    Vector3& operator-=(const Vector3& rhs);
    Vector3& operator*=(const Vector3& rhs);

    Vector3& operator*=(real rhs);
    Vector3& operator/=(real rhs);

    real& operator[](int idx);
    const real& operator[](int idx) const;
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

inline Vector3 operator*(const Vector3& lhs, const Vector3& rhs)
{
    Vector3 ret = lhs;

    ret *= rhs;
    
    return ret;
}

inline Vector3 operator*(const Vector3& lhs, real rhs)
{
    Vector3 ret = lhs;

    ret *= rhs;
    
    return ret;
}

inline Vector3 operator*(real lhs, const Vector3& rhs)
{
    Vector3 ret = rhs;

    ret *= lhs;
    
    return ret;
}

inline Vector3 operator/(const Vector3& lhs, real rhs)
{
    Vector3 ret = lhs;

    ret /= rhs;
    
    return ret;
}

inline Vector3 operator/(real lhs, const Vector3& rhs)
{
    Vector3 ret;

    ret.x = lhs / rhs.x;
    ret.y = lhs / rhs.y;
    ret.z = lhs / rhs.z;
    
    return ret;
}