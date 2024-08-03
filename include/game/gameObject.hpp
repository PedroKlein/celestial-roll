#pragma once

#include <algorithm>
#include <map>
#include <memory>
#include <vector>
#include "component.hpp"
#include "componentType.hpp"
#include "graphics/renderer.hpp"

enum class ObjectType {
    Player,
    Platform,
    DeathBox,
    Light,
    Camera,
};

class GameObject {
public:
    virtual ~GameObject() {
        components.clear();
        physicsComponents.clear();
    }

    template<typename T>
    void addComponent(std::shared_ptr<T> component) {
        const ComponentType type = getComponentType<T>();
        components[type] = std::static_pointer_cast<Component>(component);
        component->setGameObject(this);
        component->initialize();

        if (isPhysicsComponent(type)) {
            physicsComponents.push_back(component);
        }
    }

    template<typename T>
    void removeComponent() {
        const ComponentType type = getComponentType<T>();

        auto it = components.find(type);
        if (it != components.end()) {
            if (isPhysicsComponent(type)) {
                auto physIt =
                        std::find_if(physicsComponents.begin(), physicsComponents.end(),
                                     [&it](const std::shared_ptr<Component> &comp) { return comp == it->second; });
                if (physIt != physicsComponents.end()) {
                    physicsComponents.erase(physIt);
                }
            }
            it->second->setGameObject(nullptr);
            components.erase(it);
        }
    }

    template<typename T>
    [[nodiscard]] std::shared_ptr<T> getComponent() const {
        const ComponentType type = getComponentType<T>();
        if (const auto it = components.find(type); it != components.end()) {
            return std::dynamic_pointer_cast<T>(it->second);
        }
        return nullptr;
    }

    void updatePhysics(const float deltaTime) const {
        getComponent<Transform>()->saveState();

        for (auto &comp: physicsComponents) {
            if (comp->isEnabled()) {
                comp->update(deltaTime);
            }
        }
    }

    virtual void render(const float alpha) {

        if (const auto renderer = getComponent<Renderer>()) {
            renderer->update(alpha);
        }
    }

    [[nodiscard]] virtual ObjectType getObjectType() const = 0;

private:
    std::map<ComponentType, std::shared_ptr<Component>> components;
    std::vector<std::shared_ptr<Component>> physicsComponents;

    static bool isPhysicsComponent(const ComponentType type) {
        return type == ComponentType::Gravity || type == ComponentType::RigidBody || type == ComponentType::Animation;
    }
};
