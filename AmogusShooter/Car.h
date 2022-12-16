#pragma once
#include "Entity.h"
class Car :
    public Entity
{
public:
    void Update();
    BoundingBox boundingBox();
    Car(Model model, Vector3 position, Vector3 speed = { 0.0f, 0.0f, 0.0f }, int hitpoints = 100);
};

