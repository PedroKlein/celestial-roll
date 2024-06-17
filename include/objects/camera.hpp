#pragma once

#include "game/gameObject.hpp"
#include "glm/trigonometric.hpp"
#include "input/inputObserver.hpp"
#include "interpolatedTransform.hpp"
#include "matrixUtils.hpp"
#include <cmath>
#include <glm/vec4.hpp>

class Camera : public GameObject, public InputObserver
{
  public:
    Camera(const glm::vec3 &position = glm::vec3(0.0f, 0.0f, 0.0f), float yaw = 0.0f, float pitch = 0.0f)
        : front(glm::vec4(0.0f, 0.0f, -1.0f, 0.0f)), isFreeCam(true), worldUp(glm::vec4(0.0f, 1.0f, 0.0f, 0.0f))
    {
        this->yaw = glm::radians(yaw);
        this->pitch = glm::radians(pitch);
        transform = std::make_shared<Transform>(position);
        addComponent(transform);

        updateCameraVectors();
    }

    void setTarget(const InterpolatedTransform &transform, float distance)
    {
        this->target = &transform;
        this->transform->position = target->getPosition() - glm::normalize(front) * distance;
        this->distance = distance;
        isFreeCam = false;
        updateCameraVectors();
    }

    glm::vec4 getPosition() const
    {
        return transform->getPosition();
    }

    glm::vec4 getFront() const
    {
        return front;
    }

    glm::vec4 getRight() const
    {
        return right;
    }

    glm::mat4 getViewMatrix()
    {

        glm::vec4 w = -front;
        glm::vec4 u = MatrixUtils::crossProduct(worldUp, w);

        w = MatrixUtils::normalize(w);
        u = MatrixUtils::normalize(u);

        glm::vec4 v = MatrixUtils::crossProduct(w, u);

        glm::vec4 origin = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);

        glm::vec4 vector = transform->getPosition() - origin;

        float ux = u.x;
        float uy = u.y;
        float uz = u.z;
        float vx = v.x;
        float vy = v.y;
        float vz = v.z;
        float wx = w.x;
        float wy = w.y;
        float wz = w.z;

        return MatrixUtils::Matrix(ux, uy, uz, MatrixUtils::dotProduct(-u, vector), // Line 1
                                   vx, vy, vz, MatrixUtils::dotProduct(-v, vector), // Line 2
                                   wx, wy, wz, MatrixUtils::dotProduct(-w, vector), // Line 3
                                   0.0f, 0.0f, 0.0f, 1.0f                           // Line 4
        );
    }

    void processKeyboard(Action action, float deltaTime) override
    {
        if (!isFreeCam)
        {
            updateCameraVectors();
            return;
        }

        float velocity = movementSpeed * deltaTime;
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

    void processMouseMovement(double dx, double dy) override
    {
        yaw -= 0.01f * dx * mouseSensitivity;
        pitch += 0.01f * dy * mouseSensitivity;

        float pitchMax = glm::radians(89.0f);
        float pitchMin = -pitchMax;

        if (pitch > pitchMax)
            pitch = pitchMax;

        if (pitch < pitchMin)
            pitch = pitchMin;

        updateCameraVectors();
    }

    void updateCameraVectors()
    {
        glm::vec4 direction;
        direction.x = cos(pitch) * sin(yaw);
        direction.y = sin(pitch);
        direction.z = cos(pitch) * cos(yaw);
        direction.w = 0.0f;

        if (isFreeCam)
        {
            front = direction;
            right = MatrixUtils::crossProduct(front, worldUp);
        }
        else
        {
            transform->position = target->getPosition() - distance * direction;
            front = MatrixUtils::normalize(target->getPosition() - transform->getPosition());
            right = MatrixUtils::crossProduct(front, worldUp);
        }
    }

    ObjectType getObjectType() const override
    {
        return ObjectType::Camera;
    }

  private:
    const InterpolatedTransform *target;

    std::shared_ptr<Transform> transform;

    glm::vec4 front;
    glm::vec4 up;
    glm::vec4 right;
    glm::vec4 worldUp;

    float yaw = 0.0f;
    float pitch = 0.0f;
    float distance = 0.0f;
    float movementSpeed = 4.5f;
    float mouseSensitivity = 0.5f;

    bool isFreeCam;
};
