#include "engine_pch.h"
#include "Vector3D.h"

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
