#pragma once

#include "gameObject.hpp"
#include "inputObserver.hpp"

class Player : public GameObject, public InputObserver
{
  public:
    Player(GameObject &gameObject) : GameObject(gameObject)
    {
    }

    void processKeyboard(Action action, float deltaTime) override
    {
        if (!inputEnabled)
        {
            return;
        }

        if (action == FORWARD)
        {
            position += glm::vec4(0.0f, 0.0f, -1.0f, 0.0f) * deltaTime * movementSpeed;
        }
        if (action == BACKWARD)
        {
            position += glm::vec4(0.0f, 0.0f, 1.0f, 0.0f) * deltaTime * movementSpeed;
        }
        if (action == LEFT)
        {
            position += glm::vec4(-1.0f, 0.0f, 0.0f, 0.0f) * deltaTime * movementSpeed;
        }
        if (action == RIGHT)
        {
            position += glm::vec4(1.0f, 0.0f, 0.0f, 0.0f) * deltaTime * movementSpeed;
        }
    }

  private:
    float movementSpeed = 5.0f;
};