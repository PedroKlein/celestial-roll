#pragma once

#include "game/component.hpp"
#include "game/gameObject.hpp"
#include "rigidBody.hpp"
#include <memory>

class GravityComponent : public Component
{
  public:
    GravityComponent(const glm::vec4 &gravity = glm::vec4(0.0f, -9.81f, 0.0f, 0.0f)) : gravity(gravity)
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
        // testing wihtout grounded check
        // if (rigidBody && !rigidBody->isGrounded)
        // {
        rigidBody->applyForce(gravity * rigidBody->getMass());
        // }
    }

    void setGravity(const glm::vec4 &newGravity)
    {
        gravity = newGravity;
    }

    glm::vec4 getGravity() const
    {
        return gravity;
    }

  private:
    glm::vec4 gravity;
    std::shared_ptr<RigidBody> rigidBody;
};
