#include "engine_pch.h"
#include "Matrix4x4.h"

Matrix4x4::Matrix4x4()
{
    m[0] = Vector4(1.0, 0.0, 0.0, 0.0);
    m[1] = Vector4(0.0, 1.0, 0.0, 0.0);
    m[2] = Vector4(0.0, 0.0, 1.0, 0.0);
    m[3] = Vector4(0.0, 0.0, 0.0, 1.0);
}

real Matrix4x4::Determinant() const
{
    const real det2d_23 = m[2][2] * m[3][3] - m[3][2] * m[2][3];
    const real det2d_13 = m[2][1] * m[3][3] - m[3][1] * m[2][3];
    const real det2d_12 = m[2][1] * m[3][2] - m[3][1] * m[2][2];
    const real det2d_03 = m[2][0] * m[3][3] - m[3][3] * m[2][3];
    const real det2d_02 = m[2][0] * m[3][2] - m[3][0] * m[2][2];
    const real det2d_01 = m[2][0] * m[3][1] - m[3][0] * m[2][1];

    const real det3d_123 = m[1][1] * det2d_23 - m[1][2] * det2d_13 + m[1][3] * det2d_12;
    const real det3d_023 = m[1][0] * det2d_23 - m[1][2] * det2d_03 + m[1][3] * det2d_02;
    const real det3d_013 = m[1][0] * det2d_13 - m[1][1] * det2d_03 + m[1][3] * det2d_01;
    const real det3d_012 = m[1][0] * det2d_12 - m[1][1] * det2d_02 + m[1][2] * det2d_01;

    const real determinant = m[0][0] * det3d_123 - m[0][1] * det3d_023 + m[0][2] * det3d_013 - m[0][3] * det3d_012;
    
    return determinant;
}

Matrix4x4 Matrix4x4::Inverse() const
{
    return Matrix4x4();
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
