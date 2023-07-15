#pragma once

#include <glad/glad.h>

class VBO
{
public:
	VBO(const void* vertices, const int size);

	void Bind();
	void Unbind();

private:
	unsigned int m_ID;
};