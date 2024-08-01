#include "engine_pch.h"
#include "Vector2.h"

#include "math/Math.h"

Vector2 Vector2::Normalize() const
{
    const real invLength = static_cast<real>(1.0) / Length(*this);

    return *this * invLength;
}

bool Vector2::IsEqual(const Vector2& vector, real error) const
{
    return ApproxEqual(x, vector.x, error)
        && ApproxEqual(y, vector.y, error);
}

bool Vector2::IsZero(real error) const
{
    return ApproxZero(x, error)
        && ApproxZero(y, error);
}

std::string Vector2::ToString() const
{
    return "{ "
        + std::to_string(x) + ", "
        + std::to_string(y) + " }";
}

real Vector2::Dot(const Vector2& lhs, const Vector2& rhs)
{
    return lhs.x * rhs.x + lhs.y * rhs.y;
}

real Vector2::Length(const Vector2& vector)
{
    return Sqrt(LengthSqr(vector));
}

real Vector2::LengthSqr(const Vector2& vector)
{
    return vector.x * vector.x + vector.y * vector.y;
}

Vector2 Vector2::Normalize(const Vector2& vector)
{
    return vector / Length(vector);
}

Vector2 Vector2::operator-() const
{
    return { -x, -y };
}

Vector2& Vector2::operator+=(const Vector2& rhs)
{
    x += rhs.x;
    y += rhs.y;

    return *this;
}

Vector2& Vector2::operator-=(const Vector2& rhs)
{
    x -= rhs.x;
    y -= rhs.y;

    return *this;
}

Vector2& Vector2::operator*=(const Vector2& rhs)
{
    x *= rhs.x;
    y *= rhs.y;
    
    return *this;
}

Vector2& Vector2::operator*=(real rhs)
{
    x *= rhs;
    y *= rhs;

    return *this;
}

Vector2& Vector2::operator/=(real rhs)
{
    const real invRhs = static_cast<real>(1.0) / rhs;
    
    *this *= invRhs;

    return *this;
}

real& Vector2::operator[](int idx)
{
    return *(&x + (idx % 2));
}

const real& Vector2::operator[](int idx) const
{
    return *(&x + (idx % 2));
}
