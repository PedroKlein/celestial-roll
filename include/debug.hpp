#pragma once

#include <glm/glm.hpp>
#include <iostream>

class Debug
{
  public:
    static void printMatrix(glm::mat4 m)
    {
        for (int i = 0; i < 4; i++)
        {
            for (int j = 0; j < 4; j++)
            {
                std::cout << m[i][j] << " ";
            }
            std::cout << std::endl;
        }

        std::cout << std::endl;
    }

    static void printVec(const glm::vec3 v)
    {
        std::cout << "(" << v.x << ", " << v.y << ", " << v.z << ")" << std::endl;
    }

    static void printVec(const glm::vec4 v)
    {
        std::cout << "(" << v.x << ", " << v.y << ", " << v.z << ", " << v.w << ")" << std::endl;
    }
};