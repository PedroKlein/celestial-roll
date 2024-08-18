#pragma once

#include <glm/vec3.hpp>
#include <iostream>
#include "game/component.hpp"
#include "game/gameObject.hpp"
#include "interpolatedTransform.hpp"
#include "transform.hpp"

struct LightInfo {
    glm::vec4 position;
    glm::vec4 color;
    float constant;
    float linear;
    float quadratic;
    float pad;
};

class LightEmitter final : public Component {
public:
    explicit LightEmitter(const glm::vec4 &color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), float constant = 1.0f,
                          float linear = 0.09f, float quadratic = 0.032f) :
        color(color), constant(constant), linear(linear), quadratic(quadratic) {}

    void initialize() override {
        const auto renderer = gameObject->getComponent<Renderer>();

        if (renderer) {
            interpolatedTransform = renderer->interpolatedTransform;
            return;
        }

        const auto transform = gameObject->getComponent<Transform>();
        if (!transform) {
            std::cerr << "LightEmitter requires a Transform component.\n";
        }

        interpolatedTransform = std::make_shared<InterpolatedTransform>(transform.get());
    }

    [[nodiscard]] glm::vec4 getPosition() const { return interpolatedTransform->getPosition(); }

    [[nodiscard]] glm::vec4 getColor() const { return color; }

    void setColor(const glm::vec4 &newColor) { color = newColor; }

    [[nodiscard]] LightInfo getLight() const {
        // slightly above the object
        return LightInfo{interpolatedTransform->getPosition() + glm::vec4(0, 1.2f, 0, 1.0f), color, constant, linear,
                         quadratic};
    }

private:
    std::shared_ptr<InterpolatedTransform> interpolatedTransform;
    glm::vec4 color;
    float constant = 1.0f;
    float linear = 0.09f;
    float quadratic = 0.032f;
};
