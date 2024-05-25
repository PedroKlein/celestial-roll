#pragma once

// forward declarations
class Transform;
class Collider;
class PhysicsMaterial;
class GravityComponent;
class RigidBody;
class Renderer;

// order of update is important
enum class ComponentType
{
    Transform,       // dont have update
    Collider,        // dont have update
    PhysicsMaterial, // dont have update
    Gravity,         // have update
    RigidBody,       // have update
    Renderer,        // have update
};

template <typename T> ComponentType getComponentType();