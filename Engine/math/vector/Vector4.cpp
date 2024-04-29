#include "engine_pch.h"
#include "Vector4.h"

#include "math/Math.h"

Vector4& Vector4::Normalize()
{
    const real invLength = static_cast<real>(1.0) / Length(*this);

    *this *= invLength;

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
    return vector.x * vector.x + vector.y * vector.y + vector.z * vector.z + vector.w * vector.w;
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
    const real invRhs = static_cast<real>(1.0) / rhs;
    
    *this *= invRhs;

    return *this;
}