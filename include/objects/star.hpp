#pragma once

#include "bezierAnimation.hpp"
#include "collision/sphereCollider.hpp"
#include "game/gameObject.hpp"
#include "graphics/materialManager.hpp"
#include "graphics/meshManager.hpp"
#include "graphics/renderer.hpp"
#include "physics/physicsMaterial.hpp"

class Star : public GameObject {
public:
    virtual ~Star() = default;

    explicit Star(const Transform &transform, const glm::vec4 &color = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f),
                  float mass = 2.0e13f) {
        ShaderManager::getInstance().loadShader(DEFAULT_VERTEX_SHADER_PATH, "resources/shaders/star.frag", "star");

        this->transform = std::make_shared<Transform>(transform);
        addComponent(this->transform);

        addComponent(std::make_shared<RigidBody>(mass));

        addComponent(std::make_shared<Renderer>(
                MeshManager::getInstance().getMesh("resources/models/sphere.obj"),
                MaterialManager::getInstance().getMaterial("resources/materials/default.mtl", "star", true)));

        addComponent(std::make_shared<LightEmitter>(color, 1.0f, 1.0e-2f, 5.0e-3f));

        addComponent(std::make_shared<SphereCollider>(transform.scale.x / 2.0f));
    }

    [[nodiscard]] ObjectType getObjectType() const override { return ObjectType::Star; }

private:
    std::shared_ptr<Transform> transform;
};