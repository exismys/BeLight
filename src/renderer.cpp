#include <iostream>
#include <cmath>
#include <stb_image_write.h>
#include "renderer.hpp"
#include "mathematics.hpp"

void render_simulation(Renderer& renderer, Simulation& simulation) {
    for (Particle& particle: simulation.particles) {
        // std::cout << "x: " << particle.pos.x << ", y: " << particle.pos.y << '\n';
        draw_circle_solid(renderer, particle.pos, particle.radius, particle.color);
    }
}

// Geometry
void draw_circle_solid(Renderer& renderer, Vec2 pos, float radius, Color color) {
    IVec2 screen_pos = world_to_screen(renderer, pos);

    int cx = screen_pos.x;
    int cy = screen_pos.y;
    int r = static_cast<int>(std::ceil(radius));
    int r2 = radius * radius;

    for (int y = cy - r; y <= cy + r; y++) {
        for (int x = cx - r; x <= cx + r; x++) {
            int dx = cx - x;
            int dy = cy - y;
            if (dx * dx + dy * dy <= r2) {
                put_pixel(renderer, IVec2{x, y}, color);
            }
        }
    }
}

// Export framebuffer
void save_framebuffer(Renderer& renderer, const char* filename) {
    stbi_write_png(filename, renderer.width, renderer.height, 4, renderer.framebuffer.data(), renderer.width * sizeof(uint32_t));
}

// put_pixel primitive setup
void draw_point(Renderer& renderer, Vec2 world_point, Color color) {
    IVec2 screen_point = world_to_screen(renderer, world_point);
    put_pixel(renderer, screen_point, color);
}

IVec2 world_to_screen(const Renderer& renderer, const Vec2 point) {
    return IVec2{
        static_cast<int>(std::round(renderer.width * 0.5 + point.x)),
        static_cast<int>(std::round(renderer.height * 0.5 - point.y))
    };
}

void put_pixel(Renderer& renderer, IVec2 screen_point, Color color) {
    if (
        screen_point.x >= renderer.width || screen_point.x < 0 ||
        screen_point.y >= renderer.height || screen_point.y < 0
    ) {
        // std::cerr << "Error: Invalid screen point" << screen_point.x << ", " << screen_point.y << '\n';
        return;
    }

    uint32_t rgba = (
        (color.a << 24) |
        (color.r << 16) |
        (color.g << 8) |
        color.b
    );
    renderer.framebuffer[screen_point.y * renderer.width + screen_point.x] = rgba;
}
