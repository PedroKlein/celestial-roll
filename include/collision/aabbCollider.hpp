#pragma once

#include "collider.hpp"
#include "collisionDetector.hpp"
// #include "sphereCollider.hpp"

// TODO: refactor this to exclude circular dependency with sphereCollider
class AABBCollider : public Collider
{
  public:
    AABBCollider(const glm::vec3 &min, const glm::vec3 &max)
        : minBounds(min, 0.0f), maxBounds(max, 0.0f), Collider(ColliderType::AABB)
    {
    }

    CollisionResult checkCollision(const Collider &other) const override
    {
        // not implement properly yet since we only handle collision from the player perspective, which is a sphere

        const AABBCollider *otherBox = dynamic_cast<const AABBCollider *>(&other);
        if (otherBox && transform)
        {

            return CollisionDetector::aabbToAABB(getMinBounds(), getMaxBounds(), otherBox->getMinBounds(),
                                                 otherBox->getMaxBounds());
        }

        // const SphereCollider *otherSphere = dynamic_cast<const SphereCollider *>(&other);
        // if (otherSphere && transform)
        // {
        //     auto result = CollisionDetector::sphereCube(otherSphere->getPosition(), otherSphere->getRadius(),
        //                                                 getMinBounds(), getMaxBounds());

        //     return CollisionResult{result.collided, transformNormal(otherBox->getRotationMatrix(), result.normal)};
        // }

        return CollisionResult{false, glm::vec4(0.0f)};
    }

    glm::vec4 getMinBounds() const
    {
        return minBounds + transform->getPosition();
    }

    glm::vec4 getMaxBounds() const
    {
        return maxBounds + transform->getPosition();
    }

  private:
    glm::vec4 minBounds, maxBounds;
};