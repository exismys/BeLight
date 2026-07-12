#include "simulation.hpp"

Simulation create_simulation() {

    std::vector<RigidBody> bodies;

    bodies.push_back(RigidBody{
        {0.0f, 0.0f, 10.5f},
        {0.0f, 0.0f, 0.0f},
        {0.0f, 0.0f, 0.0f},

        1.0f,
        1.0f
    });

    bodies.push_back(RigidBody{
        {2.0f, 2.0f, 8.5f},
        {0.0f, 0.0f, 0.0f},
        {0.0f, 0.0f, 0.0f},

        1.0f,
        1.0f
    });

    return Simulation {
        bodies
    };
}

void update_simulation(Simulation& sim, float dt) {
    for (RigidBody& body: sim.bodies) {
        apply_force(body, Vec3{1.0, 1.0, 1.0});
        update_kinematics(body, dt);
    }
}

void update_kinematics(RigidBody& body, float dt) {
    body.velocity += body.acceleration * dt;
    body.position += body.velocity * dt;
}

void apply_force(RigidBody& body, Vec3 force) {
    body.acceleration = force / body.mass;
}
