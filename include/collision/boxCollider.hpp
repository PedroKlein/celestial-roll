#pragma once

#include "collider.hpp"
#include "collisionDetector.hpp"

class BoxCollider : public Collider
{
  public:
    BoxCollider(const glm::vec3 &min, const glm::vec3 &max) : minBounds(min, 0.0f), maxBounds(max, 0.0f)
    {
    }

    bool checkCollision(const Collider &other) const override
    {
        const BoxCollider *otherBox = dynamic_cast<const BoxCollider *>(&other);
        if (otherBox && transform)
        {

            return CollisionDetector::cubeCube(transform->getPosition() + minBounds,
                                               transform->getPosition() + maxBounds,
                                               otherBox->transform->getPosition() + otherBox->minBounds,
                                               otherBox->transform->getPosition() + otherBox->maxBounds);
        }
        return false;
    }

    glm::vec4 getMinBounds() const
    {
        return transform->getPosition() + minBounds;
    }
    glm::vec4 getMaxBounds() const
    {
        return transform->getPosition() + maxBounds;
    }

  private:
    glm::vec4 minBounds, maxBounds;
};