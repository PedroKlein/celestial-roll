#pragma once

#include "bezierAnimation.hpp"
#include "collision/aabbCollider.hpp"
#include "collision/obbCollider.hpp"
#include "game/gameObject.hpp"
#include "graphics/materialManager.hpp"
#include "graphics/meshManager.hpp"
#include "graphics/renderer.hpp"
#include "physics/physicsMaterial.hpp"

class DeathBox : public GameObject {
public:
    virtual ~DeathBox() = default;

    explicit DeathBox(const Transform &transform) {
        this->transform = std::make_shared<Transform>(transform);
        addComponent(this->transform);

        if (this->transform->rotation == glm::quat()) {
            collider = std::make_shared<AABBCollider>(-this->transform->scale, this->transform->scale);
            addComponent(collider);
        } else {
            collider = std::make_shared<OBBCollider>(this->transform->scale);
            addComponent(collider);
        }
    }

    [[nodiscard]] ObjectType getObjectType() const override { return ObjectType::DeathBox; }

private:
    std::shared_ptr<Transform> transform;
    std::shared_ptr<Collider> collider;
};
