#pragma once

#include "game/component.hpp"
#include "transform.hpp"
#include <glm/glm.hpp>
#include <iostream>
#include <memory>

class RigidBody : public Component
{
  public:
    glm::vec4 velocity;
    glm::vec4 acceleration;
    bool isGrounded = false;

    RigidBody(float mass) : mass(mass), velocity(0), acceleration(0), forceAccumulator(0)
    {
        if (mass != 0)
        {
            inverseMass = 1.0f / mass;
        }
        else
        {
            inverseMass = 0;
        }
    }

    void initialize() override
    {
        transform = gameObject->getComponent<Transform>();
        if (!transform)
        {
            std::cerr << "RigidBody requires a Transform component.\n";
        }
    }

    void update(float deltaTime) override
    {
        if (!transform)
        {
            return;
        }

        updatePhysics(deltaTime);

        transform->position += velocity * deltaTime;
        isGrounded = false;
    }

    void applyForce(const glm::vec4 &force)
    {
        forceAccumulator += force;
    }

    void clearAccumulator()
    {
        forceAccumulator = glm::vec4(0, 0, 0, 0);
    }

    void updatePhysics(float deltaTime)
    {
        if (mass != 0)
        {
            acceleration = forceAccumulator * inverseMass;
        }

        velocity += acceleration * deltaTime;

        clearAccumulator();

        velocity *= pow(0.99f, deltaTime);

        transform->position += velocity * deltaTime;
    }

    float getMass() const
    {
        return mass;
    }

    void setMass(float newMass)
    {
        mass = newMass;
        if (newMass != 0)
        {
            inverseMass = 1.0f / newMass;
        }
        else
        {
            inverseMass = 0;
        }
    }

  private:
    std::shared_ptr<Transform> transform;
    glm::vec4 forceAccumulator;
    float mass;
    float inverseMass;
};
