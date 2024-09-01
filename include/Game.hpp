#pragma once

#include "HeadFile.hpp"
#include "ResourceManager.hpp"
#include "SpriteRenderer.hpp"
#include "GameObject.hpp"
#include "GameLevel.hpp"
#include "BallObject.hpp"

enum GameStats
{
	GAME_ACTIVE,
	GAME_MENU,
	GAME_WIN
};

class Game
{
public:
	Game(int width,int height);
	~Game();
	void init();
	void processInput(float dt);
	void updata(float dt);
	void render();

	GameStats m_state;
	bool m_Keys[1024];
	int m_Width, m_Height;
	std::vector<GameLevel> m_Levels;
	unsigned int m_Level;
};