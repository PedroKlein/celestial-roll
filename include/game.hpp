#pragma once

#include "camera.hpp"
#include "gameObject.hpp"
#include "gameState.hpp"
#include "inputHandler.hpp"
#include "player.hpp"
#include "renderer.hpp"
#include "shader.hpp"
#include <memory>

class Game
{
  public:
    Game(float initialWidth, float initialHeight)
        : deltaTime(0.0f), lastFrame(0.0f), shader("shaders/shader_vertex.glsl", "shaders/shader_fragment.glsl"),
          renderer(), inputHandler()
    {
        viewRatio = initialWidth / initialHeight;

        this->freeCam = std::make_unique<Camera>(glm::vec4(0.0f, 0.0f, -3.0f, 1.0f));
        this->playerCam = std::make_unique<Camera>();

        Mesh cowMesh("models/cow.obj");

        GameObject cow(cowMesh, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.5f, 0.5f, 0.5f));

        auto littleCow = std::make_shared<GameObject>(cowMesh, glm::vec3(5.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f),
                                                      glm::vec3(0.5f, 0.1f, 0.1f));

        this->player = std::make_unique<Player>(cow, *playerCam.get());

        this->gameState = std::make_unique<GameState>(*freeCam.get(), *playerCam.get(), *player.get());

        scene.push_back(std::shared_ptr<Player>(this->player.get(), [](Player *) {})); // use a no-op deleter
        scene.push_back(littleCow);

        inputHandler.addObserver(player.get());
        inputHandler.addObserver(freeCam.get());
        inputHandler.addObserver(playerCam.get());
        inputHandler.addObserver(gameState.get());
    }

    void tick()
    {
        updateDeltaTime();
        inputHandler.processInput(deltaTime);

        renderer.clear();

        glm::mat4 view = gameState->getIsEagleView() ? freeCam->getViewMatrix() : playerCam->getViewMatrix();
        glm::mat4 projection = MatrixUtils::perspectiveMatrix(glm::radians(80.0f), viewRatio, -0.1f, -100.0f);

        renderer.renderScene(scene, shader, view, projection);
    }

    InputHandler *getInputHandler()
    {
        return &inputHandler;
    }

    void setViewRatio(float width, float height)
    {
        viewRatio = width / height;
    }

  private:
    std::unique_ptr<Camera> freeCam;
    std::unique_ptr<Camera> playerCam;
    std::unique_ptr<Player> player;
    Shader shader;
    Renderer renderer;
    std::vector<std::shared_ptr<GameObject>> scene;
    InputHandler inputHandler;
    std::unique_ptr<GameState> gameState;

    float viewRatio;

    float deltaTime;
    float lastFrame;

    void updateDeltaTime()
    {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
    }
};