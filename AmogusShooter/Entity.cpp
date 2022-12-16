#include "Entity.h"
Entity::Entity(Model model, Vector3 spos, Vector3 speed = { 0.0f,0.0f,0.0f }, int hitpoints = 100) : pos(spos), mod(model), spd(speed), hp(hitpoints) {}
Vector3 Entity::position() { return pos; }
void Entity::position(Vector3 newPos) { pos = newPos; }
void Entity::offsetPosition(Vector3 offset) { pos.x += offset.x; pos.y += offset.y; pos.z += offset.z; }
void Entity::decSpeed(Vector3 offset) { spd.x -= offset.x; spd.y -= offset.y; spd.z -= offset.z; }
void Entity::incSpeed(Vector3 offset) { spd.x += offset.x; spd.y += offset.y; spd.z += offset.z; }
void Entity::Stop() { spd = { 0.0f,0.0f,0.0f }; }
void Entity::Update() { pos.x += spd.x; pos.y += spd.y; pos.z += spd.z; }
void Entity::Draw() 
{ 
	DrawModel(mod, pos, scale, tint); 
	//DrawBoundingBox(boundingBox(),PURPLE); 
}
void Entity::shader(Shader shader)
{
	for (int i = 0; i < mod.materialCount; i++)
		mod.materials[i].shader = shader;
}

int Entity::hitpoints()
{
	return hp;
}

BoundingBox Entity::boundingBox()
{
	return BoundingBox{ Vector3{ pos.x - 2, pos.y - 2,pos.z - 2 },
						Vector3{ pos.x + 2, pos.y + 2,pos.z + 2 }
	};
}

void Entity::damage(int hp) {
	this->hp -= hp;
}

void Entity::hitpoints(int hp)
{
	this->hp = hp;
}
