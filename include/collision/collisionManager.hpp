#pragma once

#include "collider.hpp"
#include "objects/player.hpp"
#include <iostream>
#include <memory>
#include <vector>

class CollisionManager
{
  public:
    void setPlayer(Player *player)
    {
        this->player = player;
    }

    void registerObject(std::shared_ptr<GameObject> object)
    {
        objectsWithColliders.push_back(object);
    }

    void checkCollisions()
    {
        if (!player || objectsWithColliders.empty())
            return;

        for (auto &object : objectsWithColliders)
        {
            std::shared_ptr<Collider> collider = object->getComponent<Collider>();

            auto collisionResult = player->getCollider()->checkCollision(*collider);

            if (collisionResult.collided)
            {
                player->handleCollision(*object.get(), collisionResult.normal, collisionResult.penetrationDepth);
            }
        }
    }

  private:
    Player *player;
    std::vector<std::shared_ptr<GameObject>> objectsWithColliders;
};
