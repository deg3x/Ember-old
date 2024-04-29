#pragma once

#include "Definitions.h"
#include "math/vector/Vector4.h"

struct ENGINE_API Matrix4x4
{
private:
    Vector4 values[4];

public:
    Matrix4x4();

    real Determinant() const;
    Matrix4x4 Inverse() const;
    Matrix4x4 Transpose() const;

    Matrix4x4& operator+=(const Matrix4x4& rhs);
    Matrix4x4& operator-=(const Matrix4x4& rhs);
    Matrix4x4& operator*=(const Matrix4x4& rhs);

    Matrix4x4& operator*=(real rhs);
    Matrix4x4& operator/=(real rhs);

    Vector4& operator[](int idx);
    const Vector4& operator[](int idx) const;
};

inline Matrix4x4 operator+(const Matrix4x4& lhs, const Matrix4x4& rhs)
{
    Matrix4x4 ret = lhs;

    ret += rhs;

    return ret;
}

inline Matrix4x4 operator-(const Matrix4x4& lhs, const Matrix4x4& rhs)
{
    Matrix4x4 ret = lhs;

    ret -= rhs;

    return ret;
}

inline Matrix4x4 operator*(const Matrix4x4& lhs, const Matrix4x4& rhs)
{
    Matrix4x4 ret = lhs;

    ret *= rhs;

    return ret;
}