#include "game/component.hpp"
#include "game/gameObject.hpp"

void Component::setGameObject(GameObject *obj)
{
    gameObject = obj;
}