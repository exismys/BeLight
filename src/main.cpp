#include <SDL2/SDL.h>
#include <iostream>
#include <cstdint>
#include <vector>

constexpr uint32_t WIDTH = 800;
constexpr uint32_t HEIGHT = 600;
struct Renderer {
    int width;
    int height;
    std::vector<uint32_t> framebuffer;
};

void put_pixel(Renderer& renderer, int x, int y, uint32_t color) {
    renderer.framebuffer[y * renderer.width + x] = color;
}

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
        SDL_WINDOW_SHOWN
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
        }

        // Update framebuffer
        std::fill(renderer.framebuffer.begin(), renderer.framebuffer.end(), 0xFF202020);
        put_pixel(renderer, WIDTH / 2, HEIGHT / 2, 0xFFFFFFFF);

        SDL_UpdateTexture(
            texture,
            nullptr,
            renderer.framebuffer.data(),
            WIDTH * sizeof(uint32_t)
        );

        SDL_RenderClear(sdl_renderer);
        SDL_RenderCopy(sdl_renderer, texture, nullptr, nullptr);
        SDL_RenderPresent(sdl_renderer);
    }

    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}