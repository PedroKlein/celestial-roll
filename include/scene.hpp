#pragma once

#include "camera.hpp"
#include "gameObject.hpp"
#include "gameState.hpp"
#include "physicsObject.hpp"
#include "player.hpp"
#include <memory>
#include <vector>

class Scene
{
  public:
    Scene() = default;

    void init()
    {
        this->freeCam = std::make_unique<Camera>(glm::vec4(0.0f, 0.0f, -3.0f, 1.0f));
        this->playerCam = std::make_unique<Camera>();

        Mesh cowMesh("models/cow.obj");
        Mesh platformMesh("models/platform.obj");

        PhysicsObject cow(cowMesh, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f),
                          glm::vec3(0.5f, 0.5f, 0.5f), 10.0f);

        auto littleCow = std::make_shared<GameObject>(cowMesh, glm::vec3(5.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f),
                                                      glm::vec3(0.5f, 0.1f, 0.1f));

        auto platform = std::make_shared<GameObject>(platformMesh, glm::vec3(0.0f, -20.0f, 0.0f),
                                                     glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f));

        platform->createBoxCollider(glm::vec3(-10.0f, -0.1f, -10.0f), glm::vec3(10.0f, 0.2f, 10.0f));

        this->player = std::make_unique<Player>(cow, *playerCam.get());
        this->player->createBoxCollider(glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(0.5f, 0.5f, 0.5f));

        this->gameState = std::make_unique<GameState>(*freeCam.get(), *playerCam.get(), *player.get());

        addObject(littleCow);
        addObject(platform);
    }

    void update(float deltaTime, float viewRatio)
    {
        viewMatrix = gameState->getIsEagleView() ? freeCam->getViewMatrix() : playerCam->getViewMatrix();
        projectionMatrix = MatrixUtils::perspectiveMatrix(glm::radians(80.0f), viewRatio, -0.1f, -100.0f);

        // for (auto &obj : physicsObjects)
        // {
        //     obj->update(deltaTime);
        // }

        player->update(deltaTime, objects);
    }

    void addObject(std::shared_ptr<GameObject> object)
    {
        objects.push_back(object);
    }

    void addPhysicsObject(std::shared_ptr<PhysicsObject> object)
    {
        physicsObjects.push_back(object);
    }

    glm::mat4 getViewMatrix() const
    {
        return viewMatrix;
    }

    glm::mat4 getProjectionMatrix() const
    {
        return projectionMatrix;
    }

    const std::vector<std::shared_ptr<GameObject>> &getObjects() const
    {
        return objects;
    }

    const std::vector<std::shared_ptr<PhysicsObject>> &getPhysicsObjects() const
    {
        return physicsObjects;
    }

    Player *getPlayer() const
    {
        return player.get();
    }

    Camera *getFreeCam()
    {
        return freeCam.get();
    }

    Camera *getPlayerCam()
    {
        return playerCam.get();
    }

    GameState *getGameState()
    {
        return gameState.get();
    }

  private:
    std::vector<std::shared_ptr<GameObject>> objects;
    std::vector<std::shared_ptr<PhysicsObject>> physicsObjects;
    std::unique_ptr<Player> player;
    std::unique_ptr<Camera> freeCam;
    std::unique_ptr<Camera> playerCam;
    std::unique_ptr<GameState> gameState;

    glm::mat4 viewMatrix;
    glm::mat4 projectionMatrix;
};