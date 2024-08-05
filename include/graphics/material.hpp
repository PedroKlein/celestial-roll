#pragma once

#include <glm/glm.hpp>
#include <memory>
#include <string>
#include <unordered_map>
#include "shader.hpp"
#include "texture.hpp"

enum class MaterialTextureType { Diffuse, Specular, Normal, Height, Roughness, AO };

class Material {
public:
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    float shininess;
    bool isOpaque;
    std::unordered_map<MaterialTextureType, std::shared_ptr<Texture>> textures;
    std::shared_ptr<Shader> shader;

    explicit Material(const glm::vec3 &ambient, const glm::vec3 &diffuse, const glm::vec3 &specular, float shininess,
                      bool isOpaque = true) :
        ambient(ambient), diffuse(diffuse), specular(specular), shininess(shininess), isOpaque(isOpaque) {}

    void setTexture(MaterialTextureType type, const std::string &texturePath) {
        textures[type] = std::make_shared<Texture>(texturePath);
    }

    void setShader(const std::shared_ptr<Shader> &shader) { this->shader = shader; }

    [[nodiscard]] bool getIsOpaque() const { return isOpaque; }

    void applyShaderProperties() const {
        shader->setVec3("material.ambient", ambient);
        shader->setVec3("material.diffuse", diffuse);
        shader->setVec3("material.specular", specular);
        shader->setFloat("material.shininess", shininess);

        applyTextures();
    }

private:
    void applyTextures() const {
        int textureUnit = 0;
        for (const auto &[type, texture]: textures) {
            glActiveTexture(GL_TEXTURE0 + textureUnit);
            texture->bind();
            shader->setInt(getUniformName(type), textureUnit++);
        }
    }

    static std::string getUniformName(MaterialTextureType type) {
        switch (type) {
            case MaterialTextureType::Diffuse:
                return "material.diffuseTexture";
            case MaterialTextureType::Specular:
                return "material.specularTexture";
            case MaterialTextureType::Normal:
                return "material.normalTexture";
            case MaterialTextureType::Height:
                return "material.heightTexture";
            case MaterialTextureType::Roughness:
                return "material.roughnessTexture";
            case MaterialTextureType::AO:
                return "material.aoTexture";
            default:
                return ""; // Error handling or assertion could be added here
        }
    }
};
