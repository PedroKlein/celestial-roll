#pragma once

#include "collision/aabbCollider.hpp"
#include "game/gameObject.hpp"
#include "graphics/mesh.hpp"
#include "graphics/meshManager.hpp"
#include "graphics/renderer.hpp"
#include "physics/physicsMaterial.hpp"
#include <glm/ext/matrix_transform.hpp>

class Platform : public GameObject
{
  public:
    Platform(const glm::vec3 &position, const glm::vec3 &scale = glm::vec3(1.0f),
             const glm::vec3 &rotation = glm::vec3(0.0f))
    {
        transform = std::make_shared<Transform>(position, scale, rotation);
        addComponent(transform);

        addComponent(std::make_shared<Renderer>(MeshManager::getInstance().getMesh("resources/models/cube.obj")));

        if (rotation == glm::vec3(0.0f))
        {
            collider = std::make_shared<AABBCollider>(-scale, scale);
            addComponent(collider);
        }
        else
        {
            collider = std::make_shared<OBBCollider>(scale);
            addComponent(collider);
        }

        addComponent(std::make_shared<PhysicsMaterial>(0.1f, 0.0f));
    }

    ObjectType getObjectType() const override
    {
        return ObjectType::Platform;
    }

  private:
    std::shared_ptr<Transform> transform;
    std::shared_ptr<Collider> collider;
};