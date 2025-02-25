#pragma once

#include <fstream>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>

class Shader {
public:
    unsigned int ID;
    static std::unordered_map<std::string, unsigned int> shaderCache;

    void initialize(const char *vertexPath, const char *fragmentPath) {
        const unsigned int vertex = loadShader(vertexPath, GL_VERTEX_SHADER);
        const unsigned int fragment = loadShader(fragmentPath, GL_FRAGMENT_SHADER);

        ID = glCreateProgram();
        glAttachShader(ID, vertex);
        glAttachShader(ID, fragment);
        glLinkProgram(ID);
        checkCompileErrors(ID, "PROGRAM");

        glUseProgram(ID);
        bindUniformBlock("Matrices", 0);
        bindUniformBlock("Lights", 1);
        bindUniformBlock("Common", 2);

        glDetachShader(ID, vertex);
        glDetachShader(ID, fragment);
    }

    static unsigned int loadShader(const std::string &shaderPath, const GLenum shaderType) {
        if (shaderCache.find(shaderPath) != shaderCache.end()) {
            return shaderCache[shaderPath];
        }

        std::string shaderCode;
        std::ifstream shaderFile;

        shaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        try {
            shaderFile.open(shaderPath);
            std::stringstream shaderStream;
            shaderStream << shaderFile.rdbuf();
            shaderFile.close();
            shaderCode = shaderStream.str();
        } catch (std::ifstream::failure &e) {
            std::cerr << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ: " << e.what() << std::endl;
            std::exit(EXIT_FAILURE);
        }

        const char *code = shaderCode.c_str();
        unsigned int shader = glCreateShader(shaderType);
        glShaderSource(shader, 1, &code, nullptr);
        glCompileShader(shader);
        checkCompileErrors(shader, shaderType == GL_VERTEX_SHADER ? "VERTEX" : "FRAGMENT");

        shaderCache[shaderPath] = shader;
        return shader;
    }

    void use() const { glUseProgram(ID); }

    void setBool(const std::string &name, const bool value) const {
        glUniform1i(glGetUniformLocation(ID, name.c_str()), static_cast<int>(value));
    }
    void setInt(const std::string &name, const int value) const {
        glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
    }
    void setFloat(const std::string &name, const float value) const {
        glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
    }
    void setVec2(const std::string &name, const glm::vec2 &value) const {
        glUniform2fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
    }
    void setVec3(const std::string &name, const glm::vec3 &value) const {
        glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
    }
    void setVec4(const std::string &name, const glm::vec4 &value) const {
        glUniform4fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
    }
    void setMat2(const std::string &name, const glm::mat2 &mat) const {
        glUniformMatrix2fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }
    void setMat3(const std::string &name, const glm::mat3 &mat) const {
        glUniformMatrix3fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }
    void setMat4(const std::string &name, const glm::mat4 &mat) const {
        glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }

    static void clearShaderCache() {
        for (auto &[shaderName, shaderId]: shaderCache) {
            glDeleteShader(shaderId);
        }
        shaderCache.clear();
    }

private:
    static void checkCompileErrors(const GLuint shader, const std::string &type) {
        GLint success;
        GLchar infoLog[1024];
        if (type != "PROGRAM") {
            glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
            if (!success) {
                glGetShaderInfoLog(shader, 1024, nullptr, infoLog);
                std::cerr << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n"
                          << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
            }
        } else {
            glGetProgramiv(shader, GL_LINK_STATUS, &success);
            if (!success) {
                glGetProgramInfoLog(shader, 1024, nullptr, infoLog);
                std::cerr << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n"
                          << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
            }
        }
    }

    void bindUniformBlock(const std::string &blockName, const GLuint bindingPoint) const {
        const GLuint blockIndex = glGetUniformBlockIndex(ID, blockName.c_str());
        if (blockIndex == GL_INVALID_INDEX) {
            std::cerr << "ERROR::SHADER::Could not find uniform block '" << blockName << "'" << std::endl;
            return;
        }
        glUniformBlockBinding(ID, blockIndex, bindingPoint);
    }
};
