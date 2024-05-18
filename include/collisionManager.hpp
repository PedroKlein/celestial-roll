#pragma once

#include "collider.hpp"
#include "player.hpp"
#include <iostream>
#include <memory>
#include <vector>

class CollisionManager
{
  public:
    void setPlayer(std::shared_ptr<Player> player)
    {
        player = player;
    }

    void registerCollider(std::shared_ptr<Collider> collider)
    {
        colliders.push_back(collider);
    }

    void checkCollisions()
    {
        if (!player || colliders.empty())
            return;

        for (const auto &collider : colliders)
        {
            if (player->getCollider()->checkCollision(*collider))
            {
                handleCollision(player->getCollider(), collider);
            }
        }
    }

    void handleCollision(std::shared_ptr<Collider> playerCollider, std::shared_ptr<Collider> otherCollider)
    {
        std::cout << "Collision Detected!" << std::endl;
    }

  private:
    std::shared_ptr<Player> player;
    std::vector<std::shared_ptr<Collider>> colliders;
};

CollisionManager collisionManager();
