#include "ThreadPool.h"

std::vector<ThreadHandler*> ThreadPool::m_Threads;

ThreadHandler::ThreadHandler(void(*job)(), void(*callback)())
	: m_Job(job), m_Callback(callback), m_Done(false)
{
	std::thread([this]() {
		this->Execute();
	}).detach();
}

void ThreadHandler::Execute()
{
	m_Job();

	m_Done = true;
}

bool ThreadHandler::Check()
{
	if (m_Done)
	{
		if (m_Callback)
		{
			m_Callback();
		}
	}

	return m_Done;
}

void ThreadPool::Dispatch(void(*job)(), void(*callback)())
{
	m_Threads.push_back(new ThreadHandler(job, callback));
}

void ThreadPool::CheckThreads()
{
	for (std::vector<ThreadHandler*>::iterator it = m_Threads.begin(); it != m_Threads.end();)
	{
		ThreadHandler* thread = *it;

		if (thread->Check())
		{
			it = m_Threads.erase(it);

			delete thread;
		}
		else
		{
			++it;
		}
	}
}