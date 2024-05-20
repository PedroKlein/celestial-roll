#pragma once

#include "collision/boxCollider.hpp"
#include "game/gameObject.hpp"
#include "graphics/mesh.hpp"
#include "graphics/renderer.hpp"
#include "physics/physicsMaterial.hpp"

class Platform : public GameObject
{
  public:
    Platform(const std::shared_ptr<Mesh> &mesh, const glm::vec3 &position, const glm::vec3 &scale = glm::vec3(1.0f))
    {
        transform = std::make_shared<Transform>(position, scale, glm::vec3(45.0f, 0.0f, 0.0f));
        addComponent(transform);

        // TODO: create a global mesh hashmap to avoid loading the same mesh multiple times
        addComponent(std::make_shared<Renderer>(mesh));

        boxCollider = std::make_shared<BoxCollider>(glm::vec3(-10.0f, -1.0f, -10.0f), glm::vec3(10.0f, 1.0f, 10.0f));
        addComponent(boxCollider);

        addComponent(std::make_shared<PhysicsMaterial>(1.0f, 1.0f));
    }

    ObjectType getObjectType() const override
    {
        return ObjectType::Platform;
    }

  private:
    std::shared_ptr<Transform> transform;
    std::shared_ptr<BoxCollider> boxCollider;
};