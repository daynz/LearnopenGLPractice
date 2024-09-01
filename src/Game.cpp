#include "Game.hpp"

SpriteRenderer* Renderer;
GameObject* Player;
BallObject* Ball;
ParticleGenerator* Particles;

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
}

void Game::init()
{
	ResourceManager::loadShader("../shader/spritev.glsl", "../shader/spritef.glsl", nullptr, "sprite");
	ResourceManager::loadShader("../shader/particlev.glsl", "../shader/particlef.glsl", nullptr, "particle");

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

	Renderer = new SpriteRenderer(ResourceManager::getShader("sprite"));
	Particles = new ParticleGenerator(
		ResourceManager::getShader("particle"),
		ResourceManager::getTexture("particle"),
		500
	);

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
		if (this->m_Keys[GLFW_KEY_R])
		{
			Ball->reset(Player->m_position + glm::vec2(PLAYER_SIZE.x / 2.0f - BALL_RADIUS, -BALL_RADIUS * 2.0f), INITIAL_BALL_VELOCITY);
		}
	}
}

void Game::updata(float dt)
{
	Ball->move(dt, this->m_Width);
	this->doCollisions();
	Particles->updata(dt, *Ball, 2, glm::vec2(Ball->m_Radius / 2.0f));
}

void Game::render()
{
	if (this->m_state == GAME_ACTIVE)
	{
		//Renderer->DrawSprite(ResourceManager::getTexture("face"), glm::vec2(200, 200), glm::vec2(300, 400), 45.0f, glm::vec3(0.0f, 1.0f, 0.0f));
		Renderer->DrawSprite(ResourceManager::getTexture("background"), glm::vec2(0.0f, 0.0f), glm::vec2(this->m_Width, this->m_Height), 0.0f);
		this->m_Levels[this->m_Level].draw(*Renderer);
		Player->Draw(*Renderer);
		Particles->Draw();
		Ball->Draw(*Renderer);
	}
}

void Game::doCollisions()
{
	for (GameObject& box : this->m_Levels[this->m_Level].m_bricks)
	{
		if (!box.m_destroyed)
		{
			////if (checkCollision(*Ball,box))
			//if(checkCollisionCircle(*Ball, box))
			//{
			//	if (!box.m_isSolid)
			//	{
			//		box.m_destroyed = true;
			//	}
			//}
			Collision collision = checkCollisionCircle(*Ball, box);
			if (std::get<0>(collision))
			{
				if (!box.m_isSolid)
				{
					box.m_destroyed = true;
				}
				Direction dir = std::get<1>(collision);
				glm::vec2 diff_vector = std::get<2>(collision);
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
	Collision result = checkCollisionCircle(*Ball, *Player);
	if (!Ball->m_Stuck && std::get<0>(result))
	{
		float centerBoard = Player->m_position.x + Player->m_size.x / 2.0f;
		float distance = (Ball->m_position.x + Ball->m_Radius) - centerBoard;
		float percentage = distance / (Player->m_size.x / 2.0f);
		float strength = 2.0f;
		glm::vec2 oldVelocity = Ball->m_velocity;
		Ball->m_velocity.x = INITIAL_BALL_VELOCITY.x * percentage * strength;
		Ball->m_velocity.y = -Ball->m_velocity.y;
		Ball->m_velocity = glm::normalize(Ball->m_velocity) * glm::length(oldVelocity);
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
	//return (glm::length(difference) < one.m_Radius);
}

bool Game::checkCollision(BallObject& one, GameObject& two)
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
