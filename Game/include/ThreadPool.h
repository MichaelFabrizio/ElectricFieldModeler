#pragma once
#include <thread>

class ThreadPool
{
public:
	ThreadPool();

	void PerformJob(void (*f)(int));
	std::thread RequestThread();
private:
	unsigned int maxThreads_;
};