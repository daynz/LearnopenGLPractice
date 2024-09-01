#pragma once
#include "HeadFile.hpp"
#include "Shader.hpp"
#include "Texture.hpp"
#include "GameObject.hpp"

struct Particle
{
	glm::vec2 m_Position, m_Velocity;
	glm::vec4 m_Color;
	float m_Life;

	Particle() :m_Position(0.0f), m_Velocity(0.0f), m_Color(1.0f), m_Life(0.0f) {}
};

class ParticleGenerator
{
public:
	ParticleGenerator(Shader shader, Texture2D texture, unsigned int amount);
	void updata(float dt, GameObject& object, unsigned int newParticles, glm::vec2 offset = glm::vec2(0.0f, 0.0f));
	void Draw();

private:
	std::vector<Particle> particles;
	unsigned int amount;
	Shader shader;
	Texture2D texture;
	unsigned int VAO;
	void init();
	unsigned int firstUnusedParticle();
	void respawnParticle(Particle& particle, GameObject& object, glm::vec2 offset = glm::vec2(0.0f, 0.0f));
};