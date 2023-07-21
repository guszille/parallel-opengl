#include "VBO.h"

VBO::VBO() : m_ID()
{
	glGenBuffers(1, &m_ID);

	std::cout << "VBO() [ID: " << m_ID << "]\n";
}

VBO::VBO(const float* vertices, int size) : m_ID()
{
	glGenBuffers(1, &m_ID);
	glBindBuffer(GL_ARRAY_BUFFER, m_ID);
	glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void VBO::Fill(const float* vertices, int size)
{
	glBindBuffer(GL_ARRAY_BUFFER, m_ID);
	glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void VBO::Bind()
{
	glBindBuffer(GL_ARRAY_BUFFER, m_ID);
}

void VBO::Unbind()
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}