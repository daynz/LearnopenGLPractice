#pragma once

#include "HeadFile.hpp"

#include "Texture.hpp"
#include "Shader.hpp"
#include "SpriteRenderer.hpp"

class PostProcessor
{
public:
	Shader PostProcessingShader;
	Texture2D Texture;
	unsigned int m_Width, m_Height;
	bool Confuse, Chaos, Shake;
	PostProcessor(Shader shader, unsigned int width, unsigned int height);
	void BeginRender();
	void EndRender();
	void Render(float time);
private:
	unsigned int MSFBO, FBO;
	unsigned int RBO;
	unsigned int VAO;
	void initRenderData();
};