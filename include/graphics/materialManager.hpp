#pragma once

#include <string>
#include <unordered_map>
#include "material.hpp"

// TODO: refacor the usage of shader name
class MaterialManager {
public:
    static MaterialManager &getInstance() {
        static MaterialManager instance;
        return instance;
    }

    std::shared_ptr<Material> getMaterial(const std::string &filename, const std::string &shaderName = "default") {
        const auto key = filename + shaderName;
        if (const auto it = materials.find(key); it != materials.end()) {
            return it->second;
        }

        std::cout << "Loading material: " << key << std::endl;
        auto material = std::make_shared<Material>(filename, shaderName);
        materials[key] = material;
        return material;
    }

    MaterialManager(MaterialManager const &) = delete;
    void operator=(MaterialManager const &) = delete;

private:
    MaterialManager() {}

    std::unordered_map<std::string, std::shared_ptr<Material>> materials;
};
