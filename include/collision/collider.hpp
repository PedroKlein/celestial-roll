#pragma once

#include "game/component.hpp"
#include "game/gameObject.hpp"
#include "transform.hpp"
#include <iostream>
#include <memory>

enum class ColliderType
{
    Box,
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

    virtual bool checkCollision(const Collider &other) const = 0;

  protected:
    ColliderType type;
    std::shared_ptr<Transform> transform;
};