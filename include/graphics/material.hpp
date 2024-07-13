#pragma once

#include <glm/glm.hpp>
#include <memory>
#include <string>
#include "mtlLoader.hpp"
#include "shader.hpp"
#include "shaderManager.hpp"
#include "texture.hpp"

class Material {
public:
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    float shininess;
    std::shared_ptr<Texture> diffuseTexture;
    std::shared_ptr<Shader> shader;

    explicit Material(const std::string &materialPath, const std::string &shaderName = "default") :
        ambient(1.0f), diffuse(1.0f), specular(1.0f), shininess(32.0f), loader(materialPath) {
        if (!loader.loadMaterial()) {
            throw std::runtime_error("Failed to load material");
        }

        const RawMaterial rawMat = loader.getRawMaterial();

        ambient = glm::vec3(rawMat.ambient[0], rawMat.ambient[1], rawMat.ambient[2]);
        diffuse = glm::vec3(rawMat.diffuse[0], rawMat.diffuse[1], rawMat.diffuse[2]);
        specular = glm::vec3(rawMat.specular[0], rawMat.specular[1], rawMat.specular[2]);
        shininess = rawMat.shininess;

        if (!rawMat.diffuseTexturePath.empty()) {
            setDiffuseTexture(rawMat.diffuseTexturePath);
        }

        shader = ShaderManager::getInstance().getShader(shaderName);
    }

    void setDiffuseTexture(const std::string &texturePath) {
        diffuseTexture = std::make_shared<Texture>(texturePath.c_str(), "diffuse");
    }

    void applyShaderProperties() const {
        shader->setVec3("material.ambient", ambient);
        shader->setVec3("material.diffuse", diffuse);
        shader->setVec3("material.specular", specular);
        shader->setFloat("material.shininess", shininess);

        glActiveTexture(GL_TEXTURE0);

        if (diffuseTexture) {
            glBindTexture(GL_TEXTURE_2D, diffuseTexture->getID());
            shader->setInt("material.diffuseTexture", 0);
        } else {
            glBindTexture(GL_TEXTURE_2D, 0);
            shader->setInt("material.diffuseTexture", 0);
        }
    }

private:
    MtlLoader loader;
};
