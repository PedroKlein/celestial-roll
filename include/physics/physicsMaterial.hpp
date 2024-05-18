#pragma once

#include "component.hpp"
#include <memory>

class PhysicsMaterial : public Component
{
  public:
    PhysicsMaterial(float friction, float bounciness) : friction(friction), bounciness(bounciness)
    {
    }

    float getFriction() const
    {
        return friction;
    }

    void setFriction(float newFriction)
    {
        friction = newFriction;
    }

    float getBounciness() const
    {
        return bounciness;
    }

    void setBounciness(float newBounciness)
    {
        bounciness = newBounciness;
    }

  private:
    float friction;
    float bounciness;
};
