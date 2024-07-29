#pragma once

#include "Definitions.h"
#include "math/vector/Vector3.h"

struct Matrix4x4;

// NOTE: Some of the member functions work under the assumption
// that the quaternions are already normalized (for simplicity and efficiency).
// We shouldn't really be working with non-normalized quaternions anyway.
struct ENGINE_API Quaternion
{
    real w;
    real x;
    real y;
    real z;

    Quaternion() : w(1.0), x(0.0), y(0.0), z(0.0) {}
    Quaternion(real w, real x, real y, real z) : w(w), x(x), y(y), z(z) {}
    explicit Quaternion(const Vector3& vector);
    Quaternion(const Vector3& axis, real angle);
    Quaternion(const Matrix4x4& matrix);

    Quaternion Inverse() const;
    Quaternion Normalize() const;
    Quaternion Renormalize() const;
    Vector3 RotateVector(const Vector3& vector) const;
    Vector3 ToEuler() const;

    bool IsEqual(const Quaternion& quat, real error = EPSILON) const;
    bool IsZero(real error = EPSILON) const;

    static real Dot(const Quaternion& lhs, const Quaternion& rhs);
    static real Length(const Quaternion& quat);
    static real LengthSqr(const Quaternion& quat);
    static Quaternion Inverse(const Quaternion& quat);
    static Quaternion Normalize(const Quaternion& quat);
    static Quaternion Renormalize(const Quaternion& quat);
    static Quaternion Conjugate(const Quaternion& quat);
    
    static Quaternion FromEuler(const Vector3& angles);
    static Quaternion FromEuler(real pitch, real yaw, real roll);

    Quaternion operator-() const;
    
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

inline Vector3 operator*(const Quaternion& lhs, const Vector3& rhs)
{
    const Vector3 ret = Vector3(lhs * Quaternion(rhs) * lhs.Inverse());

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