#include "rasterizer.hpp"

std::vector<float> interpolate(Vec2 p1, Vec2 p2) {
    std::vector<float> values;
    if (std::round(p1.x) == std::round(p2.x)) {
        values.push_back(p1.y);
        return values;
    } 

    float y = p1.y;
    float slope = (p2.y - p1.y) / (p2.x - p1.x);
    for (int i = std::round(p1.x); i <= std::round(p2.x); i++) {
        values.push_back(y);
        y = y + slope;
    }
    return values;
}

void draw_line(Renderer& renderer, Vec2 p1, Vec2 p2, Color color) {
    if (std::abs(p2.x - p1.x) > std::abs(p2.y - p1.y)) {
        if (p1.x > p2.x) {
            Vec2 temp = p1;
            p1 = p2;
            p2 = temp;
        }

        int x1 = std::round(p1.x);
        int x2 = std::round(p2.x);

        std::vector<float> y_values = interpolate(p1, p2);
        for (int x = x1; x <= x2; x++) {
            put_pixel(renderer, IVec2{x, static_cast<int>(std::round(y_values[x - x1]))}, color);

        }
    } else {
        if (p1.y > p2.y) {
            Vec2 temp = p1;
            p1 = p2;
            p2 = temp;
        }

        int y1 = std::round(p1.y);
        int y2 = std::round(p2.y);

        std::vector<float> x_values = interpolate(Vec2{p1.y, p1.x}, Vec2{p2.y, p2.x});
        for (int y = y1; y <= y2; y++) {
            put_pixel(renderer, IVec2{static_cast<int>(std::round(x_values[y - y1])), y}, color);
        }
    }
}