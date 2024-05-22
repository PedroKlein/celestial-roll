#pragma once

#include "collision/boxCollider.hpp"
#include "game/gameObject.hpp"
#include "graphics/mesh.hpp"
#include "graphics/meshManager.hpp"
#include "graphics/renderer.hpp"
#include "physics/physicsMaterial.hpp"

class Platform : public GameObject
{
  public:
    Platform(const glm::vec3 &position, const glm::vec3 &scale = glm::vec3(1.0f))
    {
        transform = std::make_shared<Transform>(position, scale, glm::vec3(0.0f, 0.0f, 0.0f));
        addComponent(transform);

        addComponent(std::make_shared<Renderer>(MeshManager::getInstance().getMesh("resources/models/cube.obj")));

        boxCollider = std::make_shared<BoxCollider>(-scale, scale);
        addComponent(boxCollider);

        addComponent(std::make_shared<PhysicsMaterial>(1.0f, 0.9f));
    }

    ObjectType getObjectType() const override
    {
        return ObjectType::Platform;
    }

  private:
    std::shared_ptr<Transform> transform;
    std::shared_ptr<BoxCollider> boxCollider;
};