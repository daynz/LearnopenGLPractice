#pragma once
#include "HeadFile.hpp"
#include "Texture.hpp"
#include "Shader.hpp"

class ResourceManager
{
public:
	static Shader loadShader(const char* vShaderFile, const char* fShaderFile, const char* gShaderFile, std::string name);
	static Shader getShader(std::string name);
	static Texture2D loadTexture(const char* file, bool alpha, std::string name);
	static Texture2D getTexture(std::string name);
	static void clear();
private:
	static std::map<std::string, Shader> m_shader;
	static std::map<std::string, Texture2D> m_texture;
	ResourceManager() {};
	static Shader loadShaderFromFile(const char* vShaderFile, const char* fShaderFile, const char* gShaderFile = nullptr);
	static Texture2D loadTextureFromFile(const char* file, bool alpha);
};