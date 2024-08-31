#pragma once
#include "HeadFile.hpp"

class Texture2D
{
public:
	Texture2D();
	
	void Generate(unsigned int width, unsigned int height, unsigned char* data);
	
	void Bind() const;

	unsigned int ID() const;
	unsigned int Width()const;
	unsigned int Height()const;
	unsigned int Internal_Format()const;
	unsigned int Image_Format()const;
	unsigned int Wrap_S()const;
	unsigned int Wrap_T()const;
	unsigned int Filter_Min()const;
	unsigned int Filter_Max()const;

	unsigned int m_ID;
	unsigned int m_Width, m_Height;
	unsigned int m_Internal_Format;
	unsigned int m_Image_Format;
	unsigned int m_Wrap_S;
	unsigned int m_Wrap_T;
	unsigned int m_Filter_Min;
	unsigned int m_Filter_Max;
};