#include <cmath>
#include <memory>
#include <span>
#include <utility>
#include <vector>
#include <chrono>
#include <iostream>

#include "rasterizer.hpp"
#include "mathematics.hpp"
#include "renderer.hpp"
#include "types.hpp"

const float PI = 3.14159265359f;

const float viewport_size_x = 1;
const float viewport_size_y = 1;
const float viewport_z = 1;

const float one_by_sqrt_two = 1 / std::sqrt(2);

Plane planes[5] = {
    { Vec3{0, 0, 1}, -1 },
    { Vec3{one_by_sqrt_two, 0, one_by_sqrt_two}, 0 },
    { Vec3{-one_by_sqrt_two, 0, one_by_sqrt_two}, 0 },
    { Vec3{0, one_by_sqrt_two, one_by_sqrt_two}, 0 },
    { Vec3{0, -one_by_sqrt_two, one_by_sqrt_two}, 0 },
};

ObjectMode object_mode = ObjectMode::FILLED;

auto start_time = std::chrono::steady_clock::now();

float get_runtime_seconds() {
    auto now = std::chrono::steady_clock::now();

    return std::chrono::duration<float>(
        now - start_time
    ).count();
}

Mesh create_sphere_mesh(int latitudes, int longitudes, float radius) {
    std::vector<Vec3> vertices;
    std::vector<Triangle> triangles;

    for (int i = 0; i <= latitudes; i++) {
        float theta = i * PI / latitudes;
        float sin_theta = std::sin(theta);
        float cos_theta = std::cos(theta);

        for (int j = 0; j <= longitudes; j++) {
            float phi = j * 2.0f * PI / longitudes;
            float sin_phi = std::sin(phi);
            float cos_phi = std::cos(phi);

            float x = radius * sin_theta * cos_phi;
            float y = radius * cos_theta;
            float z = radius * sin_theta * sin_phi;

            vertices.push_back({x, y, z});
        }
    }

    for (int i = 0; i < latitudes; i++) {
        for (int j = 0; j < longitudes; j++) {

            // Indices for the 4 corners of the current quad
            int top_left = i * (longitudes + 1) + j;
            int top_right = top_left + 1;
            int bottom_left = (i + 1) * (longitudes + 1) + j;
            int bottom_right = bottom_left + 1;


            // Triangles with clockwise winding
            triangles.push_back({
                {top_left, top_right, bottom_left},
                Colors::Yellow 
            });

            triangles.push_back({
                {bottom_left, top_right, bottom_right},
                Colors::Yellow
            });
        }
    }
    
    return Mesh {
        vertices,
        triangles
    };
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

    scene.meshes.push_back(std::make_unique<Mesh>(create_sphere_mesh(10, 10, 1.0f)));
    Mesh* sphere_mesh = scene.meshes[1].get();

    // scene.objects.push_back({
    //     cube_mesh,
    //     Vec3{0.5,0.5, 0.5},
    //     Vec3{0, 0, 0},
    //     Vec3{0, 0, 2.5}
    // });

    scene.objects.push_back({
        sphere_mesh,
        Vec3{0.5,0.5, 0.5},
        Vec3{0, 0, 0},
        Vec3{0, 0, 2.5}
    });

    scene.object_mode = ObjectMode::FILLED;

    std::vector<LightRast> light_sources;
    light_sources.push_back(
        LightRast{
            .type = LightTypeRast::Ambient,
            .intensity = 0.2, 
        }
    );
    light_sources.push_back(
        LightRast{
            .type = LightTypeRast::Point,
            .intensity = 0.6, 
            .position = Vec3{2, 1, 0},
        }
    );
    light_sources.push_back(
        LightRast{
            .type = LightTypeRast::Directional,
            .intensity = 0.2, 
            .direction = Vec3{1, 4, 4},
        }
    );

    scene.light_sources = light_sources;

    return scene;
}

void render_scene_rast(Renderer& renderer, Scene_Rast& scene) {

    Mat4 view = rotation_x_matrix(-scene.camera.rotation.x) *
                rotation_y_matrix(-scene.camera.rotation.y) *
                rotation_z_matrix(-scene.camera.rotation.z) *
                translation_matrix(-scene.camera.position);


    for (Object& object: scene.objects) {
        render_object(renderer, object, view, scene);
    }
}

void render_object(Renderer& renderer, Object& object, Mat4& view, Scene_Rast& scene) {

    Mat4 model = translation_matrix(object.position) *
                 rotation_z_matrix(/*object.rotation.z*/ get_runtime_seconds()) *
                 rotation_y_matrix(object.rotation.y) *
                 rotation_x_matrix(/*object.rotation.x*/ get_runtime_seconds()) *
                 scale_matrix(object.scale);

    Mat4 view_model = view * model;

    std::vector<Vec4> transformed_vertices;

    for (const Vec3& v: object.mesh->vertices) {

        Vec4 local{v.x, v.y, v.z, 1};
        Vec4 world = view_model * local;

        transformed_vertices.push_back(world);
    }

    for (const Triangle& t: object.mesh->triangles) {

        //----------------------------------------------------------------------
        // transformed_vertices has vertex in Vec4 format {x, y, z, w} intended
        // for transformations using 4 by 4 matrices.

        // We need to convert them into Vec3 format.
        //----------------------------------------------------------------------
        Vec3 p0 = { 
            transformed_vertices[t.v[0]].x, 
            transformed_vertices[t.v[0]].y,
            transformed_vertices[t.v[0]].z
        };

        Vec3 p1 = { 
            transformed_vertices[t.v[1]].x, 
            transformed_vertices[t.v[1]].y,
            transformed_vertices[t.v[1]].z
        };

        Vec3 p2 = { 
            transformed_vertices[t.v[2]].x, 
            transformed_vertices[t.v[2]].y,
            transformed_vertices[t.v[2]].z
        };
        //----------------------------------------------------------------------

        Triangle3D triangle_to_clip = {p0, p1, p2, t.color};

        // This call also updates the Triangle3D instance's normal attr
        if (is_back_face(triangle_to_clip)) {
            continue;
        }

        std::vector<Triangle3D> clipped = clip_triangle(triangle_to_clip, planes);

        for (Triangle3D& t: clipped) {
            render_triangle(renderer, t, scene);
        }
    }
}

bool is_back_face(Triangle3D& triangle) {
    Vec3 normal = get_triangle_normal(triangle);

    triangle.normal = normal;

    Vec3 view_vector = - triangle.p0;

    if (dot_product(normal, view_vector) <= 0) {
        return true;
    }
    return false;
}

Vec3 get_triangle_normal(Triangle3D& triangle) {
    Vec3 p0p1 = triangle.p1 - triangle.p0;
    Vec3 p0p2 = triangle.p2 - triangle.p0;

    Vec3 normal = cross_product(p0p1, p0p2);
    return normal;
}

std::vector<Triangle3D> clip_triangle(Triangle3D triangle, std::span<Plane> planes) {
    std::vector<Triangle3D> triangles = { triangle };

    for (Plane& plane: planes) {

        std::vector<Triangle3D> new_triagles;

        for (Triangle3D& t: triangles) {
            std::vector<Triangle3D> clipped = clip_triangle_against_plane(t, plane);
            new_triagles.insert(
                new_triagles.end(),
                clipped.begin(),
                clipped.end()
            );
        }

        triangles = std::move(new_triagles);

        if (triangles.empty()) {
            break;
        }
    }

    return triangles;
}

std::vector<Triangle3D> clip_triangle_against_plane(Triangle3D& triangle, Plane& plane) {
    float d0 = signed_distance(triangle.p0, plane);
    float d1 = signed_distance(triangle.p1, plane);
    float d2 = signed_distance(triangle.p2, plane);

    if (d0 > 0 && d1 > 0 && d2 > 0) {
        return {triangle};
    }

    if (d0 < 0 && d1 < 0 && d2 <0) {
        return {};
    }

    // Case: only one of the vertices is inside

    if (d0 > 0 && d1 < 0 && d2 < 0) {
        Vec3 a = triangle.p0;
        Vec3 b_dash = plane_line_intersection(triangle.p0, triangle.p1, plane);
        Vec3 c_dash = plane_line_intersection(triangle.p0, triangle.p2, plane);
        return { Triangle3D{a, b_dash, c_dash, triangle.color} };
    }

    if (d0 < 0 && d1 > 0 && d2 < 0) {
        Vec3 a_dash = plane_line_intersection(triangle.p1, triangle.p0, plane);
        Vec3 b = triangle.p1;
        Vec3 c_dash = plane_line_intersection(triangle.p1, triangle.p2, plane);
        return { Triangle3D{a_dash, b, c_dash, triangle.color} };
    }

    if (d0 < 0 && d1 < 0 && d2 > 0) {
        Vec3 a_dash = plane_line_intersection(triangle.p2, triangle.p0, plane);
        Vec3 b_dash = plane_line_intersection(triangle.p2, triangle.p1, plane);
        Vec3 c = triangle.p2;
        return { Triangle3D{a_dash, b_dash, c, triangle.color} };
    }

    // Case: two of the vertices are inside

    if (d0 > 0 && d1 > 0 && d2 < 0) {
        Vec3 a = triangle.p0;
        Vec3 b = triangle.p1;
        Vec3 a_dash = plane_line_intersection(triangle.p0, triangle.p2, plane);
        Vec3 b_dash = plane_line_intersection(triangle.p1, triangle.p2, plane);
        return {
            {a, b, a_dash, triangle.color},
            {a_dash, b, b_dash, triangle.color}
        };
    }

    if (d0 > 0 && d1 < 0 && d2 > 0) {
        Vec3 a = triangle.p0;
        Vec3 c = triangle.p2; 
        Vec3 a_dash = plane_line_intersection(triangle.p0, triangle.p1, plane);
        Vec3 c_dash = plane_line_intersection(triangle.p2, triangle.p1, plane);
        return {
            {a,  c, a_dash, triangle.color},
            {a_dash, c, c_dash, triangle.color}
        };
    }

    if (d0 < 0 && d1 > 0 && d2 > 0) {
        Vec3 b = triangle.p1;
        Vec3 c = triangle.p2;
        Vec3 b_dash = plane_line_intersection(triangle.p1, triangle.p0, plane);
        Vec3 c_dash = plane_line_intersection(triangle.p2, triangle.p0, plane);
        return {
            {b, c, b_dash, triangle.color},
            {b_dash, c, c_dash, triangle.color}
        };
    }

    return {};
}

float signed_distance(Vec3 vertex, Plane& plane) {
    return 
    vertex.x * plane.normal.x +
    vertex.y * plane.normal.y +
    vertex.z * plane.normal.z +
    plane.D;
}

Vec3 plane_line_intersection(Vec3 a, Vec3 b, Plane plane) {

    //----------------------------------------------------------------------
    // We have plane equation: n . p + D = 0
    // Side ab can be expressed with a parametric equation: p = a + t(b - a)

    // Point p, at some point, must lie on the plane, if it's to intersect.
    // So, it must satisfy the the equation of the plane: 
    // n . (a + t(b - a)) + D = 0

    // From this, we get the value of t, and subsequently the point of
    // intersection from the parametric equation by substituting the value 
    // of t.
    //----------------------------------------------------------------------
    float t = ( - plane.D - dot_product(plane.normal, a) ) / dot_product(plane.normal, b - a);

    Vec3 intersection_point = a + t * (b -a);

    return intersection_point;
}

void render_triangle(Renderer& renderer, const Triangle3D& triangle, Scene_Rast& scene) {
    if (object_mode == ObjectMode::FILLED) {
        draw_triangle_filled(
            renderer,
            triangle,
            scene
        );
        return;
    }
    draw_triangle_wireframe(
        renderer,
        project_vertex(renderer, triangle.p0),
        project_vertex(renderer, triangle.p1),
        project_vertex(renderer, triangle.p2),
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

void draw_triangle_filled(Renderer& renderer, const Triangle3D& triangle, Scene_Rast& scene) {

    Vec3 t1 = triangle.p0;
    Vec3 t2 = triangle.p1;
    Vec3 t3 = triangle.p2;

    Vec3 center = (t1 + t2 + t3) / 3.0;

    float i = compute_lighting_rast(triangle.normal, center, scene);

    Vec2 p1 = project_vertex(renderer, triangle.p0);
    Vec2 p2 = project_vertex(renderer, triangle.p1);
    Vec2 p3 = project_vertex(renderer, triangle.p2);

    if (p1.y > p2.y) {
        std::swap(p1, p2);
        std::swap(t1, t2);
    }
    if (p1.y > p3.y) {
        std::swap(p1, p3);
        std::swap(t1, t3);
    }
    if (p2.y > p3.y) {
        std::swap(p2, p3);
        std::swap(t2, t3);
    }

    std::vector<float> x_values_p12 = interpolate(
        Vec2{p1.y, p1.x},
        Vec2{p2.y, p2.x} 
    );

    std::vector<float> x_values_p23 = interpolate(
        Vec2{p2.y, p2.x},
        Vec2{p3.y, p3.x} 
    );

    std::vector<float> x_values_p13 = interpolate(
        Vec2{p1.y, p1.x},
        Vec2{p3.y, p3.x} 
    );

    std::vector<float> one_by_z_values_p12 = interpolate(
        Vec2{p1.y, 1 / t1.z},
        Vec2{p2.y, 1 / t2.z} 
    );

    std::vector<float> one_by_z_values_p23 = interpolate(
        Vec2{p2.y, 1 / t2.z},
        Vec2{p3.y, 1 / t3.z} 
    );

    std::vector<float> one_by_z_values_p13 = interpolate(
        Vec2{p1.y, 1 / t1.z},
        Vec2{p3.y, 1 / t3.z} 
    );

    x_values_p12.pop_back();
    std::vector<float> x_values_p123 = x_values_p12;
    x_values_p123.insert(x_values_p123.end(), x_values_p23.begin(), x_values_p23.end());

    one_by_z_values_p12.pop_back();
    std::vector<float> one_by_z_values_p123 = one_by_z_values_p12;
    one_by_z_values_p123.insert(one_by_z_values_p123.end(), one_by_z_values_p23.begin(), one_by_z_values_p23.end());

    int mid = x_values_p12.size() / 2;
    std::vector<float> x_values_left, x_values_right, one_by_z_values_left, one_by_z_values_right;
    if (x_values_p13[mid] < x_values_p123[mid]) {
        x_values_left = x_values_p13;
        x_values_right = x_values_p123;
        one_by_z_values_left = one_by_z_values_p13;
        one_by_z_values_right = one_by_z_values_p123;
    } else {
        x_values_left = x_values_p123;
        x_values_right = x_values_p13;
        one_by_z_values_left = one_by_z_values_p123;
        one_by_z_values_right = one_by_z_values_p13;
    }

    for (int y = std::round(p1.y); y <= std::round(p3.y); y++) {
        float x_left = x_values_left[y - p1.y];
        float x_right = x_values_right[y - p1.y];
        float one_by_z_left = one_by_z_values_left[y - p1.y];
        float one_by_z_right = one_by_z_values_right[y - p1.y];

        //----------------------------------------------------------------------
        // We are avoiding using interpolate function here because it's doing
        // heap allocation for each scanline per frame causing a drop in frame
        // rate.
        // std::vector<float> z_values = interpolate(Vec2{x_left, z_left}, Vec2{x_right, z_right});

        // Instead, we are calculating z_value manually.
        //----------------------------------------------------------------------
        float one_by_z_value = one_by_z_left;
        float d_one_by_z = (one_by_z_right - one_by_z_left) / (x_right - x_left);

        for (int x = std::round(x_left); x <= std::round(x_right); x++) {
            if (one_by_z_value > get_depth_value(renderer, Vec2{static_cast<float>(x), static_cast<float>(y)})) {
                draw_point(renderer, Vec2{static_cast<float>(x), static_cast<float>(y)}, triangle.color * i);
                update_depth_buffer(renderer, Vec2{static_cast<float>(x), static_cast<float>(y)}, one_by_z_value);
            }
            one_by_z_value += d_one_by_z;
        }
    }
}

float compute_lighting_rast(Vec3 normal, Vec3 point, Scene_Rast& scene) {
    float i = 0.0f;
    for (LightRast& light: scene.light_sources) {
        if (light.type == LightTypeRast::Ambient) {
            i += light.intensity;
        } else {
            Vec3 l;
            float max_t;
            if (light.type == LightTypeRast::Point) {
                l = light.position - point;
                max_t = 1.0;
            } else {
                l = light.direction;
                max_t = std::numeric_limits<float>::infinity();
            }

            // auto [shadow_sphere, shadow_t] = closest_intersection(point, l, 0.001, max_t, scene);
            // if  (shadow_sphere.radius != 0.0) {
            //     continue;
            // }

            float n_dot_l = dot_product(normal, l);
            if (n_dot_l > 0) {
                i += light.intensity * n_dot_l / (magnitude(normal) * magnitude(l));
            }

            // if (specular != 1) {
            //     Vec3 r = reflect_ray(normal, l);
            //     float r_dot_v = dot_product(r, view);
            //     if (r_dot_v > 0) {
            //         i += light.intensity * std::pow(r_dot_v / (magnitude(r) * magnitude(view)), specular);
            //     }
            // }
        }
    }
    return i;
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
