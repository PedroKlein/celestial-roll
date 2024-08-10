#pragma once

#include "bezierAnimation.hpp"
#include "collision/aabbCollider.hpp"
#include "game/gameObject.hpp"
#include "graphics/materialManager.hpp"
#include "graphics/meshManager.hpp"
#include "graphics/renderer.hpp"
#include "physics/physicsMaterial.hpp"

enum PlatformType { Normal, Ice, Jump };

constexpr glm::vec3 defaultSize = {10.0f, 1.0f, 5.0f};

class Platform : public GameObject {
public:
    virtual ~Platform() = default;

    explicit Platform(const Transform &transform, const std::string &materialName = "default", float boucinness = 0.0f,
                      float friction = 0.1f, const bool isOpaque = true) : boucinness(boucinness), friction(friction) {
        this->transform = std::make_shared<Transform>(transform);
        addComponent(this->transform);

        addComponent(std::make_shared<Renderer>(MeshManager::getInstance().getMesh("resources/models/platform.obj"),
                                                MaterialManager::getInstance().getMaterial(materialName)));

        if (this->transform->rotation == glm::quat()) {
            collider = std::make_shared<AABBCollider>(-defaultSize * this->transform->scale,
                                                      defaultSize * this->transform->scale);
            addComponent(collider);
        } else {
            collider = std::make_shared<OBBCollider>(defaultSize * this->transform->scale);
            addComponent(collider);
        }

        addComponent(std::make_shared<PhysicsMaterial>(friction, boucinness));
    }

    [[nodiscard]] ObjectType getObjectType() const override { return ObjectType::Platform; }

    virtual PlatformType getPlatformType() const { return PlatformType::Normal; }

private:
    std::shared_ptr<Transform> transform;
    std::shared_ptr<Collider> collider;

protected:
    float boucinness;
    float friction;
};

class IcePlatform final : public Platform {
public:
    explicit IcePlatform(const Transform &transform) : Platform(transform, "ice", 0.0f, 0.001f, false) {}

    PlatformType getPlatformType() const override { return PlatformType::Ice; }
};

class JumpPlatform final : public Platform {
public:
    explicit JumpPlatform(const Transform &transform) : Platform(transform, "tiles", 0.8f, 0.1f) {}

    PlatformType getPlatformType() const override { return PlatformType::Jump; }
};
