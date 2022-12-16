#pragma once
#include "Entity.h"
class Bonus :
    public Entity
{
public:
    void Update();
    BoundingBox boundingBox();
    Bonus(Model model, Vector3 position, Vector3 speed = { 0.0f, 0.05f, 0.5f }, int hitpoints = 100);
};

