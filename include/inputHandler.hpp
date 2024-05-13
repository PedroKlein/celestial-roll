#pragma once

#include "inputObserver.hpp"
#include <GLFW/glfw3.h>
#include <functional>
#include <iostream>
#include <vector>

using GetCursorPosCallback = std::function<void(double *, double *)>;

class InputHandler
{
  public:
    InputHandler()
        : wKeyPressed(false), aKeyPressed(false), sKeyPressed(false), dKeyPressed(false), leftMouseButtonPressed(false)
    {
    }

    void addObserver(InputObserver *observer)
    {
        observers.push_back(observer);
    }

    void processInput(float deltaTime)
    {
        if (wKeyPressed)
            notifyObserversOfKeyboard(FORWARD, GLFW_PRESS, deltaTime);
        if (aKeyPressed)
            notifyObserversOfKeyboard(LEFT, GLFW_PRESS, deltaTime);
        if (sKeyPressed)
            notifyObserversOfKeyboard(BACKWARD, GLFW_PRESS, deltaTime);
        if (dKeyPressed)
            notifyObserversOfKeyboard(RIGHT, GLFW_PRESS, deltaTime);
    }

    void keyCallback(int key, int action)
    {
        if (key == GLFW_KEY_W)
            wKeyPressed = (action != GLFW_RELEASE);
        if (key == GLFW_KEY_A)
            aKeyPressed = (action != GLFW_RELEASE);
        if (key == GLFW_KEY_S)
            sKeyPressed = (action != GLFW_RELEASE);
        if (key == GLFW_KEY_D)
            dKeyPressed = (action != GLFW_RELEASE);
    }

    void cursorPosCallback(double xpos, double ypos)
    {
        if (!leftMouseButtonPressed)
            return;

        float dx = xpos - lastCursorPosX;
        float dy = ypos - lastCursorPosY;

        notifyObserversOfMouseMovement(dx, dy);

        lastCursorPosX = xpos;
        lastCursorPosY = ypos;
    }

    void mouseButtonCallback(int button, int action, GetCursorPosCallback getCursorPosCallback)
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

    void notifyObserversOfKeyboard(int key, int action, float deltaTime = 0.0f)
    {
        for (InputObserver *observer : observers)
        {
            if (observer->inputEnabled)
            {
                observer->processKeyboard(key, action, deltaTime);
            }
        }
    }

    void notifyObserversOfMouseMovement(double dx, double dy)
    {
        for (InputObserver *observer : observers)
        {
            if (observer->inputEnabled)
                observer->processMouseMovement(dx, dy);
        }
    }

    void notifyObserversOfMouseButton(int button, int action)
    {
        for (InputObserver *observer : observers)
        {
            if (observer->inputEnabled)
                observer->processMouseButton(button, action);
        }
    }
};