#pragma once

#include <SDL2/SDL.h>
#include "mathematics.h"
#include "objects.h"

struct Renderer_SDL {
    SDL_Renderer* renderer;
    int width;
    int height;
};

void draw_particle_sdl(Renderer_SDL& renderer, Particle& particle);
void draw_circle_sdl(Renderer_SDL& renderer, Vec2 pos, float radius); 
IVec2 world_to_screen(const Renderer_SDL& renderer, const Vec2 point);
 