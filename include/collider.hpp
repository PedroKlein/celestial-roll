#pragma once

#include "transform.hpp"
#include <memory>

class Collider
{

  public:
    const Transform &transform;

    Collider(const Transform &transform) : transform(transform)
    {
    }

    virtual bool checkCollision(const Collider &other) const = 0;
    virtual std::unique_ptr<Collider> clone() const = 0;
};