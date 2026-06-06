
#include <iostream>
#include <cstdint>
#include <vector>
#include <chrono>

#include <SDL2/SDL.h>
#include <SDL_keycode.h>

#include "renderer.hpp"
#include "mathematics.hpp"
#include "simulation.hpp"
#include "ray_tracing.hpp"
#include "text.hpp"

constexpr uint32_t WIDTH = 2400;
constexpr uint32_t HEIGHT = 1200;

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
    Scene scene = create_scene();

    // Load font
    Text text("assets/fonts/UbuntuMono[wght].ttf");

    auto start_time = std::chrono::steady_clock::now();
    double accumulator = 0.0;
    constexpr double dt = 1.0 / 60.0;

    bool running = true;
    bool export_frame = false;
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
                if (event.key.keysym.sym == SDLK_e) {
                    export_frame = true;
                }
            }

            // Mouse events
            if (event.type == SDL_MOUSEWHEEL) {
                update_projection_plane_z(event.wheel.y * 0.1f);
            }

            if (event.type == SDL_MOUSEBUTTONDOWN) {
                if (event.button.button == SDL_BUTTON_LEFT) {
                    update_viewport_x(0.1);
                } else if (event.button.button == SDL_BUTTON_RIGHT) {
                    update_viewport_x(-0.1);
                }
            }
        }

        auto end_time = std::chrono::steady_clock::now();
        double frame_time = std::chrono::duration<double>(end_time - start_time).count();
        start_time = end_time;

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
        main_loop(renderer, scene);

        text.draw_text(renderer, "FPS", IVec2{10, 20}, 24.0f, Color{255, 255, 255, 255});

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