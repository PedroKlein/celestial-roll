#pragma once

#include "collision/boxCollider.hpp"
#include "game/gameObject.hpp"
#include "graphics/mesh.hpp"
#include "graphics/renderer.hpp"

class Platform : public GameObject
{
  public:
    Platform(const std::shared_ptr<Mesh> &mesh, const glm::vec3 &position)
    {
        transform = std::make_shared<Transform>(position);
        addComponent(transform);

        // TODO: create a global mesh hashmap to avoid loading the same mesh multiple times
        addComponent(std::make_shared<Renderer>(mesh));

        boxCollider = std::make_shared<BoxCollider>(glm::vec3(-5.0f, -0.5f, -5.0f), glm::vec3(5.0f, 0.5f, 5.0f));
        addComponent(boxCollider);
    }

    ObjectType getObjectType() const override
    {
        return ObjectType::Platform;
    }

  private:
    std::shared_ptr<Transform> transform;
    std::shared_ptr<BoxCollider> boxCollider;
};