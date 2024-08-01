#include "engine_pch.h"
#include "Vector4.h"

#include "math/Math.h"

real Vector4::Length() const
{
    return Sqrt(x * x + y * y + z * z + w * w);
}

real Vector4::LengthSqr() const
{
    return x * x + y * y + z * z + w * w;
}

Vector4 Vector4::Normalize() const
{
    const real invLength = static_cast<real>(1.0) / Length(*this);

    return *this * invLength;
}

bool Vector4::IsEqual(const Vector4& vector, real error) const
{
    return ApproxEqual(x, vector.x, error)
        && ApproxEqual(y, vector.y, error)
        && ApproxEqual(z, vector.z, error)
        && ApproxEqual(w, vector.w, error);
}

bool Vector4::IsZero(real error) const
{
    return ApproxZero(x, error)
        && ApproxZero(y, error)
        && ApproxZero(z, error)
        && ApproxZero(w, error);
}

std::string Vector4::ToString() const
{
    return "{ "
        + std::to_string(x) + ", "
        + std::to_string(y) + ", "
        + std::to_string(z) + ", "
        + std::to_string(w) + " }";
}

real Vector4::Dot(const Vector4& lhs, const Vector4& rhs)
{
    return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z + lhs.w * rhs.w;
}

real Vector4::Length(const Vector4& vector)
{
    return Sqrt(vector.x * vector.x + vector.y * vector.y + vector.z * vector.z + vector.w * vector.w);
}

real Vector4::LengthSqr(const Vector4& vector)
{
    return vector.x * vector.x + vector.y * vector.y + vector.z * vector.z + vector.w * vector.w;
}

Vector4 Vector4::Normalize(const Vector4& vector)
{
    return vector / Length(vector);
}

Vector4 Vector4::operator-() const
{
    return { -x, -y, -z, -w };
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

Vector4& Vector4::operator*=(const Vector4& rhs)
{
    x *= rhs.x;
    y *= rhs.y;
    z *= rhs.z;
    w *= rhs.w;

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

real& Vector4::operator[](int idx)
{
    return *(&x + (idx % 4));
}

const real& Vector4::operator[](int idx) const
{
    return *(&x + (idx % 4));
}