#include <cmath>
#include "mathematics.hpp"

// Matrix operations

Mat4 identity_matrix() {
    Mat4 m{};

    m.m[0][0] = 1;
    m.m[1][1] = 1;
    m.m[2][2] = 1;
    m.m[3][3] = 1;

    return m;
}

Mat4 translation_matrix(const Vec3& v) {
    Mat4 m = identity_matrix();

    m.m[0][3] = v.x;
    m.m[1][3] = v.y;
    m.m[2][3] = v.z;

    return m;
}

Vec4 operator*(const Mat4& m, const Vec4& v) {
    return Vec4 {
        m.m[0][0] * v.x +
        m.m[0][1] * v.y +
        m.m[0][2] * v.z +
        m.m[0][3] * v.w,

        m.m[1][0] * v.x +
        m.m[1][1] * v.y +
        m.m[1][2] * v.z +
        m.m[1][3] * v.w,

        m.m[2][0] * v.x +
        m.m[2][1] * v.y +
        m.m[2][2] * v.z +
        m.m[2][3] * v.w,

        m.m[3][0] * v.x +
        m.m[3][1] * v.y +
        m.m[3][2] * v.z +
        m.m[3][3] * v.w
    };
}

Mat4 operator*(const Mat4& a, const Mat4& b) {
    Mat4 result{};

    for (int row = 0; row < 4; row++) {
        for (int col = 0; col < 4; col++) {

            float value = 0;

            for (int k = 0; k < 4; k++) {
                value += a.m[row][k] * b.m[k][col];
            }

            result.m[row][col] = value;
        }
    }

    return result;
}

// Vec4 operations
Vec4 operator+(const Vec4& a, const Vec4& b) {
    return Vec4{
        a.x + b.x,
        a.y + b.y,
        a.z + b.z,
        a.w + b.w
    };
}

Vec4& operator+=(Vec4& a, const Vec4& b) {
    a.x += b.x;
    a.y += b.y;
    a.z += b.z;
    a.w += b.w;
    return a;
}

Vec4 operator-(const Vec4& a, const Vec4& b) {
    return Vec4{
        a.x - b.x,
        a.y - b.y,
        a.z - b.z,
        a.w - b.w
    };
}

Vec4 operator-(const Vec4& a) {
    return Vec4{
        -a.x,
        -a.y,
        -a.z,
        -a.w
    };
}

Vec4& operator-=(Vec4& a, const Vec4& b) {
    a.x -= b.x;
    a.y -= b.y;
    a.z -= b.z;
    a.w -= b.w;
    return a;
}

Vec4 operator*(const Vec4& a, float b) {
    return Vec4{
        a.x * b,
        a.y * b,
        a.z * b,
        a.w * b
    };
}

Vec4 operator*(float a, const Vec4& b) {
    return Vec4{
        b.x * a,
        b.y * a,
        b.z * a,
        b.w * a
    };
}

Vec4& operator*=(Vec4& a, float b) {
    a.x *= b;
    a.y *= b;
    a.z *= b;
    a.w *= b;
    return a;
}

Vec4 operator/(const Vec4& a, float b) {
    return Vec4{
        a.x / b,
        a.y / b,
        a.z / b,
        a.w / b
    };
}

Vec4& operator/=(Vec4& a, float b) {
    a.x /= b;
    a.y /= b;
    a.z /= b;
    a.w /= b;
    return a;
}

float dot_product(const Vec4& a, const Vec4& b) {
    return
        a.x * b.x +
        a.y * b.y +
        a.z * b.z +
        a.w * b.w;
}

float magnitude(const Vec4& a) {
    return std::sqrt(dot_product(a, a));
}

// Vector 3 operations
Vec3 operator+(const Vec3& a, const Vec3& b) {
    return Vec3{
        a.x + b.x,
        a.y + b.y,
        a.z + b.z
    };
}

Vec3& operator+=(Vec3& a, const Vec3& b) {
    a.x += b.x;
    a.y += b.y;
    a.z += b.z;
    return a;
}

Vec3 operator-(const Vec3& a, const Vec3& b) {
    return Vec3 {
        a.x - b.x,
        a.y - b.y,
        a.z - b.z
    };
}

Vec3 operator-(const Vec3& a) {
    return Vec3{
        - a.x,
        - a.y,
        - a.z
    };
}

Vec3& operator-=(Vec3& a, const Vec3& b) {
    a.x -= b.x;
    a.y -= b.y;
    a.z -= b.z;
    return a;
}

Vec3 operator*(const Vec3& a, float b) {
    return Vec3{
        a.x * b,
        a.y * b,
        a.z * b
    };
}

Vec3 operator*(float a, const Vec3& b) {
    return Vec3{
        b.x * a,
        b.y * a,
        b.z * a
    };
}

Vec3& operator*=(Vec3& a, float b) {
    a.x *= b;
    a.y *= b;
    a.z *= b;
    return a;
}

Vec3 operator/(const Vec3& a, float b) {
    return Vec3{
        a.x / b,
        a.y / b,
        a.z / b
    };
}

Vec3& operator/=(Vec3& a, float b) {
    a.x /= b;
    a.y /= b;
    a.z /= b;
    return a;
}

IVec3 operator+(const IVec3& a, const IVec3& b) {
    return IVec3{
        a.x + b.x,
        a.y + b.y,
        a.z + b.z
    };
}

IVec3 operator-(const IVec3& a, const IVec3& b) {
    return IVec3{
        a.x - b.x,
        a.y - b.y,
        a.z - b.z
    };
}

Vec3 operator*(const IVec3& a, float b) {
    return Vec3{
        a.x * b,
        a.y * b,
        a.z * b
    };
}

float dot_product(const Vec3& a, const Vec3& b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

float magnitude(const Vec3& a) {
    return std::sqrt(a.x * a.x + a.y * a.y + a.z * a.z);
}

Vec3 rotate_around_x(const Vec3& v, float angle) {
    float cos_angle = std::cos(angle);
    float sin_angle = std::sin(angle);

    return Vec3{
        v.x,
        v.y * cos_angle - v.z * sin_angle,
        v.y * sin_angle + v.z * cos_angle,
    };
}

Vec3 rotate_around_y(const Vec3& v, float angle) {
    float cos_angle = std::cos(angle);
    float sin_angle = std::sin(angle);

    return Vec3{
        v.x * cos_angle - v.z * sin_angle,
        v.y,
        v.x * sin_angle + v.z * cos_angle
    };
}

Vec3 rotate_around_z(const Vec3& v, float angle) {
    float cos_angle = std::cos(angle);
    float sin_angle = std::sin(angle);

    return Vec3{
        v.x * cos_angle - v.y * sin_angle,
        v.x * sin_angle + v.y * cos_angle,
        v.z
    };
}


// Vec2 operations
Vec2 operator+(const Vec2& a, const Vec2& b) {
    return Vec2{
        a.x + b.x,
        a.y + b.y
    };
}

Vec2& operator+=(Vec2& a, const Vec2& b) {
    a.x += b.x;
    a.y += b.y;
    return a;
}

Vec2 operator-(const Vec2& a, const Vec2& b) {
    return Vec2{
        a.x - b.x,
        a.y - b.y
    };
}

Vec2& operator-=(Vec2& a, const Vec2& b) {
    a.x -= b.x;
    a.y -= b.y;
    return a;
}

Vec2 operator*(const Vec2& a, float b) {
    return Vec2{
        a.x * b,
        a.y * b,
    };
}

Vec2& operator*=(Vec2& a, float b) {
    a.x *= b;
    a.y *= b;
    return a;
};

Vec2 operator/(const Vec2& a, float b) {
    return Vec2 {
        a.x / b,
        a.y / b
    };
}

Vec2& operator/=(Vec2& a, float b) {
    a.x /= b;
    a.y /= b;
    return a;
}

IVec2 operator+(const IVec2& a, const IVec2& b) {
    return IVec2{
        a.x + b.x,
        a.y + b.y
    };
}

IVec2 operator-(const IVec2& a, const IVec2& b) {
    return IVec2{
        a.x - b.x,
        a.y - b.y
    };
}

Vec2 operator*(const IVec2& a, float b) {
    return Vec2{
        a.x * b,
        a.y * b,
    };
}

Vec2 swap_components(const Vec2& a) {
    return Vec2{
        a.y,
        a.x
    };
}
