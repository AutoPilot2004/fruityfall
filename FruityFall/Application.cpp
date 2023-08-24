#include "Application.h"

#include "Random.h"

#include <string>
#include <stdint.h>
#include <raylib.h>

static constexpr uint32_t WINDOW_WIDTH = 1000;
static constexpr uint32_t WINDOW_HEIGHT = 1000;

static constexpr Color TEXT_COLOR = { 130, 30, 250, 255 };

static const char* WINDOW_TITLE = "Fruity Fall 0.1";
static const char* GAME_TITLE = "FRUITY FALL";
static const char* START_MESSAGE = "Press any key to start";
static const char* RESTART_MESSAGE = "Press any key to restart";
static const char* EXIT_MESSAGE = "Press [ESC] to exit";

static constexpr int FONT_SIZE_GAME_TITLE = 40;
static constexpr int FONT_SIZE_START_RESTART_MESSAGE = FONT_SIZE_GAME_TITLE - 5;
static constexpr int FONT_SIZE_EXIT_MESSAGE = FONT_SIZE_START_RESTART_MESSAGE - 5;

static constexpr int TARGET_FPS = 60;

static int frameCount;

static float spawnSpeedFactor;
static float spawnSpeed;
static float fallSpeed;
static float rotationSpeed;

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
	m_bucket->setX((WINDOW_WIDTH / 2.0f));
	m_bucket->setY(WINDOW_HEIGHT - (m_bucket->getHeight() / 2.0f));

	Fruit::initFruitTextures();
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
	rotationSpeed = 3.0f;
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

void Application::onRender()
{
	BeginDrawing();
	ClearBackground({ 230, 145, 235, 255 });

	if (m_gameState != GameState::MENU) {
		DrawText(std::string("Score: " + std::to_string(m_score)).c_str(), 0, 0, 30, TEXT_COLOR);
		DrawText(std::string("Lives: " + std::to_string(m_lives)).c_str(), 0, 30, 30, TEXT_COLOR);

		for (const auto& fruit : m_fruits) {
			fruit->onRender();
		}

		m_bucket->onRender();

		if (m_gameState == GameState::GAME_OVER) {
			DrawRectangle(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, Fade(WHITE, 0.7f));

			DrawText(GAME_TITLE, WINDOW_WIDTH / 2.0f - (MeasureText(GAME_TITLE, FONT_SIZE_GAME_TITLE) / 2.0), WINDOW_HEIGHT / 2.0f - 75, FONT_SIZE_GAME_TITLE, TEXT_COLOR);
			DrawText(RESTART_MESSAGE, WINDOW_WIDTH / 2.0f - (MeasureText(RESTART_MESSAGE, FONT_SIZE_START_RESTART_MESSAGE) / 2.0), WINDOW_HEIGHT / 2.0f - (FONT_SIZE_START_RESTART_MESSAGE / 2.0f), FONT_SIZE_START_RESTART_MESSAGE, TEXT_COLOR);
			DrawText(EXIT_MESSAGE, WINDOW_WIDTH / 2.0f - (MeasureText(EXIT_MESSAGE, FONT_SIZE_EXIT_MESSAGE) / 2.0), (WINDOW_HEIGHT / 2.0f + 25), FONT_SIZE_EXIT_MESSAGE, TEXT_COLOR);
		}
	}
	else {
		DrawRectangle(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, Fade(WHITE, 0.7f));

		DrawText(GAME_TITLE, WINDOW_WIDTH / 2.0f - (MeasureText(GAME_TITLE, FONT_SIZE_GAME_TITLE) / 2.0), WINDOW_HEIGHT / 2.0f - 75, FONT_SIZE_GAME_TITLE, TEXT_COLOR);
		DrawText(START_MESSAGE, WINDOW_WIDTH / 2.0f - (MeasureText(START_MESSAGE, FONT_SIZE_START_RESTART_MESSAGE) / 2.0), WINDOW_HEIGHT / 2.0f - (FONT_SIZE_START_RESTART_MESSAGE / 2.0f), FONT_SIZE_START_RESTART_MESSAGE, TEXT_COLOR);
		DrawText(EXIT_MESSAGE, WINDOW_WIDTH / 2.0f - (MeasureText(EXIT_MESSAGE, FONT_SIZE_EXIT_MESSAGE) / 2.0), (WINDOW_HEIGHT / 2.0f + 25), FONT_SIZE_EXIT_MESSAGE, TEXT_COLOR);
	}

	EndDrawing();
}