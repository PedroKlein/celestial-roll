#pragma once

#include "bezierAnimation.hpp"
#include "collision/sphereCollider.hpp"
#include "game/gameObject.hpp"
#include "graphics/materialManager.hpp"
#include "graphics/meshManager.hpp"
#include "graphics/renderer.hpp"
#include "physics/physicsMaterial.hpp"

class Duck : public GameObject {
public:
    virtual ~Duck() = default;

    explicit Duck(const Transform &transform) {
        this->transform = std::make_shared<Transform>(transform);
        addComponent(this->transform);

        addComponent(std::make_shared<Renderer>(MeshManager::getInstance().getMesh("resources/models/duck.obj"),
                                                MaterialManager::getInstance().getMaterial("duck")));
    }

    [[nodiscard]] ObjectType getObjectType() const override { return ObjectType::Other; }

private:
    std::shared_ptr<Transform> transform;
};
