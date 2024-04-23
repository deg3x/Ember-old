#include "engine_pch.h"
#include "Vector3D.h"

float Vector3D::Dot(const Vector3D& lhs, const Vector3D& rhs)
{
    return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z;
}

float Vector3D::Length(const Vector3D& vector)
{
    return glm::sqrt(LengthSqr(vector));
}

float Vector3D::LengthSqr(const Vector3D& vector)
{
    return vector.x * vector.x + vector.y * vector.y + vector.z * vector.z;
}

Vector3D Vector3D::Cross(const Vector3D& lhs, const Vector3D& rhs)
{
    Vector3D ret;

    ret.x = lhs.y * rhs.z - rhs.y * lhs.z;
    ret.y = lhs.x * rhs.z - rhs.x * lhs.z;
    ret.z = lhs.x * rhs.y - rhs.x * lhs.y;

    return ret;
}

Vector3D Vector3D::Normalize(const Vector3D& vector)
{
    return vector / Length(vector);
}

Vector3D& Vector3D::operator+=(const Vector3D& rhs)
{
    x += rhs.x;
    y += rhs.y;
    z += rhs.z;

    return *this;
}

Vector3D& Vector3D::operator-=(const Vector3D& rhs)
{
    x -= rhs.x;
    y -= rhs.y;
    z -= rhs.z;

    return *this;
}

Vector3D& Vector3D::operator*=(float rhs)
{
    x *= rhs;
    y *= rhs;
    z *= rhs;

    return *this;
}

Vector3D& Vector3D::operator/=(float rhs)
{
    x /= rhs;
    y /= rhs;
    z /= rhs;

    return *this;
}
