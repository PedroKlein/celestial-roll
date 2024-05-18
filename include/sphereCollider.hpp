#pragma once

#include "collider.hpp"
#include "collisionDetector.hpp"

class SphereCollider : public Collider
{
  public:
    SphereCollider(float radius) : radius(radius)
    {
    }

    bool checkCollision(const Collider &other) const override
    {
        auto otherSphere =
            std::dynamic_pointer_cast<SphereCollider>(std::shared_ptr<Collider>(const_cast<Collider *>(&other)));
        if (otherSphere && transform)
        {
            return CollisionDetector::pointSphere(transform->getPosition(), otherSphere->transform->getPosition(),
                                                  radius + otherSphere->radius);
        }
        return false;
    }

  private:
    float radius;
};