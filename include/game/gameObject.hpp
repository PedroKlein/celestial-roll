#pragma once

#include "component.hpp"
#include "componentType.hpp"
#include <map>
#include <memory>

enum class ObjectType
{
    Player,
    Platform,
    Camera,
};

class GameObject
{
  public:
    ~GameObject()
    {
        components.clear();
    }

    template <typename T> void addComponent(std::shared_ptr<T> component)
    {
        ComponentType type = getComponentType<T>();
        components[type] = std::static_pointer_cast<Component>(component);
        component->setGameObject(this);
        component->initialize();
    }

    template <typename T> std::shared_ptr<T> getComponent() const
    {
        ComponentType type = getComponentType<T>();
        auto it = components.find(type);
        if (it != components.end())
        {
            return std::dynamic_pointer_cast<T>(it->second);
        }
        return nullptr;
    }

    void update(float deltaTime)
    {
        for (auto &pair : components)
        {
            pair.second->update(deltaTime);
        }
    }

    virtual ObjectType getObjectType() const = 0;

  private:
    std::map<ComponentType, std::shared_ptr<Component>> components;
};