#include "Texture.hpp"
#include "stb_image.h"

Texture2D::Texture2D()
	: m_Width(0), m_Height(0), m_Internal_Format(GL_RGB), m_Image_Format(GL_RGB), m_Wrap_S(GL_REPEAT), m_Wrap_T(GL_REPEAT), m_Filter_Min(GL_LINEAR), m_Filter_Max(GL_LINEAR)
{
	glGenTextures(1, &this->m_ID);
}

void Texture2D::Generate(GLuint width, GLuint height,unsigned char* data)
{
	this->m_Width = width;
	this->m_Height = height;
	// Create Texture
	glBindTexture(GL_TEXTURE_2D, this->m_ID);
	glTexImage2D(GL_TEXTURE_2D, 0, this->m_Internal_Format, width, height, 0, this->m_Image_Format, GL_UNSIGNED_BYTE, data);
	// Set Texture wrap and filter modes
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, this->m_Wrap_S);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, this->m_Wrap_T);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, this->m_Filter_Min);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, this->m_Filter_Max);
	// Unbind texture
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture2D::Bind() const
{
	glBindTexture(GL_TEXTURE_2D, this->m_ID);
}

GLuint Texture2D::ID() const
{
	return m_ID;
}

GLuint Texture2D::Width() const
{
	return m_Width;
}

GLuint Texture2D::Height() const
{
	return m_Height;
}

GLuint Texture2D::Internal_Format() const
{
	return m_Internal_Format;
}

GLuint Texture2D::Image_Format() const
{
	return m_Image_Format;
}

GLuint Texture2D::Wrap_S() const
{
	return m_Wrap_S;
}

GLuint Texture2D::Wrap_T() const
{
	return m_Wrap_T;
}

GLuint Texture2D::Filter_Min() const
{
	return m_Filter_Min;
}

GLuint Texture2D::Filter_Max() const
{
	return m_Filter_Max;
}

