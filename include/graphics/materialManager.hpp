#pragma once

#include <string>
#include <unordered_map>
#include "material.hpp"

class MaterialManager {
public:
    static MaterialManager &getInstance() {
        static MaterialManager instance;
        return instance;
    }

    void saveMaterial(const std::shared_ptr<Material> &material, const std::string &name) {
        materials[name] = material;
    }

    std::shared_ptr<Material> getMaterial(const std::string &name) {
        if (const auto it = materials.find(name); it != materials.end()) {
            return it->second;
        }

        return nullptr;
    }

    MaterialManager(MaterialManager const &) = delete;
    void operator=(MaterialManager const &) = delete;

private:
    MaterialManager() {}

    std::unordered_map<std::string, std::shared_ptr<Material>> materials;
};
