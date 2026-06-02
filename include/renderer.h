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

void render_simulation(Renderer& renderer, Simulation& simulation);

void draw_point(Renderer& renderer, Vec2 world_point, uint32_t color);

void put_pixel(Renderer& renderer, IVec2 screen_point, uint32_t color);

IVec2 world_to_screen(const Renderer& Renderer, const Vec2 point);