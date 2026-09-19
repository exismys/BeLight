#include "simulation.hpp"
#include "mathematics.hpp"
#include "types.hpp"
#include <cmath>
#include <unistd.h>

Simulation create_simulation() {

    std::vector<RigidBody> bodies;

    bodies.push_back(RigidBody{
        {3.4641f, 0.0f, 20.0f},
        {0.0f,  2.88675f, 0.0f},
        {0.0f, 0.0f, 0.0f},

        50.0f,
        0.5f,

        Colors::Yellow
    });

    bodies.push_back(RigidBody{
        {-1.7321f, 3.0f, 20.0f},
        {-2.5f, -1.44338f, 0.0f},
        {0.0f, 0.0f, 0.0f},

        50.0f,
        0.5f,

        Colors::Cyan
    });

    bodies.push_back(RigidBody{
        {-1.7321f, -3.0f, 20.0f},
        {2.5f, -1.44338f, 0.0f},
        {0.0f, 0.0f, 0.0f},

        50.0f,
        0.5f,

        Colors::LightGray
    });

    // Three body configuration #1
    // bodies[0].position = {  5.7735f,  0.0f, 25.0f };
    // bodies[1].position = { -2.88675f,  5.0f, 25.0f };
    // bodies[2].position = { -2.88675f, -5.0f, 25.0f };

    // bodies[0].velocity = {  0.0f,     2.23607f, 0.0f };
    // bodies[1].velocity = { -1.93649f, -1.11803f, 0.0f };
    // bodies[2].velocity = {  1.93649f, -1.11803f, 0.0f };

    // Three body configuration #2
    // bodies[0].position = { -0.970004f,  0.243087f, 25.0f };
    // bodies[1].position = {  0.970004f, -0.243087f, 25.0f };
    // bodies[2].position = {  0.0f,       0.0f,      25.0f };

    // bodies[0].velocity = {  3.296f,  3.057f, 0.0f };
    // bodies[1].velocity = {  3.296f,  3.057f, 0.0f };
    // bodies[2].velocity = { -6.592f, -6.114f, 0.0f };

    // Three body configuration #3
    bodies[0].position = { -4.85002f,  1.21544f, 25.0f };
    bodies[1].position = {  4.85002f, -1.21544f, 25.0f };
    bodies[2].position = {  0.0f,      0.0f,      25.0f };

    bodies[0].velocity = {  1.475f,  1.368f, 0.0f };
    bodies[1].velocity = {  1.475f,  1.368f, 0.0f };
    bodies[2].velocity = { -2.950f, -2.736f, 0.0f };

    // Planet position
    bodies.push_back(RigidBody{
        {-3.0f, -3.0f, 30.5f},
        {0.6f, -2.5f, -2.5f},
        {0.0f, 0.0f, 0.0f},

        0.25f,
        0.25f,

        Colors::Green
    });

    bodies[3].position = { 10.0f, 0.0f, 25.0f };
    bodies[3].velocity = { 0.0f, 2.04124f, 0.0f };


    for (RigidBody& body: bodies) {
        body.trail.push_back(body.position);
    }

    return Simulation {
        bodies
    };
}

void update_simulation(Simulation& sim, float dt) {
    apply_gravitational_force(sim);

    for (RigidBody& body: sim.bodies) {
        update_kinematics(body, dt);
    }
}

void apply_gravitational_force(Simulation &sim) {

    float G = 1.0f;

    for (size_t i = 0; i < sim.bodies.size(); i++) {
        sim.bodies[i].acceleration = {0.0f, 0.0f, 0.0f};
    }

    for (size_t i = 0; i < sim.bodies.size(); i++) {
        for (size_t j = i + 1; j < sim.bodies.size(); j++) {

            RigidBody& b1 = sim.bodies[i];
            RigidBody& b2 = sim.bodies[j];

            Vec3 displacement_v = b2.position - b1.position;
            float distance_sq = dot_product(displacement_v, displacement_v);

            if (distance_sq < 0.1f) continue;

            float distance = std::sqrt(distance_sq);

            Vec3 force_dir = displacement_v / distance;

            float force_mag = (G * sim.bodies[i].mass * sim.bodies[j].mass) / distance_sq;
            Vec3 force_ij = force_mag * force_dir;

            apply_force(b1, force_ij);
            apply_force(b2, - force_ij);
        }
    }

}

void apply_force(RigidBody& body, Vec3 force) {
    body.acceleration += force / body.mass;
}

void update_kinematics(RigidBody& body, float dt) {
    body.velocity += body.acceleration * dt;
    body.position += body.velocity * dt;
    update_motion_trail(body);
}

void update_motion_trail(RigidBody& body) {
    if (body.trail.size() >= 400) {
        body.trail.pop_front();
    }
    body.trail.push_back(body.position);
}
