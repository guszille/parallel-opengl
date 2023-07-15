#include "ThreadPool.h"

std::vector<std::future<void>> ThreadPool::m_Futures;

void ThreadPool::Execute(std::function<void()> job)
{
	m_Futures.push_back(std::async(std::launch::async, job));
}