#pragma once

#include "collider.hpp"
#include "collisionDetector.hpp"

#include "aabbCollider.hpp"
#include "obbCollider.hpp"

class SphereCollider : public Collider
{
  public:
    SphereCollider(float radius) : radius(radius), Collider(ColliderType::Sphere)
    {
    }

    CollisionResult checkCollision(const Collider &other) const override
    {
        CollisionResult result{false, glm::vec4(0.0f)};

        const AABBCollider *otherAABB = dynamic_cast<const AABBCollider *>(&other);

        if (otherAABB && transform)
        {

            result = CollisionDetector::sphereToAABB(getPosition(), getRadius(), otherAABB->getMinBounds(),
                                                     otherAABB->getMaxBounds());

            result.normal = transformNormal(otherAABB->getRotationMatrix(), result.normal);
        }

        const OBBCollider *otherOBB = dynamic_cast<const OBBCollider *>(&other);

        if (otherOBB && transform)
        {
            result = CollisionDetector::sphereToOBB(getPosition(), getRadius(), otherOBB->getPosition(),
                                                    otherOBB->getHalfWidths(), otherOBB->getRotationMatrix());

            result.normal = transformNormal(otherOBB->getRotationMatrix(), result.normal);
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