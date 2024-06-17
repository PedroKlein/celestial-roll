#pragma once

#include "collider.hpp"
#include "collisionDetector.hpp"
// #include "sphereCollider.hpp"

// TODO: refactor this to exclude circular dependency with sphereCollider
class OBBCollider : public Collider
{
  public:
    OBBCollider(const glm::vec3 &center, const glm::vec3 &halfWidths, const glm::mat3 &orientation)
        : Collider(ColliderType::OBB), center(center, 0.0f), halfWidths(halfWidths), orientation(orientation)
    {
    }

    glm::vec4 getCenter() const
    {
        return center + transform->getPosition();
    }

    glm::vec3 getHalfWidths() const
    {
        return halfWidths;
    }

    glm::mat3 getOrientation() const
    {
        return orientation;
    }

    CollisionResult checkCollision(const Collider &other) const override
    {
        // not implement yet since we only handle collision from the player perspective, which is a sphere

        return CollisionResult{false, glm::vec4(0.0f)};
    }

  private:
    glm::vec4 center;
    glm::vec3 halfWidths;
    glm::mat3 orientation;
};