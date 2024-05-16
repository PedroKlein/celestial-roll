#pragma once

#include "gameObject.hpp"
#include <glm/glm.hpp>
#include <vector>

class PhysicsObject : public GameObject
{
  public:
    static const glm::vec4 GRAVITY;

    PhysicsObject(const Mesh &mesh, float mass) : GameObject(mesh), velocity(0.0f), mass(mass)
    {
    }
    PhysicsObject(const Mesh &mesh, const glm::vec3 &position, float mass)
        : GameObject(mesh, position), velocity(0.0f), mass(mass)
    {
    }
    PhysicsObject(const Mesh &mesh, const glm::vec3 &position, const glm::vec3 &scale, float mass)
        : GameObject(mesh, position, scale), velocity(0.0f), mass(mass)
    {
    }
    PhysicsObject(const Mesh &mesh, const glm::vec3 &position, const glm::vec3 &rotation, const glm::vec3 &scale,
                  float mass)
        : GameObject(mesh, position, rotation, scale), velocity(0.0f), mass(mass)
    {
    }

    void addGravitationalForce(const PhysicsObject &source)
    {
        static const float G = 6.67430e-11; // Gravitational constant

        glm::vec4 r = source.getPosition() - getPosition();
        float distance = MatrixUtils::norm(r);
        glm::vec4 direction = MatrixUtils::normalize(r);

        glm::vec4 force = G * (mass * source.mass) / (distance * distance) * direction;
        addForce(force);
    }

    void update(float deltaTime)
    {
        glm::vec4 netForce = GRAVITY * mass;
        for (const glm::vec4 &force : forces)
        {
            netForce += force;
        }

        velocity += netForce / mass * deltaTime;

        transform.position += glm::vec3(velocity) * deltaTime;

        forces.clear();
    }

  private:
    std::vector<glm::vec4> forces;

    glm::vec4 velocity;
    float mass;

    void addForce(const glm::vec4 &force)
    {
        forces.push_back(force);
    }
};

const glm::vec4 PhysicsObject::GRAVITY = glm::vec4(0.0f, -9.8f, 0.0f, 0.0f);