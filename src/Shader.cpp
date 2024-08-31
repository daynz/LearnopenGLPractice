#include "HeadFile.hpp"
#include "Shader.hpp"
Shader::Shader()
{
	
}

Shader& Shader::use()
{
	glUseProgram(this->m_ID);
	return *this;
}

void Shader::compile(const GLchar* vertexSource, const GLchar* fragmentSource, const GLchar* geometrySource /*= nullptr*/)
{
	GLuint sVertex, sFragment, gShader;
	// Vertex Shader
	sVertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(sVertex, 1, &vertexSource, NULL);
	glCompileShader(sVertex);
	checkCompileErrors(sVertex, "VERTEX");
	// Fragment Shader
	sFragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(sFragment, 1, &fragmentSource, NULL);
	glCompileShader(sFragment);
	checkCompileErrors(sFragment, "FRAGMENT");
	// If geometry shader source code is given, also compile geometry shader
	if (geometrySource != nullptr)
	{
		gShader = glCreateShader(GL_GEOMETRY_SHADER);
		glShaderSource(gShader, 1, &geometrySource, NULL);
		glCompileShader(gShader);
		checkCompileErrors(gShader, "GEOMETRY");
	}
	// Shader Program
	this->m_ID = glCreateProgram();
	glAttachShader(this->m_ID, sVertex);
	glAttachShader(this->m_ID, sFragment);
	if (geometrySource != nullptr)
		glAttachShader(this->m_ID, gShader);
	glLinkProgram(this->m_ID);
	checkCompileErrors(this->m_ID, "PROGRAM");
	glDeleteShader(sVertex);
	glDeleteShader(sFragment);
	if (geometrySource != nullptr)
		glDeleteShader(gShader);
}

void Shader::SetFloat(const char* name, float value, bool useShader)
{
	if (useShader)
		this->use();
	glUniform1f(glGetUniformLocation(this->m_ID, name), value);
}
void Shader::SetInteger(const char* name, int value, bool useShader)
{
	if (useShader)
		this->use();
	glUniform1i(glGetUniformLocation(this->m_ID, name), value);
}
void Shader::SetVector2f(const char* name, float x, float y, bool useShader)
{
	if (useShader)
		this->use();
	glUniform2f(glGetUniformLocation(this->m_ID, name), x, y);
}
void Shader::SetVector2f(const char* name, const glm::vec2& value, bool useShader)
{
	if (useShader)
		this->use();
	glUniform2f(glGetUniformLocation(this->m_ID, name), value.x, value.y);
}
void Shader::SetVector3f(const char* name, float x, float y, float z, bool useShader)
{
	if (useShader)
		this->use();
	glUniform3f(glGetUniformLocation(this->m_ID, name), x, y, z);
}
void Shader::SetVector3f(const char* name, const glm::vec3& value, bool useShader)
{
	if (useShader)
		this->use();
	glUniform3f(glGetUniformLocation(this->m_ID, name), value.x, value.y, value.z);
}
void Shader::SetVector4f(const char* name, float x, float y, float z, float w, bool useShader)
{
	if (useShader)
		this->use();
	glUniform4f(glGetUniformLocation(this->m_ID, name), x, y, z, w);
}
void Shader::SetVector4f(const char* name, const glm::vec4& value, bool useShader)
{
	if (useShader)
		this->use();
	glUniform4f(glGetUniformLocation(this->m_ID, name), value.x, value.y, value.z, value.w);
}
void Shader::SetMatrix4(const char* name, const glm::mat4& matrix, bool useShader)
{
	if (useShader)
		this->use();
	glUniformMatrix4fv(glGetUniformLocation(this->m_ID, name), 1, false, glm::value_ptr(matrix));
}


void Shader::checkCompileErrors(GLuint object, std::string type)
{
	GLint success;
	GLchar infoLog[1024];
	if (type != "PROGRAM")
	{
		glGetShaderiv(object, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(object, 1024, NULL, infoLog);
			std::cout << "| ERROR::SHADER: Compile-time error: Type: " << type << "\n"
				<< infoLog << "\n -- --------------------------------------------------- -- "
				<< std::endl;
		}
	}
	else
	{
		glGetProgramiv(object, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(object, 1024, NULL, infoLog);
			std::cout << "| ERROR::Shader: Link-time error: Type: " << type << "\n"
				<< infoLog << "\n -- --------------------------------------------------- -- "
				<< std::endl;
		}
	}
}