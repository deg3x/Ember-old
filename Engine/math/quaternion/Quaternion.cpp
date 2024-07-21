#include "engine_pch.h"
#include "Quaternion.h"

#include "logger/Logger.h"
#include "math/Math.h"
#include "math/matrix/Matrix4x4.h"

Quaternion::Quaternion(const Vector3& vector)
{
    w = 0.0;
    x = vector.x;
    y = vector.y;
    z = vector.z;
}

Quaternion::Quaternion(const Vector3& axis, real angle)
{
    const real halfAngle    = angle * 0.5f;
    const real halfAngleCos = Cos(halfAngle);
    const real halfAngleSin = Sin(halfAngle);
    
    w = halfAngleCos;
    x = halfAngleSin * axis.x;
    y = halfAngleSin * axis.y;
    z = halfAngleSin * axis.z;
}

Quaternion::Quaternion(const Matrix4x4& matrix)
{
    const real fourWSqr =   matrix[0][0] + matrix[1][1] + matrix[2][2] + matrix[3][3];
    const real fourXSqr =   matrix[0][0] - matrix[1][1] - matrix[2][2] + matrix[3][3];
    const real fourYSqr = - matrix[0][0] + matrix[1][1] - matrix[2][2] + matrix[3][3];
    const real fourZSqr = - matrix[0][0] - matrix[1][1] + matrix[2][2] + matrix[3][3];

    real maxComp   = fourWSqr;
    uint8_t maxIdx = 0;

    if (fourXSqr > maxComp)
    {
        maxComp = fourXSqr;
        maxIdx  = 1;
    }
    if (fourYSqr > maxComp)
    {
        maxComp = fourYSqr;
        maxIdx  = 2;
    }
    if (fourZSqr > maxComp)
    {
        maxComp = fourZSqr;
        maxIdx  = 3;
    }

    const real maxSqrt = Sqrt(maxComp);
    const real invTwoMaxSqrt = static_cast<real>(1.0) / static_cast<real>(2.0) * maxSqrt;

    switch (maxIdx)
    {
    case 0:
        w = static_cast<real>(0.5) * maxSqrt;
        x = (matrix[1][2] - matrix[2][1]) * invTwoMaxSqrt;
        y = (matrix[2][0] - matrix[0][2]) * invTwoMaxSqrt;
        z = (matrix[0][1] - matrix[1][0]) * invTwoMaxSqrt;
        return;
    case 1:
        w = (matrix[1][2] - matrix[2][1]) * invTwoMaxSqrt;
        x = static_cast<real>(0.5) * maxSqrt;
        y = (matrix[0][1] + matrix[1][0]) * invTwoMaxSqrt;
        z = (matrix[0][2] + matrix[2][0]) * invTwoMaxSqrt;
        return;
    case 2:
        w = (matrix[2][0] - matrix[0][2]) * invTwoMaxSqrt;
        x = (matrix[0][1] + matrix[1][0]) * invTwoMaxSqrt;
        y = static_cast<real>(0.5) * maxSqrt;
        z = (matrix[1][2] + matrix[2][1]) * invTwoMaxSqrt;
        return;
    case 3:
        w = (matrix[0][1] - matrix[1][0]) * invTwoMaxSqrt;
        x = (matrix[0][2] + matrix[2][0]) * invTwoMaxSqrt;
        y = (matrix[1][2] + matrix[2][1]) * invTwoMaxSqrt;
        z = static_cast<real>(0.5) * maxSqrt;
        return;
    default:
        Logger::Log(LogCategory::ERROR, "Unexpected index in matrix to quaternion conversion", "Quaternion(const Matrix4x4&)");
    }
}

Quaternion Quaternion::Inverse() const
{
    return Conjugate(*this) / LengthSqr(*this);
}

Quaternion& Quaternion::Normalize()
{
    const real length = Length(*this);

    w /= length;
    x /= length;
    y /= length;
    z /= length;

    return *this;
}

Quaternion& Quaternion::Renormalize()
{
    const real length = FastInvSqrtApproxOne(LengthSqr(*this));

    w /= length;
    x /= length;
    y /= length;
    z /= length;

    return *this;
}

Vector3 Quaternion::RotateVector(const Vector3& vector) const
{
    return Vector3(*this * Quaternion(vector) * Inverse(*this));
}

Vector3 Quaternion::ToEuler() const
{
    return {0, 0, 0};
}

real Quaternion::Dot(const Quaternion& lhs, const Quaternion& rhs)
{
    return lhs.w * rhs.w + lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z;
}

real Quaternion::Length(const Quaternion& quat)
{
    return Sqrt(LengthSqr(quat));
}

real  Quaternion::LengthSqr(const Quaternion& quat)
{
    return quat.w * quat.w + quat.x * quat.x + quat.y * quat.y + quat.z * quat.z;
}

Quaternion Quaternion::Inverse(const Quaternion& quat)
{
    return Conjugate(quat) / LengthSqr(quat);
}

Quaternion Quaternion::Normalize(const Quaternion& quat)
{
    return quat / Length(quat);
}

Quaternion Quaternion::Renormalize(const Quaternion& quat)
{
    const real length = FastInvSqrtApproxOne(LengthSqr(quat));

    return quat / length;
}

Quaternion Quaternion::Conjugate(const Quaternion& quat)
{
    Quaternion ret;

    ret.w =  quat.w;
    ret.x = -quat.x;
    ret.y = -quat.y;
    ret.z = -quat.z;

    return ret;
}

Vector3 Quaternion::ToEuler(const Quaternion& quat)
{
    return {0, 0, 0};
}

Quaternion Quaternion::FromEuler(real pitch, real yaw, real roll)
{
    // We rotate in X -> Y -> Z by pitch, yaw, roll angles (in radians) respectively
    const real cosP = Cos(pitch * static_cast<real>(0.5));
    const real cosY = Cos(yaw * static_cast<real>(0.5));
    const real cosR = Cos(roll * static_cast<real>(0.5));
    const real sinP = Sin(pitch * static_cast<real>(0.5));
    const real sinY = Sin(yaw * static_cast<real>(0.5));
    const real sinR = Sin(roll * static_cast<real>(0.5));
    
    const Quaternion ret = {
        cosP * cosY * cosR + sinP * sinY * sinR,
        sinP * cosY * cosR - cosP * sinY * sinR,
        cosP * sinY * cosR + sinP * cosY * sinR,
        cosP * cosY * sinR - sinP * sinY * cosR,
    };
    
    return ret;
}

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

Quaternion& Quaternion::operator+=(real rhs)
{
    w += rhs;

    return *this;
}

Quaternion& Quaternion::operator-=(real rhs)
{
    w -= rhs;

    return *this;
}

Quaternion& Quaternion::operator*=(real rhs)
{
    w *= rhs;
    x *= rhs;
    y *= rhs;
    z *= rhs;

    return *this;
}

Quaternion& Quaternion::operator/=(real rhs)
{
    w /= rhs;
    x /= rhs;
    y /= rhs;
    z /= rhs;

    return *this;
}
