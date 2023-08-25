#include "Fruit.h"

#include "Random.h"

namespace
{
	constexpr int NUM_OF_FRUIT_TEXTURES = 4;

	const std::string fruitTexture[NUM_OF_FRUIT_TEXTURES] = {
		"assets/images/apple.png",
		"assets/images/lime.png",
		"assets/images/orange.png",
		"assets/images/pear.png",
	};
}

Fruit::Fruit()
{
	setRandomTexureOfFruit();
}

Fruit::Fruit(float posX, float posY)
	: Object(posX, posY)
{
	Fruit();
}

void Fruit::initFruitTextures()
{
	for (int i = 0; i < NUM_OF_FRUIT_TEXTURES; i++) {
		m_objectTextures[fruitTexture[i]] = LoadTexture(fruitTexture[i].c_str());
	}
}

void Fruit::setRandomTexureOfFruit()
{
	setTexture(fruitTexture[Random::uint32(0, NUM_OF_FRUIT_TEXTURES - 1)]);
}

Texture2D Fruit::getRandomTextureOfFruit()
{
	return getFromObjectTextures(fruitTexture[Random::uint32(0, NUM_OF_FRUIT_TEXTURES - 1)]);
}