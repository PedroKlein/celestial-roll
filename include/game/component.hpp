#pragma once

class GameObject; // forward declaration

class Component
{
  public:
    virtual ~Component()
    {
    }

    void setGameObject(GameObject *obj);

    void enable();
    void disable();
    bool isEnabled() const;

    virtual void initialize()
    {
    }

    virtual void update(float deltaTime) {};

  protected:
    GameObject *gameObject;
    bool enabled = true;
};