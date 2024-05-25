#pragma once

#include "component.hpp"
#include "componentType.hpp"
#include "graphics/renderer.hpp"
#include "transform.hpp"
#include <map>
#include <memory>
#include <vector>

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
        physicsComponents.clear();
    }

    template <typename T> void addComponent(std::shared_ptr<T> component)
    {
        ComponentType type = getComponentType<T>();
        components[type] = std::static_pointer_cast<Component>(component);
        component->setGameObject(this);
        component->initialize();

        if (isPhysicsComponent(type))
        {
            physicsComponents.push_back(component);
        }
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

    void updatePhysics(float deltaTime)
    {
        for (auto &comp : physicsComponents)
        {
            if (comp->isEnabled())
            {
                comp->update(deltaTime);
            }
        }
    }

    void render(float alpha)
    {
        auto renderer = getComponent<Renderer>();

        if (renderer)
        {
            renderer->update(alpha);
        }
    }

    virtual ObjectType getObjectType() const = 0;

  private:
    std::map<ComponentType, std::shared_ptr<Component>> components;
    std::vector<std::shared_ptr<Component>> physicsComponents;

    bool isPhysicsComponent(ComponentType type)
    {
        return type == ComponentType::Gravity || type == ComponentType::RigidBody;
    }
};
