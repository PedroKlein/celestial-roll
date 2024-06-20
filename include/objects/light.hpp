#pragma once

#include "game/gameObject.hpp"
#include "graphics/lightEmitter.hpp"
#include "transform.hpp"
#include <memory>

class Light : public GameObject
{
  public:
    Light(const Transform &transform, const glm::vec3 &color = glm::vec3(1.0f, 1.0f, 1.0f), float intensity = 1.0f)
    {

        this->transform = std::make_shared<Transform>(transform);
        addComponent(this->transform);

        this->lightEmitter = std::make_shared<LightEmitter>(color, intensity);
        addComponent(this->lightEmitter);
    }

    ObjectType getObjectType() const override
    {
        return ObjectType::Light;
    }

  private:
    std::shared_ptr<Transform> transform;
    std::shared_ptr<LightEmitter> lightEmitter;
};
