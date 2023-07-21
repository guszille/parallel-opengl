#pragma once

#include <vector>
#include <thread>
#include <iostream>

#include <GLFW/glfw3.h>

class ThreadHandler
{
public:
	ThreadHandler(void(*job)(), void(*callback)() = nullptr);

	void Execute();
	bool Check();

private:
	void(*m_Job)();
	void(*m_Callback)();
	bool m_Done;
};

class ThreadPool
{
public:
	ThreadPool() = delete;

	static void Dispatch(void(*job)(), void(*callback)() = nullptr);
	static void CheckThreads();

private:
	static std::vector<ThreadHandler*> m_Threads;
};