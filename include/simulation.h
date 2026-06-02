#pragma once

#include <cstdint>
#include <vector>
#include "mathematics.h"

struct Particle {
    Vec2 pos;
    Vec2 vel;
    Vec2 acc;
    float mass;
    float radius;
    uint32_t color; 
};

struct Simulation {
    std::vector<Particle> particles;
};

Simulation create_simulation();
void update_simulation(Simulation& sim, float);

void update_particle(Particle& particle, float dt);
void apply_force(Particle& particle, Vec2 force); 
