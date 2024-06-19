#pragma once

#include "material.hpp"
#include <memory>
#include <string>
#include <unordered_map>

class MaterialManager
{
  public:
    static MaterialManager &getInstance()
    {
        static MaterialManager instance;
        return instance;
    }

    std::shared_ptr<Material> getMaterial(const std::string &filename)
    {
        auto it = materials.find(filename);
        if (it != materials.end())
        {
            return it->second;
        }
        else
        {
            std::shared_ptr<Material> material = std::make_shared<Material>(filename);
            materials[filename] = material;
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