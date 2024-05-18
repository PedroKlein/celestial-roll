#pragma once

#include "component.hpp"
#include "gameObject.hpp"
#include "rigidBody.hpp"
#include <memory>

class GravityComponent : public Component
{
  public:
    GravityComponent(const glm::vec3 &gravity = glm::vec3(0.0f, -9.81f, 0.0f)) : gravity(gravity)
    {
    }

    void initialize() override
    {
        rigidBody = gameObject->getComponent<RigidBody>();
        if (!rigidBody)
        {
            std::cerr << "GravityComponent requires a RigidBody component to function.\n";
        }
    }

    void update(float deltaTime) override
    {
        if (rigidBody)
        {
            rigidBody->applyForce(gravity * rigidBody->getMass());
        }
    }

    void setGravity(const glm::vec3 &newGravity)
    {
        gravity = newGravity;
    }

    glm::vec3 getGravity() const
    {
        return gravity;
    }

  private:
    glm::vec3 gravity;
    std::shared_ptr<RigidBody> rigidBody;
};
