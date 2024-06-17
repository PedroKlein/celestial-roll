#pragma once

#include "collision/aabbCollider.hpp"
#include "game/gameObject.hpp"
#include "graphics/mesh.hpp"
#include "graphics/meshManager.hpp"
#include "graphics/renderer.hpp"
#include "physics/physicsMaterial.hpp"

class Platform : public GameObject
{
  public:
    Platform(const glm::vec3 &position, const glm::vec3 &scale = glm::vec3(1.0f),
             const glm::vec3 &rotation = glm::vec3(0.0f))
    {
        transform = std::make_shared<Transform>(position, scale, rotation);
        addComponent(transform);

        addComponent(std::make_shared<Renderer>(MeshManager::getInstance().getMesh("resources/models/cube.obj")));

        aabbCollider = std::make_shared<AABBCollider>(-scale, scale);
        addComponent(aabbCollider);

        addComponent(std::make_shared<PhysicsMaterial>(0.1f, 0.0f));
    }

    ObjectType getObjectType() const override
    {
        return ObjectType::Platform;
    }

  private:
    std::shared_ptr<Transform> transform;
    std::shared_ptr<AABBCollider> aabbCollider;
};