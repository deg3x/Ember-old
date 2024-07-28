#pragma once

#include "Definitions.h"
#include "math/vector/Vector3.h"
#include "math/vector/Vector4.h"

struct Quaternion;

// Matrices are stored in a column first layout
// We are using a left handed coordinate system
struct ENGINE_API Matrix4x4
{
private:
    Vector4 m[4];

public:
    Matrix4x4();
    Matrix4x4(const Quaternion& quat);
    Matrix4x4(const Vector4& colA, const Vector4& colB, const Vector4& colC, const Vector4& colD);

    real Determinant() const;
    Matrix4x4 Inverse() const;
    Matrix4x4 Transpose() const;
    Vector3 ToEuler() const;

    static Matrix4x4 CreateTranslation(const Vector3& translation);
    static Matrix4x4 CreateScale(const Vector3& scale);
    static Matrix4x4 CreateRotation(const Vector3& axis, real angle);
    static Matrix4x4 FromEuler(const Vector3& angles);
    static Matrix4x4 FromEulerX(real pitch);
    static Matrix4x4 FromEulerY(real yaw);
    static Matrix4x4 FromEulerZ(real roll);

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

inline Vector4 operator*(const Matrix4x4& lhs, const Vector4& rhs)
{
    Vector4 ret;

    ret.x = lhs[0][0] * rhs.x + lhs[1][0] * rhs.y + lhs[2][0] * rhs.z + lhs[3][0] * rhs.w;
    ret.y = lhs[0][1] * rhs.x + lhs[1][1] * rhs.y + lhs[2][1] * rhs.z + lhs[3][1] * rhs.w;
    ret.z = lhs[0][2] * rhs.x + lhs[1][2] * rhs.y + lhs[2][2] * rhs.z + lhs[3][2] * rhs.w;
    ret.w = lhs[0][3] * rhs.x + lhs[1][3] * rhs.y + lhs[2][3] * rhs.z + lhs[3][3] * rhs.w;

    return ret;
}

// Treat the Vector3 as a Vector4 with the w component being 0.
inline Vector3 operator*(const Matrix4x4& lhs, const Vector3& rhs)
{
    Vector3 ret;

    ret.x = lhs[0][0] * rhs.x + lhs[1][0] * rhs.y + lhs[2][0] * rhs.z;
    ret.y = lhs[0][1] * rhs.x + lhs[1][1] * rhs.y + lhs[2][1] * rhs.z;
    ret.z = lhs[0][2] * rhs.x + lhs[1][2] * rhs.y + lhs[2][2] * rhs.z;

    return ret;
}