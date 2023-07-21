#pragma once

#include <iostream>

#include <glad/glad.h>

class VBO
{
public:
	VBO();
	VBO(const float* vertices, int size);

	void Fill(const float* vertices, int size);
	void Bind();
	void Unbind();

private:
	unsigned int m_ID;
};