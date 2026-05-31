#include "objects.h"

void update_particle(Particle& particle, float dt) {
    particle.vel += particle.acc * dt;
    particle.pos += particle.vel * dt;
}