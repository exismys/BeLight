#pragma once

#include <cstdlib>
#include <vector>
#include <cmath>

#include "mathematics.hpp"
#include "renderer.hpp"
#include "types.hpp"

struct Triangle {
    int v[3];
    Color color;
};

struct Mesh {
    std::vector<Vec3> vertices;
    std::vector<Triangle> triangles;
};

struct Object {
    Mesh* mesh;
    
    Vec3 scale = {1, 1, 1};
    Vec3 rotation;
    Vec3 position;
};

Mesh create_cube_mesh();
void render_object(Renderer& renderer, Object& object);
void render_triangle(Renderer& renderer, const Triangle& triangle, const std::vector<Vec2>& projected_vertices);

std::vector<float> interpolate(Vec2 p1, Vec2 p2);

void draw_line(Renderer& renderer, Vec2 p1, Vec2 p2, Color color);

void draw_triangle_wireframe(Renderer& renderer, Vec2 p1, Vec2 p2, Vec2 p3, Color color);
void draw_triangle_filled(Renderer& renderer, Vec2 p1, Vec2 p2, Vec2 p3, Color color);
void draw_triangle_shaded(Renderer& renderer, Vec2 p1, Vec2 p2, Vec2 p3, Color color, float h1, float h2, float h3);

Vec2 viewport_to_canvas(Renderer& renderer, Vec3 point);
Vec2 project_vertex(Renderer& renderer, Vec3 vertex);

void draw_cube(Renderer& renderer);
