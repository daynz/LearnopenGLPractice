#pragma once

#include "HeadFile.hpp"
#include "ResourceManager.hpp"
#include "SpriteRenderer.hpp"
#include "GameObject.hpp"
#include "GameLevel.hpp"
#include "BallObject.hpp"
#include "ParticleGenerator.hpp"
#include "PostProcessor.hpp"
#include "PowerUp.hpp"

enum GameStats
{
	GAME_ACTIVE,
	GAME_MENU,
	GAME_WIN
};

enum Direction {
	UP,
	RIGHT,
	DOWN,
	LEFT
};

typedef std::tuple<bool, Direction, glm::vec2> Collision;

const glm::vec2 PLAYER_SIZE(200.0f, 20.0f);
const float PLAYER_VELOCITY(500.0f);

const glm::vec2 INITIAL_BALL_VELOCITY(200.0f, -500.0f);
const float BALL_RADIUS = 12.5f;

class Game
{
public:
	Game(int width,int height);
	~Game();
	void init();
	void processInput(float dt);
	void updata(float dt);
	void render();
	void doCollisions();
	bool checkCollision(GameObject& one, GameObject& two);
	Collision checkCollisionCircle(BallObject& one, GameObject& two);
	inline float clamp(float value, float min, float max);
	Direction VectorDirection(glm::vec2 target);
	void spawnPowerUps(GameObject& block);
	void updataPowerUps(float dt);

	GameStats m_state;
	bool m_Keys[1024];
	int m_Width, m_Height;
	std::vector<GameLevel> m_Levels;
	unsigned int m_Level;
	std::vector<PowerUp> PowerUps;
};