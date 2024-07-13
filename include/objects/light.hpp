#pragma once

#include "game/gameObject.hpp"
#include "graphics/lightEmitter.hpp"
#include "transform.hpp"

class Light final : public GameObject
{
  public:
    virtual ~Light() = default;

    explicit Light(const Transform &transform, const glm::vec4 &color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f))
    {

        this->transform = std::make_shared<Transform>(transform);
        addComponent(this->transform);

        this->lightEmitter = std::make_shared<LightEmitter>(color);
        addComponent(this->lightEmitter);
    }

    [[nodiscard]] ObjectType getObjectType() const override
    {
        return ObjectType::Light;
    }

  private:
    std::shared_ptr<Transform> transform;
    std::shared_ptr<LightEmitter> lightEmitter;
};
