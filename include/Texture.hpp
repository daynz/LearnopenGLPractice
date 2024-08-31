#pragma once
#include "HeadFile.hpp"

class Texture2D
{
public:
	Texture2D();
	
	void Generate(GLuint width, GLuint height, unsigned char* data);
	
	void Bind() const;

	GLuint ID() const;
	GLuint Width()const;
	GLuint Height()const;
	GLuint Internal_Format()const;
	GLuint Image_Format()const;
	GLuint Wrap_S()const;
	GLuint Wrap_T()const;
	GLuint Filter_Min()const;
	GLuint Filter_Max()const;

	GLuint m_ID;
	GLuint m_Width, m_Height;
	GLuint m_Internal_Format;
	GLuint m_Image_Format;
	GLuint m_Wrap_S;
	GLuint m_Wrap_T;
	GLuint m_Filter_Min;
	GLuint m_Filter_Max;
};