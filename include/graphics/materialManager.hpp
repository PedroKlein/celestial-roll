#pragma once

#include "material.hpp"
#include <memory>
#include <string>
#include <unordered_map>

// TODO: refacor the usage of shader name
class MaterialManager
{
  public:
    static MaterialManager &getInstance()
    {
        static MaterialManager instance;
        return instance;
    }

    std::shared_ptr<Material> getMaterial(const std::string &filename, const std::string &shaderName = "default")
    {
        auto key = filename + shaderName;
        auto it = materials.find(key);
        if (it != materials.end())
        {
            return it->second;
        }
        else
        {
            std::cout << "Loading material: " << key << std::endl;
            std::shared_ptr<Material> material = std::make_shared<Material>(filename, shaderName);
            materials[key] = material;
            return material;
        }
    }

    MaterialManager(MaterialManager const &) = delete;
    void operator=(MaterialManager const &) = delete;

  private:
    MaterialManager()
    {
    }

    std::unordered_map<std::string, std::shared_ptr<Material>> materials;
};