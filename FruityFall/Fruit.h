#pragma once
#include "Object.h"

#include <unordered_map>

class Fruit : public Object
{
public:
	Fruit();
	Fruit(float posX, float posY);
	~Fruit() override = default;

	static void initFruitTextures();

	void setRandomTexureOfFruit();

	static Texture2D getRandomTextureOfFruit();

private:
};