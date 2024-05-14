#pragma once

#include "gameObject.hpp"
#include "inputObserver.hpp"

class Player : public GameObject, public InputObserver
{
  public:
    Player(GameObject &gameObject, Camera &camera) : GameObject(gameObject), camera(camera)
    {
        camera.setTarget(*this, 5.0f);
    }

    void processKeyboard(Action action, float deltaTime) override
    {
        if (!inputEnabled)
        {
            return;
        }

        // moves the player only in the xz plane
        glm::vec4 cameraFrontInXZ = glm::vec4(camera.getFront().x, 0.0f, camera.getFront().z, 0.0f);
        glm::vec4 cameraRightInXZ = glm::vec4(camera.getRight().x, 0.0f, camera.getRight().z, 0.0f);

        cameraFrontInXZ = glm::normalize(cameraFrontInXZ);
        cameraRightInXZ = glm::normalize(cameraRightInXZ);

        if (action == FORWARD)
        {
            position += cameraFrontInXZ * deltaTime * movementSpeed;
        }
        if (action == BACKWARD)
        {
            position -= cameraFrontInXZ * deltaTime * movementSpeed;
        }
        if (action == LEFT)
        {
            position -= cameraRightInXZ * deltaTime * movementSpeed;
        }
        if (action == RIGHT)
        {
            position += cameraRightInXZ * deltaTime * movementSpeed;
        }
    }

  private:
    Camera &camera;

    float movementSpeed = 5.0f;
};