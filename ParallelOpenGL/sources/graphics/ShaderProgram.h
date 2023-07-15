#pragma once

#include <fstream>
#include <sstream>
#include <iostream>

#include <glad/glad.h>

class ShaderProgram
{
public:
	ShaderProgram(const char* vertexShaderFilepath, const char* fragmentShaderFilepath);
	ShaderProgram(const char* vertexShaderFilepath, const char* geometryShaderFilepath, const char* fragmentShaderFilepath);

	void Bind();
	void Unbind();

	void SetUniform1f(const char* uniformName, float data);
	void SetUniform3f(const char* uniformName, float x, float y, float z);

private:
	unsigned int m_ID;

	const unsigned int CreateShader(const char* filepath, int shaderType);
};