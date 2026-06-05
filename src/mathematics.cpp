#include <cmath>
#include "mathematics.hpp"

// Vector 3 operator overloads
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

// Vec2 operator overloads
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
