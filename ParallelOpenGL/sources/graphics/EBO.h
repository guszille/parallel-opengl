#pragma once

#include <glad/glad.h>

class EBO
{
public:
	EBO(const unsigned int* indices, const int size);

	void Bind();
	void Unbind();

private:
	unsigned int m_ID;
};