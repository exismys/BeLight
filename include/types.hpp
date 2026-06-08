#pragma once

#include <cstdint>
#include <algorithm>

struct Color{
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t a;
};

namespace Colors {
    inline constexpr Color Black     { 0,   0,   0,   255 };
    inline constexpr Color White     { 255, 255, 255, 255 };

    inline constexpr Color Red       { 255, 0,   0,   255 };
    inline constexpr Color Green     { 0,   255, 0,   255 };
    inline constexpr Color Blue      { 0,   0,   255, 255 };

    inline constexpr Color Yellow    { 255, 255, 0,   255 };
    inline constexpr Color Cyan      { 0,   255, 255, 255 };
    inline constexpr Color Magenta   { 255, 0,   255, 255 };

    inline constexpr Color Orange    { 255, 165, 0,   255 };
    inline constexpr Color Purple    { 128, 0,   128, 255 };
    inline constexpr Color Pink      { 255, 192, 203, 255 };

    inline constexpr Color Brown     { 165, 42,  42,  255 };
    inline constexpr Color Gray      { 128, 128, 128, 255 };
    inline constexpr Color DarkGray  { 64,  64,  64,  255 };
    inline constexpr Color LightGray { 192, 192, 192, 255 };
}

inline Color operator*(const Color& color, float f) {
    return Color{
        static_cast<uint8_t>(std::clamp(color.r * f, 0.0f, 255.0f)),
        static_cast<uint8_t>(std::clamp(color.g * f, 0.0f, 255.0f)),
        static_cast<uint8_t>(std::clamp(color.b * f, 0.0f, 255.0f)),
        color.a
    };
}

inline Color operator+(const Color& c1, const Color& c2) {
    return Color{
        static_cast<uint8_t>(std::min(int(c1.r) + int(c2.r), 255)),
        static_cast<uint8_t>(std::min(int(c1.g) + int(c2.g), 255)),
        static_cast<uint8_t>(std::min(int(c1.b) + int(c2.b), 255)),
        c1.a
    };
}
