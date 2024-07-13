#pragma once

#include <iostream>
#include "collisionDetector.hpp"
#include "game/component.hpp"
#include "game/gameObject.hpp"
#include "transform.hpp"

enum class ColliderType {
    OBB,
    AABB,
    Sphere,
};

class Collider : public Component {
public:
    explicit Collider(const ColliderType type) : type(type), transform(nullptr) {}

    void initialize() override {
        transform = gameObject->getComponent<Transform>();
        if (!transform) {
            std::cerr << "Collider requires a Transform component.\n";
        }
    }

    [[nodiscard]] std::shared_ptr<Transform> getTransform() const { return transform; }

    [[nodiscard]] ColliderType getType() const { return type; }

    [[nodiscard]] glm::mat4 getRotationMatrix() const { return transform->getRotationMatrix(); }

    [[nodiscard]] glm::vec4 getPosition() const { return transform->getPosition(); }

    [[nodiscard]] virtual CollisionResult checkCollision(const Collider &other) const = 0;

protected:
    ColliderType type;
    std::shared_ptr<Transform> transform;
};
