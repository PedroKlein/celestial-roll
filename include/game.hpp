#pragma once

#include "camera.hpp"
#include "gameObject.hpp"
#include "inputHandler.hpp"
#include "renderer.hpp"
#include "shader.hpp"

class Game
{
  public:
    Game(float initialWidth, float initialHeight)
        : deltaTime(0.0f), lastFrame(0.0f), shader("shaders/shader_vertex.glsl", "shaders/shader_fragment.glsl"),
          renderer(), camera(glm::vec4(0.0f, 0.0f, -3.0f, 1.0f)), inputHandler()
    {
        viewRatio = initialWidth / initialHeight;

        Mesh cowMesh("models/cow.obj");

        GameObject cow(cowMesh, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f),
                       glm::vec3(0.01f, 0.01f, 0.01f));
        scene.push_back(cow);

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
    std::vector<GameObject> scene;
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