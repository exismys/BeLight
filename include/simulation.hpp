#pragma once

#include <vector>

#include "mathematics.hpp"

struct RigidBody {
    Vec3 position;
    Vec3 velocity;
    Vec3 acceleration;

    float mass;
    float radius;
};

struct Simulation {
    std::vector<RigidBody> bodies;
};

Simulation create_simulation();
void update_simulation(Simulation& sim, float);

void update_kinematics(RigidBody& body, float dt);
void apply_force(RigidBody& body, Vec3 force); 
