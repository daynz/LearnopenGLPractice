#include "ResourceManager.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

std::map<std::string, Shader> ResourceManager::m_shader;
std::map<std::string, Texture2D> ResourceManager::m_texture;

Shader ResourceManager::loadShader(const char* vShaderFile, const char* fShaderFile, const char* gShaderFile, std::string name)
{
	m_shader[name] = loadShaderFromFile(vShaderFile, fShaderFile, gShaderFile);
	return m_shader[name];
}

Shader ResourceManager::getShader(std::string name)
{
	return m_shader[name];
}

Texture2D ResourceManager::loadTexture(const char* file, bool alpha, std::string name)
{
	m_texture[name] = loadTextureFromFile(file, alpha);
	return m_texture[name];
}

Texture2D ResourceManager::getTexture(std::string name)
{
	return m_texture[name];
}

void ResourceManager::clear()
{
	for (auto iter : m_shader)
	{
		glDeleteProgram(iter.second.ID());
	}
	for (auto iter : m_texture)
	{
		glDeleteProgram(iter.second.ID());
	}
}

Shader ResourceManager::loadShaderFromFile(const char* vShaderFile, const char* fShaderFile, const char* gShaderFile /*= nullptr*/)
{
	// 1. Retrieve the vertex/fragment source code from filePath
	std::string vertexCode;
	std::string fragmentCode;
	std::string geometryCode;
	try
	{
		// Open files
		std::ifstream vertexShaderFile(vShaderFile);
		std::ifstream fragmentShaderFile(fShaderFile);
		std::stringstream vShaderStream, fShaderStream;
		// Read file's buffer contents into streams
		vShaderStream << vertexShaderFile.rdbuf();
		fShaderStream << fragmentShaderFile.rdbuf();
		// close file handlers
		vertexShaderFile.close();
		fragmentShaderFile.close();
		// Convert stream into string
		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();
		// If geometry shader path is present, also load a geometry shader
		if (gShaderFile != nullptr)
		{
			std::ifstream geometryShaderFile(gShaderFile);
			std::stringstream gShaderStream;
			gShaderStream << geometryShaderFile.rdbuf();
			geometryShaderFile.close();
			geometryCode = gShaderStream.str();
		}
	}
	catch (std::exception e)
	{
		std::cout << "ERROR::SHADER: Failed to read shader files" << std::endl;
	}
	const char* vShaderCode = vertexCode.c_str();
	const char* fShaderCode = fragmentCode.c_str();
	const char* gShaderCode = geometryCode.c_str();
	// 2. Now create shader object from source code
	Shader shader;
	shader.compile(vShaderCode, fShaderCode, gShaderFile != nullptr ? gShaderCode : nullptr);
	return shader;
}

Texture2D ResourceManager::loadTextureFromFile(const char* file, bool alpha)
{
	// Create Texture object
	Texture2D texture;
	if (alpha)
	{
		texture.m_Internal_Format = GL_RGBA;
		texture.m_Image_Format = GL_RGBA;
	}
	// Load image
	int width, height, nrChannels;
	unsigned char* data = stbi_load(file, &width, &height, &nrChannels, 0);
	if (!data)
	{
		std::cout << "texture load error" << std::endl;
	}
	// Now generate texture
	texture.Generate(width, height, data);
	// And finally free image data
	stbi_image_free(data);
	return texture;
}

