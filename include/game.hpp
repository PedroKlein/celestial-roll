#pragma once

#include "camera.hpp"
#include "inputHandler.hpp"
#include "renderer.hpp"
#include "shader.hpp"

// TODO: separate input handling from game logic
class Game
{
  public:
    Game(float initialWidth, float initialHeight)
        : deltaTime(0.0f), lastFrame(0.0f), shader("shaders/shader_vertex.glsl", "shaders/shader_fragment.glsl"),
          renderer(), camera(glm::vec4(0.0f, 0.0f, -3.0f, 1.0f)), inputHandler()
    {
        viewRatio = initialWidth / initialHeight;

        Mesh mesh("models/cow.obj");
        scene.push_back(mesh);

        inputHandler.addObserver(&camera);
    }

    void tick()
    {
        updateDeltaTime();
        inputHandler.processInput(deltaTime);

        renderer.clear();

        glm::mat4 view = camera.getViewMatrix();
        glm::mat4 projection = Camera::perspectiveMatrix(glm::radians(45.0f), viewRatio, -0.1f, -10.0f);

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
    Shader shader;
    Renderer renderer;
    std::vector<Mesh> scene;
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