#pragma once

#include "graphics/lightEmitter.hpp"
#include "graphics/renderer.hpp"
#include "graphics/shaderManager.hpp"
#include "objects/platform.hpp"
#include "objects/player.hpp"
#include <array>
#include <memory>
#include <unordered_map>

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
        setupLightsUBO();
        setupViewPosUBO();
    }

    ~RenderManager()
    {
        glDeleteBuffers(1, &uboMatrices);
    }

    void render(float alpha, const glm::mat4 &viewMatrix, const glm::mat4 &projectionMatrix, const glm::vec3 &viewPos)
    {
        clear();

        updateMatricesUBO(viewMatrix, projectionMatrix);
        updateViewPosUBO(viewPos);

        // batch rendering
        for (auto &entry : renderersByShader)
        {
            glUseProgram(entry.first);

            if (player.getShaderId() == entry.first)
            {
                player.render(alpha);
            }

            for (auto &renderer : entry.second)
            {
                renderer->update(alpha);
            }
        }
    }

    void addRenderer(const std::shared_ptr<Renderer> &renderer)
    {
        int shaderID = renderer->material->shader->ID;
        renderersByShader[shaderID].push_back(renderer);
    }

    void addLightEmitter(const std::shared_ptr<LightEmitter> &lightEmitter)
    {
        std::vector<LightInfo> lightsData;

        for (size_t i = 0; i < MAX_LIGHTS; i++)
        {
            if (!lightEmitters[i])
            {
                lightEmitters[i] = lightEmitter;
                // currently only static light emmiters are suporeted
                activeLights.push_back(lightEmitter->getLight());
                updateLightsUBO();
                return;
            }
        }

        std::cerr << "Max light emitters reached" << std::endl;
    }

  private:
    static constexpr GLuint UBO_MATRICES_BINDING_POINT = 0;
    static constexpr GLuint UBO_LIGHTS_BINDING_POINT = 1;
    static constexpr GLuint UBO_VIEW_POS_BINDING_POINT = 2;
    static constexpr size_t MAX_LIGHTS = 10;
    GLuint uboMatrices;
    GLuint uboLights;
    GLuint uboViewPos;

    std::unordered_map<int, std::vector<std::shared_ptr<Renderer>>> renderersByShader;
    std::array<std::shared_ptr<LightEmitter>, MAX_LIGHTS> lightEmitters;
    std::vector<LightInfo> activeLights;

    Player &player;

    void setupMatricesUBO()
    {
        glGenBuffers(1, &uboMatrices);
        glBindBuffer(GL_UNIFORM_BUFFER, uboMatrices);
        glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), NULL, GL_STATIC_DRAW);
        glBindBuffer(GL_UNIFORM_BUFFER, 0);
        glBindBufferBase(GL_UNIFORM_BUFFER, UBO_MATRICES_BINDING_POINT, uboMatrices);
    }

    void updateMatricesUBO(const glm::mat4 &viewMatrix, const glm::mat4 &projectionMatrix)
    {
        glBindBuffer(GL_UNIFORM_BUFFER, uboMatrices);
        glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), &viewMatrix[0][0]);
        glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), &projectionMatrix[0][0]);
        glBindBuffer(GL_UNIFORM_BUFFER, 0);
    }

    void setupLightsUBO()
    {
        glGenBuffers(1, &uboLights);
        glBindBuffer(GL_UNIFORM_BUFFER, uboLights);
        glBufferData(GL_UNIFORM_BUFFER, sizeof(LightsUBO), NULL, GL_STATIC_DRAW);
        glBindBuffer(GL_UNIFORM_BUFFER, 0);
        glBindBufferBase(GL_UNIFORM_BUFFER, UBO_LIGHTS_BINDING_POINT, uboLights);
    }

    void updateLightsUBO()
    {
        LightsUBO lightsUBO;
        lightsUBO.lightCount = activeLights.size();
        std::memcpy(lightsUBO.lights, activeLights.data(), activeLights.size() * sizeof(LightInfo));

        glBindBuffer(GL_UNIFORM_BUFFER, uboLights);
        glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(LightsUBO), &lightsUBO);
        glBindBuffer(GL_UNIFORM_BUFFER, 0);
    }

    void setupViewPosUBO()
    {
        glGenBuffers(1, &uboViewPos);
        glBindBuffer(GL_UNIFORM_BUFFER, uboViewPos);
        glBufferData(GL_UNIFORM_BUFFER, sizeof(glm::vec3), NULL, GL_STATIC_DRAW);
        glBindBuffer(GL_UNIFORM_BUFFER, 0);
        glBindBufferBase(GL_UNIFORM_BUFFER, UBO_VIEW_POS_BINDING_POINT, uboViewPos);
    }

    void updateViewPosUBO(const glm::vec3 &viewPos)
    {
        glBindBuffer(GL_UNIFORM_BUFFER, uboViewPos);
        glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::vec3), &viewPos);
        glBindBuffer(GL_UNIFORM_BUFFER, 0);
    }

    void clear()
    {
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    struct LightsUBO
    {
        int lightCount;
        // padding to ensure data alignment, vec4 is 16 bytes, so 4 for int and more 12 to align to 16
        char pad[12];
        LightInfo lights[MAX_LIGHTS];
    };
};