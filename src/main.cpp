#include <glad/glad.h>

#include "game.hpp"
#include "matrixUtils.hpp"
#include "mesh.hpp"
#include "objLoader.hpp"
#include "renderer.hpp"
#include "shader.hpp"
#include "texture.hpp"
#include <camera.hpp>

#include <GLFW/glfw3.h>

#include <iostream>

constexpr float INITIAL_WIDTH = 800.0f;
constexpr float INITIAL_HEIGHT = 600.0f;

void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    Game *game = static_cast<Game *>(glfwGetWindowUserPointer(window));
    game->keyCallback(key, action);
}

void mouseButtonCallback(GLFWwindow *window, int button, int action, int mods)
{
    Game *game = static_cast<Game *>(glfwGetWindowUserPointer(window));
    game->mouseButtonCallback(button, action,
                              [&window](double *xpos, double *ypos) { glfwGetCursorPos(window, xpos, ypos); });
}

void cursorPosCallback(GLFWwindow *window, double xpos, double ypos)
{
    Game *game = static_cast<Game *>(glfwGetWindowUserPointer(window));
    game->cursorPosCallback(xpos, ypos);
}

void scrollCallback(GLFWwindow *window, double xoffset, double yoffset)
{
    Game *game = static_cast<Game *>(glfwGetWindowUserPointer(window));
}

void framebufferSizeCallback(GLFWwindow *window, int width, int height)
{
    Game *game = static_cast<Game *>(glfwGetWindowUserPointer(window));
    game->setViewRatio(width, height);

    glViewport(0, 0, width, height);
}

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window = glfwCreateWindow(INITIAL_WIDTH, INITIAL_HEIGHT, "Celestial Roll", nullptr, nullptr);
    if (window == nullptr)
    {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
    glfwSetKeyCallback(window, keyCallback);
    glfwSetCursorPosCallback(window, cursorPosCallback);
    glfwSetScrollCallback(window, scrollCallback);
    glfwSetMouseButtonCallback(window, mouseButtonCallback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    Game game(INITIAL_WIDTH, INITIAL_HEIGHT);
    glfwSetWindowUserPointer(window, &game);

    while (!glfwWindowShouldClose(window))
    {
        game.tick();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
