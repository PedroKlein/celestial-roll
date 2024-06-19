#pragma once

#include "collision/collisionManager.hpp"
#include "game/gameObject.hpp"
#include "game/gameState.hpp"
#include "graphics/shaderManager.hpp"
#include "objects/camera.hpp"
#include "objects/platform.hpp"
#include "objects/player.hpp"
#include <algorithm>
#include <memory>
#include <vector>

class Scene
{
  public:
    Scene() : collisionManager()
    {
    }

    void init()
    {
        // ensures that at least the default shader is loaded, refactor this
        ShaderManager::getInstance().loadShader(DEFAULT_VERTEX_SHADER_PATH, DEFAULT_FRAGMENT_SHADER_PATH, "default");
        Platform::initializeShaders();

        this->freeCam = std::make_unique<Camera>(glm::vec3(0.0f, -10.0f, -20.0f), 0.0f, -30.0f);
        this->playerCam = std::make_unique<Camera>(glm::vec3(0.0f, 0.0f, 0.0f), 0.0f, -30.0f);

        this->player = std::make_unique<Player>(*playerCam.get());

        this->gameState = std::make_unique<GameState>(*freeCam.get(), *playerCam.get(), *player.get());

        addObject(std::make_shared<Platform>(Transform{glm::vec3(0.0f, -40.0f, 0.0f), glm::vec3(10.0f, 1.0f, 10.0f)}));

        addObject(std::make_shared<IcePlatform>(
            Transform{glm::vec3(10.0f, -60.0f, 0.0f), glm::vec3(10.0f, 1.0f, 10.0f), glm::vec3(30.0f, 0.0f, 0.0f)}));

        addObject(std::make_shared<IcePlatform>(
            Transform{glm::vec3(10.0f, -60.0f, -12.0f), glm::vec3(10.0f, 1.0f, 10.0f), glm::vec3(-20.0f, 0.0f, 0.0f)}));

        collisionManager.setPlayer(player.get());
    }

    void updatePhysics(float deltaTime)
    {
        if (gameState->getIsPaused())
        {
            return;
        }

        player->updatePhysics(deltaTime);

        for (auto &obj : objects)
        {
            obj->updatePhysics(deltaTime);
        }

        collisionManager.checkCollisions();
    }

    // TODO: create a render manager
    void render(float alpha, float viewRatio)
    {
        viewMatrix = gameState->getIsEagleView() ? freeCam->getViewMatrix() : playerCam->getViewMatrix();
        projectionMatrix = MatrixUtils::perspectiveMatrix(glm::radians(80.0f), viewRatio, -0.1f, -1000.0f);

        clear();

        player->render(alpha, viewMatrix, projectionMatrix);

        for (auto &obj : objects)
        {
            obj->render(alpha, viewMatrix, projectionMatrix);
        }
    }

    // this is dumb, could be a different list for rendering with ID sorted
    void addObject(std::shared_ptr<GameObject> object)
    {
        std::shared_ptr<Collider> collider = object->getComponent<Collider>();
        if (collider)
        {
            collisionManager.registerObject(object);
        }

        objects.push_back(object);

        std::shared_ptr<Renderer> renderer = object->getComponent<Renderer>();
        if (renderer)
        {
            // sort the objects vector based on the shader ID
            std::sort(objects.begin(), objects.end(),
                      [](const std::shared_ptr<GameObject> &a, const std::shared_ptr<GameObject> &b) {
                          auto rendererA = a->getComponent<Renderer>();
                          auto rendererB = b->getComponent<Renderer>();

                          if (rendererA && rendererB)
                          {
                              return rendererA->material->shader->ID < rendererB->material->shader->ID;
                          }

                          else if (rendererA)
                          {
                              return true;
                          }
                          else if (rendererB)
                          {
                              return false;
                          }

                          return false;
                      });
        }
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

    CollisionManager collisionManager;

    glm::mat4 viewMatrix;
    glm::mat4 projectionMatrix;

    void clear()
    {
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }
};