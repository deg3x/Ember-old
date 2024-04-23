#include "engine_pch.h"
#include "Vector2.h"

#include "math/Math.h"

Vector2& Vector2::Normalize()
{
    const real length = Length(*this);

    x /= length;
    y /= length;

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
    x /= rhs;
    y /= rhs;

    return *this;
}
