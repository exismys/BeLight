#pragma once

#include <cstdlib>
#include <memory>
#include <vector>
#include <cmath>
#include <span>

#include "mathematics.hpp"
#include "renderer.hpp"
#include "types.hpp"

enum struct ObjectMode {
    WIREFRAME,
    FILLED
};

struct Triangle {
    int v[3];
    Color color;
};

struct Triangle3D {
    Vec3 p0;
    Vec3 p1;
    Vec3 p2;
    Color color;
};

struct Plane{
    Vec3 normal;
    float D;
};

struct Mesh {
    std::vector<Vec3> vertices;
    std::vector<Triangle> triangles;
};

struct Camera {
    Vec3 position;
    Vec3 rotation;
};

struct Object {
    const Mesh* mesh;
    
    Vec3 scale = {1, 1, 1};
    Vec3 rotation;
    Vec3 position;
};

struct Scene_Rast {
    std::vector<std::unique_ptr<Mesh>> meshes;
    std::vector<Object> objects;
    Camera camera;

    ObjectMode object_mode;
};

Scene_Rast create_scene_rast();
std::vector<Object> clip_scene(std::vector<Object>& objects, Plane planes[]);
Object clip_object(Object& object, Plane planes[]);
Object clip_object_against_plane(Object object, Plane plane);
std::vector<Triangle3D> clip_triangle(Triangle3D triangle, std::span<Plane> planes);
std::vector<Triangle3D> clip_triangle_against_plane(Triangle3D& triangle, Plane& plane);
Vec3 plane_line_intersection(Vec3 a, Vec3 b, Plane plane);
float signed_distance(Vec3 vertex, Plane& plane);
void render_scene_rast(Renderer& renderer, Scene_Rast& scene);
Mesh create_cube_mesh();
void render_object(Renderer& renderer, Object& object, Mat4& view);
void render_triangle(Renderer& renderer, const Triangle3D& triangle);

std::vector<float> interpolate(Vec2 p1, Vec2 p2);

void draw_line(Renderer& renderer, Vec2 p1, Vec2 p2, Color color);

void draw_triangle_wireframe(Renderer& renderer, Vec2 p1, Vec2 p2, Vec2 p3, Color color);
void draw_triangle_filled(Renderer& renderer, const Triangle3D& triangle);
void draw_triangle_shaded(Renderer& renderer, Vec2 p1, Vec2 p2, Vec2 p3, Color color, float h1, float h2, float h3);

Vec2 viewport_to_canvas(Renderer& renderer, Vec3 point);
Vec2 project_vertex(Renderer& renderer, Vec3 vertex);

void draw_cube(Renderer& renderer);
