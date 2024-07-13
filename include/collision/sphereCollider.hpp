#pragma once

#include "collider.hpp"
#include "collisionDetector.hpp"

#include "aabbCollider.hpp"
#include "obbCollider.hpp"

class SphereCollider : public Collider {
public:
    explicit SphereCollider(const float radius) : Collider(ColliderType::Sphere), radius(radius) {}

    [[nodiscard]] CollisionResult checkCollision(const Collider &other) const override {
        CollisionResult result{false, glm::vec4(0.0f)};

        if (const auto otherAABB = dynamic_cast<const AABBCollider *>(&other); otherAABB && transform) {

            result = CollisionDetector::sphereToAABB(getPosition(), getRadius(), otherAABB->getMinBounds(),
                                                     otherAABB->getMaxBounds());
        }

        if (const auto otherOBB = dynamic_cast<const OBBCollider *>(&other); otherOBB && transform) {
            result = CollisionDetector::sphereToOBB(getPosition(), getRadius(), otherOBB->getPosition(),
                                                    otherOBB->getHalfWidths(), otherOBB->getRotationMatrix());
        }

        return result;
    }

    [[nodiscard]] float getRadius() const { return radius; }

private:
    float radius;
};
