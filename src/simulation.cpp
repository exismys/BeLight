#include <iostream>
#include "simulation.hpp"

Simulation create_simulation() {
    // Initialize particles
    std::vector<Particle> particles;
    for (int i = 0; i < 4; i++) {
        particles.push_back(Particle{ Vec2{-200.0f, -200.0f + i * 100}, Vec2{0, 0}, Vec2{0, 0}, float(i + 1), float(i + 1) * 10, 0xFFFFFFFF});
    }

    return Simulation{
        particles
    };
}

void update_simulation(Simulation& sim, float dt) {
    // std::cout << "particle.acc: " << sim.particles[0].acc.x << '\n';
    for (Particle& particle: sim.particles) {
        apply_force(particle, Vec2{1.0, 1.0});
        update_particle(particle, dt);
    }
}

void update_particle(Particle& particle, float dt) {
    particle.vel += particle.acc * dt;
    particle.pos += particle.vel * dt;
}

void apply_force(Particle& particle, Vec2 force) {
    particle.acc = force / particle.mass;
}