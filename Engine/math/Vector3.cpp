#include "engine_pch.h"
#include "Vector3.h"

#include "Math.h"

float Vector3::Dot(const Vector3& lhs, const Vector3& rhs)
{
    return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z;
}

float Vector3::Length(const Vector3& vector)
{
    return Sqrt(LengthSqr(vector));
}

float Vector3::LengthSqr(const Vector3& vector)
{
    return vector.x * vector.x + vector.y * vector.y + vector.z * vector.z;
}

Vector3 Vector3::Cross(const Vector3& lhs, const Vector3& rhs)
{
    Vector3 ret;

    ret.x = lhs.y * rhs.z - rhs.y * lhs.z;
    ret.y = lhs.x * rhs.z - rhs.x * lhs.z;
    ret.z = lhs.x * rhs.y - rhs.x * lhs.y;

    return ret;
}

Vector3 Vector3::Normalize(const Vector3& vector)
{
    return vector / Length(vector);
}

Vector3& Vector3::operator+=(const Vector3& rhs)
{
    x += rhs.x;
    y += rhs.y;
    z += rhs.z;

    return *this;
}

Vector3& Vector3::operator-=(const Vector3& rhs)
{
    x -= rhs.x;
    y -= rhs.y;
    z -= rhs.z;

    return *this;
}

Vector3& Vector3::operator*=(float rhs)
{
    x *= rhs;
    y *= rhs;
    z *= rhs;

    return *this;
}

Vector3& Vector3::operator/=(float rhs)
{
    x /= rhs;
    y /= rhs;
    z /= rhs;

    return *this;
}
