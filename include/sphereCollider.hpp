#pragma once

#include "collider.hpp"
#include "collisionDetector.hpp"

class SphereCollider : public Collider
{
  public:
    SphereCollider(const Transform &transform, float radius) : Collider(transform), radius(radius)
    {
    }

    float getRadius() const
    {
        return radius;
    }

    void setRadius(float newRadius)
    {
        radius = newRadius;
    }

    bool checkCollision(const Collider &other) const override
    {
        const SphereCollider *otherSphere = dynamic_cast<const SphereCollider *>(&other);
        if (otherSphere)
        {
            return CollisionDetector::pointSphere(transform.position, otherSphere->transform.position, radius);
        }

        return false;
    }

    std::unique_ptr<Collider> clone() const override
    {
        return std::make_unique<SphereCollider>(*this);
    }

  private:
    float radius;
};