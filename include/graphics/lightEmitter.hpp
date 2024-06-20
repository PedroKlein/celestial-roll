#pragma once

#include "game/component.hpp"
#include "game/gameObject.hpp"
#include "transform.hpp"
#include <glm/vec3.hpp>
#include <iostream>
#include <memory>

class LightEmitter : public Component
{
  public:
    LightEmitter(const glm::vec3 &color = glm::vec3(1.0f, 1.0f, 1.0f), float intensity = 1.0f)
        : color(color), intensity(intensity)
    {
    }

    void initialize() override
    {
        transform = gameObject->getComponent<Transform>();
        if (!transform)
        {
            std::cerr << "LightEmitter requires a Transform component.\n";
        }
    }

    glm::vec3 getPosition() const
    {
        return transform ? transform->getPosition() : glm::vec3(0.0f);
    }

    glm::vec3 getColor() const
    {
        return color;
    }

    void setColor(const glm::vec3 &newColor)
    {
        color = newColor;
    }

    float getIntensity() const
    {
        return intensity;
    }

    void setIntensity(float newIntensity)
    {
        intensity = newIntensity;
    }

  private:
    std::shared_ptr<Transform> transform;
    glm::vec3 color;
    float intensity;
};