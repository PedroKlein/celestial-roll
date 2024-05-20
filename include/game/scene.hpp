#pragma once

#include "collision/collisionManager.hpp"
#include "game/gameObject.hpp"
#include "game/gameState.hpp"
#include "objects/camera.hpp"
#include "objects/platform.hpp"
#include "objects/player.hpp"
#include <memory>
#include <vector>

class Scene
{
  public:
    Scene() = default;

    void init()
    {
        this->freeCam = std::make_unique<Camera>(glm::vec3(0.0f, 0.0f, -3.0f));
        this->playerCam = std::make_unique<Camera>(glm::vec3(0.0f, 0.0f, 0.0f), 0.0f, -30.0f);

        this->player = std::make_unique<Player>(*playerCam.get());

        this->gameState = std::make_unique<GameState>(*freeCam.get(), *playerCam.get(), *player.get());

        addObject(std::make_shared<Platform>(glm::vec3(0.0f, -10.0f, 0.0f), glm::vec3(10.0f, 1.0f, 10.0f)));

        _collisionManager.setPlayer(player.get());
    }

    void update(float deltaTime, float viewRatio)
    {
        viewMatrix = gameState->getIsEagleView() ? freeCam->getViewMatrix() : playerCam->getViewMatrix();

        projectionMatrix = MatrixUtils::perspectiveMatrix(glm::radians(80.0f), viewRatio, -0.1f, -100.0f);

        _globalShader.setMat4("view", viewMatrix);
        _globalShader.setMat4("projection", projectionMatrix);

        clear();

        _collisionManager.checkCollisions();

        for (auto &obj : objects)
        {
            obj->update(deltaTime);
        }

        player->update(deltaTime);
    }

    void addObject(std::shared_ptr<GameObject> object)
    {
        std::shared_ptr<Collider> collider = object->getComponent<Collider>();
        if (collider)
        {
            _collisionManager.registerObject(object);
        }
        objects.push_back(object);
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
    std::unique_ptr<Player> player;
    std::unique_ptr<Camera> freeCam;
    std::unique_ptr<Camera> playerCam;
    std::unique_ptr<GameState> gameState;

    glm::mat4 viewMatrix;
    glm::mat4 projectionMatrix;

    void clear()
    {
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }
};