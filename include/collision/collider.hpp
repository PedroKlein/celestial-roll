#pragma once

#include "game/component.hpp"
#include "game/gameObject.hpp"
#include "transform.hpp"
#include <iostream>
#include <memory>

class Collider : public Component
{
  public:
    Collider() : transform(nullptr)
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

    virtual bool checkCollision(const Collider &other) const = 0;

  protected:
    std::shared_ptr<Transform> transform;
};