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
    int cx = screen_pos.x;
    int cy = screen_pos.y;
    int r = static_cast<int>(radius);
    for (int y = cy - r; y <= cy + r; y++) {
        for (int x = cx - r; x <= cx + r; x++) {
            int dx = cx - x;
            int dy = cy - y;
            if (dx * dx + dy * dy <= r * r) {
                SDL_RenderDrawPoint(renderer.renderer, x, y);
            }
        }
    }
}

IVec2 world_to_screen(const Renderer_SDL& renderer, const Vec2 point) {
    return IVec2{
        static_cast<int>(renderer.width / 2 + point.x),
        static_cast<int>(renderer.height / 2 - point.y)
    };
}