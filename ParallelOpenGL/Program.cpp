// ParallelOpenGL
// Using multi-threading programming with OpenGL.

#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "sources/graphics/VAO.h"
#include "sources/graphics/VBO.h"
#include "sources/graphics/EBO.h"
#include "sources/graphics/ShaderProgram.h"

#include "sources/threading/ThreadPool.h"

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;

ShaderProgram* g_QuadSP;
VAO* g_QuadVAO;
VBO* g_QuadVBO;
EBO* g_QuadEBO;

bool g_IsQuadReady = false;

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void process_input(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
}

void setup_application()
{
	float quadVertices[] = {
		 0.5f,  0.5f,  0.0f,  // top right
		 0.5f, -0.5f,  0.0f,  // bottom right
		-0.5f, -0.5f,  0.0f,  // bottom left
		-0.5f,  0.5f,  0.0f   // top left
	};

	unsigned int quadIndices[] = {
		0, 1, 3,  // first triangle
		1, 2, 3   // second triangle
	};

	g_QuadSP = new ShaderProgram("sources/shaders/vertex_shader.glsl", "sources/shaders/fragment_shader.glsl");

	g_QuadSP->Bind();
	g_QuadSP->SetUniform3f("uColor", 1.0f, 0.5f, 0.25f);
	g_QuadSP->Unbind();

	g_QuadVAO = new VAO();
	g_QuadVBO = new VBO(quadVertices, sizeof(quadVertices));
	g_QuadEBO = new EBO(quadIndices, sizeof(quadIndices));

	g_QuadVAO->Bind();
	g_QuadVBO->Bind();
	g_QuadEBO->Bind();

	g_QuadVAO->SetVertexAttribute(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)(0));

	g_QuadVAO->Unbind();
	g_QuadVBO->Unbind();
	g_QuadEBO->Unbind();

	g_IsQuadReady = true;
}

int main()
{
	if (!glfwInit())
	{
		std::cout << "Failed to initialize GLFW!" << std::endl;

		return -1;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "ParallelOpenGL", NULL, NULL);

	if (!window)
	{
		std::cout << "Failed to create a GLFW window!" << std::endl;
		glfwTerminate();

		return -1;
	}

	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD!" << std::endl;
		glfwTerminate();

		return -1;
	}

	setup_application();

	while (!glfwWindowShouldClose(window))
	{
		process_input(window);

		glClearColor(0.2f, 0.3f, 0.4f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		if (g_IsQuadReady) // Draw.
		{
			g_QuadSP->Bind();
			g_QuadVAO->Bind();

			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

			g_QuadSP->Unbind();
			g_QuadVAO->Unbind();
		}

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();

	return 0;
}