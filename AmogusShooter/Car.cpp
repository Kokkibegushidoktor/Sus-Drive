#include "Car.h"
#include "raymath.h"

void Car::Update()
{
	static bool moving = false;
	static float oldpos = 0;

	if (moving) {
		if (fabs(pos.x - oldpos) > 4.5) { Stop(); moving = false; }
	}
	else {
		if (IsKeyPressed(KEY_A)&&pos.x>-4) {
			oldpos = pos.x;
			moving = true;
			spd.x = -0.2;
		}
		else if (IsKeyPressed(KEY_D)&&pos.x<4) {
			oldpos = pos.x;
			moving = true;
			spd.x = 0.2;
		}
	}
	Entity::Update();
}

BoundingBox Car::boundingBox()
{
	return BoundingBox{ Vector3{ pos.x - 2, pos.y, pos.z - 3 },
						Vector3{ pos.x + 2, pos.y + 2.5f,pos.z + 3 }
	};
}

Car::Car(Model model, Vector3 position, Vector3 speed, int hitpoints) : Entity(model, position, speed, hitpoints)
{
}
