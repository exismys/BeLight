#include "SDL2/SDL.h"
#include "renderer_sdl.h"
#include "renderer.h"
#include "mathematics.h"
#include "objects.h"

void draw_particle_sdl(Renderer_SDL& renderer, Particle& particle) {
    draw_circle_sdl(renderer, particle.pos, particle.radius);
}

void draw_circle_sdl(Renderer_SDL& renderer, Vec2 pos, float radius) {
    IVec2 screen_pos = world_to_screen(renderer, pos);
    SDL_RenderDrawPoint(renderer.renderer, screen_pos.x, screen_pos.y);
    SDL_RenderDrawPoint(renderer.renderer, screen_pos.x + 1, screen_pos.y + 1);
    SDL_RenderDrawPoint(renderer.renderer, screen_pos.x + 2, screen_pos.y + 2);
}

IVec2 world_to_screen(const Renderer_SDL& renderer, const Vec2 point) {
    return IVec2{
        static_cast<int>(renderer.width / 2 + point.x),
        static_cast<int>(renderer.height / 2 - point.y)
    };
}