#pragma once
#include <raylib.h>
class Entity
{
protected:
	Vector3 spd;
	int hp = 100;
	Model mod;
	Vector3 pos;
public:
	Entity(Model model, Vector3 position, Vector3 speed, int hitpoints);
	virtual void Update();
	virtual void Draw();
	virtual void offsetPosition(Vector3);
	Vector3 position();
	virtual void position(Vector3);
	void decSpeed(Vector3);
	void incSpeed(Vector3);
	void Stop();
	Color tint = WHITE;
	float scale = 1;
	void shader(Shader);
	int hitpoints();
	void hitpoints(int);
	void damage(int);
	virtual BoundingBox boundingBox();
};

