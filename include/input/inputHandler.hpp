#pragma once

#include "inputObserver.hpp"
#include <GLFW/glfw3.h>
#include <functional>
#include <vector>

using GetCursorPosCallback = std::function<void(double *, double *)>;

class InputHandler
{
  public:
    InputHandler()
        : wKeyPressed(false), aKeyPressed(false), sKeyPressed(false), dKeyPressed(false), leftMouseButtonPressed(false),
          lastCursorPosX(0),
          lastCursorPosY(0) {
    }

    void addObserver(InputObserver *observer)
    {
        observers.push_back(observer);
    }

    void processInput(const float deltaTime) const {
        if (wKeyPressed)
            notifyObserversOfKeyboard(FORWARD, deltaTime);
        if (aKeyPressed)
            notifyObserversOfKeyboard(LEFT, deltaTime);
        if (sKeyPressed)
            notifyObserversOfKeyboard(BACKWARD, deltaTime);
        if (dKeyPressed)
            notifyObserversOfKeyboard(RIGHT, deltaTime);
    }

    void keyCallback(const int key, const int action)
    {
        if (key == GLFW_KEY_W)
            wKeyPressed = (action != GLFW_RELEASE);
        if (key == GLFW_KEY_A)
            aKeyPressed = (action != GLFW_RELEASE);
        if (key == GLFW_KEY_S)
            sKeyPressed = (action != GLFW_RELEASE);
        if (key == GLFW_KEY_D)
            dKeyPressed = (action != GLFW_RELEASE);

        if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
            notifyObserversOfKeyboard(JUMP);

        if (key == GLFW_KEY_P && action == GLFW_PRESS)
            notifyObserversOfKeyboard(PAUSE);
        if (key == GLFW_KEY_E && action == GLFW_PRESS)
            notifyObserversOfKeyboard(EAGLE_VIEW);
    }

    void cursorPosCallback(const double xpos, const double ypos)
    {
        if (!leftMouseButtonPressed)
            return;

        const double dx = xpos - lastCursorPosX;
        const double dy = ypos - lastCursorPosY;

        notifyObserversOfMouseMovement(dx, dy);

        lastCursorPosX = xpos;
        lastCursorPosY = ypos;
    }

    void mouseButtonCallback(const int button, const int action, const GetCursorPosCallback &getCursorPosCallback)
    {
        if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
        {
            getCursorPosCallback(&lastCursorPosX, &lastCursorPosY);
            leftMouseButtonPressed = true;
        }

        if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
        {
            leftMouseButtonPressed = false;
        }
    }

  private:
    std::vector<InputObserver *> observers;

    bool wKeyPressed;
    bool aKeyPressed;
    bool sKeyPressed;
    bool dKeyPressed;
    bool leftMouseButtonPressed;
    double lastCursorPosX;
    double lastCursorPosY;

    void notifyObserversOfKeyboard(const Action action, const float deltaTime = 0.0f) const {
        for (InputObserver *observer : observers)
        {
            if (observer->getInputEnabled())
            {
                observer->processKeyboard(action, deltaTime);
            }
        }
    }

    void notifyObserversOfMouseMovement(const double dx, const double dy) const {
        for (InputObserver *observer : observers)
        {
            if (observer->getInputEnabled())
                observer->processMouseMovement(dx, dy);
        }
    }

    void notifyObserversOfMouseButton(const int button, const int action) const {
        for (InputObserver *observer : observers)
        {
            if (observer->getInputEnabled())
                observer->processMouseButton(button, action);
        }
    }
};