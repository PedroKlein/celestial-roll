#pragma once

#include "game/component.hpp"
#include "game/gameObject.hpp"
#include "transform.hpp"
#include <glm/vec3.hpp>
#include <iostream>
#include <memory>

struct LightInfo
{
    glm::vec4 position;
    glm::vec4 color;
};

class LightEmitter : public Component
{
  public:
    LightEmitter(const glm::vec4 &color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)) : color(color)
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

    glm::vec4 getPosition() const
    {
        return transform->getPosition();
    }

    glm::vec4 getColor() const
    {
        return color;
    }

    void setColor(const glm::vec4 &newColor)
    {
        color = newColor;
    }

    LightInfo getLight() const
    {
        return LightInfo{transform->getPosition(), color};
    }

  private:
    std::shared_ptr<Transform> transform;
    glm::vec4 color;
};