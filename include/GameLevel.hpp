#pragma once
#include "HeadFile.hpp"
#include "SpriteRenderer.hpp"
#include "GameObject.hpp"
#include "ResourceManager.hpp"

class GameLevel
{
public:
	std::vector<GameObject> m_bricks;

	GameLevel() {}
	void load(const char* file, unsigned int levelWidth, unsigned int levelHeight);
	void draw(SpriteRenderer& renderer);
	bool isCompleted();

private:
	void init(std::vector<std::vector<unsigned int>> tileData, unsigned int levelWidth, unsigned int levelHeight);
};