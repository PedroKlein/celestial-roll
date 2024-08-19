#include "graphics/renderer.hpp"
#include "game/gameObject.hpp"

Renderer::Renderer(const std::shared_ptr<Mesh> &mesh, const std::shared_ptr<Material> &material, float offset,
                   glm::vec4 *offsetOrientation) :
    mesh(mesh), material(material), offset(offset), offsetOrientation(offsetOrientation) {}

void Renderer::initialize() {
    const auto transform = gameObject->getComponent<Transform>();
    if (!transform) {
        std::cerr << "Renderer requires a Transform component." << std::endl;
    }

    interpolatedTransform = std::make_shared<InterpolatedTransform>(transform.get());
}

void Renderer::update(const float alpha) {
    if (!mesh || !interpolatedTransform) {
        std::cerr << "Renderer component is not properly initialized." << std::endl;
        return;
    }

    interpolatedTransform->calculateInterpolation(alpha);

    if (offsetOrientation) {
        interpolatedTransform->interpolatedPosition += offset * -glm::vec3(*offsetOrientation);
    }

    material->shader->setMat4("model", interpolatedTransform->getInterpolatedModelMatrix());
    applyCustomProperties();
    mesh->draw(*material);
}

void Renderer::applyCustomProperties() {
    for (const auto &[name, value]: customProperties.floatProperties) {
        material->shader->setFloat(name, value);
    }

    for (const auto &[name, value]: customProperties.vec3Properties) {
        material->shader->setVec3(name, value);
    }
}

void Renderer::setCustomFloatProperty(const std::string &name, float value) {
    customProperties.floatProperties[name] = value;
}

void Renderer::setCustomVec3Property(const std::string &name, const glm::vec3 &value) {
    customProperties.vec3Properties[name] = value;
}
