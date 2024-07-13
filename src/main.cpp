#define TINYOBJLOADER_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION

#include <glad/glad.h>

#include "game/game.hpp"
#include "graphics/shader.hpp"

#include <GLFW/glfw3.h>

#include <iostream>

constexpr float INITIAL_WIDTH = 800.0f;
constexpr float INITIAL_HEIGHT = 600.0f;

std::unordered_map<std::string, unsigned int> Shader::shaderCache;

void keyCallback(GLFWwindow *window, const int key, int scancode, const int action, int mods) {
    const auto game = static_cast<Game *>(glfwGetWindowUserPointer(window));
    game->getInputHandler()->keyCallback(key, action);
}

void mouseButtonCallback(GLFWwindow *window, const int button, const int action, int mods) {
    const auto game = static_cast<Game *>(glfwGetWindowUserPointer(window));
    game->getInputHandler()->mouseButtonCallback(
            button, action, [&window](double *xpos, double *ypos) { glfwGetCursorPos(window, xpos, ypos); });
}

void cursorPosCallback(GLFWwindow *window, const double xpos, const double ypos) {
    const auto game = static_cast<Game *>(glfwGetWindowUserPointer(window));
    game->getInputHandler()->cursorPosCallback(xpos, ypos);
}

void scrollCallback(GLFWwindow *window, double xoffset, double yoffset) {
    const auto game = static_cast<Game *>(glfwGetWindowUserPointer(window));
}

void framebufferSizeCallback(GLFWwindow *window, const int width, const int height) {
    const auto game = static_cast<Game *>(glfwGetWindowUserPointer(window));
    game->setViewRatio(width, height);

    glViewport(0, 0, width, height);
}

int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window = glfwCreateWindow(INITIAL_WIDTH, INITIAL_HEIGHT, "Celestial Roll", nullptr, nullptr);
    if (window == nullptr) {
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

    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    Game game(INITIAL_WIDTH, INITIAL_HEIGHT);
    glfwSetWindowUserPointer(window, &game);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_DEPTH_TEST);

    // enable vsync
    // glfwSwapInterval(1);

    while (!glfwWindowShouldClose(window)) {
        game.tick();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
