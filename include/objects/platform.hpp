#pragma once

#include "bezierAnimation.hpp"
#include "collision/aabbCollider.hpp"
#include "game/gameObject.hpp"
#include "graphics/materialManager.hpp"
#include "graphics/meshManager.hpp"
#include "graphics/renderer.hpp"
#include "physics/physicsMaterial.hpp"

class Platform : public GameObject {
public:
    virtual ~Platform() = default;

    explicit Platform(const Transform &transform,
                      const std::string &materialFilePath = "resources/materials/default.mtl",
                      const std::string &shaderName = "default", float boucinness = 0.0f, float friction = 0.1f,
                      const bool isOpaque = true) : boucinness(boucinness), friction(friction) {
        this->transform = std::make_shared<Transform>(transform);
        addComponent(this->transform);

        addComponent(std::make_shared<Renderer>(
                MeshManager::getInstance().getMesh("resources/models/cube.obj"),
                MaterialManager::getInstance().getMaterial(materialFilePath, shaderName, isOpaque)));

        if (this->transform->rotation == glm::quat()) {
            collider = std::make_shared<AABBCollider>(-this->transform->scale, this->transform->scale);
            addComponent(collider);
        } else {
            collider = std::make_shared<OBBCollider>(this->transform->scale);
            addComponent(collider);
        }

        addComponent(std::make_shared<PhysicsMaterial>(friction, boucinness));
    }

    [[nodiscard]] ObjectType getObjectType() const override { return ObjectType::Platform; }

    // TODO: this is dumb, refactor
    static void initializeShaders() {
        ShaderManager::getInstance().loadShader(DEFAULT_VERTEX_SHADER_PATH, "resources/shaders/ice.frag", "ice");
    }

private:
    std::shared_ptr<Transform> transform;
    std::shared_ptr<Collider> collider;

protected:
    float boucinness;
    float friction;
};

class IcePlatform final : public Platform {
public:
    explicit IcePlatform(const Transform &transform) :
        Platform(transform, "resources/materials/ice.mtl", "ice", 0.0f, 0.0f, false) {}
};

class JumpPlatform final : public Platform {
public:
    explicit JumpPlatform(const Transform &transform) :
        Platform(transform, "resources/materials/sphere.mtl", "default", 0.8f, 0.1f) {}
};
