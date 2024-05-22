#pragma once

#include "texture.hpp"
#include <glm/glm.hpp>
#include <memory>

struct Material
{
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    float shininess;

    std::shared_ptr<Texture> diffuseTexture;

    Material() : ambient(1.0f), diffuse(1.0f), specular(1.0f), shininess(32.0f)
    {
    }

    void setDiffuseTexture(const std::string &texturePath)
    {
        diffuseTexture = std::make_shared<Texture>(texturePath.c_str(), "diffuse");
    }
};