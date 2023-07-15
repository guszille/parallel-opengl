#include "ShaderProgram.h"

ShaderProgram::ShaderProgram(const char* vertexShaderFilepath, const char* fragmentShaderFilepath)
	: m_ID()
{
	int success;
	char infoLog[512];

	unsigned int vertexShaderID = CreateShader(vertexShaderFilepath, GL_VERTEX_SHADER);
	unsigned int fragmentShaderID = CreateShader(fragmentShaderFilepath, GL_FRAGMENT_SHADER);

	m_ID = glCreateProgram();

	glAttachShader(m_ID, vertexShaderID);
	glAttachShader(m_ID, fragmentShaderID);

	glLinkProgram(m_ID);

	glGetProgramiv(m_ID, GL_LINK_STATUS, &success);

	if (!success)
	{
		glGetProgramInfoLog(m_ID, 512, NULL, infoLog);

		std::cout << "[ERROR] SHADER PROGRAM: Linkage failed!\n" << infoLog << std::endl;
	}

	glDeleteShader(vertexShaderID);
	glDeleteShader(fragmentShaderID);
}

ShaderProgram::ShaderProgram(const char* vertexShaderFilepath, const char* geometryShaderFilepath, const char* fragmentShaderFilepath)
	: m_ID()
{
	int success;
	char infoLog[512];

	unsigned int vertexShaderID = CreateShader(vertexShaderFilepath, GL_VERTEX_SHADER);
	unsigned int geometryShaderID = CreateShader(geometryShaderFilepath, GL_GEOMETRY_SHADER);
	unsigned int fragmentShaderID = CreateShader(fragmentShaderFilepath, GL_FRAGMENT_SHADER);

	m_ID = glCreateProgram();

	glAttachShader(m_ID, vertexShaderID);
	glAttachShader(m_ID, geometryShaderID);
	glAttachShader(m_ID, fragmentShaderID);

	glLinkProgram(m_ID);

	glGetProgramiv(m_ID, GL_LINK_STATUS, &success);

	if (!success)
	{
		glGetProgramInfoLog(m_ID, 512, NULL, infoLog);

		std::cout << "[ERROR] SHADER PROGRAM: Linkage failed!\n" << infoLog << std::endl;
	}

	glDeleteShader(vertexShaderID);
	glDeleteShader(geometryShaderID);
	glDeleteShader(fragmentShaderID);
}

void ShaderProgram::Bind()
{
	glUseProgram(m_ID);
}

void ShaderProgram::Unbind()
{
	glUseProgram(0);
}

void ShaderProgram::SetUniform1f(const char* uniformName, float data)
{
	int uniformLocation = glGetUniformLocation(m_ID, uniformName);

	if (uniformLocation > -1)
	{
		glUniform1f(uniformLocation, data);
	}
	else
	{
		std::cout << "[ERROR] SHADER PROGRAM: Failed to get location of uniform \"" << uniformName << "\"." << std::endl;
	}
}

void ShaderProgram::SetUniform3f(const char* uniformName, float x, float y, float z)
{
	int uniformLocation = glGetUniformLocation(m_ID, uniformName);

	if (uniformLocation > -1)
	{
		glUniform3f(uniformLocation, x, y, z);
	}
	else
	{
		std::cout << "[ERROR] SHADER PROGRAM: Failed to get location of uniform \"" << uniformName << "\"." << std::endl;
	}
}

const unsigned int ShaderProgram::CreateShader(const char* filepath, int shaderType)
{
	int success;
	char infoLog[512];

	std::ifstream fileStream(filepath);
	std::stringstream stringStream; stringStream << fileStream.rdbuf();
	std::string shaderSource = stringStream.str();

	const char* shaderCode = shaderSource.c_str();
	unsigned int shaderID = glCreateShader(shaderType);

	glShaderSource(shaderID, 1, &shaderCode, NULL);
	glCompileShader(shaderID);
	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		glGetShaderInfoLog(shaderID, 512, NULL, infoLog);

		std::cout << "[ERROR] SHADER PROGRAM: Compilation failed!\n" << infoLog << std::endl;

		glDeleteShader(shaderID);

		return -1;
	}

	return shaderID;
}