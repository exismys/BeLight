#pragma once

#include <iostream>
#include <cstdint>
#include <vector>
#include <limits>
#include <utility>
#include <cmath>
#include "mathematics.h"
#include "renderer.h"

// Simulation constants
constexpr int screen_width = 600;
constexpr int screen_height = 600;
constexpr float viewport_size = 1;
constexpr float projection_plane_z = 1;
constexpr Vec3 camera_position = Vec3{0, 0, 0};
constexpr uint32_t background_color = 0xFFFFFFFF;

struct Sphere {
    Vec3 pos;
    float radius;
    uint32_t color;
};

struct Scene {
   std::vector<Sphere> spheres; 
};

Scene create_scene() {
    std::vector<Sphere> spheres;
    // for (int i = 0; i < 4; i++) {
        spheres.push_back(
            Sphere{
                Vec3 {0, -1, 3},
                1,
                0xFFFFFFFF
            }
        );
        spheres.push_back(
            Sphere{
                Vec3 {-2, 0, 4},
                1,
                0xFFFFFFFF
            }
        );
        spheres.push_back(
            Sphere{
                Vec3 {2, 0, 4},
                1,
                0xFFFFFFFF
            }
        );
    // }

    return Scene{
        spheres
    };
}

Vec3 screen_to_viewport(Vec2& point) {
    return Vec3{
        point.x * viewport_size / screen_width,
        point.y * viewport_size / screen_height,
        projection_plane_z
    };
}

std::pair<float, float> intersect_ray_sphere(Vec3 origin, Vec3 direction, Sphere sphere) {
    // Equation: t^2(d.d) + t(2(co.d)) + co.co - r2 = 0
    // Extracting a, b, and c with: (a * t^2) + (b * t) + c = 0
    Vec3 oc = origin - sphere.pos;

    float a = dot_product(direction, direction);
    float b = dot_product(oc, direction) * 2;
    float c = dot_product (oc, oc) - sphere.radius * sphere.radius;

    float discriminant = b * b - 4 * a * c;
    if (discriminant < 0) {
        return {std::numeric_limits<float>::infinity(), std::numeric_limits<float>::infinity()};
    }

    float t1 = (-b + std::sqrt(discriminant)) / (2 * a);
    float t2 = (-b - std::sqrt(discriminant)) / (2 * a);
    return {t1, t2};
}

uint32_t trace_ray(Vec3 origin, Vec3 direction, float min_t, float max_t, Scene& scene) {
    float closest_t = std::numeric_limits<float>::infinity();
    Sphere closest_sphere = Sphere{};
    for (Sphere& sphere: scene.spheres) {
        std::pair<float, float> t = intersect_ray_sphere(origin, direction, sphere);
        if (t.first < closest_t && t.first > min_t && t.first < max_t) {
            closest_t = t.first;
            closest_sphere = sphere;
        }
        if (t.second < closest_t && t.second > min_t && t.second < max_t) {
            closest_t = t.second;
            closest_sphere = sphere;
        }
    }
    if (closest_sphere.radius == 0.0) {
        return background_color;
    } 

    return closest_sphere.color;
}

void main_loop(Renderer& renderer, Scene& scene) {
    for (int x = - screen_width / 2; x < screen_width / 2; x++) {
        for (int y = - screen_height / 2; y < screen_height / 2; y++) {
            Vec2 point = Vec2{x, y};
            Vec3 direction = screen_to_viewport(point);
            uint32_t color = trace_ray(camera_position, direction, 1, std::numeric_limits<float>::infinity(), scene);
            put_pixel(renderer, IVec2{x, y}, color);
        }
    }
}
