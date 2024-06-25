#pragma once

#include "collider.hpp"
#include "objects/player.hpp"
#include <iostream>
#include <memory>
#include <vector>

class CollisionManager
{
  public:
    CollisionManager(Player &player) : player(player)
    {
    }

    void registerObject(std::shared_ptr<GameObject> object)
    {
        objectsWithColliders.push_back(object);
    }

    void checkCollisions(float deltaTime)
    {
        const static float epsilon = 0.01f;

        if (objectsWithColliders.empty())
            return;

        bool isGrounded = false;

        for (auto &object : objectsWithColliders)
        {
            std::shared_ptr<Collider> collider = object->getComponent<Collider>();

            auto collisionResult = player.getCollider()->checkCollision(*collider);

            if (collisionResult.collided)
            {
                player.handleCollision(*object.get(), collisionResult.normal, collisionResult.penetrationDepth,
                                       deltaTime);
                isGrounded = collisionResult.normal.y > epsilon;
            }
        }

        player.setIsGrounded(isGrounded);
    }

  private:
    Player &player;
    std::vector<std::shared_ptr<GameObject>> objectsWithColliders;
};
