#pragma once

#include "component.hpp"
#include "transform.hpp"
#include <glm/glm.hpp>
#include <iostream>
#include <memory>

class RigidBody : public Component
{
  public:
    RigidBody(float mass) : mass(mass), velocity(0, 0, 0), acceleration(0, 0, 0), forceAccumulator(0, 0, 0)
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
    }

    void applyForce(const glm::vec3 &force)
    {
        forceAccumulator += force;
    }

    void clearAccumulator()
    {
        forceAccumulator = glm::vec3(0, 0, 0);
    }

    void updatePhysics(float deltaTime)
    {
        if (mass != 0)
        {
            acceleration = forceAccumulator * inverseMass;
        }

        velocity += acceleration * deltaTime;

        velocity *= pow(0.99f, deltaTime); // example damping

        clearAccumulator();
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
    glm::vec3 velocity;
    glm::vec3 acceleration;
    glm::vec3 forceAccumulator;
    float mass;
    float inverseMass;
};
