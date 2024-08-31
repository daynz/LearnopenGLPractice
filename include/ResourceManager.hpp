#pragma once
#include "HeadFile.hpp"
#include "Texture.hpp"
#include "Shader.hpp"

class ResourceManager
{
public:
	static Shader loadShader(const GLchar* vShaderFile, const GLchar* fShaderFile, const GLchar* gShaderFile, std::string name);
	static Shader getShader(std::string name);
	static Texture2D loadTexture(const GLchar* file, GLboolean alpha, std::string name);
	static Texture2D getTexture(std::string name);
	static void clear();
private:
	static std::map<std::string, Shader> m_shader;
	static std::map<std::string, Texture2D> m_texture;
	ResourceManager() {};
	static Shader loadShaderFromFile(const GLchar* vShaderFile, const GLchar* fShaderFile, const GLchar* gShaderFile = nullptr);
	static Texture2D loadTextureFromFile(const GLchar* file, GLboolean alpha);
};