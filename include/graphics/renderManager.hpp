#pragma once

#include "graphics/lightEmitter.hpp"
#include "graphics/renderer.hpp"
#include "objects/platform.hpp"
#include "objects/player.hpp"
#include <memory>

class RenderManager
{
  public:
    static void initializeShaders()
    {
        ShaderManager::getInstance().loadShader(DEFAULT_VERTEX_SHADER_PATH, DEFAULT_FRAGMENT_SHADER_PATH, "default");
        Platform::initializeShaders();
    }

    RenderManager(Player &player) : player(player)
    {
        setupMatricesUBO();
    }

    ~RenderManager()
    {
        glDeleteBuffers(1, &uboMatrices);
    }

    void render(float alpha, const glm::mat4 &viewMatrix, const glm::mat4 &projectionMatrix)
    {
        clear();

        updateMatricesUBO(viewMatrix, projectionMatrix);

        player.render(alpha);

        for (auto &renderer : renderers)
        {
            renderer->update(alpha);
        }
    }

    void addRenderer(const std::shared_ptr<Renderer> &renderer)
    {
        renderers.push_back(renderer);
    }

    void addLightEmitter(const std::shared_ptr<LightEmitter> &lightEmitter)
    {
        lightEmitters.push_back(lightEmitter);
    }

  private:
    static constexpr GLuint UBO_BINDING_POINT = 0;
    GLuint uboMatrices;

    std::vector<std::shared_ptr<Renderer>> renderers;
    std::vector<std::shared_ptr<LightEmitter>> lightEmitters;

    Player &player;

    void setupMatricesUBO()
    {
        glGenBuffers(1, &uboMatrices);
        glBindBuffer(GL_UNIFORM_BUFFER, uboMatrices);
        glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), NULL, GL_STATIC_DRAW);
        glBindBuffer(GL_UNIFORM_BUFFER, 0);
        glBindBufferBase(GL_UNIFORM_BUFFER, UBO_BINDING_POINT, uboMatrices);
    }

    void updateMatricesUBO(const glm::mat4 &viewMatrix, const glm::mat4 &projectionMatrix)
    {
        glBindBuffer(GL_UNIFORM_BUFFER, uboMatrices);
        glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), &viewMatrix[0][0]);
        glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), &projectionMatrix[0][0]);
        glBindBuffer(GL_UNIFORM_BUFFER, 0);
    }

    void clear()
    {
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }
};