#pragma once

#include <glm/vec4.hpp>
#include "game/gameObject.hpp"
#include "glm/trigonometric.hpp"
#include "graphics/materialManager.hpp"
#include "graphics/meshManager.hpp"
#include "input/inputObserver.hpp"
#include "interpolatedTransform.hpp"
#include "math/matrix.hpp"
#include "math/vector.hpp"

class Camera final : public GameObject, public InputObserver {
public:
    static glm::vec4 getWorldUp() { return {0.0f, 1.0f, 0.0f, 0.0f}; }

    explicit Camera(const glm::vec3 &position = glm::vec3(0.0f, 0.0f, 0.0f), const float yaw = 0.0f,
                    const float pitch = 0.0f) : front(glm::vec4(0.0f, 0.0f, -1.0f, 0.0f)), isFreeCam(true) {
        this->yaw = glm::radians(yaw);
        this->pitch = glm::radians(pitch);
        transform = std::make_shared<Transform>(position, glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(90.0f, 0.0f, 0.0f));
        addComponent(transform);

        // addComponent(std::make_shared<Renderer>(MeshManager::getInstance().getMesh("resources/models/camera.obj"),
        //                                         MaterialManager::getInstance().getMaterial("camera")));

        updateCameraVectors();
    }

    void setTarget(const InterpolatedTransform &transform, const float distance) {
        this->target = &transform;
        this->transform->position = target->getPosition() - glm::normalize(front) * distance;
        this->distance = distance;
        isFreeCam = false;
        updateCameraVectors();
    }

    [[nodiscard]] glm::vec4 getPosition() const { return transform->getPosition(); }

    [[nodiscard]] glm::vec4 getFront() const { return front; }

    [[nodiscard]] glm::vec4 getRight() const { return right; }

    [[nodiscard]] glm::mat4 getViewMatrix() const {

        glm::vec4 w = -front;
        glm::vec4 u = math::crossProduct(getWorldUp(), w);

        w = math::normalize(w);
        u = math::normalize(u);

        const glm::vec4 v = math::crossProduct(w, u);

        constexpr glm::vec4 origin = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);

        const glm::vec4 vector = transform->getPosition() - origin;

        return math::Matrix(u.x, u.y, u.z, math::dotProduct(-u, vector), // Line 1
                            v.x, v.y, v.z, math::dotProduct(-v, vector), // Line 2
                            w.x, w.y, w.z, math::dotProduct(-w, vector), // Line 3
                            0.0f, 0.0f, 0.0f, 1.0f // Line 4
        );
    }

    void processKeyboard(const Action action, const float deltaTime) override {
        if (!isFreeCam) {
            updateCameraVectors();
            return;
        }

        const float velocity = movementSpeed * deltaTime;
        glm::vec4 movement(0.0f, 0.0f, 0.0f, 0.0f);

        if (action == FORWARD)
            movement += front * velocity;
        if (action == BACKWARD)
            movement -= front * velocity;
        if (action == LEFT)
            movement -= right * velocity;
        if (action == RIGHT)
            movement += right * velocity;

        transform->position += movement;
    }

    void processMouseMovement(const double dx, const double dy) override {
        yaw -= 0.01f * dx * mouseSensitivity;
        pitch += 0.01f * dy * mouseSensitivity;

        constexpr float pitchMax = glm::radians(89.0f);
        constexpr float pitchMin = -pitchMax;

        if (pitch > pitchMax)
            pitch = pitchMax;

        if (pitch < pitchMin)
            pitch = pitchMin;

        updateCameraVectors();
    }

    void updateCameraVectors() {
        const glm::quat pitchQuat = glm::angleAxis(pitch, glm::vec3(1, 0, 0));
        const glm::quat yawQuat = glm::angleAxis(yaw, glm::vec3(0, 1, 0));

        const glm::quat orientation = yawQuat * pitchQuat;

        constexpr glm::vec4 defaultFront(0.0f, 0.0f, -1.0f, 0.0f);
        const glm::vec4 front = orientation * defaultFront;

        if (isFreeCam) {
            this->front = front;
            this->right = math::normalize(math::crossProduct(this->front, getWorldUp()));
        } else {
            transform->position = target->getPosition() - distance * front;
            this->front = math::normalize(target->getPosition() - transform->getPosition());
            this->right = math::normalize(math::crossProduct(this->front, getWorldUp()));
        }
    }

    [[nodiscard]] ObjectType getObjectType() const override { return ObjectType::Camera; }

private:
    const InterpolatedTransform *target{};

    std::shared_ptr<Transform> transform;

    glm::vec4 front;
    glm::vec4 right{};

    float yaw = 0.0f;
    float pitch = 0.0f;
    float distance = 0.0f;
    float movementSpeed = 4.5f;
    float mouseSensitivity = 0.5f;

    bool isFreeCam;
};
