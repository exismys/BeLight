
#include <SDL_keyboard.h>
#include <SDL_stdinc.h>
#include <cmath>
#include <format>
#include <iostream>
#include <cstdint>
#include <numbers>
#include <string>
#include <vector>
#include <chrono>

#include <SDL2/SDL.h>
#include <SDL_keycode.h>

#include "renderer.hpp"
#include "mathematics.hpp"
#include "simulation.hpp"
#include "ray_tracer.hpp"
#include "rasterizer.hpp"
#include "text.hpp"

constexpr uint32_t WIDTH = 1000;
constexpr uint32_t HEIGHT = 1000;

int main() {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cerr << "SDL_Init failed: " << SDL_GetError() << '\n';
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow(
        "BeLight",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        WIDTH,
        HEIGHT,
        SDL_WINDOW_RESIZABLE
    );
    if (window == nullptr) {
        std::cerr << "SDL_CreateWindow failed: " << SDL_GetError() << '\n';
        SDL_Quit();
        return 1;
    }

    SDL_Renderer* sdl_renderer = SDL_CreateRenderer(
        window,
        -1,
        SDL_RENDERER_ACCELERATED
    );
    if (sdl_renderer == nullptr) {
        std::cerr << SDL_GetError() << '\n';
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    SDL_Texture* texture = SDL_CreateTexture(
        sdl_renderer,
        SDL_PIXELFORMAT_ARGB8888,
        SDL_TEXTUREACCESS_STREAMING,
        WIDTH,
        HEIGHT
    );
    if (texture == nullptr) {
        std::cerr << SDL_GetError() << '\n';
        SDL_DestroyRenderer(sdl_renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    Renderer renderer{
        std::vector<uint32_t>(WIDTH * HEIGHT),
        WIDTH,
        HEIGHT
    };

    // Simulation simulation = create_simulation();
    Scene scene = create_scene(); // ray traced scene
    Scene_Rast scene_rast = create_scene_rast();
    

    // Load font
    Text text("assets/fonts/UbuntuMono[wght].ttf");

    auto start_time = std::chrono::steady_clock::now();
    double accumulator = 0.0;
    double frame_timer = 0.0;
    int frame_count = 0;
    float average_fps = 0;
    constexpr double dt = 1.0 / 60.0;

    bool running = true;
    bool export_frame = false;

    SDL_SetRelativeMouseMode(SDL_TRUE);
    SDL_Event event;

    while (running) {
        // Process events
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }

            // Window resize event
            if (event.type == SDL_WINDOWEVENT) {
                if (event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED) {
                    int width = event.window.data1;
                    int height = event.window.data2;
                    renderer.width = width;
                    renderer.height = height;
                    renderer.framebuffer.resize(width * height);
                    SDL_DestroyTexture(texture);
                    texture = SDL_CreateTexture(
                        sdl_renderer,
                        SDL_PIXELFORMAT_ARGB8888,
                        SDL_TEXTUREACCESS_STREAMING,
                        width,
                        height
                    );
                    if (texture == nullptr) {
                        std::cerr << SDL_GetError() << '\n';
                        running = false;
                    }
                }
            }

            // Key events
            if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_ESCAPE) {
                    running = false;
                }

                if (event.key.keysym.sym == SDLK_1) {
                    update_light_pos(scene, -0.1f);
                }
                if (event.key.keysym.sym == SDLK_2) {
                    update_light_pos(scene, 0.1f);
                }
                if (event.key.keysym.sym == SDLK_UP) {
                    update_camera_pos(Vec3{0, 0, 0.1});
                }
                if (event.key.keysym.sym == SDLK_DOWN) {
                    update_camera_pos(Vec3{0, 0, -0.1});
                }
                if (event.key.keysym.sym == SDLK_LEFT) {
                    update_camera_pos(Vec3{-0.1, 0, 0});
                }
                if (event.key.keysym.sym == SDLK_RIGHT) {
                    update_camera_pos(Vec3{0.1, 0, 0});
                }
                if (event.key.keysym.sym == SDLK_e) {
                    export_frame = true;
                }
            }

            // Mouse events
            if (event.type == SDL_MOUSEWHEEL) {
                update_projection_plane_z(event.wheel.y * 0.1f);
            }

            // if (event.type == SDL_MOUSEBUTTONDOWN) {
            //     if (event.button.button == SDL_BUTTON_LEFT) {
            //     } else if (event.button.button == SDL_BUTTON_RIGHT) {
            //     }
            // }

            if (event.type == SDL_MOUSEMOTION) {

                const float sensitivity = 0.002f;

                // ------------------------------------------------------------------
                // event.motion.xrel = +ve when the mouse moves right.

                // Coordinate system convention: +ve yaw = anti-clockwise rotation
                // about y-axis (camera turns left).

                // We need camera to turn right for +ve yaw.
                // So, we make the event.motion.xrel -ve.
                // ------------------------------------------------------------------
                scene_rast.camera.rotation.y += -event.motion.xrel * sensitivity;

                // ------------------------------------------------------------------
                // event.motion.xrel = +ve when the mouse moves down.

                // Coordinate system convention: +ve pitch = anti-clockwise rotation
                // about x-axis (camera turns down).

                // We need this behavior to remain same.
                // ------------------------------------------------------------------
                scene_rast.camera.rotation.x += event.motion.yrel * sensitivity;

                scene_rast.camera.rotation.x = std::clamp(
                    scene_rast.camera.rotation.x,
                    -1.5f,
                    1.5f
                );
            }
        }

        // -------------------------------------------------------------
        // Handle Camera movement
        // -------------------------------------------------------------
        const Uint8* keyboard = SDL_GetKeyboardState(nullptr);

        float pitch = scene_rast.camera.rotation.x;
        float yaw = scene_rast.camera.rotation.y;

        Vec3 forward = {
            - std::cos(pitch) * std::sin(yaw),
            std::sin(pitch),
            std::cos(pitch) * std::cos(yaw)
        };

        Vec3 movement{};
        float speed = 5.0f;

        if (keyboard[SDL_SCANCODE_W])
            movement += forward;
        if (keyboard[SDL_SCANCODE_S])
            movement -= forward;

        scene_rast.camera.position += movement * speed * dt;
        // -------------------------------------------------------------

        auto end_time = std::chrono::steady_clock::now();
        double frame_time = std::chrono::duration<double>(end_time - start_time).count();
        start_time = end_time;

        frame_timer += frame_time;
        frame_count += 1;
        if (frame_timer >= 0.5) {
            average_fps = frame_count / frame_timer;
            frame_timer = 0.0f;
            frame_count = 0;
        }

        // Update simulation
        // Fixed timestep integration
        accumulator += frame_time;
        while (accumulator >= dt) {
            // update_simulation(simulation, dt);
            accumulator -= dt;
        }

        // Clear framebuffer
        std::fill(renderer.framebuffer.begin(), renderer.framebuffer.end(), 0xFF202020);

        // Modify framebuffer
        // render_simulation(renderer, simulation);
        // render_ray_traced_scene(renderer, scene);
        // draw_line(renderer, {100, 100}, {500, 500}, Color{255, 255, 255, 255});
        // draw_triangle_filled(renderer, Vec2{-200, 200}, Vec2{200, 200}, Vec2{0, -200}, Color{255, 255, 0, 255});
        // draw_triangle_shaded(renderer, Vec2{-200, 200}, Vec2{200, 200}, Vec2{0, -200}, Color{255, 255, 0, 255}, 0.0, 0.0, 1.0);
        // draw_triangle_wireframe(renderer, Vec2{-200, 200}, Vec2{200, 200}, Vec2{0, -200}, Color{255, 255, 255, 255});
        // draw_cube(renderer);
        render_scene_rast(renderer, scene_rast);
        // Render info
        text.draw_text(renderer, std::format("FPS: {:.2f}", average_fps), IVec2{10, 40}, 24.0f, Color{255, 255, 255, 255});

        if (export_frame) {
            save_framebuffer(renderer, "rendered.png");
            export_frame = false;
        }

        SDL_UpdateTexture(
            texture,
            nullptr,
            renderer.framebuffer.data(),
            renderer.width * sizeof(uint32_t)
        );

        SDL_RenderCopy(sdl_renderer, texture, nullptr, nullptr);
        SDL_RenderPresent(sdl_renderer);
    }

    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(sdl_renderer);
    SDL_DestroyWindow(window);

    SDL_Quit();

    return 0;
}