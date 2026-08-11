#pragma once

#include <deque>
#include <vector>

#include "mathematics.hpp"

struct RigidBody {
    Vec3 position;
    Vec3 velocity;
    Vec3 acceleration;

    float mass;
    float radius;

    std::deque<Vec3> trail;
};

struct Simulation {
    std::vector<RigidBody> bodies;
};

Simulation create_simulation();
void update_simulation(Simulation& sim, float);

void update_kinematics(RigidBody& body, float dt);
void update_motion_trail(RigidBody& body);
void apply_force(RigidBody& body, Vec3 force);
void apply_gravitational_force(Simulation& sim);
