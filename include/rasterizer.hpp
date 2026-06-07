#pragma once

#include <cstdlib>
#include <vector>
#include <cmath>

#include "mathematics.hpp"
#include "renderer.hpp"
#include "types.hpp"

std::vector<float> interpolate(Vec2 p1, Vec2 p2);

void draw_line(Renderer& renderer, Vec2 p1, Vec2 p2, Color color);

void draw_triangle_wireframe(Renderer& renderer, Vec2 p1, Vec2 p2, Vec2 p3, Color color);
void draw_triangle_filled(Renderer& renderer, Vec2 p1, Vec2 p2, Vec2 p3, Color color);
void draw_triangle_shaded(Renderer& renderer, Vec2 p1, Vec2 p2, Vec2 p3, Color color, float h1, float h2, float h3);