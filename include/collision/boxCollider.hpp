#pragma once

#include "collider.hpp"
#include "collisionDetector.hpp"
// #include "sphereCollider.hpp"

// TODO: refactor this to exclude circular dependency with sphereCollider

// currently its only using AABB collision detection, so rotated objects won't have a rotated collider
class BoxCollider : public Collider
{
  public:
    BoxCollider(const glm::vec3 &min, const glm::vec3 &max)
        : minBounds(min, 0.0f), maxBounds(max, 0.0f), Collider(ColliderType::Box)
    {
    }

    CollisionResult checkCollision(const Collider &other) const override
    {
        const BoxCollider *otherBox = dynamic_cast<const BoxCollider *>(&other);
        if (otherBox && transform)
        {

            auto result = CollisionDetector::cubeCube(getMinBounds(), getMaxBounds(), otherBox->getMinBounds(),
                                                      otherBox->getMaxBounds());

            return CollisionResult{result.collided, transformNormal(otherBox->getRotationMatrix(), result.normal)};
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