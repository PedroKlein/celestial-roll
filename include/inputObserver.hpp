#pragma once

#include "action.hpp"

class InputObserver
{
  public:
    virtual void processKeyboard(Action action, float deltaTime)
    {
    }
    virtual void processMouseMovement(double dx, double dy)
    {
    }
    virtual void processMouseButton(int button, int action)
    {
    }

    bool inputEnabled = true;
};