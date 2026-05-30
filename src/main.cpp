#include <SDL2/SDL.h>
#include <iostream>
#include <cstdint>
#include <vector>
#include "renderer.h"
#include "mathematics.h"

enum class RenderMode {
    Software,
    SDL
};

RenderMode mode = RenderMode::SDL;

constexpr uint32_t WIDTH = 800;
constexpr uint32_t HEIGHT = 600;

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
        WIDTH,
        HEIGHT,
        std::vector<uint32_t>(WIDTH * HEIGHT)
    };

    bool running = true;
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
                }
            }

            // Key events
            if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_ESCAPE) {
                    running = false;
                }

                if (event.key.keysym.sym == SDLK_1) {
                    mode = RenderMode::Software;
                }
                if (event.key.keysym.sym == SDLK_2) {
                    mode = RenderMode::SDL;
                }
            }
        }

        if (mode == RenderMode::Software) {
            // Update framebuffer
            std::fill(renderer.framebuffer.begin(), renderer.framebuffer.end(), 0xFF202020);
            draw_point(renderer, Vec2 {static_cast<float>(renderer.width / 4), static_cast<float>(renderer.height / 4)}, 0xFFFFFFFF);

            SDL_UpdateTexture(
                texture,
                nullptr,
                renderer.framebuffer.data(),
                renderer.width * sizeof(uint32_t)
            );

            SDL_RenderCopy(sdl_renderer, texture, nullptr, nullptr);
        } else {
            SDL_SetRenderDrawColor(sdl_renderer, 32, 32, 32, 255);
            SDL_RenderClear(sdl_renderer);
            SDL_SetRenderDrawColor(sdl_renderer, 255, 255, 255, 255);
            SDL_RenderDrawLine(sdl_renderer, 100, 100, 700, 500);
        }

        SDL_RenderPresent(sdl_renderer);
    }

    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}