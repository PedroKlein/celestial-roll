#pragma once

#include <string>
#include <unordered_map>
#include "shader.hpp"

#define DEFAULT_VERTEX_SHADER_PATH "resources/shaders/default.vert"
#define DEFAULT_FRAGMENT_SHADER_PATH "resources/shaders/default.frag"

class ShaderManager {
public:
    static ShaderManager &getInstance() {
        static ShaderManager instance;
        return instance;
    }

    ShaderManager(const ShaderManager &) = delete;
    ShaderManager &operator=(const ShaderManager &) = delete;

    std::shared_ptr<Shader> loadShader(const std::string &vertexPath, const std::string &fragmentPath,
                                       const std::string &name) {
        if (const auto it = shaders.find(name); it != shaders.end()) {
            return it->second;
        }

        auto shader = std::make_shared<Shader>();
        shader->initialize(vertexPath.c_str(), fragmentPath.c_str());
        shaders[name] = shader;
        return shader;
    }

    std::shared_ptr<Shader> getShader(const std::string &name) {
        if (const auto it = shaders.find(name); it != shaders.end()) {
            return it->second;
        }

        throw std::runtime_error("Shader not found: " + name);
    }

    // get shader by id
    std::shared_ptr<Shader> getShader(const int id) {
        for (auto &[shaderName, shader]: shaders) {
            if (shader->ID == id) {
                return shader;
            }
        }

        throw std::runtime_error("Shader not found with ID: " + std::to_string(id));
    }

private:
    ShaderManager() {}

    std::unordered_map<std::string, std::shared_ptr<Shader>> shaders;
};
