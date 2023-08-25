#include "Application.h"

#include "Random.h"

#include <string>
#include <stdint.h>
#include <raylib.h>

namespace
{
	constexpr uint32_t WINDOW_WIDTH = 1000;
	constexpr uint32_t WINDOW_HEIGHT = 1000;

	constexpr int TARGET_FPS = 60;

	const char* WINDOW_TITLE = "Fruity Fall 0.1";
	const char* GAME_TITLE = "FRUITY FALL";
	const char* START_MESSAGE = "Press any key to start";
	const char* RESTART_MESSAGE = "Press any key to restart";
	const char* EXIT_MESSAGE = "Press [ESC] to exit";

	constexpr int FONT_SIZE_GAME_TITLE = 40;
	constexpr int FONT_SIZE_START_RESTART_MESSAGE = FONT_SIZE_GAME_TITLE - 5;
	constexpr int FONT_SIZE_EXIT_MESSAGE = FONT_SIZE_START_RESTART_MESSAGE - 5;

	constexpr Color TEXT_COLOR = { 130, 30, 250, 255 };

	int frameCount;

	float spawnSpeedFactor;
	float spawnSpeed;
	float fallSpeed;
}

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

	Fruit::initFruitTextures();

	m_bucket = std::make_unique<Bucket>();
	m_bucket->setScale(0.2f);
	m_bucket->setX((WINDOW_WIDTH / 2.0f));
	m_bucket->setY(WINDOW_HEIGHT - (m_bucket->getHeight() / 2.0f));
}

void Application::reset()
{
	m_fruits.clear();
	m_score = 0;
	m_lives = 3;

	frameCount = 0;

	spawnSpeedFactor = 1.0f;
	spawnSpeed = (1.0f / spawnSpeedFactor) * TARGET_FPS;
	fallSpeed = 6.0f;
}

void Application::gameLoop()
{
	while (!WindowShouldClose()) {
		onUpdate();
		onRender();
	}
}

void Application::onUpdate()
{
	processInput();

	if (m_gameState == GameState::PLAYING) {
		if (frameCount >= spawnSpeed) {
			m_fruits.push_back(std::make_unique<Fruit>());
			const auto& fruit = m_fruits.back();
			fruit->setX(Random::uint32(fruit->getWidth() / 2.0f, WINDOW_WIDTH - (fruit->getWidth() / 2.0f)));
			fruit->setY(-fruit->getHeight() / 2.0f);
			frameCount = 0;
		}


		for (int i = 0; i < m_fruits.size(); i++) {
			m_fruits[i]->offsetY(fallSpeed);

			if (m_fruits[i]->getY() > m_bucket->getY() - (m_bucket->getHeight() / 3.0f)) {
				static auto destroyFruit = [&]()
					{
						m_fruits[i] = std::move(m_fruits.back());
						m_fruits.pop_back();
						i--;
					};

				if (m_fruits[i]->getX() > m_bucket->getX() - (m_bucket->getWidth() / 2.0f) &&
					m_fruits[i]->getX() < m_bucket->getX() + (m_bucket->getWidth() / 2.0f) &&
					m_fruits[i]->getY() < m_bucket->getY() - (m_bucket->getHeight() / 4.0f)) {
					destroyFruit();

					m_score++;
				}
				else if (m_fruits[i]->getY() >= WINDOW_HEIGHT + m_fruits[i]->getHeight() / 2.0f) {
					destroyFruit();

					m_lives--;
				}
			}
		}

		if (m_lives <= 0) {
			m_gameState = GameState::GAME_OVER;
		}

		frameCount++;
	}
}

void Application::processInput()
{
	if (m_gameState == GameState::PLAYING)
		m_bucket->setX(GetMousePosition().x);
	else
		if (GetKeyPressed()) {
			reset();
			m_gameState = GameState::PLAYING;
		}
}

static void drawCenteredText(const std::string& text, int fontSize, int offsetX, int offsetY)
{
	DrawText(text.c_str(), ((WINDOW_WIDTH / 2.0f) - (MeasureText(text.c_str(), fontSize) / 2.0f)) + offsetX, ((WINDOW_HEIGHT / 2.0f) - (fontSize / 2.0f)) + offsetY, fontSize, TEXT_COLOR);
}

static void drawCenteredText(const char* text, int fontSize, int offsetX, int offsetY)
{
	DrawText(text, ((WINDOW_WIDTH / 2.0f) - (MeasureText(text, fontSize) / 2.0f)) + offsetX, ((WINDOW_HEIGHT / 2.0f) - (fontSize / 2.0f)) + offsetY, fontSize, TEXT_COLOR);
}

static void drawText(const std::string& text, int posX, int posY, int fontSize)
{
	DrawText(text.c_str(), posX, posY, fontSize, TEXT_COLOR);
}

static void fadeBackground(Color color, float alpha)
{
	DrawRectangle(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, Fade(color, alpha));
}

static void drawStartMessage(bool restart = false)
{
	fadeBackground(WHITE, 0.7f);

	drawCenteredText(GAME_TITLE, FONT_SIZE_GAME_TITLE, 0, -75);
	drawCenteredText(restart ? RESTART_MESSAGE : START_MESSAGE, FONT_SIZE_START_RESTART_MESSAGE, 0, 0);
	drawCenteredText(EXIT_MESSAGE, FONT_SIZE_EXIT_MESSAGE, 0, 40);
}

void Application::onRender()
{
	BeginDrawing();
	ClearBackground({ 230, 145, 235, 255 });

	if (m_gameState != GameState::MENU) {
		drawText("Score: " + std::to_string(m_score), 0, 0, 30);
		drawText("Lives: " + std::to_string(m_lives), 0, 30, 30);

		for (const auto& fruit : m_fruits) {
			fruit->onRender();
		}

		m_bucket->onRender();

		if (m_gameState == GameState::GAME_OVER) {
			drawStartMessage(true);
		}
	}
	else {
		drawStartMessage();
	}

	EndDrawing();
}