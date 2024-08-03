#pragma once

#include <glad/glad.h>
#include <iostream>
#include <string>
#include <vector>
#include "stb_image.h"

enum class TextureType { Texture2D, CubeMap };

class Texture {
public:
    Texture(const char *texturePath, const GLint wrapMode = GL_REPEAT, const GLint minFilter = GL_LINEAR_MIPMAP_LINEAR,
            const GLint magFilter = GL_LINEAR) : textureType(TextureType::Texture2D), path(texturePath) {
        glGenTextures(1, &ID);
        if (ID == 0)
            throw std::runtime_error("Failed to generate texture");

        glBindTexture(GL_TEXTURE_2D, ID);
        loadSingleTexture(texturePath, wrapMode, minFilter, magFilter);
    }

    // Constructor for cube maps
    Texture(const std::vector<std::string> &faces) : textureType(TextureType::CubeMap) {
        glGenTextures(1, &ID);
        if (ID == 0)
            throw std::runtime_error("Failed to generate texture");

        glBindTexture(GL_TEXTURE_CUBE_MAP, ID);
        loadCubeMap(faces);
    }

    void bind() const {
        if (textureType == TextureType::Texture2D) {
            glBindTexture(GL_TEXTURE_2D, ID);
        } else if (textureType == TextureType::CubeMap) {
            glBindTexture(GL_TEXTURE_CUBE_MAP, ID);
        }
    }

    [[nodiscard]] unsigned int getID() const { return ID; }

    [[nodiscard]] TextureType getType() const { return textureType; }

    [[nodiscard]] std::string getPath() const { return path; }

    ~Texture() {
        if (ID != 0)
            glDeleteTextures(1, &ID);
    }

private:
    unsigned int ID{};
    TextureType textureType;
    std::string path;

    void loadSingleTexture(const char *texturePath, GLint wrapMode, GLint minFilter, GLint magFilter) {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapMode);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapMode);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);

        int width, height, nrChannels;
        unsigned char *data = stbi_load(texturePath, &width, &height, &nrChannels, 0);
        if (data) {
            GLenum format = (nrChannels == 1) ? GL_RED : (nrChannels == 3) ? GL_RGB : GL_RGBA;
            glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
        } else {
            std::cerr << "Failed to load texture at path: " << texturePath << std::endl;
        }
        stbi_image_free(data);
    }

    void loadCubeMap(const std::vector<std::string> &faces) {
        int width, height, nrChannels;
        for (int i = 0; i < faces.size(); i++) {
            unsigned char *data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
            if (data) {
                GLenum format = (nrChannels == 1) ? GL_RED : (nrChannels == 3) ? GL_RGB : GL_RGBA;
                glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE,
                             data);
                stbi_image_free(data);
            } else {
                std::cerr << "Cube map texture failed to load at path: " << faces[i] << std::endl;
            }
        }
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    }
};
