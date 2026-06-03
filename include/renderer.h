#pragma once

#include <vector>
#include <cstdint>
#include "mathematics.h"
#include "simulation.h"

struct Renderer {
    std::vector<uint32_t> framebuffer;
    int width;
    int height;
};

// Render Simulation objects
void render_simulation(Renderer& renderer, Simulation& simulation);

// Geometry
void draw_circle(Renderer& renderer, Vec2 pos, float radius, uint32_t color);
void draw_circle_solid(Renderer& renderer, Vec2 pos, float radius, uint32_t color);
void draw_rect(Renderer& renderer, Vec2 pos, Vec2 size, uint32_t color);
void draw_rect_solid(Renderer& renderer, Vec2 pos, Vec2 size, uint32_t color);
void draw_line(Renderer& renderer, Vec2 pos1, Vec2 pos2, uint32_t color);

// put_pixel primitive setup
void draw_point(Renderer& renderer, Vec2 world_point, uint32_t color);
IVec2 world_to_screen(const Renderer& Renderer, const Vec2 point);
void put_pixel(Renderer& renderer, IVec2 screen_point, uint32_t color);