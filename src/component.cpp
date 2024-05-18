#include "component.hpp"
#include "gameObject.hpp"

void Component::setGameObject(GameObject *obj)
{
    gameObject = obj;
}