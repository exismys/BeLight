#include <cmath>
#include <memory>
#include <utility>
#include <vector>
#include <chrono>

#include "rasterizer.hpp"
#include "mathematics.hpp"
#include "renderer.hpp"
#include "types.hpp"

const float viewport_size_x = 3;
const float viewport_size_y = 2;
const float viewport_z = 1;

ObjectMode object_mode = ObjectMode::FILLED;

auto start_time = std::chrono::steady_clock::now();

float get_runtime_seconds() {
    auto now = std::chrono::steady_clock::now();

    return std::chrono::duration<float>(
        now - start_time
    ).count();
}

Mesh create_cube_mesh() {
    std::vector<Vec3> vertices = {
        {-1, -1, -1},
        { 1, -1, -1},
        { 1,  1, -1},
        {-1,  1, -1},

        {-1, -1,  1},
        { 1, -1,  1},
        { 1,  1,  1},
        {-1,  1,  1} 
    };

    std::vector<Triangle> triangles = {
        // Negative Z face
        {{0, 2, 1}, Colors::Blue},
        {{0, 3, 2}, Colors::Blue},

        // Positive Z face
        {{4, 5, 6}, Colors::Red},
        {{4, 6, 7}, Colors::Red},
                   
        // Negative X face (left)
        {{0, 7, 3}, Colors::Green},
        {{0, 4, 7}, Colors::Green},
                   
        // Positive X face (right)
        {{1, 2, 6}, Colors::Cyan},
        {{1, 6, 5}, Colors::Cyan},
                   
        // Positive Y face (top)
        {{3, 7, 6}, Colors::Magenta},
        {{3, 6, 2}, Colors::Magenta},
                   
        // Negative Y face (bottom)
        {{0, 1, 5}, Colors::Orange},
        {{0, 5, 4}, Colors::Orange}
    };

    return Mesh {
        vertices,
        triangles
    };
};

Scene_Rast create_scene_rast() {
    Scene_Rast scene;

    scene.meshes.push_back(std::make_unique<Mesh>(create_cube_mesh()));
    Mesh* cube_mesh = scene.meshes[0].get();

    scene.objects.push_back({
        cube_mesh,
        Vec3{5,5, 5},
        Vec3{0, 0, 0},
        Vec3{0, 0, 10}
    });

    scene.objects.push_back({
        cube_mesh,
        Vec3{1,1, 1},
        Vec3{0, 0, 0},
        Vec3{15, 15, 10}
    });

    scene.objects.push_back({
        cube_mesh,
        Vec3{2,2, 2},
        Vec3{0, 0, 0},
        Vec3{-15, -15, 10}
    });

    scene.object_mode = ObjectMode::FILLED;

    return scene;
}

void render_scene_rast(Renderer& renderer, Scene_Rast& scene) {

    Mat4 view = rotation_x_matrix(-scene.camera.rotation.x) *
                rotation_y_matrix(-scene.camera.rotation.y) *
                rotation_z_matrix(-scene.camera.rotation.z) *
                translation_matrix(-scene.camera.position); 

    for (Object& object: scene.objects) {
        render_object(renderer, object, view);
    }
}

void render_object(Renderer& renderer, Object& object, Mat4& view) {

    Mat4 model = translation_matrix(object.position) *
                 rotation_z_matrix(object.rotation.z) *
                 rotation_y_matrix(object.rotation.y) *
                 rotation_x_matrix(object.rotation.x) *
                 scale_matrix(object.scale);

    Mat4 view_model = view * model;

    std::vector<Vec2> projected_vertices;
    std::vector<Vec4> transformed_vertices;

    for (const Vec3& v: object.mesh->vertices) {

        Vec4 local{v.x, v.y, v.z, 1};
        Vec4 world = view_model * local;

        projected_vertices.push_back(project_vertex(renderer, {world.x, world.y, world.z}));
        transformed_vertices.push_back(world);
    }

    for (const Triangle& t: object.mesh->triangles) {
        if (
            transformed_vertices[t.v[0]].z < 1 || 
            transformed_vertices[t.v[1]].z < 1 || 
            transformed_vertices[t.v[2]].z < 1
        ) {
            continue;
        }
        render_triangle(renderer, t, projected_vertices);
    }
}

void render_triangle(Renderer& renderer, const Triangle& triangle, const std::vector<Vec2>& projected_vertices) {
    if (object_mode == ObjectMode::FILLED) {
        draw_triangle_filled(
            renderer,
            projected_vertices[triangle.v[0]],
            projected_vertices[triangle.v[1]],
            projected_vertices[triangle.v[2]],
            triangle.color
        );
        return;
    }
    draw_triangle_wireframe(
        renderer,
        projected_vertices[triangle.v[0]],
        projected_vertices[triangle.v[1]],
        projected_vertices[triangle.v[2]],
        triangle.color
    );
}

Vec2 viewport_to_canvas(Renderer& renderer, Vec3 point) {
    return Vec2{
        point.x * (renderer.width / viewport_size_x),
        point.y * (renderer.height / viewport_size_y)
    };
}

Vec2 project_vertex(Renderer& renderer, Vec3 vertex) {
    return viewport_to_canvas(renderer, Vec3{
        vertex.x * viewport_z / vertex.z, 
        vertex.y * viewport_z / vertex.z,
        vertex.z
    });
}


void draw_triangle_shaded(Renderer& renderer, Vec2 p1, Vec2 p2, Vec2 p3, Color color, float h1, float h2, float h3) {
    if (p1.y > p2.y) {
        std::swap(p1, p2);
        std::swap(h1, h2);
    }
    if (p1.y > p3.y) {
        std::swap(p1, p3);
        std::swap(h1, h3);
    }
    if (p2.y > p3.y) {
        std::swap(p2, p3);
        std::swap(h2, h3);
    }

    std::vector<float> x_values_p12 = interpolate(swap_components(p1), swap_components(p2));
    std::vector<float> x_values_p23 = interpolate(swap_components(p2), swap_components(p3));
    std::vector<float> x_values_p13 = interpolate(swap_components(p1), swap_components(p3));

    std::vector<float> h12 = interpolate(Vec2{p1.y, h1}, Vec2{p2.y, h2});
    std::vector<float> h23 = interpolate(Vec2{p2.y, h2}, Vec2{p3.y, h3});
    std::vector<float> h13 = interpolate(Vec2{p1.y, h1}, Vec2{p3.y, h3});

    x_values_p12.pop_back();
    std::vector<float> x_values_p123 = x_values_p12;
    x_values_p123.insert(x_values_p123.end(), x_values_p23.begin(), x_values_p23.end());

    h12.pop_back();
    std::vector<float> h123 = h12;
    h123.insert(h123.end(), h23.begin(), h23.end());

    int mid = x_values_p12.size() / 2;
    std::vector<float> x_values_left, x_values_right;
    std::vector<float> h_values_left, h_values_right;

    if (x_values_p13[mid] < x_values_p123[mid]) {
        x_values_left = x_values_p13;
        h_values_left = h13;
        x_values_right = x_values_p123;
        h_values_right = h123;
    } else {
        x_values_left = x_values_p123;
        h_values_left = h123;
        x_values_right = x_values_p13;
        h_values_right = h13;
    }

    for (int y = std::round(p1.y); y <= std::round(p3.y); y++) {
        float x_left = x_values_left[y - p1.y];
        float x_right = x_values_right[y - p1.y];
        float h_left = h_values_left[y - p1.y];
        float h_right = h_values_right[y - p1.y];

        std::vector<float> h_values = interpolate(Vec2{x_left, h_left}, Vec2{x_right, h_right});

        for (int x = std::round(x_left); x <= std::round(x_right); x++) {
            Color shade = color * h_values[x - x_left];
            draw_point(renderer, Vec2{static_cast<float>(x), static_cast<float>(y)}, shade);
        }
    }
}

void draw_triangle_filled(Renderer& renderer, Vec2 p1, Vec2 p2, Vec2 p3, Color color) {
    if (p1.y > p2.y) std::swap(p1, p2);
    if (p1.y > p3.y) std::swap(p1, p3);
    if (p2.y > p3.y) std::swap(p2, p3);

    std::vector<float> x_values_p12 = interpolate(swap_components(p1), swap_components(p2));
    std::vector<float> x_values_p23 = interpolate(swap_components(p2), swap_components(p3));
    std::vector<float> x_values_p13 = interpolate(swap_components(p1), swap_components(p3));

    x_values_p12.pop_back();
    std::vector<float> x_values_p123 = x_values_p12;
    x_values_p123.insert(x_values_p123.end(), x_values_p23.begin(), x_values_p23.end());

    int mid = x_values_p12.size() / 2;
    std::vector<float> x_values_left, x_values_right;
    if (x_values_p13[mid] < x_values_p123[mid]) {
        x_values_left = x_values_p13;
        x_values_right = x_values_p123;
    } else {
        x_values_left = x_values_p123;
        x_values_right = x_values_p13;
    }

    for (int y = std::round(p1.y); y <= std::round(p3.y); y++) {
        for (int x = x_values_left[y - p1.y]; x <= x_values_right[y - p1.y]; x++) {
            draw_point(renderer, Vec2{static_cast<float>(x), static_cast<float>(y)}, color);
        }
    }
}

void draw_triangle_wireframe(Renderer& renderer, Vec2 p1, Vec2 p2, Vec2 p3, Color color) {
    draw_line(renderer, p1, p2, color);
    draw_line(renderer, p2, p3, color);
    draw_line(renderer, p3, p1, color);
}

std::vector<float> interpolate(Vec2 p1, Vec2 p2) {
    std::vector<float> values;
    if (std::round(p1.x) == std::round(p2.x)) {
        values.push_back(p1.y);
        return values;
    } 

    float y = p1.y;
    float slope = (p2.y - p1.y) / (p2.x - p1.x);
    for (int i = std::round(p1.x); i <= std::round(p2.x); i++) {
        values.push_back(y);
        y = y + slope;
    }
    return values;
}

void draw_line(Renderer& renderer, Vec2 p1, Vec2 p2, Color color) {
    if (std::abs(p2.x - p1.x) > std::abs(p2.y - p1.y)) {
        if (p1.x > p2.x) {
            Vec2 temp = p1;
            p1 = p2;
            p2 = temp;
        }

        int x1 = std::round(p1.x);
        int x2 = std::round(p2.x);

        std::vector<float> y_values = interpolate(p1, p2);
        for (int x = x1; x <= x2; x++) {
            draw_point(renderer, Vec2{static_cast<float>(x), y_values[x - x1]}, color);
        }
    } else {
        if (p1.y > p2.y) {
            Vec2 temp = p1;
            p1 = p2;
            p2 = temp;
        }

        int y1 = std::round(p1.y);
        int y2 = std::round(p2.y);

        std::vector<float> x_values = interpolate(Vec2{p1.y, p1.x}, Vec2{p2.y, p2.x});
        for (int y = y1; y <= y2; y++) {
            draw_point(renderer, Vec2{x_values[y - y1], static_cast<float>(y)}, color);
        }
    }
}

void draw_cube(Renderer& renderer) {
    Vec3 v_a_front{-1, 1, 1};
    Vec3 v_b_front{1, 1, 1};
    Vec3 v_c_front{1, -1, 1};
    Vec3 v_d_front{-1, -1, 1};

    Vec3 v_a_back{-1, 1, 3};
    Vec3 v_b_back{1, 1, 3};
    Vec3 v_c_back{1, -1, 3};
    Vec3 v_d_back{-1, -1, 3};

    draw_line(
        renderer, 
        project_vertex(renderer, v_a_front),
        project_vertex(renderer, v_b_front),
        Colors::Blue
    );
    draw_line(
        renderer, 
        project_vertex(renderer, v_b_front),
        project_vertex(renderer, v_c_front),
        Colors::Blue
    );
    draw_line(
        renderer, 
        project_vertex(renderer, v_c_front),
        project_vertex(renderer, v_d_front),
        Colors::Blue
    );
    draw_line(
        renderer, 
        project_vertex(renderer, v_d_front),
        project_vertex(renderer, v_a_front),
        Colors::Blue
    );

    draw_line(
        renderer, 
        project_vertex(renderer, v_a_back),
        project_vertex(renderer, v_b_back),
        Colors::Red
    );
    draw_line(
        renderer, 
        project_vertex(renderer, v_b_back),
        project_vertex(renderer, v_c_back),
        Colors::Red
    );
    draw_line(
        renderer, 
        project_vertex(renderer, v_c_back),
        project_vertex(renderer, v_d_back),
        Colors::Red
    );
    draw_line(
        renderer, 
        project_vertex(renderer, v_d_back),
        project_vertex(renderer, v_a_back),
        Colors::Red
    );

    draw_line(
        renderer, 
        project_vertex(renderer, v_a_front),
        project_vertex(renderer, v_a_back),
        Colors::Green
    );
    draw_line(
        renderer, 
        project_vertex(renderer, v_b_front),
        project_vertex(renderer, v_b_back),
        Colors::Green
    );
    draw_line(
        renderer, 
        project_vertex(renderer, v_c_front),
        project_vertex(renderer, v_c_back),
        Colors::Green
    );
    draw_line(
        renderer, 
        project_vertex(renderer, v_d_front),
        project_vertex(renderer, v_d_back),
        Colors::Green
    );
}
