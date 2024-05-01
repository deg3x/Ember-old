#pragma once

#include "Definitions.h"
#include "math/vector/Vector4.h"

struct ENGINE_API Matrix4x4
{
private:
    Vector4 m[4];

public:
    Matrix4x4();
    Matrix4x4(const Vector4& colA, const Vector4& colB, const Vector4& colC, const Vector4& colD);

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