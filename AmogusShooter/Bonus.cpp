#include "Bonus.h"
#include <raymath.h>

BoundingBox Bonus::boundingBox()
{
	return BoundingBox{ Vector3{ pos.x - 1.9f, pos.y-2.0f, pos.z - 1 },
						Vector3{ pos.x + 1.9f, pos.y + 2.0f,pos.z + 1 }
	};
}

Bonus::Bonus(Model model, Vector3 position, Vector3 speed, int hitpoints) : Entity(model, position, speed, hitpoints) {}

void Bonus::Update() 
{
	static float rotato = 0;
	static float rotatoSpd = 0.03f;
	if (pos.y>3||pos.y<0) spd.y = -spd.y;
	rotato += rotatoSpd;
	if (rotato > 6.28) rotato = 0;
	mod.transform = MatrixRotateY(rotato);
	Entity::Update();
}
