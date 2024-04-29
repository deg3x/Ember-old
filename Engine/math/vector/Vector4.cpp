#include "engine_pch.h"
#include "Vector4.h"

#include "math/Math.h"

Vector4& Vector4::Normalize()
{
    const real length = Length(*this);

    x /= length;
    y /= length;
    z /= length;
    w /= length;

    return *this;
}

real Vector4::Dot(const Vector4& lhs, const Vector4& rhs)
{
    return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z + lhs.w * rhs.w;
}

real Vector4::Length(const Vector4& vector)
{
    return Sqrt(LengthSqr(vector));
}

real Vector4::LengthSqr(const Vector4& vector)
{
    return Dot(vector, vector);
}

Vector4 Vector4::Normalize(const Vector4& vector)
{
    return vector / Length(vector);
}

Vector4& Vector4::operator+=(const Vector4& rhs)
{
    x += rhs.x;
    y += rhs.y;
    z += rhs.z;
    w += rhs.w;

    return *this;
}

Vector4& Vector4::operator-=(const Vector4& rhs)
{
    x -= rhs.x;
    y -= rhs.y;
    z -= rhs.z;
    w -= rhs.w;

    return *this;
}

Vector4& Vector4::operator*=(real rhs)
{
    x *= rhs;
    y *= rhs;
    z *= rhs;
    w *= rhs;

    return *this;
}

Vector4& Vector4::operator/=(real rhs)
{
    x /= rhs;
    y /= rhs;
    z /= rhs;
    w /= rhs;

    return *this;
}