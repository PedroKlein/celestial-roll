#pragma once

#include "collider.hpp"
#include "collisionDetector.hpp"
#include <glm/glm.hpp>
#include <iostream>

class BoxCollider : public Collider
{
  public:
    BoxCollider(const Transform &transform, const glm::vec3 &min, const glm::vec3 &max)
        : Collider(transform), minBounds(min), maxBounds(max)
    {
    }

    bool checkCollision(const Collider &other) const override
    {
        const BoxCollider *otherbox = dynamic_cast<const BoxCollider *>(&other);

        if (otherbox)
        {
            return CollisionDetector::cubeCube(getMinBounds(), getMaxBounds(), otherbox->getMinBounds(),
                                               otherbox->getMaxBounds());
        }

        return false;
    }

    std::unique_ptr<Collider> clone() const override
    {
        return std::make_unique<BoxCollider>(*this);
    }

    glm::vec3 getMinBounds() const
    {
        return transform.position + minBounds;
    }

    glm::vec3 getMaxBounds() const
    {
        return transform.position + maxBounds;
    }

  private:
    glm::vec3 minBounds;
    glm::vec3 maxBounds;
};