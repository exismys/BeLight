#pragma once

// Vector with 2 components
struct Vec2 {
    float x;
    float y;
};

struct IVec2 {
    int x;
    int y;
};

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

// Vector with 3 components
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
Vec3 rotate_around_x(const Vec3& v, float angle);
Vec3 rotate_around_y(const Vec3& v, float angle);
Vec3 rotate_around_z(const Vec3& v, float angle);

IVec3 operator+(const IVec3& a, const IVec3& b);
IVec3 operator-(const IVec3& a, const IVec3& b);
Vec3 operator*(const IVec3& a, float b);

// Vector with 4 components
struct Vec4 {
    float x;
    float y;
    float z;
    float w;
};

Vec4 operator+(const Vec4& a, const Vec4& b);
Vec4& operator+=(Vec4& a, const Vec4& b);

Vec4 operator-(const Vec4& a, const Vec4& b);
Vec4 operator-(const Vec4& a);
Vec4& operator-=(Vec4& a, const Vec4& b);

Vec4 operator*(const Vec4& a, float b);
Vec4 operator*(float a, const Vec4& b);
Vec4& operator*=(Vec4& a, float b);

Vec4 operator/(const Vec4& a, float b);
Vec4& operator/=(Vec4& a, float b);

float dot_product(const Vec4& a, const Vec4& b);
float magnitude(const Vec4& a);

// Matrix 
struct Mat4 {
    float m[4][4];
};

Mat4 identity_matrix();
Mat4 translation_matrix(const Vec3& v);

Vec4 operator*(const Mat4& m, const Vec4& v);
Mat4 operator*(const Mat4& a, const Mat4& b);
