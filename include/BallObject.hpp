#pragma once
#include "GameObject.hpp"
#include "HeadFile.hpp"
#include "Texture.hpp"
class BallObject : public GameObject
{
public:
	BallObject();
	BallObject(glm::vec2 pos, float radius, glm::vec2 velocity, Texture2D sprite);
	~BallObject();
	
	float m_Radius;
	bool m_Stuck;
	bool m_Sticky, m_PassThrough;

	glm::vec2 move(float dt, unsigned int window_width);
	void reset(glm::vec2 position, glm::vec2 velocity);

};
