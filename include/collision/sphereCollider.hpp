#pragma once

#include "collider.hpp"
#include "collisionDetector.hpp"

#include "boxCollider.hpp"

class SphereCollider : public Collider
{
  public:
    SphereCollider(float radius) : radius(radius), Collider(ColliderType::Sphere)
    {
    }

    CollisionResult checkCollision(const Collider &other) const override
    {
        CollisionResult result{false, glm::vec4(0.0f)};

        const BoxCollider *otherBox = dynamic_cast<const BoxCollider *>(&other);
        if (otherBox && transform)
        {

            result = CollisionDetector::sphereCube(getPosition(), getRadius(), otherBox->getMinBounds(),
                                                   otherBox->getMaxBounds());

            result.normal = transformNormal(otherBox->getRotationMatrix(), result.normal);
        }

        return result;
    }

    float getRadius() const
    {
        return radius;
    }

  private:
    float radius;
};