#pragma once

struct Vec2 {
    float x;
    float y;
};

struct IVec2 {
    int x;
    int y;
};

// Overload operators
Vec2 operator+(const Vec2& a, const Vec2& b);
Vec2& operator+=(Vec2& a, const Vec2& b);
Vec2 operator-(const Vec2& a, const Vec2& b);
Vec2 operator-(const Vec2& a);
Vec2& operator-=(Vec2& a, const Vec2& b);
Vec2 operator*(const Vec2& a, float b);
Vec2 operator*(float a, const Vec2& b);
Vec2& operator*=(Vec2& a, float b);
Vec2 operator/(const Vec2& a, float b); 
Vec2& operator/=(Vec2& a, float b);

IVec2 operator+(const IVec2& a, const IVec2& b);
IVec2 operator-(const IVec2& a, const IVec2& b);
Vec2 operator*(const IVec2& a, float b);

Vec2 swap_components(const Vec2& a);

struct Vec3 {
    float x;
    float y;
    float z;
};

struct IVec3 {
  int x;
  int y;
  int z;
};

Vec3 operator+(const Vec3& a, const Vec3& b);
Vec3& operator+=(Vec3& a, const Vec3& b);
Vec3 operator-(const Vec3& a, const Vec3& b);
Vec3 operator-(const Vec3& a);
Vec3& operator-=(Vec3& a, const Vec3& b);
Vec3 operator*(const Vec3& a, float b);
Vec3 operator*(float a, const Vec3& b);
Vec3& operator*=(Vec3& a, float b);
Vec3 operator/(const Vec3& a, float b); 
Vec3& operator/=(Vec3& a, float b);

float dot_product(const Vec3& a, const Vec3& b);
float magnitude(const Vec3& a);

IVec3 operator+(const IVec3& a, const IVec3& b);
IVec3 operator-(const IVec3& a, const IVec3& b);
Vec3 operator*(const IVec3& a, float b);