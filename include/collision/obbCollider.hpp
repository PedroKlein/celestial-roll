#pragma once

#include "collider.hpp"
#include "collisionDetector.hpp"
// #include "sphereCollider.hpp"

// TODO: refactor this to exclude circular dependency with sphereCollider
class OBBCollider : public Collider
{
  public:
    OBBCollider( const glm::vec3 &halfWidths)
        : Collider(ColliderType::OBB), halfWidths(halfWidths)
    {
    }

    glm::vec3 getHalfWidths() const
    {
        return halfWidths;
    }


    CollisionResult checkCollision(const Collider &other) const override
    {
        // not implement yet since we only handle collision from the player perspective, which is a sphere
        return CollisionResult{false, glm::vec4(0.0f)};
    }

  private:
    glm::vec3 halfWidths;
};