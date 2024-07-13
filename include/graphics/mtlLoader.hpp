#pragma once

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

struct RawMaterial {
    std::string name;
    float shininess{};
    float ambient[3]{};
    float specular[3]{};
    float diffuse[3]{};
    float opticalDensity{};
    float transparency{};
    int illumination{};
    std::string diffuseTexturePath;
};

// only loads the first material form the file
class MtlLoader {
public:
    explicit MtlLoader(std::string filename) : filename(std::move(filename)) {}

    bool loadMaterial() {
        std::ifstream file(filename);
        if (!file.is_open()) {
            std::cerr << "Failed to open file: " << filename << std::endl;
            return false;
        }

        std::string line, prefix;

        while (getline(file, line)) {
            std::istringstream lineStream(line);
            lineStream >> prefix;
            if (prefix == "newmtl") {
                if (!rawMaterial.name.empty()) {
                    rawMaterial = RawMaterial();
                }
                lineStream >> rawMaterial.name;
            } else if (prefix == "Ns") {
                lineStream >> rawMaterial.shininess;
            } else if (prefix == "Ka") {
                lineStream >> rawMaterial.ambient[0] >> rawMaterial.ambient[1] >> rawMaterial.ambient[2];
            } else if (prefix == "Ks") {
                lineStream >> rawMaterial.specular[0] >> rawMaterial.specular[1] >> rawMaterial.specular[2];
            } else if (prefix == "Ke") {
                lineStream >> rawMaterial.diffuse[0] >> rawMaterial.diffuse[1] >> rawMaterial.diffuse[2];
            } else if (prefix == "Ni") {
                lineStream >> rawMaterial.opticalDensity;
            } else if (prefix == "d") {
                lineStream >> rawMaterial.transparency;
            } else if (prefix == "illum") {
                lineStream >> rawMaterial.illumination;
            } else if (prefix == "map_Kd") {
                lineStream >> rawMaterial.diffuseTexturePath;
            }
        }

        file.close();
        return true;
    }

    [[nodiscard]] const RawMaterial &getRawMaterial() const { return rawMaterial; }

private:
    RawMaterial rawMaterial;
    std::string filename;
};
