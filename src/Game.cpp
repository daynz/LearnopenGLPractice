#include "Game.hpp"

SpriteRenderer* Renderer;

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
	ResourceManager::loadTexture("../assets/texture/awesomeface.png", true, "face");

	Renderer = new SpriteRenderer(ResourceManager::getShader("sprite"));
}

void Game::processInput(float dt)
{
}

void Game::updata(float dt)
{
}

void Game::render()
{
	Renderer->DrawSprite(ResourceManager::getTexture("face"), glm::vec2(200, 200), glm::vec2(300, 400), 45.0f, glm::vec3(0.0f, 1.0f, 0.0f));
}
