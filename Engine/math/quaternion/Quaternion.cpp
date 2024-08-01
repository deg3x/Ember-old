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
    const real invTwoMaxSqrt = static_cast<real>(1.0) / (static_cast<real>(2.0) * maxSqrt);

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

Quaternion Quaternion::Normalize() const
{
    const real invLength = static_cast<real>(1.0) / Length(*this);

    return *this * invLength;
}

Quaternion Quaternion::Renormalize() const
{
    const real invLength = FastInvSqrtApproxOne(LengthSqr(*this));

    return *this * invLength;
}

Quaternion Quaternion::Conjugate() const
{
    Quaternion ret;

    ret.w =  w;
    ret.x = -x;
    ret.y = -y;
    ret.z = -z;

    return ret;
}

Vector3 Quaternion::RotateVector(const Vector3& vector) const
{
    return Vector3(*this * Quaternion(vector) * Inverse(*this));
}

Vector3 Quaternion::ToEuler() const
{
    // Check if pitch is +/- 90 degrees, in which case we require special handling.
    const real pitchTest = w * x + y * z;

    if (ApproxEqual(Abs(pitchTest), 0.5))
    {
        return { HALF_PI, Sign(pitchTest) * Atan2(y, x), 0.0 };
    }
    
    real pitch = Atan2(static_cast<real>(2.0) * (w * x + y * z), static_cast<real>(1.0) - static_cast<real>(2.0) * (x * x + y * y));
    real yaw   = -Asin(static_cast<real>(2.0) * (x * z - w * y));
    real roll  = Atan2(static_cast<real>(2.0) * (w * z + x * y), static_cast<real>(1.0) - static_cast<real>(2.0) * (y * y + z * z));

    return { pitch, yaw, roll };
}

bool Quaternion::IsEqual(const Quaternion& quat, float error) const
{
    return ApproxEqual(w, quat.w, error)
        && ApproxEqual(x, quat.x, error)
        && ApproxEqual(y, quat.y, error)
        && ApproxEqual(z, quat.z, error);
}

bool Quaternion::IsZero(float error) const
{
    return ApproxZero(w, error)
        && ApproxZero(x, error)
        && ApproxZero(y, error)
        && ApproxZero(z, error);
}

std::string Quaternion::ToString() const
{
    return "{ "
        + std::to_string(w) + ", "
        + std::to_string(x) + ", "
        + std::to_string(y) + ", "
        + std::to_string(z) + " }";
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
    const real invLength = FastInvSqrtApproxOne(LengthSqr(quat));

    return quat * invLength;
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

Quaternion Quaternion::FromEuler(const Vector3& angles)
{
    return FromEuler(angles.x, angles.y, angles.z);
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

Quaternion Quaternion::operator-() const
{
    return {-w, -x, -y, -z};
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
    Quaternion result;

    result.w = w * rhs.w - x * rhs.x - y * rhs.y - z * rhs.z;
    result.x = w * rhs.x + x * rhs.w + y * rhs.z - z * rhs.y;
    result.y = w * rhs.y - x * rhs.z + y * rhs.w + z * rhs.x;
    result.z = w * rhs.z + x * rhs.y - y * rhs.x + z * rhs.w;

    w = result.w;
    x = result.x;
    y = result.y;
    z = result.z;

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
