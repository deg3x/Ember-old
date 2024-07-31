#include "engine_pch.h"
#include "Matrix4x4.h"

#include "logger/Logger.h"
#include "math/Math.h"
#include "math/quaternion/Quaternion.h"

constexpr Matrix4x4::Matrix4x4(real diagonal)
{
    m[0] = Vector4(diagonal, 0.0, 0.0, 0.0);
    m[1] = Vector4(0.0, diagonal, 0.0, 0.0);
    m[2] = Vector4(0.0, 0.0, diagonal, 0.0);
    m[3] = Vector4(0.0, 0.0, 0.0, diagonal);
}

Matrix4x4::Matrix4x4(const Quaternion& quat)
{
    const real ww = quat.w * quat.w;
    const real wx = quat.w * quat.x;
    const real wy = quat.w * quat.y;
    const real wz = quat.w * quat.z;
    const real xx = quat.x * quat.x;
    const real xy = quat.x * quat.y;
    const real xz = quat.x * quat.z;
    const real yy = quat.y * quat.y;
    const real yz = quat.y * quat.z;
    const real zz = quat.z * quat.z;
    
    m[0][0] = static_cast<real>(2.0) * (ww + xx) - static_cast<real>(1.0);
    m[0][1] = static_cast<real>(2.0) * (xy + wz);
    m[0][2] = static_cast<real>(2.0) * (xz - wy);
    m[0][3] = 0.0;
    m[1][0] = static_cast<real>(2.0) * (xy - wz);
    m[1][1] = static_cast<real>(2.0) * (ww + yy) - static_cast<real>(1.0);
    m[1][2] = static_cast<real>(2.0) * (wx + yz);
    m[1][3] = 0.0;
    m[2][0] = static_cast<real>(2.0) * (wy + xz);
    m[2][1] = static_cast<real>(2.0) * (yz - wx);
    m[2][2] = static_cast<real>(2.0) * (ww + zz) - static_cast<real>(1.0);
    m[3][0] = 0.0;
    m[3][1] = 0.0;
    m[3][2] = 0.0;
    m[3][3] = static_cast<real>(1.0);
}

Matrix4x4::Matrix4x4(const Vector4& colA, const Vector4& colB, const Vector4& colC, const Vector4& colD)
{
    m[0] = colA;
    m[1] = colB;
    m[2] = colC;
    m[3] = colD;
}

real Matrix4x4::Determinant() const
{
    const real det2d_23 = m[2][2] * m[3][3] - m[2][3] * m[3][2];
    const real det2d_13 = m[1][2] * m[3][3] - m[1][3] * m[3][2];
    const real det2d_12 = m[1][2] * m[2][3] - m[1][3] * m[2][2];
    const real det2d_03 = m[0][2] * m[3][3] - m[0][3] * m[3][2];
    const real det2d_02 = m[0][2] * m[2][3] - m[0][3] * m[2][2];
    const real det2d_01 = m[0][2] * m[1][3] - m[0][3] * m[1][2];

    const real det3d_123 = m[1][1] * det2d_23 - m[2][1] * det2d_13 + m[3][1] * det2d_12;
    const real det3d_023 = m[0][1] * det2d_23 - m[2][1] * det2d_03 + m[3][1] * det2d_02;
    const real det3d_013 = m[0][1] * det2d_13 - m[1][1] * det2d_03 + m[3][1] * det2d_01;
    const real det3d_012 = m[0][1] * det2d_12 - m[1][1] * det2d_02 + m[2][1] * det2d_01;
    
    return m[0][0] * det3d_123 - m[1][0] * det3d_023 + m[2][0] * det3d_013 - m[3][0] * det3d_012;
}

Matrix4x4 Matrix4x4::Inverse() const
{
    const real det_23_23 = m[2][2] * m[3][3] - m[2][3] * m[3][2];
    const real det_23_13 = m[1][2] * m[3][3] - m[1][3] * m[3][2];
    const real det_23_12 = m[1][2] * m[2][3] - m[1][3] * m[2][2];
    const real det_23_03 = m[0][2] * m[3][3] - m[0][3] * m[3][2];
    const real det_23_02 = m[0][2] * m[2][3] - m[0][3] * m[2][2];
    const real det_23_01 = m[0][2] * m[1][3] - m[0][3] * m[1][2];

    const real det_123_123 = m[1][1] * det_23_23 - m[2][1] * det_23_13 + m[3][1] * det_23_12;
    const real det_123_023 = m[0][1] * det_23_23 - m[2][1] * det_23_03 + m[3][1] * det_23_02;
    const real det_123_013 = m[0][1] * det_23_13 - m[1][1] * det_23_03 + m[3][1] * det_23_01;
    const real det_123_012 = m[0][1] * det_23_12 - m[1][1] * det_23_02 + m[2][1] * det_23_01;

    const real determinant = m[0][0] * det_123_123 - m[1][0] * det_123_023 + m[2][0] * det_123_013 - m[3][0] * det_123_012;

    if (ApproxZero(determinant))
    {
        LogEntry entry;
        entry.context  = "Matrix4x4::Inverse";
        entry.message  = "Attempting to inverse non-invertible matrix";
        entry.category = LogCategory::ERROR;

        Logger::Log(entry);
        
        return Matrix4x4();
    }

    const real det_13_23 = m[2][1] * m[3][3] - m[2][3] * m[3][1];
    const real det_13_13 = m[1][1] * m[3][3] - m[1][3] * m[3][1];
    const real det_13_12 = m[1][1] * m[2][3] - m[1][3] * m[2][1];
    const real det_13_03 = m[0][1] * m[3][3] - m[0][3] * m[3][1];
    const real det_13_02 = m[0][1] * m[2][3] - m[0][3] * m[2][1];
    const real det_13_01 = m[0][1] * m[1][3] - m[0][3] * m[1][1];
    
    const real det_12_23 = m[2][1] * m[3][2] - m[2][2] * m[3][1];
    const real det_12_13 = m[1][1] * m[3][2] - m[1][2] * m[3][1];
    const real det_12_12 = m[1][1] * m[2][2] - m[1][2] * m[2][1];
    const real det_12_03 = m[0][1] * m[3][2] - m[0][2] * m[3][1];
    const real det_12_02 = m[0][1] * m[2][2] - m[0][2] * m[2][1];
    const real det_12_01 = m[0][1] * m[1][2] - m[0][2] * m[1][1];

    const real det_023_123 = m[1][0] * det_23_23 - m[2][0] * det_23_13 + m[3][0] * det_23_12;
    const real det_023_023 = m[0][0] * det_23_23 - m[2][0] * det_23_03 + m[3][0] * det_23_02;
    const real det_023_013 = m[0][0] * det_23_13 - m[1][0] * det_23_03 + m[3][0] * det_23_01;
    const real det_023_012 = m[0][0] * det_23_12 - m[1][0] * det_23_02 + m[2][0] * det_23_01;

    const real det_013_123 = m[1][0] * det_13_23 - m[2][0] * det_13_13 + m[3][0] * det_13_12;
    const real det_013_023 = m[0][0] * det_13_23 - m[2][0] * det_13_03 + m[3][0] * det_13_02;
    const real det_013_013 = m[0][0] * det_13_13 - m[1][0] * det_13_03 + m[3][0] * det_13_01;
    const real det_013_012 = m[0][0] * det_13_12 - m[1][0] * det_13_02 + m[2][0] * det_13_01;

    const real det_012_123 = m[1][0] * det_12_23 - m[2][0] * det_12_13 + m[3][0] * det_12_12;
    const real det_012_023 = m[0][0] * det_12_23 - m[2][0] * det_12_03 + m[3][0] * det_12_02;
    const real det_012_013 = m[0][0] * det_12_13 - m[1][0] * det_12_03 + m[3][0] * det_12_01;
    const real det_012_012 = m[0][0] * det_12_12 - m[1][0] * det_12_02 + m[2][0] * det_12_01;

    const real invDet = static_cast<real>(1.0) / determinant;
    
    Matrix4x4 inverse;
    
    inverse.m[0][0] =  det_123_123 * invDet;
    inverse.m[0][1] = -det_123_023 * invDet;
    inverse.m[0][2] =  det_123_013 * invDet;
    inverse.m[0][3] = -det_123_012 * invDet;

    inverse.m[1][0] = -det_023_123 * invDet;
    inverse.m[1][1] =  det_023_023 * invDet;
    inverse.m[1][2] = -det_023_013 * invDet;
    inverse.m[1][3] =  det_023_012 * invDet;

    inverse.m[2][0] =  det_013_123 * invDet;
    inverse.m[2][1] = -det_013_023 * invDet;
    inverse.m[2][2] =  det_013_013 * invDet;
    inverse.m[2][3] = -det_013_012 * invDet;

    inverse.m[3][0] = -det_012_123 * invDet;
    inverse.m[3][1] =  det_012_023 * invDet;
    inverse.m[3][2] = -det_012_013 * invDet;
    inverse.m[3][3] =  det_012_012 * invDet;
    
    return inverse;
}

Matrix4x4 Matrix4x4::Transpose() const
{
    Matrix4x4 transpose;

    transpose.m[0][0] = m[0][0];
    transpose.m[0][1] = m[1][0];
    transpose.m[0][2] = m[2][0];
    transpose.m[0][3] = m[3][0];

    transpose.m[1][0] = m[0][1];
    transpose.m[1][1] = m[1][1];
    transpose.m[1][2] = m[2][1];
    transpose.m[1][3] = m[3][1];

    transpose.m[2][0] = m[0][2];
    transpose.m[2][1] = m[1][2];
    transpose.m[2][2] = m[2][2];
    transpose.m[2][3] = m[3][2];

    transpose.m[3][0] = m[0][3];
    transpose.m[3][1] = m[1][3];
    transpose.m[3][2] = m[2][3];
    transpose.m[3][3] = m[3][3];
    
    return transpose;
}

Vector3 Matrix4x4::ToEuler() const
{
    const real pitch =  Atan2(m[1][2], m[2][2]);
    const real yaw   = -Asin(m[0][2]);
    const real roll  =  Atan2(m[0][1], m[0][0]);

    return { pitch, yaw, roll };
}

Matrix4x4 Matrix4x4::CreateTranslation(const Vector3& translation)
{
    Matrix4x4 ret = Matrix4x4();

    ret[3][0] = translation.x;
    ret[3][1] = translation.y;
    ret[3][2] = translation.z;
    
    return ret;
}

Matrix4x4 Matrix4x4::CreateScale(const Vector3& scale)
{
    Matrix4x4 ret = Matrix4x4();

    ret[0][0] = scale.x;
    ret[1][1] = scale.y;
    ret[2][2] = scale.z;
    
    return ret;
}

Matrix4x4 Matrix4x4::CreateRotation(const Vector3& axis, real angle)
{
    const real cos = Cos(angle);
    const real sin = Sin(angle);
    const real xSin = axis.x * sin;
    const real ySin = axis.y * sin;
    const real zSin = axis.z * sin;
    const real oneMinusCos = static_cast<real>(1.0) - cos;

    Matrix4x4 ret = Matrix4x4();

    ret[0][0] = axis.x * axis.x * oneMinusCos + cos;
    ret[0][1] = axis.x * axis.y * oneMinusCos + zSin;
    ret[0][2] = axis.x * axis.z * oneMinusCos - ySin;

    ret[1][0] = axis.x * axis.y * oneMinusCos - zSin;
    ret[1][1] = axis.y * axis.y * oneMinusCos + cos;
    ret[1][2] = axis.y * axis.z * oneMinusCos + xSin;
    
    ret[2][0] = axis.x * axis.z * oneMinusCos + ySin;
    ret[2][1] = axis.y * axis.z * oneMinusCos - xSin;
    ret[2][2] = axis.z * axis.z * oneMinusCos + cos;
    
    return ret;
}

Matrix4x4 Matrix4x4::FromEuler(const Vector3& angles)
{
    return FromEuler(angles.x, angles.y, angles.z);
}

// Rotates around X -> Y -> Z in this order
Matrix4x4 Matrix4x4::FromEuler(real pitch, real yaw, real roll)
{
    const real cosX = Cos(pitch);
    const real cosY = Cos(yaw);
    const real cosZ = Cos(roll);
    const real sinX = Sin(pitch);
    const real sinY = Sin(yaw);
    const real sinZ = Sin(roll);

    Matrix4x4 ret = Matrix4x4();

    ret[0][0] =  cosY * cosZ;
    ret[0][1] =  sinZ * cosY;
    ret[0][2] = -sinY;
    
    ret[1][0] =  sinY * sinX * cosZ - cosX * sinZ;
    ret[1][1] =  sinY * sinX * sinZ + cosX * cosZ;
    ret[1][2] =  cosY * sinX;

    ret[2][0] =  sinY * cosX * cosZ + sinX * sinZ;
    ret[2][1] =  sinY * cosX * sinZ - sinX * cosZ;
    ret[2][2] =  cosY * cosX;

    return ret;
}

Matrix4x4 Matrix4x4::FromEulerX(real pitch)
{
    Matrix4x4 ret = Matrix4x4();

    const real angleCos = Cos(pitch);
    const real angleSin = Sin(pitch);

    ret[1][1] =  angleCos;
    ret[1][2] =  angleSin;
    ret[2][1] = -angleSin;
    ret[2][2] =  angleCos;
    
    return ret;
}

Matrix4x4 Matrix4x4::FromEulerY(real yaw)
{
    Matrix4x4 ret = Matrix4x4();

    const real angleCos = Cos(yaw);
    const real angleSin = Sin(yaw);

    ret[0][0] =  angleCos;
    ret[0][2] = -angleSin;
    ret[2][0] =  angleSin;
    ret[2][2] =  angleCos;
    
    return ret;
}

Matrix4x4 Matrix4x4::FromEulerZ(real roll)
{
    Matrix4x4 ret = Matrix4x4();

    const real angleCos = Cos(roll);
    const real angleSin = Sin(roll);

    ret[0][0] =  angleCos;
    ret[0][1] =  angleSin;
    ret[1][0] = -angleSin;
    ret[1][1] =  angleCos;
    
    return ret;
}

Matrix4x4 Matrix4x4::Model(const Vector3& position, const Quaternion& rotation, const Vector3& scale, const Vector3& pivot)
{
    const Vector3 rotationEuler = rotation.ToEuler();
    
    Matrix4x4 model = CreateTranslation(position);

    model *= CreateTranslation(pivot);
    model *= FromEuler(rotationEuler);
    model *= CreateTranslation(-pivot);
    model *= CreateScale(scale);

    return model;
}

Matrix4x4 Matrix4x4::LookAt(const Vector3& eye, const Vector3& target, const Vector3& up)
{
    const Vector3 forward = (target - eye).Normalize();
    const Vector3 right   = Vector3::Cross(up, forward).Normalize();
    const Vector3 localUp = Vector3::Cross(forward, right).Normalize();

    Matrix4x4 ret = Matrix4x4();

    ret[0][0] = -right.x;
    ret[0][1] =  localUp.x;
    ret[0][2] =  forward.x;
    
    ret[1][0] = -right.y;
    ret[1][1] =  localUp.y;
    ret[1][2] =  forward.y;
    
    ret[2][0] = -right.z;
    ret[2][1] =  localUp.z;
    ret[2][2] =  forward.z;

    ret[3][0] =  Vector3::Dot(right, eye);
    ret[3][1] = -Vector3::Dot(localUp, eye);
    ret[3][2] = -Vector3::Dot(forward, eye);

    return ret;
}

Matrix4x4 Matrix4x4::Perspective(real fovX, real aspectRatio, real near, real far)
{
    const real invHalfFovTan  = static_cast<real>(1.0) / Tan(fovX * static_cast<real>(0.5));
    const real invAspectRatio = static_cast<real>(1.0) / aspectRatio;
    const real farMinusNear   = far - near;

    Matrix4x4 ret = Matrix4x4(0.0);

    ret[0][0] = invHalfFovTan * invAspectRatio;
    ret[1][1] = invHalfFovTan;
    ret[2][2] = (far + near) / farMinusNear;
    ret[2][3] = static_cast<real>(1.0);
    ret[3][2] = static_cast<real>(-2.0) * far * near / farMinusNear;

    return ret;
}

Matrix4x4 Matrix4x4::Orthographic(real left, real right, real bottom, real top, real near, real far)
{
    Matrix4x4 ret = Matrix4x4();

    const real invRightMinusLeft = static_cast<real>(1.0) / (right - left);
    const real invTopMinusBottom = static_cast<real>(1.0) / (top - bottom);
    const real invFarMinusNear   = static_cast<real>(1.0) / (far - near);

    ret[0][0] = static_cast<real>(2.0) * invRightMinusLeft;
    ret[1][1] = static_cast<real>(2.0) * invTopMinusBottom;
    ret[2][2] = static_cast<real>(2.0) * invFarMinusNear;
    ret[3][0] = -(right + left) * invRightMinusLeft; 
    ret[3][1] = -(top + bottom) * invTopMinusBottom; 
    ret[3][2] = -(far + near) * invFarMinusNear;

    return ret;
}

Matrix4x4& Matrix4x4::operator+=(const Matrix4x4& rhs)
{
    m[0] += rhs.m[0];
    m[1] += rhs.m[1];
    m[2] += rhs.m[2];
    m[3] += rhs.m[3];
    
    return *this;
}

Matrix4x4& Matrix4x4::operator-=(const Matrix4x4& rhs)
{
    m[0] -= rhs.m[0];
    m[1] -= rhs.m[1];
    m[2] -= rhs.m[2];
    m[3] -= rhs.m[3];
    
    return *this;
}

// Very unoptimized version of multiplication
// Improve in the future
Matrix4x4& Matrix4x4::operator*=(const Matrix4x4& rhs)
{
    const Matrix4x4 transpose = this->Transpose();

#define dot Vector4::Dot
    m[0] = Vector4(dot(transpose[0], rhs[0]), dot(transpose[1], rhs[0]), dot(transpose[2], rhs[0]), dot(transpose[3], rhs[0]));
    m[1] = Vector4(dot(transpose[0], rhs[1]), dot(transpose[1], rhs[1]), dot(transpose[2], rhs[1]), dot(transpose[3], rhs[1]));
    m[2] = Vector4(dot(transpose[0], rhs[2]), dot(transpose[1], rhs[2]), dot(transpose[2], rhs[2]), dot(transpose[3], rhs[2]));
    m[3] = Vector4(dot(transpose[0], rhs[3]), dot(transpose[1], rhs[3]), dot(transpose[2], rhs[3]), dot(transpose[3], rhs[3]));
#undef dot
    return *this;
}

Matrix4x4& Matrix4x4::operator*=(real rhs)
{
    m[0] *= rhs;
    m[1] *= rhs;
    m[2] *= rhs;
    m[3] *= rhs;
    
    return *this;
}

Matrix4x4& Matrix4x4::operator/=(real rhs)
{
    const real invRhs = static_cast<real>(1.0) / rhs;
    
    m[0] *= invRhs;
    m[1] *= invRhs;
    m[2] *= invRhs;
    m[3] *= invRhs;
    
    return *this;
}

Vector4& Matrix4x4::operator[](int idx)
{
    return m[idx];
}

const Vector4& Matrix4x4::operator[](int idx) const
{
    return m[idx];
}
