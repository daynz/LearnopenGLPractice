#include "Game.hpp"

SpriteRenderer* Renderer;

const glm::vec2 PLAYER_SIZE(100.0f, 20.0f);
const float PLAYER_VELOCITY(500.0f);

const glm::vec2 INITIAL_BALL_VELOCITY(100.0f, -350.0f);
const float BALL_RADIUS = 12.5f;

GameObject* Player;
BallObject* Ball;

Game::Game(int width, int height)
	:m_state(GAME_ACTIVE),m_Keys(),m_Width(width),m_Height(height)
{
	
}

Game::~Game()
{
	delete Renderer;
}

void Game::init()
{
	ResourceManager::loadShader("../shader/spritev.glsl", "../shader/spritef.glsl", nullptr, "sprite");
	
	glm::mat4 projection = glm::ortho(0.0f, (float)this->m_Width, (float)this->m_Height, 0.0f, -1.0f, 1.0f);
	ResourceManager::getShader("sprite").use().SetInteger("image", 0);
	ResourceManager::getShader("sprite").SetMatrix4("projection", projection);

	Renderer = new SpriteRenderer(ResourceManager::getShader("sprite"));

	ResourceManager::loadTexture("../assets/texture/background.jpg", false, "background");
	ResourceManager::loadTexture("../assets/texture/awesomeface.png", true, "face");
	ResourceManager::loadTexture("../assets/texture/block.png", false, "block");
	ResourceManager::loadTexture("../assets/texture/block_solid.png", false, "block_solid");
	ResourceManager::loadTexture("../assets/texture/paddle.png", true, "paddle");

	GameLevel one;
	one.load("../level/one.lvl", this->m_Width, this->m_Height / 2);
	GameLevel two;
	two.load("../level/two.lvl", this->m_Width, this->m_Height / 2);
	GameLevel three;
	three.load("../level/three.lvl", this->m_Width, this->m_Height / 2);
	GameLevel four;
	four.load("../level/four.lvl", this->m_Width, this->m_Height / 2);
	this->m_Levels.push_back(one);
	this->m_Levels.push_back(two);
	this->m_Levels.push_back(three);
	this->m_Levels.push_back(four);
	this->m_Level = 0;

	glm::vec2 playerPos = glm::vec2(this->m_Width / 2.0f - PLAYER_SIZE.x / 2.0f, this->m_Height - PLAYER_SIZE.y);
	Player = new GameObject(playerPos, PLAYER_SIZE, ResourceManager::getTexture("paddle"));

	glm::vec2 ballPos = playerPos + glm::vec2(PLAYER_SIZE.x / 2.0f - BALL_RADIUS, -BALL_RADIUS * 2.0f);
	Ball = new BallObject(ballPos, BALL_RADIUS, INITIAL_BALL_VELOCITY, ResourceManager::getTexture("face"));
}

void Game::processInput(float dt)
{
	if (this->m_state == GAME_ACTIVE)
	{
		float velocity = PLAYER_VELOCITY * dt;
		// move playerboard
		if (this->m_Keys[GLFW_KEY_A])
		{
			if (Player->m_position.x >= 0.0f)
			{
				Player->m_position.x -= velocity;
				if (Ball->m_Stuck)
				{
					Ball->m_position.x -= velocity;
				}
			}
		}
		if (this->m_Keys[GLFW_KEY_D])
		{
			if (Player->m_position.x <= this->m_Width - Player->m_size.x)
			{
				Player->m_position.x += velocity;
				if (Ball->m_Stuck)
				{
					Ball->m_position.x += velocity;
				}
			}
		}
		if (this->m_Keys[GLFW_KEY_SPACE])
		{
			Ball->m_Stuck = false;
		}
	}
}

void Game::updata(float dt)
{
	Ball->move(dt, this->m_Width);
}

void Game::render()
{
	if (this->m_state == GAME_ACTIVE)
	{
		//Renderer->DrawSprite(ResourceManager::getTexture("face"), glm::vec2(200, 200), glm::vec2(300, 400), 45.0f, glm::vec3(0.0f, 1.0f, 0.0f));
		Renderer->DrawSprite(ResourceManager::getTexture("background"), glm::vec2(0.0f, 0.0f), glm::vec2(this->m_Width, this->m_Height), 0.0f);
		this->m_Levels[this->m_Level].draw(*Renderer);
		Player->Draw(*Renderer);
		Ball->Draw(*Renderer);
	}
}
