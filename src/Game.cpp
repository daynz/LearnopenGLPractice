#include "Game.hpp"
#include <irrKlang.h>

SpriteRenderer* Renderer;
GameObject* Player;
BallObject* Ball;
ParticleGenerator* Particles;
PostProcessor* Effects;
irrklang::ISoundEngine* SoundEngine = irrklang::createIrrKlangDevice();

float ShakeTime = 0.0f;
bool isConfused = false;
bool isChao = false;

Game::Game(int width, int height)
	:m_state(GAME_ACTIVE),m_Keys(),m_Width(width),m_Height(height)
{
	
}

Game::~Game()
{
	delete Renderer;
	delete Player;
	delete Ball;
	delete Particles;
	delete Effects;
}

void Game::init()
{
	ResourceManager::loadShader("../shader/spritev.glsl", "../shader/spritef.glsl", nullptr, "sprite");
	ResourceManager::loadShader("../shader/particlev.glsl", "../shader/particlef.glsl", nullptr, "particle");
	ResourceManager::loadShader("../shader/postprocessingv.glsl", "../shader/postprocessingf.glsl", nullptr, "postprocessing");

	glm::mat4 projection = glm::ortho(0.0f, (float)this->m_Width, (float)this->m_Height, 0.0f, -1.0f, 1.0f);
	ResourceManager::getShader("sprite").use().SetInteger("image", 0);
	ResourceManager::getShader("sprite").SetMatrix4("projection", projection);
	ResourceManager::getShader("particle").use().SetInteger("sprite", 0);
	ResourceManager::getShader("particle").SetMatrix4("projection", projection);

	ResourceManager::loadTexture("../assets/texture/background.jpg", false, "background");
	ResourceManager::loadTexture("../assets/texture/awesomeface.png", true, "face");
	ResourceManager::loadTexture("../assets/texture/block.png", false, "block");
	ResourceManager::loadTexture("../assets/texture/block_solid.png", false, "block_solid");
	ResourceManager::loadTexture("../assets/texture/paddle.png", true, "paddle");
	ResourceManager::loadTexture("../assets/texture/particle.png", true, "particle");
	ResourceManager::loadTexture("../assets/texture/powerup_speed.png", true, "powerup_speed");
	ResourceManager::loadTexture("../assets/texture/powerup_sticky.png", true, "powerup_sticky");
	ResourceManager::loadTexture("../assets/texture/powerup_increase.png", true, "powerup_increase");
	ResourceManager::loadTexture("../assets/texture/powerup_confuse.png", true, "powerup_confuse");
	ResourceManager::loadTexture("../assets/texture/powerup_chaos.png", true, "powerup_chaos");
	ResourceManager::loadTexture("../assets/texture/powerup_passthrough.png", true, "powerup_passthrough");

	Renderer = new SpriteRenderer(ResourceManager::getShader("sprite"));
	Particles = new ParticleGenerator(ResourceManager::getShader("particle"), ResourceManager::getTexture("particle"), 500);
	Effects = new PostProcessor(ResourceManager::getShader("postprocessing"), this->m_Width, this->m_Height);

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

	SoundEngine->play2D("../assets/audio/breakout.mp3", true);

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
		if (this->m_Keys[GLFW_KEY_R])
		{
			Ball->reset(Player->m_position + glm::vec2(PLAYER_SIZE.x / 2.0f - BALL_RADIUS, -BALL_RADIUS * 2.0f), INITIAL_BALL_VELOCITY);
		}
		if (this->m_Keys[GLFW_KEY_1])
		{
			if (isConfused)
			{
				Effects->Confuse = true;
				isConfused = false;
			}
			else
			{
				Effects->Confuse = false;
				isConfused = true;
			}
		}
		
		if (this->m_Keys[GLFW_KEY_2])
		{
			if (isChao)
			{
				Effects->Chaos = true;
				isChao = false;
			}
			else
			{
				Effects->Chaos = false;
				isChao = true;
			}
		}
	}
}

void Game::updata(float dt)
{
	Ball->move(dt, this->m_Width);
	this->doCollisions();
	Particles->updata(dt, *Ball, 2, glm::vec2(Ball->m_Radius / 2.0f));
	this->updataPowerUps(dt);

	if (ShakeTime > 0.0f)
	{
		ShakeTime -= dt;
		if (ShakeTime <= 0.0f)
		{
			Effects->Shake = false;
		}
	}
}

void Game::render()
{
	if (this->m_state == GAME_ACTIVE)
	{
		Effects->BeginRender();
		//Renderer->DrawSprite(ResourceManager::getTexture("face"), glm::vec2(200, 200), glm::vec2(300, 400), 45.0f, glm::vec3(0.0f, 1.0f, 0.0f));
		Renderer->DrawSprite(ResourceManager::getTexture("background"), glm::vec2(0.0f, 0.0f), glm::vec2(this->m_Width, this->m_Height), 0.0f);
		this->m_Levels[this->m_Level].draw(*Renderer);
		Player->Draw(*Renderer);
		Particles->Draw();
		Ball->Draw(*Renderer);
		for (PowerUp& powerUp : this->PowerUps)
		{
			if (!powerUp.m_destroyed)
			{
				powerUp.Draw(*Renderer);
			}
		}

		Effects->EndRender();
		Effects->Render(glfwGetTime());
	}
}

void ActivatePowerUp(PowerUp& powerUp)
{
	if (powerUp.Type == "speed")
	{
		Ball->m_velocity *= 1.2;
	}
	else if (powerUp.Type == "sticky")
	{
		Ball->m_Sticky = true;
		Player->m_color = glm::vec3(1.0f, 0.5f, 1.0f);
	}
	else if (powerUp.Type == "pass-through")
	{
		Ball->m_PassThrough = true;
		Ball->m_color = glm::vec3(1.0f, 0.5f, 0.5f);
	}
	else if (powerUp.Type == "pad-size-increase")
	{
		Player->m_size.x += 50;
	}
	else if (powerUp.Type == "confuse")
	{
		if (!Effects->Chaos)
			Effects->Confuse = true; // only activate if chaos wasn't already active
	}
	else if (powerUp.Type == "chaos")
	{
		if (!Effects->Confuse)
			Effects->Chaos = true;
	}
}


void Game::doCollisions()
{
	for (GameObject& box : this->m_Levels[this->m_Level].m_bricks)
	{
		if (!box.m_destroyed)
		{
			Collision collision = checkCollisionCircle(*Ball, box);
			if (std::get<0>(collision))
			{
				if (!box.m_isSolid)
				{
					box.m_destroyed = true;
					ShakeTime = 0.2f;
					Effects->Shake = true;
					this->spawnPowerUps(box);
					SoundEngine->play2D("../assets/audio/bleep.mp3", false);
				}
				else
				{
					ShakeTime = 0.05f;
					Effects->Shake = true;
					SoundEngine->play2D("../assets/audio/solid.wav", false);
				}
				Direction dir = std::get<1>(collision);
				glm::vec2 diff_vector = std::get<2>(collision);
				if (!(Ball->m_PassThrough && !box.m_isSolid))
				{
					if (dir == LEFT || dir == RIGHT)
					{
						Ball->m_velocity.x = -Ball->m_velocity.x;
						float penetration = -Ball->m_Radius - std::abs(diff_vector.x);
						if (dir == LEFT)
						{
							Ball->m_position.x += penetration;
						}
						else
						{
							Ball->m_position.x -= penetration;
						}
					}
					else
					{
						Ball->m_velocity.y = -Ball->m_velocity.y;
						float penetration = Ball->m_Radius - std::abs(diff_vector.y);
						if (dir == UP)
						{
							Ball->m_position -= penetration;
						}
						else
						{
							Ball->m_position += penetration;
						}
					}
				}
			}
		}
	}
	for (PowerUp& powerUp : this->PowerUps)
	{
		if (!powerUp.m_destroyed)
		{
			if (powerUp.m_position.y >= this->m_Height)
				powerUp.m_destroyed = true;

			if (checkCollision(*Player, powerUp))
			{	
				ActivatePowerUp(powerUp);
				powerUp.m_destroyed = true;
				powerUp.Activated = true;
				SoundEngine->play2D("../assets/audio/powerup.wav", false);
			}
		}
	}

	Collision result = checkCollisionCircle(*Ball, *Player);
	if (!Ball->m_Stuck && std::get<0>(result))
	{
		float centerBoard = Player->m_position.x + Player->m_size.x / 2.0f;
		float distance = (Ball->m_position.x + Ball->m_Radius) - centerBoard;
		float percentage = distance / (Player->m_size.x / 2.0f);
		float strength = 2.0f;
		glm::vec2 oldVelocity = Ball->m_velocity;
		Ball->m_velocity.x = INITIAL_BALL_VELOCITY.x * percentage * strength;
		//Ball->m_velocity.y = -Ball->m_velocity.y;
		Ball->m_velocity = glm::normalize(Ball->m_velocity) * glm::length(oldVelocity);
		Ball->m_velocity.y = -1.0f * abs(Ball->m_velocity.y);
		Ball->m_Stuck = Ball->m_Sticky;

		SoundEngine->play2D("../assets/audio/bleep.wav");
	}
}

Collision Game::checkCollisionCircle(BallObject& one, GameObject& two)
{
	glm::vec2 center(one.m_position + one.m_Radius);
	glm::vec2 aabb_half_extents(two.m_size.x / 2, two.m_size.y / 2);
	glm::vec2 aabb_center(
		two.m_position.x + aabb_half_extents.x,
		two.m_position.y + aabb_half_extents.y
	);
	glm::vec2 difference = center - aabb_center;
	glm::vec2 clamped = glm::clamp(difference, -aabb_half_extents, aabb_half_extents);
	glm::vec2 closest = aabb_center + clamped;
	difference = closest - center;

	if (glm::length(difference) <= one.m_Radius)
	{
		return std::make_tuple(true, VectorDirection(difference), difference);
	}
	else
	{
		return std::make_tuple(false, UP, glm::vec2(0.0f, 0.0f));
	}
}

bool Game::checkCollision(GameObject& one, GameObject& two)
{
	bool collisionX = one.m_position.x + one.m_size.x >= two.m_position.x &&
		two.m_position.x + two.m_size.x >= one.m_position.x;
	bool collisionY = one.m_position.y + one.m_size.y >= two.m_position.y &&
		two.m_position.y + two.m_size.y >= one.m_position.y;
	return collisionX && collisionY;
}

float Game::clamp(float value, float min, float max)
{
	return std::max(min, std::min(max, value));
}

Direction Game::VectorDirection(glm::vec2 target)
{
	glm::vec2 compass[] = {
		glm::vec2(0.0f, 1.0f),	// up
		glm::vec2(1.0f, 0.0f),	// right
		glm::vec2(0.0f, -1.0f),	// down
		glm::vec2(-1.0f, 0.0f)	// left
	};
	float max = 0.0f;
	unsigned int best_match = -1;
	for (unsigned int i = 0; i < 4; i++)
	{
		float dot_product = glm::dot(glm::normalize(target), compass[i]);
		if (dot_product > max)
		{
			max = dot_product;
			best_match = i;
		}
	}
	return (Direction)best_match;
}

bool ShouldSpawn(unsigned int chance)
{
	unsigned int random = rand() % chance;
	return random == 0;
}

void Game::spawnPowerUps(GameObject& block)
{
	if (ShouldSpawn(75))
		this->PowerUps.push_back(PowerUp("speed", glm::vec3(0.5f, 0.5f, 1.0f), 0.0f, block.m_position, ResourceManager::getTexture("powerup_speed")));
	if (ShouldSpawn(75))
		this->PowerUps.push_back(PowerUp("sticky", glm::vec3(1.0f, 0.5f, 1.0f), 20.0f, block.m_position, ResourceManager::getTexture("powerup_sticky")));
	if (ShouldSpawn(75))
		this->PowerUps.push_back(PowerUp("pass-through", glm::vec3(0.5f, 1.0f, 0.5f), 10.0f, block.m_position, ResourceManager::getTexture("powerup_passthrough")));
	if (ShouldSpawn(75))
		this->PowerUps.push_back(PowerUp("pad-size-increase", glm::vec3(1.0f, 0.6f, 0.4), 0.0f, block.m_position, ResourceManager::getTexture("powerup_increase")));
	if (ShouldSpawn(15))
		this->PowerUps.push_back(PowerUp("confuse", glm::vec3(1.0f, 0.3f, 0.3f), 15.0f, block.m_position, ResourceManager::getTexture("powerup_confuse")));
	if (ShouldSpawn(15))
		this->PowerUps.push_back(PowerUp("chaos", glm::vec3(0.9f, 0.25f, 0.25f), 15.0f, block.m_position, ResourceManager::getTexture("powerup_chaos")));
}

bool IsOtherPowerUpActive(std::vector<PowerUp>& powerUps, std::string type)
{
	// Check if another PowerUp of the same type is still active
	// in which case we don't disable its effect (yet)
	for (const PowerUp& powerUp : powerUps)
	{
		if (powerUp.Activated)
			if (powerUp.Type == type)
				return true;
	}
	return false;
}

void Game::updataPowerUps(float dt)
{
	for (PowerUp& powerUp : this->PowerUps)
	{
		powerUp.m_position += powerUp.m_velocity * dt;
		if (powerUp.Activated)
		{
			powerUp.Duration -= dt;

			if (powerUp.Duration <= 0.0f)
			{
				powerUp.Activated = false;
				// deactivate effects
				if (powerUp.Type == "sticky")
				{
					if (!IsOtherPowerUpActive(this->PowerUps, "sticky"))
					{	// only reset if no other PowerUp of type sticky is active
						Ball->m_Sticky = false;
						Player->m_color = glm::vec3(1.0f);
					}
				}
				else if (powerUp.Type == "pass-through")
				{
					if (!IsOtherPowerUpActive(this->PowerUps, "pass-through"))
					{	// only reset if no other PowerUp of type pass-through is active
						Ball->m_PassThrough = false;
						Ball->m_color = glm::vec3(1.0f);
					}
				}
				else if (powerUp.Type == "confuse")
				{
					if (!IsOtherPowerUpActive(this->PowerUps, "confuse"))
					{	// only reset if no other PowerUp of type confuse is active
						Effects->Confuse = false;
					}
				}
				else if (powerUp.Type == "chaos")
				{
					if (!IsOtherPowerUpActive(this->PowerUps, "chaos"))
					{	// only reset if no other PowerUp of type chaos is active
						Effects->Chaos = false;
					}
				}
			}
		}
	}
	// Remove all PowerUps from vector that are destroyed AND !activated (thus either off the map or finished)
	// Note we use a lambda expression to remove each PowerUp which is destroyed and not activated
	this->PowerUps.erase(std::remove_if(this->PowerUps.begin(), this->PowerUps.end(),
		[](const PowerUp& powerUp) { return powerUp.m_destroyed && !powerUp.Activated; }
	), this->PowerUps.end());
}
