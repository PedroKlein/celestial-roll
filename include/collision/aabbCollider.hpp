#pragma once

#include "collider.hpp"
#include "collisionDetector.hpp"
// #include "sphereCollider.hpp"

// TODO: refactor this to exclude circular dependency with sphereCollider
class AABBCollider final : public Collider {
public:
    AABBCollider(const glm::vec3 &min, const glm::vec3 &max) :
        Collider(ColliderType::AABB), minBounds(min, 0.0f), maxBounds(max, 0.0f) {}

    [[nodiscard]] CollisionResult checkCollision(const Collider &other) const override {
        // not implement properly yet since we only handle collision from the player perspective, which is a sphere

        if (const auto *otherBox = dynamic_cast<const AABBCollider *>(&other); otherBox && transform) {

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

    [[nodiscard]] glm::vec4 getMinBounds() const { return minBounds + transform->getPosition(); }

    [[nodiscard]] glm::vec4 getMaxBounds() const { return maxBounds + transform->getPosition(); }

private:
    glm::vec4 minBounds, maxBounds;
};
