#pragma once

class GameObject; // forward declaration

class Component
{
  public:
    virtual ~Component()
    {
    }

    void setGameObject(GameObject *obj);

    virtual void initialize()
    {
    }

    virtual void update(float deltaTime) {};

  protected:
    GameObject *gameObject;
};