#pragma once

#include <iostream>
#include <cstdint>
#include <vector>
#include <limits>
#include <utility>
#include <cmath>
#include "mathematics.hpp"
#include "renderer.hpp"

// Simulation constants
int screen_width = 600;
int screen_height = 600;
constexpr float viewport_size = 1;
float viewport_x = 0;
float projection_plane_z = 1;
constexpr Vec3 camera_position = Vec3{0, 0, 0};
constexpr uint32_t background_color = 0xFFFFFFFF;

enum class LightType {
    Point,
    Directional,
    Ambient
};

struct Sphere {
    Vec3 pos;
    float radius;
    uint32_t color;
};

struct Light {
    LightType type;
    float intensity;
    Vec3 position;
    Vec3 direction;
};

struct Scene {
   std::vector<Sphere> spheres;
   std::vector<Light> light_sources; 
};

Scene create_scene() {
    std::vector<Sphere> spheres;
    // for (int i = 0; i < 4; i++) {
        spheres.push_back(
            Sphere{
                Vec3 {0, -1, 3},
                1,
                0x000000FF
            }
        );
        spheres.push_back(
            Sphere{
                Vec3 {-2, 0, 4},
                1,
                0x0000FF00
            }
        );
        spheres.push_back(
            Sphere{
                Vec3 {2, 0, 4},
                1,
                0x00FF0000
            }
        );
    // }

    std::vector<Light> light_sources;
    light_sources.push_back(
        Light{
            .type = LightType::Ambient,
            .intensity = 0.2, 
        }
    );
    light_sources.push_back(
        Light{
            .type = LightType::Point,
            .intensity = 0.6, 
            .position = Vec3{2, 1, 0},
        }
    );
    light_sources.push_back(
        Light{
            .type = LightType::Directional,
            .intensity = 0.2, 
            .direction = Vec3{1, 4, 4},
        }
    );

    return Scene{
        spheres,
        light_sources
    };
}

Vec3 screen_to_viewport(Vec2& point) {
    return Vec3{
        point.x * viewport_size / screen_width + viewport_x,
        point.y * viewport_size / screen_height,
        projection_plane_z
    };
}

float compute_lighting(Vec3 point, Vec3 normal, Scene& scene) {
    float i = 0.0f;
    for (Light& light: scene.light_sources) {
        if (light.type == LightType::Ambient) {
            i += light.intensity;
        } else {
            Vec3 l;
            if (light.type == LightType::Point) {
                l = light.position - point;
            } else {
                l = light.direction;
            }
            float n_dot_l = dot_product(normal, l);
            if (n_dot_l > 0) {
                i += light.intensity * n_dot_l / (magnitude(normal) * magnitude(l));
            }
        }
    }
    return i;
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

    Vec3 point = origin + direction * closest_t;
    Vec3 normal_with_mag = (point - closest_sphere.pos);
    Vec3 normal = normal_with_mag / magnitude(normal_with_mag);
    
    float light_intensity = compute_lighting(point, normal, scene);

    return closest_sphere.color * light_intensity;
}

void main_loop(Renderer& renderer, Scene& scene) {
    screen_width = renderer.width;
    screen_height = renderer.height;
    for (int x = - screen_width / 2; x < screen_width / 2; x++) {
        for (int y = - screen_height / 2; y < screen_height / 2; y++) {
            Vec2 point = Vec2{static_cast<float>(x), static_cast<float>(y)};
            Vec3 direction = screen_to_viewport(point);
            uint32_t color = trace_ray(camera_position, direction, 1, std::numeric_limits<float>::infinity(), scene);
            draw_point(renderer, Vec2{float(x), float(y)}, color);
        }
    }
}

void update_projection_plane_z(float change) {
    projection_plane_z += change;
}

void update_viewport_x(float change) {
    viewport_x += change;
}
