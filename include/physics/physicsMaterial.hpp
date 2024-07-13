#pragma once

#include "game/component.hpp"

class PhysicsMaterial final : public Component {
public:
    PhysicsMaterial(const float friction, const float bounciness) : friction(friction), bounciness(bounciness) {}

    [[nodiscard]] float getFriction() const { return friction; }

    void setFriction(const float newFriction) { friction = newFriction; }

    [[nodiscard]] float getBounciness() const { return bounciness; }

    void setBounciness(const float newBounciness) { bounciness = newBounciness; }

private:
    float friction;
    float bounciness;
};
