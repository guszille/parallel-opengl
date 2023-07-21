#pragma once

#include <iostream>

#include <glad/glad.h>

class VAO
{
public:
	VAO();

	void Bind();
	void Unbind();

	void SetVertexAttribute(unsigned int index, int size, int type, bool normalized, unsigned int stride, void* pointer, int divisor = 0);
	static int RetrieveMaxVertexAttributes();

private:
	unsigned int m_ID;
};