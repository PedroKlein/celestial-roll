#pragma once

#include "collisionDetector.hpp"
#include "game/component.hpp"
#include "game/gameObject.hpp"
#include "transform.hpp"
#include <iostream>
#include <memory>

enum class ColliderType
{
    OBB,
    AABB,
    Sphere,
};

class Collider : public Component
{
  public:
    Collider(ColliderType type) : transform(nullptr), type(type)
    {
    }

    void initialize() override
    {
        transform = gameObject->getComponent<Transform>();
        if (!transform)
        {
            std::cerr << "Collider requires a Transform component.\n";
        }
    }

    std::shared_ptr<Transform> getTransform() const
    {
        return transform;
    }

    ColliderType getType() const
    {
        return type;
    }

    glm::mat4 getRotationMatrix() const
    {
        return transform->getRotationMatrix();
    }

    glm::vec4 getPosition() const
    {
        return transform->getPosition();
    }

    virtual CollisionResult checkCollision(const Collider &other) const = 0;

  protected:
    ColliderType type;
    std::shared_ptr<Transform> transform;
};