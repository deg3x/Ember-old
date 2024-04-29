#include "engine_pch.h"
#include "Vector2.h"

#include "math/Math.h"

Vector2& Vector2::Normalize()
{
    const real invLength = static_cast<real>(1.0) / Length(*this);

    *this *= invLength;

    return *this;
}

float Vector2::Dot(const Vector2& lhs, const Vector2& rhs)
{
    return lhs.x * rhs.x + lhs.y * rhs.y;
}

float Vector2::Length(const Vector2& vector)
{
    return Sqrt(LengthSqr(vector));
}

float Vector2::LengthSqr(const Vector2& vector)
{
    return vector.x * vector.x + vector.y * vector.y;
}

Vector2 Vector2::Normalize(const Vector2& vector)
{
    return vector / Length(vector);
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

Vector2& Vector2::operator*=(float rhs)
{
    x *= rhs;
    y *= rhs;

    return *this;
}

Vector2& Vector2::operator/=(float rhs)
{
    const real invRhs = static_cast<real>(1.0) / rhs;
    
    *this *= invRhs;

    return *this;
}

float Vector2::operator[](int idx) const
{
    return *(&x + (idx % 2));
}