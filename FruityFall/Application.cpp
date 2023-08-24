#include "Application.h"

#include "Random.h"

#include <string>
#include <stdint.h>
#include <raylib.h>

static constexpr uint32_t WINDOW_WIDTH = 1000;
static constexpr uint32_t WINDOW_HEIGHT = 1000;

static const char* WINDOW_TITLE = "Fruity Fall 0.1";
static const char* RESTART_MESSAGE = "Press any key to restart";

static constexpr int FONT_SIZE_RESTART_MESSAGE = 35;

static constexpr int TARGET_FPS = 60;

static float spawnSpeedFactor;
static float spawnSpeed;
static float fallSpeed;

void Application::run()
{
	onInit();
	gameLoop();
}

void Application::onInit()
{
	InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE);
	HideCursor();

	SetTargetFPS(TARGET_FPS);

	reset();

	m_bucket = std::make_unique<Bucket>();
	m_bucket->setScale(0.2f);
	m_bucket->setX((WINDOW_WIDTH / 2.0f) - (m_bucket->getTex().width * m_bucket->getScale() / 2.0f));
	m_bucket->setY(WINDOW_HEIGHT - (m_bucket->getTex().height * m_bucket->getScale()));
}

void Application::reset()
{
	m_fruits.clear();
	m_score = 0;
	m_lives = 3;
	m_gameOver = false;

	spawnSpeedFactor = 4.0f;
	spawnSpeed = (16.0f / spawnSpeedFactor) * TARGET_FPS;
	fallSpeed = 3.0f;
}

void Application::gameOver()
{
	if (GetKeyPressed()) {
		m_gameOver = false;
		reset();
	}
}

void Application::gameLoop()
{
	while (!WindowShouldClose()) {
		if (!m_gameOver)
			onUpdate();
		else
			gameOver();
		onRender();
	}
}

void Application::onUpdate()
{
	static int frameCount = 0;

	if (frameCount >= spawnSpeed) {
		m_fruits.push_back(std::make_unique<Fruit>(Random::uint32(0, WINDOW_WIDTH - 100), -100.0f));
		frameCount = 0;
	}

	processInput();

	for (int i = 0; i < m_fruits.size(); i++) {
		m_fruits[i]->offsetY(fallSpeed);

		if (m_fruits[i]->getY() > m_bucket->getY()) {
			static auto destroyFruit = [&]()
				{
					m_fruits[i] = std::move(m_fruits.back());
					m_fruits.pop_back();
					i--;
				};

			if (((m_fruits[i]->getX() + (m_fruits[i]->getTex().width * m_fruits[i]->getScale() / 2.0f) > m_bucket->getX() &&
				m_fruits[i]->getX() + (m_fruits[i]->getTex().width * m_fruits[i]->getScale() / 2.0f) < m_bucket->getX() + (m_bucket->getTex().width * m_bucket->getScale())))) {
				destroyFruit();

				m_score++;
			}
			else if (m_fruits[i]->getY() >= WINDOW_HEIGHT) {
				destroyFruit();

				m_lives--;
			}
		}
	}

	if (m_lives <= 0) {
		m_gameOver = true;
	}

	frameCount++;
}

void Application::processInput()
{
	m_bucket->setX(GetMousePosition().x - (m_bucket->getTex().width * m_bucket->getScale() / 2.0f));
}

void Application::onRender()
{
	BeginDrawing();
	ClearBackground({ 230, 145, 235, 255 });

	DrawText(std::string("Score: " + std::to_string(m_score)).c_str(), 0, 0, 30, {255, 255, 255, 255});
	DrawText(std::string("Lives: " + std::to_string(m_lives)).c_str(), 0, 30, 30, {255, 255, 255, 255});

	for (const auto& fruit : m_fruits) {
		fruit->onRender();
	}

	m_bucket->onRender();

	if (m_gameOver) {
		DrawRectangle(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, Fade(WHITE, 0.7f));

		DrawText(RESTART_MESSAGE, WINDOW_WIDTH / 2.0f - (MeasureText(RESTART_MESSAGE, FONT_SIZE_RESTART_MESSAGE) / 2.0), WINDOW_HEIGHT / 2.0f - (FONT_SIZE_RESTART_MESSAGE / 2.0f), FONT_SIZE_RESTART_MESSAGE, {130, 35, 230, 255});
		DrawText("[ESC] to exit", WINDOW_WIDTH / 2.0f - (MeasureText("[ESC] to exit", FONT_SIZE_RESTART_MESSAGE - 5) / 2.0), (WINDOW_HEIGHT / 2.0f + 25), FONT_SIZE_RESTART_MESSAGE - 5, {130, 35, 230, 255});
	}

	EndDrawing();
}