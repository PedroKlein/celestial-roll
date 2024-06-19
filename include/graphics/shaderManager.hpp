#pragma once

#include "shader.hpp"
#include <memory>
#include <string>
#include <unordered_map>

#define DEFAULT_VERTEX_SHADER_PATH "resources/shaders/default.vert"
#define DEFAULT_FRAGMENT_SHADER_PATH "resources/shaders/default.frag"

class ShaderManager
{
  public:
    static ShaderManager &getInstance()
    {
        static ShaderManager instance;
        return instance;
    }

    ShaderManager(const ShaderManager &) = delete;
    ShaderManager &operator=(const ShaderManager &) = delete;

    std::shared_ptr<Shader> loadShader(const std::string &vertexPath, const std::string &fragmentPath,
                                       const std::string &name)
    {
        auto it = shaders.find(name);
        if (it != shaders.end())
        {
            return it->second;
        }

        std::shared_ptr<Shader> shader = std::make_shared<Shader>();
        shader->initialize(vertexPath.c_str(), fragmentPath.c_str());
        shaders[name] = shader;
        return shader;
    }

    std::shared_ptr<Shader> getShader(const std::string &name)
    {
        auto it = shaders.find(name);
        if (it != shaders.end())
        {
            return it->second;
        }

        throw std::runtime_error("Shader not found: " + name);
    }

  private:
    ShaderManager()
    {
    }

    std::unordered_map<std::string, std::shared_ptr<Shader>> shaders;
};