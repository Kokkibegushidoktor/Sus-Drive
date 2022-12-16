#include "Roadblock.h"

BoundingBox Roadblock::boundingBox()
{
	return BoundingBox{ Vector3{ pos.x - 1.9f, pos.y, pos.z - 1 },
						Vector3{ pos.x + 1.9f, pos.y + 2.0f,pos.z + 1 }
	};
}

Roadblock::Roadblock(Model model, Vector3 position, Vector3 speed, int hitpoints) : Entity(model, position, speed, hitpoints){}
