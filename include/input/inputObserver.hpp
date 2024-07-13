#pragma once

#include "action.hpp"

class InputObserver {
public:
    virtual ~InputObserver() = default;

    virtual void processKeyboard(Action action, float deltaTime) {}
    virtual void processMouseMovement(const double dx, const double dy) {}
    virtual void processMouseButton(const int button, const int action) {}

    void setInputEnabled(const bool enabled) { inputEnabled = enabled; }

    [[nodiscard]] bool getInputEnabled() const { return inputEnabled; }

protected:
    bool inputEnabled = true;
};
