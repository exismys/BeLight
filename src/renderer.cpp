#include <cstdint>
#include <iostream>
#include <cmath>
#include <stb_image_write.h>
#include "renderer.hpp"
#include "mathematics.hpp"

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

//=============================================================
// Export frame
//=============================================================
void save_framebuffer(Renderer& renderer, const char* filename) {
    //----------------------------------------------------------------------
    // renderer.framebuffer has each element of uint32_t type in format:
    // AA RR GG BB

    // On little endian systems, the value is stored differently in
    // memory where the least significant byte comes first as in:
    // BB GG RR AA

    // But stbi_write_png interprets the value in memory as following:
    // Byte 0 -> RR, Byte 1 -> GG, Byte 2 -> BB, Byte 3 -> AA, i.e.,
    // RR GG BB AA

    // In order to allign the value, we need the following conversion:
    // AA RR GG BB -> AA BB GG RR (swap RR and BB channels)
    //----------------------------------------------------------------------
    std::vector<uint32_t> temp_buffer = renderer.framebuffer;

    for (size_t i = 0; i < temp_buffer.size(); i++) {
        uint8_t* pixel_bytes = reinterpret_cast<uint8_t*>(&temp_buffer[i]);

        uint8_t blue = pixel_bytes[0];
        uint8_t red = pixel_bytes[2];

        // swap RR and BB channels
        pixel_bytes[0] = red;
        pixel_bytes[2] = blue;
    }
    //----------------------------------------------------------------------

    stbi_write_png(filename, renderer.width, renderer.height, 4, temp_buffer.data(), renderer.width * sizeof(uint32_t));
}
//=============================================================


//=============================================================
// Put Pixel Primitive
//=============================================================
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
//=============================================================


//=============================================================
// Depth Buffer
//=============================================================
void update_depth_buffer(Renderer& renderer, const Vec2 world_point, float one_by_z_value) {
    IVec2 screen_point = world_to_screen(renderer, world_point);

    if (screen_point.x >= renderer.width || screen_point.x < 0 ||
        screen_point.y >= renderer.height || screen_point.y < 0
    ) {
        return;
    }

    renderer.depth_buffer[screen_point.y * renderer.width + screen_point.x] = one_by_z_value;
}

float get_depth_value(Renderer& renderer, const Vec2 world_point) {
    IVec2 screen_point = world_to_screen(renderer, world_point);
    return renderer.depth_buffer[screen_point.y * renderer.width + screen_point.x];
}
//=============================================================
