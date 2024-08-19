#pragma once

#include "game/component.hpp"
#include "graphics/material.hpp"
#include "graphics/mesh.hpp"
#include "interpolatedTransform.hpp"

struct CustomRenderProperties {
    std::unordered_map<std::string, float> floatProperties;
    std::unordered_map<std::string, glm::vec3> vec3Properties;
};

class Renderer final : public Component {
public:
    std::shared_ptr<Mesh> mesh;
    std::shared_ptr<Material> material;
    std::shared_ptr<InterpolatedTransform> interpolatedTransform;

    Renderer(const std::shared_ptr<Mesh> &mesh, const std::shared_ptr<Material> &material, float offset = 0.0f,
             glm::vec4 *offsetOrientation = nullptr);

    void initialize() override;

    void update(float alpha) override;

    void applyCustomProperties();

    void setCustomFloatProperty(const std::string &name, float value);

    void setCustomVec3Property(const std::string &name, const glm::vec3 &value);

private:
    float offset;
    glm::vec4 *offsetOrientation;
    CustomRenderProperties customProperties;
};
