#pragma once

#include <glm/vec3.hpp>
#include <iostream>
#include "game/component.hpp"
#include "game/gameObject.hpp"
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
        transform = gameObject->getComponent<Transform>();
        if (!transform) {
            std::cerr << "LightEmitter requires a Transform component.\n";
        }
    }

    [[nodiscard]] glm::vec4 getPosition() const { return transform->getPosition(); }

    [[nodiscard]] glm::vec4 getColor() const { return color; }

    void setColor(const glm::vec4 &newColor) { color = newColor; }

    [[nodiscard]] LightInfo getLight() const {
        return LightInfo{transform->getPosition(), color, constant, linear, quadratic};
    }

private:
    std::shared_ptr<Transform> transform;
    glm::vec4 color;
    float constant = 1.0f;
    float linear = 0.09f;
    float quadratic = 0.032f;
};
