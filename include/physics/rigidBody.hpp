#pragma once

#include <glm/glm.hpp>
#include <iostream>
#include "game/component.hpp"
#include "transform.hpp"

class RigidBody final : public Component {
public:
    glm::vec4 velocity;
    glm::vec4 inputVelocity;
    glm::vec4 acceleration;
    float maxInputVelocity = 8.0f;
    float inputVelocityDecay = 0.95f;
    bool isGrounded = false;

    explicit RigidBody(float mass) : velocity(0), inputVelocity(0), acceleration(0), forceAccumulator(0), mass(mass) {
        if (mass != 0) {
            inverseMass = 1.0f / mass;
        } else {
            inverseMass = 0;
        }
    }

    void initialize() override {
        transform = gameObject->getComponent<Transform>();
        if (!transform) {
            std::cerr << "RigidBody requires a Transform component.\n";
        }
    }

    void initValues() {
        velocity = glm::vec4(0, 0, 0, 0);
        inputVelocity = glm::vec4(0, 0, 0, 0);
        acceleration = glm::vec4(0, 0, 0, 0);
        forceAccumulator = glm::vec4(0, 0, 0, 0);
    }

    void update(float deltaTime) override {
        if (!transform) {
            return;
        }

        updatePhysics(deltaTime);

        transform->position += velocity * deltaTime;
    }

    void addInputForce(const glm::vec4 &force, float deltaTime) { inputVelocity += force * inverseMass * deltaTime; }

    void applyForce(const glm::vec4 &force) { forceAccumulator += force; }

    void clearAccumulator() { forceAccumulator = glm::vec4(0, 0, 0, 0); }

    void updatePhysics(float deltaTime) {
        if (mass != 0) {
            acceleration = forceAccumulator * inverseMass;
        }

        velocity += acceleration * deltaTime;

        if (glm::length(inputVelocity + velocity) <= maxInputVelocity) {
            velocity += inputVelocity;
        }

        clearAccumulator();

        velocity *= pow(0.99f, deltaTime);
        inputVelocity *= inputVelocityDecay;

        transform->position += velocity * deltaTime;
    }

    [[nodiscard]] float getMass() const { return mass; }

    void setMass(const float newMass) {
        mass = newMass;
        if (newMass != 0) {
            inverseMass = 1.0f / newMass;
        } else {
            inverseMass = 0;
        }
    }

private:
    std::shared_ptr<Transform> transform;
    glm::vec4 forceAccumulator;
    float mass;
    float inverseMass;
};
