#pragma once

#include "camera.hpp"
#include "renderer.hpp"
#include "shader.hpp"
#include <GLFW/glfw3.h>
#include <functional>

using GetCursorPosCallback = std::function<void(double *, double *)>;

// TODO: separate input handling from game logic
class Game
{
  public:
    Game(float initialWidth, float initialHeight)
        : wKeyPressed(false), aKeyPressed(false), sKeyPressed(false), dKeyPressed(false), deltaTime(0.0f),
          leftMouseButtonPressed(false), lastFrame(0.0f),
          shader("shaders/shader_vertex.glsl", "shaders/shader_fragment.glsl"), renderer(),
          camera(glm::vec4(0.0f, 0.0f, -3.0f, 1.0f))
    {
        viewRatio = initialWidth / initialHeight;

        Mesh mesh("models/cow.obj");
        scene.push_back(mesh);
    }

    void tick()
    {
        updateDeltaTime();
        processInput();

        renderer.clear();

        glm::mat4 view = camera.getViewMatrix();
        glm::mat4 projection = Camera::perspectiveMatrix(glm::radians(45.0f), viewRatio, -0.1f, -10.0f);

        renderer.renderScene(scene, shader, view, projection);
    }

    void keyCallback(int key, int action)
    {
        if (key == GLFW_KEY_W)
            wKeyPressed = (action != GLFW_RELEASE);
        if (key == GLFW_KEY_A)
            aKeyPressed = (action != GLFW_RELEASE);
        if (key == GLFW_KEY_S)
            sKeyPressed = (action != GLFW_RELEASE);
        if (key == GLFW_KEY_D)
            dKeyPressed = (action != GLFW_RELEASE);
    }

    void cursorPosCallback(double xpos, double ypos)
    {
        if (!leftMouseButtonPressed)
            return;

        float dx = xpos - lastCursorPosX;
        float dy = ypos - lastCursorPosY;

        camera.processMouseMovement(dx, dy);

        lastCursorPosX = xpos;
        lastCursorPosY = ypos;
    }

    void mouseButtonCallback(int button, int action, GetCursorPosCallback getCursorPosCallback)
    {
        if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
        {
            getCursorPosCallback(&lastCursorPosX, &lastCursorPosY);
            leftMouseButtonPressed = true;
        }

        if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
        {
            leftMouseButtonPressed = false;
        }
    }

    void scrollCallback(double xoffset, double yoffset)
    {
    }

    void setViewRatio(float width, float height)
    {
        std::cout << "Setting view ratio to " << width << " / " << height << std::endl;
        viewRatio = width / height;
    }

  private:
    Camera camera;
    Shader shader;
    Renderer renderer;
    std::vector<Mesh> scene;
    float viewRatio;

    bool wKeyPressed;
    bool aKeyPressed;
    bool sKeyPressed;
    bool dKeyPressed;

    bool leftMouseButtonPressed;
    double lastCursorPosX;
    double lastCursorPosY;

    float deltaTime;
    float lastFrame;

    void updateDeltaTime()
    {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
    }

    void processInput()
    {
        if (wKeyPressed)
            camera.processKeyboard(FORWARD, deltaTime);
        if (aKeyPressed)
            camera.processKeyboard(LEFT, deltaTime);
        if (sKeyPressed)
            camera.processKeyboard(BACKWARD, deltaTime);
        if (dKeyPressed)
            camera.processKeyboard(RIGHT, deltaTime);
    }
};