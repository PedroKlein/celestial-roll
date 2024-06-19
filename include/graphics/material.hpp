#pragma once

#include "mtlLoader.hpp"
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

    Material(const std::string &filename)
        : loader(filename), ambient(1.0f), diffuse(1.0f), specular(1.0f), shininess(32.0f)
    {

        if (!loader.loadMaterial())
        {
            throw std::runtime_error("Failed to load material");
        }

        RawMaterial rawMat = loader.getRawMaterial();

        ambient = glm::vec3(rawMat.ambient[0], rawMat.ambient[1], rawMat.ambient[2]);
        diffuse = glm::vec3(rawMat.diffuse[0], rawMat.diffuse[1], rawMat.diffuse[2]);
        specular = glm::vec3(rawMat.specular[0], rawMat.specular[1], rawMat.specular[2]);
        shininess = rawMat.shininess;

        if (!rawMat.diffuseTexturePath.empty())
        {
            setDiffuseTexture(rawMat.diffuseTexturePath);
        }
    }

    void setDiffuseTexture(const std::string &texturePath)
    {
        diffuseTexture = std::make_shared<Texture>(texturePath.c_str(), "diffuse");
    }

  private:
    MtlLoader loader;
};