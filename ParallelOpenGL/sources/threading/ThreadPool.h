#pragma once

#include <vector>
#include <future>

class ThreadPool
{
public:
	ThreadPool() = delete;

	static void Execute(std::function<void()> job);

private:
	static std::vector<std::future<void>> m_Futures;
};