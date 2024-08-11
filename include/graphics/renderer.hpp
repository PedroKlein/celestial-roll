#pragma once

#include "game/component.hpp"
#include "graphics/material.hpp"
#include "graphics/mesh.hpp"
#include "interpolatedTransform.hpp"

class Renderer final : public Component {
public:
    std::shared_ptr<Mesh> mesh;
    std::shared_ptr<Material> material;
    std::shared_ptr<InterpolatedTransform> interpolatedTransform;

    Renderer(const std::shared_ptr<Mesh> &mesh, const std::shared_ptr<Material> &material, float offset = 0.0f,
             glm::vec4 *offsetOrientation = nullptr);

    void initialize() override;

    void update(float alpha) override;

private:
    float offset;
    glm::vec4 *offsetOrientation;
};
