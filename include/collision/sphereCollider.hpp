#pragma once

#include "collider.hpp"
#include "collisionDetector.hpp"

class SphereCollider : public Collider
{
  public:
    SphereCollider(float radius) : radius(radius), Collider(ColliderType::Sphere)
    {
    }

    CollisionResult checkCollision(const Collider &other) const override
    {
        auto otherSphere =
            std::dynamic_pointer_cast<SphereCollider>(std::shared_ptr<Collider>(const_cast<Collider *>(&other)));
        if (otherSphere && transform)
        {
            auto result = CollisionDetector::pointSphere(
                transform->getPosition(), otherSphere->transform->getPosition(), radius + otherSphere->radius);

            return CollisionResult{result.collided, transformNormal(otherSphere->getRotationMatrix(), result.normal)};
        }
        return CollisionResult{false, glm::vec4(0.0f)};
    }

  private:
    float radius;
};