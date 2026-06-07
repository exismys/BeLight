#pragma once

#include <vector>
#include <cstdint>
#include "mathematics.hpp"
#include "simulation.hpp"
#include "types.hpp"

struct Renderer {
    std::vector<uint32_t> framebuffer;
    int width;
    int height;
};

// Render Simulation objects
void render_simulation(Renderer& renderer, Simulation& simulation);

// Geometry
void draw_circle(Renderer& renderer, Vec2 pos, float radius, Color color);
void draw_circle_solid(Renderer& renderer, Vec2 pos, float radius, Color color);
void draw_rect(Renderer& renderer, Vec2 pos, Vec2 size, Color color);
void draw_rect_solid(Renderer& renderer, Vec2 pos, Vec2 size, Color color);

// Export framebuffer
void save_framebuffer(Renderer& renderer, const char* filename);

// put_pixel primitive setup
void draw_point(Renderer& renderer, Vec2 world_point, Color color);
IVec2 world_to_screen(const Renderer& Renderer, const Vec2 point);
void put_pixel(Renderer& renderer, IVec2 screen_point, Color color);