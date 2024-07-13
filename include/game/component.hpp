#pragma once

class GameObject; // forward declaration

class Component {
public:
    virtual ~Component() = default;

    void setGameObject(GameObject *obj);

    void enable();
    void disable();
    [[nodiscard]] bool isEnabled() const;

    [[nodiscard]] const GameObject *getGameObject() const;

    virtual void initialize() {}

    virtual void update(float deltaTime){};

protected:
    GameObject *gameObject{};
    bool enabled = true;
};
