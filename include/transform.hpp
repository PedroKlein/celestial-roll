#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include "game/component.hpp"
#include "math/matrix.hpp"

class Transform final : public Component {
public:
    glm::vec4 position, previousPosition{};
    glm::vec3 scale, previousScale{};
    glm::quat rotation{}, previousRotation{};

    Transform() : position(0.0f, 0.0f, 0.0f, 1.0f), scale(1.0f), rotation(glm::quat()) { saveState(); }

    explicit Transform(const glm::vec3 &position) : position(position, 1.0f), scale(1.0f), rotation(glm::quat()) {
        saveState();
    }

    Transform(const glm::vec3 &position, const glm::vec3 &scale) :
        position(position, 1.0f), scale(scale), rotation(glm::quat()) {
        saveState();
    }

    Transform(const glm::vec3 &position, const glm::vec3 &scale, const glm::vec3 &eulerRotation) :
        position(position, 1.0f), scale(scale) {
        setRotation(eulerRotation);
        saveState();
    }

    void saveState() {
        previousPosition = position;
        previousScale = scale;
        previousRotation = rotation;
    }

    [[nodiscard]] glm::vec4 getPosition() const { return position; }

    void setPosition(const glm::vec3 &position) { this->position = glm::vec4(position, 1.0f); }

    [[nodiscard]] glm::vec3 getScale() const { return scale; }

    void setScale(const glm::vec3 &scale) { this->scale = scale; }

    [[nodiscard]] glm::quat getRotation() const { return rotation; }

    void setRotation(const glm::vec3 &eulerRotation) { rotation = glm::quat(glm::radians(eulerRotation)); }

    [[nodiscard]] glm::mat4 getModelMatrix() const {
        glm::mat4 model = math::translateMatrix(position.x, position.y, position.z);
        model *= glm::toMat4(rotation);
        model *= math::scaleMatrix(scale.x, scale.y, scale.z);
        return model;
    }

    [[nodiscard]] glm::mat4 getRotationMatrix() const { return glm::toMat4(rotation); }

    [[nodiscard]] glm::vec3 getUp() const { return glm::normalize(glm::vec3(0.0f, 1.0f, 0.0f) * rotation); }

    friend std::ostream &operator<<(std::ostream &os, const Transform &transform) {
        os << "Position: " << transform.position.x << ", " << transform.position.y << ", " << transform.position.z
           << std::endl;
        os << "Rotation: " << glm::eulerAngles(transform.rotation).x << ", " << glm::eulerAngles(transform.rotation).y
           << ", " << glm::eulerAngles(transform.rotation).z << std::endl;
        os << "Scale: " << transform.scale.x << ", " << transform.scale.y << ", " << transform.scale.z << std::endl;
        return os;
    }
};
