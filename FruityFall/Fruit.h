#pragma once
#include "Object.h"

class Fruit : public Object
{
public:
	Fruit() = default;
	Fruit(float posX, float posY);
	~Fruit() override = default;

	void setRandomTexureOfFruit();

	static Texture2D getRandomTextureOfFruit();

private:
};