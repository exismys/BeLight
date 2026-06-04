#pragma once

#include <cstdint>
#include <algorithm>

struct Color{
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t a;
};

inline Color operator*(const Color& color, float f) {
    return Color{
        static_cast<uint8_t>(std::clamp(color.r * f, 0.0f, 255.0f)),
        static_cast<uint8_t>(std::clamp(color.g * f, 0.0f, 255.0f)),
        static_cast<uint8_t>(std::clamp(color.b * f, 0.0f, 255.0f)),
        color.a
       
    };
}
