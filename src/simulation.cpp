#include "simulation.hpp"
#include "mathematics.hpp"
#include <cmath>
#include <unistd.h>

Simulation create_simulation() {

    std::vector<RigidBody> bodies;

    bodies.push_back(RigidBody{
        {-2.0f, -2.0f, 30.5f},
        {0.0f, 0.0f, 0.0f},
        {0.0f, 0.0f, 0.0f},

        1.0f,
        1.0f
    });

    bodies.push_back(RigidBody{
        {2.0f, 2.0f, 30.5f},
        {-0.6f, 0.5f, 0.0f},
        {0.0f, 0.0f, 0.0f},

        1.0f,
        1.0f
    });

    bodies.push_back(RigidBody{
        {2.0f, -2.0f, 30.5f},
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
    apply_gravitational_force(sim);
    
    for (RigidBody& body: sim.bodies) {
        update_kinematics(body, dt);
    }
}

void apply_gravitational_force(Simulation &sim) {

    float G = 20.0f;

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
}
