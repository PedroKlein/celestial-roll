#pragma once

#include "camera.hpp"
#include "gameObject.hpp"
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
          renderer(), camera(glm::vec4(0.0f, 0.0f, -3.0f, 1.0f)), inputHandler()
    {
        viewRatio = initialWidth / initialHeight;

        Mesh cowMesh("models/cow.obj");

        GameObject cow(cowMesh, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.5f, 0.5f, 0.5f));

        auto littleCow = std::make_shared<GameObject>(cowMesh, glm::vec3(5.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f),
                                                      glm::vec3(0.1f, 0.1f, 0.1f));

        this->player = std::make_shared<Player>(cow);

        scene.push_back(this->player);
        scene.push_back(littleCow);

        camera.setTarget(*scene.front().get(), 5.0f);

        inputHandler.addObserver(&camera);
        inputHandler.addObserver(player.get());
    }

    void tick()
    {
        updateDeltaTime();
        inputHandler.processInput(deltaTime);

        renderer.clear();

        glm::mat4 view = camera.getViewMatrix();
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
    Camera camera;
    std::shared_ptr<Player> player;
    Shader shader;
    Renderer renderer;
    std::vector<std::shared_ptr<GameObject>> scene;
    InputHandler inputHandler;
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