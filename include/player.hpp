#pragma once

#include "inputObserver.hpp"
#include "physicsObject.hpp"

class Player : public PhysicsObject, public InputObserver
{
  public:
    Player(PhysicsObject &physicsObject, Camera &camera) : PhysicsObject(physicsObject), camera(camera)
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
        glm::vec3 cameraFrontInXZ = glm::vec3(camera.getFront().x, 0.0f, camera.getFront().z);
        glm::vec3 cameraRightInXZ = glm::vec3(camera.getRight().x, 0.0f, camera.getRight().z);

        cameraFrontInXZ = glm::normalize(cameraFrontInXZ);
        cameraRightInXZ = glm::normalize(cameraRightInXZ);

        if (action == FORWARD)
        {
            transform.position += cameraFrontInXZ * deltaTime * movementSpeed;
        }
        if (action == BACKWARD)
        {
            transform.position -= cameraFrontInXZ * deltaTime * movementSpeed;
        }
        if (action == LEFT)
        {
            transform.position -= cameraRightInXZ * deltaTime * movementSpeed;
        }
        if (action == RIGHT)
        {
            transform.position += cameraRightInXZ * deltaTime * movementSpeed;
        }
    }

    void update(float deltaTime)
    {
        PhysicsObject::update(deltaTime);

        camera.updateCameraVectors();
    }

  private:
    Camera &camera;

    float movementSpeed = 5.0f;
};