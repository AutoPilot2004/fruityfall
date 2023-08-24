#pragma once

#include <vector>
#include <memory>

#include "Fruit.h"
#include "Bucket.h"

class Application
{
public:
	void run();

private:
	void onInit();
	void reset();
	void gameOver();

	void gameLoop();

	void onUpdate();

	void processInput();

	void onRender();


private:
	std::unique_ptr<Bucket> m_bucket;

	std::vector<std::unique_ptr<Fruit>> m_fruits;

	uint32_t m_score;
	uint8_t m_lives;

	bool m_gameOver;
};
