#pragma once

#include "component.hpp"
#include <memory>
#include <vector>

class GameObject
{
  public:
    ~GameObject()
    {
        components.clear();
    }

    void addComponent(std::shared_ptr<Component> component)
    {
        components.push_back(component);
        component->setGameObject(this);
        component->initialize();
    }

    template <typename T> std::shared_ptr<T> getComponent() const
    {
        for (const auto &comp : components)
        {
            std::shared_ptr<T> castComp = std::dynamic_pointer_cast<T>(comp);
            if (castComp)
            {
                return castComp;
            }
        }
        return nullptr;
    }

    void update(float deltaTime)
    {
        for (auto &comp : components)
        {
            comp->update(deltaTime);
        }
    }

  private:
    std::vector<std::shared_ptr<Component>> components;
};