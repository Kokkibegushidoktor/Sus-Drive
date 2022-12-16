#pragma once
#include "Entity.h"
class Roadblock :
    public Entity
{
public:
    BoundingBox boundingBox();
    Roadblock(Model model, Vector3 position, Vector3 speed = { 0.0f, 0.0f, 0.5f }, int hitpoints = 100);
};

