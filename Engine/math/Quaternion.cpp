#include "engine_pch.h"
#include "Quaternion.h"

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
