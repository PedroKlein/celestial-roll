#pragma once

#include <GLFW/glfw3.h>
#include <array>
#include <memory>
#include <unordered_map>
#include "graphics/lightEmitter.hpp"
#include "graphics/renderer.hpp"
#include "graphics/shaderManager.hpp"
#include "objects/debugAxis.hpp"
#include "objects/platform.hpp"
#include "objects/player.hpp"
#include "objects/skybox.hpp"

class RenderManager {
public:
    static void initializeShaders() {
        ShaderManager::getInstance().loadShader(DEFAULT_VERTEX_SHADER_PATH, DEFAULT_FRAGMENT_SHADER_PATH, "default");
        ShaderManager::getInstance().loadShader(DEFAULT_VERTEX_SHADER_PATH, "resources/shaders/ice.frag", "ice");
        ShaderManager::getInstance().loadShader(DEFAULT_VERTEX_SHADER_PATH, "resources/shaders/star.frag", "star");
        ShaderManager::getInstance().loadShader("resources/shaders/gouraud.vert", "resources/shaders/gouraud.frag",
                                                "gouraud");
    }

    static void initializeMaterials() {
        auto defaultShader = ShaderManager::getInstance().getShader("default");
        auto iceShader = ShaderManager::getInstance().getShader("ice");
        auto starShader = ShaderManager::getInstance().getShader("star");
        auto gouraudShader = ShaderManager::getInstance().getShader("gouraud");

        auto defaultMaterial = std::make_shared<Material>(glm::vec3(1.0f), glm::vec3(0.5f), glm::vec3(0.0f), 1.0f);
        defaultMaterial->setShader(defaultShader);
        defaultMaterial->setPBRTexture("resources/textures/platform");
        MaterialManager::getInstance().saveMaterial(defaultMaterial, "default");

        auto tilesMaterial = std::make_shared<Material>(glm::vec3(1.0f), glm::vec3(0.5f), glm::vec3(0.0f), 32.0f);
        tilesMaterial->setShader(defaultShader);
        tilesMaterial->setPBRTexture("resources/textures/tiles");
        MaterialManager::getInstance().saveMaterial(tilesMaterial, "tiles");

        auto iceMaterial = std::make_shared<Material>(glm::vec3(1.0f), glm::vec3(0.6f), glm::vec3(1.0f), 1.0f, false);
        iceMaterial->setShader(iceShader);
        iceMaterial->setPBRTexture("resources/textures/ice");
        MaterialManager::getInstance().saveMaterial(iceMaterial, "ice");

        auto starMaterial = std::make_shared<Material>(glm::vec3(1.1f), glm::vec3(0.6f), glm::vec3(0.5f), 1.0f);
        starMaterial->setShader(starShader);
        MaterialManager::getInstance().saveMaterial(starMaterial, "star");

        auto playerMaterial = std::make_shared<Material>(glm::vec3(1.0f), glm::vec3(0.0f), glm::vec3(0.8f), 1.0f);
        playerMaterial->setShader(defaultShader);
        playerMaterial->setPBRTexture("resources/textures/marble");
        MaterialManager::getInstance().saveMaterial(playerMaterial, "player");

        auto cameraMaterial = std::make_shared<Material>(glm::vec3(1.0f), glm::vec3(0.6f), glm::vec3(0.0f), 1.0f);
        cameraMaterial->setShader(gouraudShader);
        cameraMaterial->setTexture(MaterialTextureType::Diffuse, "resources/textures/camera.jpg"); // -z and y
        MaterialManager::getInstance().saveMaterial(cameraMaterial, "camera");

        auto duckMaterial = std::make_shared<Material>(glm::vec3(1.0f), glm::vec3(0.6f), glm::vec3(0.0f), 1.0f);
        duckMaterial->setShader(defaultShader);
        duckMaterial->setTexture(MaterialTextureType::Diffuse, "resources/textures/duck.jpg");
        MaterialManager::getInstance().saveMaterial(duckMaterial, "duck");
    }

    explicit RenderManager(Player &player) : player(player), debugAxis(), skybox() {
        setupMatricesUBO();
        setupLightsUBO();
        setupCommonUBO();
    }

    ~RenderManager() { glDeleteBuffers(1, &uboMatrices); }

    void render(const float alpha, const glm::mat4 &viewMatrix, const glm::mat4 &projectionMatrix,
                const glm::vec3 &viewPos) {
        clear();

        updateMatricesUBO(viewMatrix, projectionMatrix);
        updateCommonUBO(viewPos, glfwGetTime());
        updateLightsUBO();

        skybox.render();

        // batch rendering opaque objects
        for (auto &[shaderId, renderers]: renderersByOpaqueShader) {
            glUseProgram(shaderId);

            if (player.getShaderId() == shaderId) {
                player.render(alpha);
            }

            for (const auto &renderer: renderers) {
                renderer->update(alpha);
            }
        }

        glDepthMask(GL_FALSE);

        // batch rendering non-opaque objects
        for (auto &[shaderId, renderers]: renderersByNonOpaqueShader) {
            glUseProgram(shaderId);

            for (const auto &renderer: renderers) {
                renderer->update(alpha);
            }
        }

        glDepthMask(GL_TRUE);

        // debugAxis.render(player.getPosition());
    }

    void addRenderer(const std::shared_ptr<Renderer> &renderer) {
        const unsigned int shaderID = renderer->material->shader->ID;
        const bool isOpaque = renderer->material->getIsOpaque();

        if (isOpaque) {
            renderersByOpaqueShader[shaderID].push_back(renderer);
        } else {
            renderersByNonOpaqueShader[shaderID].push_back(renderer);
        }
    }

    void addLightEmitter(const std::shared_ptr<LightEmitter> &lightEmitter) {
        std::vector<LightInfo> lightsData;

        for (size_t i = 0; i < MAX_LIGHTS; i++) {
            if (!lightEmitters[i]) {
                lightEmitters[i] = lightEmitter;
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
    GLuint uboMatrices{};
    GLuint uboLights{};
    GLuint uboCommon{};

    std::unordered_map<unsigned int, std::vector<std::shared_ptr<Renderer>>> renderersByOpaqueShader;
    std::unordered_map<unsigned int, std::vector<std::shared_ptr<Renderer>>> renderersByNonOpaqueShader;
    std::array<std::shared_ptr<LightEmitter>, MAX_LIGHTS> lightEmitters;

    Player &player;
    DebugAxis debugAxis;
    Skybox skybox;

    void setupMatricesUBO() {
        glGenBuffers(1, &uboMatrices);
        glBindBuffer(GL_UNIFORM_BUFFER, uboMatrices);
        glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), nullptr, GL_STATIC_DRAW);
        glBindBuffer(GL_UNIFORM_BUFFER, 0);
        glBindBufferBase(GL_UNIFORM_BUFFER, UBO_MATRICES_BINDING_POINT, uboMatrices);
    }

    void updateMatricesUBO(const glm::mat4 &viewMatrix, const glm::mat4 &projectionMatrix) const {
        glBindBuffer(GL_UNIFORM_BUFFER, uboMatrices);
        glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), &viewMatrix[0][0]);
        glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), &projectionMatrix[0][0]);
        glBindBuffer(GL_UNIFORM_BUFFER, 0);
    }

    void setupLightsUBO() {
        glGenBuffers(1, &uboLights);
        glBindBuffer(GL_UNIFORM_BUFFER, uboLights);
        glBufferData(GL_UNIFORM_BUFFER, sizeof(LightsUBO), nullptr, GL_STATIC_DRAW);
        glBindBuffer(GL_UNIFORM_BUFFER, 0);
        glBindBufferBase(GL_UNIFORM_BUFFER, UBO_LIGHTS_BINDING_POINT, uboLights);
    }

    void updateLightsUBO() const {
        std::vector<LightInfo> activeLights;

        for (size_t i = 0; i < MAX_LIGHTS; i++) {
            if (lightEmitters[i]) {
                activeLights.push_back(lightEmitters[i]->getLight());
            }
        }

        LightsUBO lightsUBO{};
        lightsUBO.lightCount = activeLights.size();
        std::memcpy(lightsUBO.lights, activeLights.data(), activeLights.size() * sizeof(LightInfo));

        glBindBuffer(GL_UNIFORM_BUFFER, uboLights);
        glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(LightsUBO), &lightsUBO);
        glBindBuffer(GL_UNIFORM_BUFFER, 0);
    }

    void setupCommonUBO() {
        glGenBuffers(1, &uboCommon);
        glBindBuffer(GL_UNIFORM_BUFFER, uboCommon);
        glBufferData(GL_UNIFORM_BUFFER, sizeof(CommonUBO), nullptr, GL_STATIC_DRAW);
        glBindBuffer(GL_UNIFORM_BUFFER, 0);
        glBindBufferBase(GL_UNIFORM_BUFFER, UBO_VIEW_POS_BINDING_POINT, uboCommon);
    }

    void updateCommonUBO(const glm::vec3 &viewPos, const float time) const {
        CommonUBO commonUBO{};
        commonUBO.viewPos = viewPos;
        commonUBO.time = time;
        glBindBuffer(GL_UNIFORM_BUFFER, uboCommon);
        glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(CommonUBO), &commonUBO);
        glBindBuffer(GL_UNIFORM_BUFFER, 0);
    }

    static void clear() {
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    struct LightsUBO {
        unsigned int lightCount;
        // padding to ensure data alignment, vec4 is 16 bytes, so 4 for int and more 12 to align to 16
        char pad[12];
        LightInfo lights[MAX_LIGHTS];
    };

    struct CommonUBO {
        float time;
        char pad[9];
        glm::vec3 viewPos;
    };
};
