#include "game/componentType.hpp"
#include "bezierAnimation.hpp"
#include "collision/aabbCollider.hpp"
#include "collision/collider.hpp"
#include "collision/obbCollider.hpp"
#include "collision/sphereCollider.hpp"
#include "graphics/lightEmitter.hpp"
#include "graphics/renderer.hpp"
#include "physics/gravityComponent.hpp"
#include "physics/physicsMaterial.hpp"
#include "physics/rigidBody.hpp"
#include "transform.hpp"

template<>
ComponentType getComponentType<Transform>() {
    return ComponentType::Transform;
}

template<>
ComponentType getComponentType<Collider>() {
    return ComponentType::Collider;
}

template<>
ComponentType getComponentType<AABBCollider>() {
    return ComponentType::Collider;
}

template<>
ComponentType getComponentType<OBBCollider>() {
    return ComponentType::Collider;
}

template<>
ComponentType getComponentType<SphereCollider>() {
    return ComponentType::Collider;
}

template<>
ComponentType getComponentType<PhysicsMaterial>() {
    return ComponentType::PhysicsMaterial;
}

template<>
ComponentType getComponentType<GravityComponent>() {
    return ComponentType::Gravity;
}

template<>
ComponentType getComponentType<RigidBody>() {
    return ComponentType::RigidBody;
}

template<>
ComponentType getComponentType<Renderer>() {
    return ComponentType::Renderer;
}

template<>
ComponentType getComponentType<LightEmitter>() {
    return ComponentType::LightEmitter;
}

template<>
ComponentType getComponentType<BezierAnimation>() {
    return ComponentType::Animation;
}
