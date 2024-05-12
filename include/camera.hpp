#pragma once

#include "glm/trigonometric.hpp"
#include "matrixUtils.hpp"
#include <cmath>
#include <glm/vec4.hpp>

enum Camera_Movement
{
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

class Camera
{
  public:
    glm::vec4 position;
    glm::vec4 front;
    glm::vec4 up;
    glm::vec4 right;
    glm::vec4 worldUp;

    float yaw;
    float pitch;
    float movementSpeed;
    float mouseSensitivity;

    Camera(glm::vec4 position = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f), glm::vec4 up = glm::vec4(0.0f, 1.0f, 0.0f, 0.0f),
           float yaw = 0.0f, float pitch = 0.0f, float movementSpeed = 2.5f, float mouseSensitivity = 0.5f)
        : front(glm::vec4(0.0f, 0.0f, -1.0f, 0.0f)), movementSpeed(movementSpeed), mouseSensitivity(mouseSensitivity)
    {
        this->position = position;
        this->worldUp = up;
        this->yaw = yaw;
        this->pitch = pitch;
        updateCameraVectors();
    }

    glm::mat4 GetViewMatrix()
    {

        glm::vec4 w = -front;
        glm::vec4 u = MatrixUtils::crossProduct(worldUp, w);

        w = MatrixUtils::normalize(w);
        u = MatrixUtils::normalize(u);

        glm::vec4 v = MatrixUtils::crossProduct(w, u);

        glm::vec4 origin = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);

        glm::vec4 vector = position - origin;

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

    void ProcessKeyboard(Camera_Movement direction, float deltaTime)
    {
        float velocity = movementSpeed * deltaTime;
        glm::vec4 movement(0.0f, 0.0f, 0.0f, 0.0f);
        if (direction == FORWARD)
            movement += front * velocity;
        if (direction == BACKWARD)
            movement -= front * velocity;
        if (direction == LEFT)
            movement -= right * velocity;
        if (direction == RIGHT)
            movement += right * velocity;

        position += glm::vec4(movement.x, movement.y, movement.z, 0.0f);
    }

    void ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch = true)
    {
        xoffset *= mouseSensitivity;
        yoffset *= mouseSensitivity;

        yaw += xoffset;
        pitch += yoffset;

        if (constrainPitch)
        {
            if (pitch > 89.0f)
                pitch = 89.0f;
            if (pitch < -89.0f)
                pitch = -89.0f;
        }

        updateCameraVectors();
    }

    static glm::mat4 orthographicMatrix(float left, float right, float bottom, float top, float near, float far)
    {
        return MatrixUtils::Matrix(2 / (right - left), 0.0f, 0.0f, -(right + left) / (right - left), // Line 1
                                   0.0f, 2 / (top - bottom), 0.0f, -(top + bottom) / (top - bottom), // Line 2
                                   0.0f, 0.0f, 2 / (far - near), -(far + near) / (far - near),       // Line 3
                                   0.0f, 0.0f, 0.0f, 1.0f                                            // Line 4
        );
    }

    static glm::mat4 perspectiveMatrix(float fieldOfView, float aspect, float near, float far)
    {
        float t = fabs(near) * tanf(fieldOfView / 2.0f);
        float b = -t;
        float r = t * aspect;
        float l = -r;

        glm::mat4 P = MatrixUtils::Matrix(near, 0.0f, 0.0f, 0.0f,              // Line 1
                                          0.0f, near, 0.0f, 0.0f,              // Line 2
                                          0.0f, 0.0f, near + far, -far * near, // Line 3
                                          0.0f, 0.0f, 1, 0.0f                  // Line 4
        );

        glm::mat4 M = orthographicMatrix(l, r, b, t, near, far);

        return -M * P;
    }

  private:
    void updateCameraVectors()
    {
        const float radYaw = glm::radians(yaw);
        const float radPitch = glm::radians(pitch);

        glm::vec4 direction;
        direction.x = cos(radPitch) * sin(radYaw);
        direction.y = sin(radPitch);
        direction.z = cos(radPitch) * cos(radPitch);
        direction.w = 0.0f;

        front = direction;
        right = MatrixUtils::crossProduct(front, worldUp);
    }
};