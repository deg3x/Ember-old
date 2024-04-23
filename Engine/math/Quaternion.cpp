#include "engine_pch.h"
#include "Quaternion.h"

#include "Math.h"

float Quaternion::Dot(const Quaternion& lhs, const Quaternion& rhs)
{
    return lhs.w * rhs.w + lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z;
}

float Quaternion::Length(const Quaternion& quat)
{
    return Sqrt(LengthSqr(quat));
}

float  Quaternion::LengthSqr(const Quaternion& quat)
{
    return quat.w * quat.w + quat.x * quat.x + quat.y * quat.y + quat.z * quat.z;
}

Quaternion Quaternion::Inverse(const Quaternion& quat)
{
    return Conjugate(quat) / LengthSqr(quat);
}

Quaternion Quaternion::Normalize(const Quaternion& quat)
{
    return quat / Length(quat);
}

Quaternion Quaternion::Conjugate(const Quaternion& quat)
{
    Quaternion ret;

    ret.w =  quat.w;
    ret.x = -quat.x;
    ret.y = -quat.y;
    ret.z = -quat.z;

    return ret;
}

Quaternion& Quaternion::operator+=(const Quaternion& rhs)
{
    x += rhs.x;
    y += rhs.y;
    z += rhs.z;
    w += rhs.w;

    return *this;
}

Quaternion& Quaternion::operator-=(const Quaternion& rhs)
{
    x -= rhs.x;
    y -= rhs.y;
    z -= rhs.z;
    w -= rhs.w;

    return *this;
}

Quaternion& Quaternion::operator*=(const Quaternion& rhs)
{
    w = w * rhs.w - x * rhs.x - y * rhs.y - z * rhs.z;
    x = w * rhs.x + x * rhs.w + y * rhs.z - z * rhs.y;
    y = w * rhs.y - x * rhs.z + y * rhs.w + z * rhs.x;
    z = w * rhs.z + x * rhs.y - y * rhs.x + z * rhs.w;

    return *this;
}

Quaternion& Quaternion::operator/=(const Quaternion& rhs)
{
    *this *= 1.0f / rhs;

    return *this;
}

Quaternion& Quaternion::operator+=(float rhs)
{
    w += rhs;

    return *this;
}

Quaternion& Quaternion::operator-=(float rhs)
{
    w -= rhs;

    return *this;
}

Quaternion& Quaternion::operator*=(float rhs)
{
    w *= rhs;
    x *= rhs;
    y *= rhs;
    z *= rhs;

    return *this;
}

Quaternion& Quaternion::operator/=(float rhs)
{
    w /= rhs;
    x /= rhs;
    y /= rhs;
    z /= rhs;

    return *this;
}
