#pragma once

#include "HeadFile.hpp"
#include "ResourceManager.hpp"

enum GameStats
{
	GAME_ACTIVE,
	GAME_MENU,
	GAME_WIN
};

class Game
{
public:
	Game(GLuint width,GLuint height);
	~Game();
	void init();
	void processInput(float dt);
	void updata(float dt);
	void render();

	GameStats m_state;
	GLboolean m_Keys[1024];
	GLuint m_Width, m_Height;
};