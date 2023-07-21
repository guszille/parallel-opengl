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

#include "sources/utils/Debug.h"

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;

GLFWwindow* g_PrimaryGLContext;
GLFWwindow* g_SecondaryGLContext;

ShaderProgram* g_QuadSP;
VAO* g_QuadVAO;
VBO* g_QuadVBO;
EBO* g_QuadEBO;

GLsync g_GLFenceSync;

float g_QuadVertices[] = {
	 0.5f,  0.5f,  0.0f,  // top right
	 0.5f, -0.5f,  0.0f,  // bottom right
	-0.5f, -0.5f,  0.0f,  // bottom left
	-0.5f,  0.5f,  0.0f   // top left
};

unsigned int g_QuadIndices[] = {
	0, 1, 3,  // first triangle
	1, 2, 3   // second triangle
};

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

void setup_quad_data_job()
{
	std::cout << "setup_quad_data_job()::begin\n";

	glfwMakeContextCurrent(g_SecondaryGLContext);

	g_QuadVBO->Fill(g_QuadVertices, sizeof(g_QuadVertices));
	g_QuadEBO->Fill(g_QuadIndices, sizeof(g_QuadIndices));

	g_GLFenceSync = glFenceSync(GL_SYNC_GPU_COMMANDS_COMPLETE, 0);

	std::this_thread::sleep_for(std::chrono::seconds(2));

	std::cout << "setup_quad_data_job()::end\n";
}

void setup_quad_data_callback()
{
	std::cout << "setup_quad_data_callback()::begin\n";

	g_QuadVAO->Bind();
	g_QuadVBO->Bind();
	g_QuadEBO->Bind();

	g_QuadVAO->SetVertexAttribute(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)(0));

	g_QuadVAO->Unbind();
	g_QuadVBO->Unbind();
	g_QuadEBO->Unbind();

	glWaitSync(g_GLFenceSync, 0, GL_TIMEOUT_IGNORED);
	glDeleteSync(g_GLFenceSync);

	g_IsQuadReady = true;

	std::cout << "setup_quad_data_callback()::end\n";
}

void setup_application()
{
	g_QuadSP = new ShaderProgram("sources/shaders/vertex_shader.glsl", "sources/shaders/fragment_shader.glsl");

	g_QuadVAO = new VAO();
	g_QuadVBO = new VBO();
	g_QuadEBO = new EBO();

	g_QuadSP->Bind();
	g_QuadSP->SetUniform3f("uColor", 1.0f, 0.5f, 0.25f);
	g_QuadSP->Unbind();

	ThreadPool::Dispatch(setup_quad_data_job, setup_quad_data_callback);
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

	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);

	g_PrimaryGLContext = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "ParallelOpenGL", NULL, NULL);

	glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);

	g_SecondaryGLContext = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "", NULL, g_PrimaryGLContext);

	if (!g_PrimaryGLContext || !g_SecondaryGLContext)
	{
		std::cout << "Failed to create GLFW contexts/windows!" << std::endl;
		glfwTerminate();

		return -1;
	}

	glfwMakeContextCurrent(g_PrimaryGLContext);
	glfwSetFramebufferSizeCallback(g_PrimaryGLContext, framebuffer_size_callback);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD!" << std::endl;
		glfwTerminate();

		return -1;
	}

	int contextFlags;
	glGetIntegerv(GL_CONTEXT_FLAGS, &contextFlags);

	if (contextFlags & GL_CONTEXT_FLAG_DEBUG_BIT)
	{
		std::cout << "OpenGL DEBUG context initialized!" << std::endl;

		glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);

		glDebugMessageCallback(CheckGLDebugMessage, nullptr);
		glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
	}

	setup_application();

	while (!glfwWindowShouldClose(g_PrimaryGLContext))
	{
		ThreadPool::CheckThreads();

		process_input(g_PrimaryGLContext);

		glClearColor(0.2f, 0.3f, 0.4f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		if (g_IsQuadReady) // Only do draw calls when the data is set up.
		{
			g_QuadSP->Bind();
			g_QuadVAO->Bind();

			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

			g_QuadVAO->Unbind();
			g_QuadSP->Unbind();
		}

		glfwSwapBuffers(g_PrimaryGLContext);

		glfwPollEvents();
	}

	glfwTerminate();

	return 0;
}