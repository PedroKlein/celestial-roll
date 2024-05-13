#pragma once

class InputObserver
{
  public:
    virtual void processKeyboard(int key, int action, float deltaTime)
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