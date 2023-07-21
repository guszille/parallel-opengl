#pragma once

#include <iostream>

#include <glad/glad.h>

class EBO
{
public:
	EBO();
	EBO(const unsigned int* indices, int size);

	void Fill(const unsigned int* indices, int size);
	void Bind();
	void Unbind();

private:
	unsigned int m_ID;
};